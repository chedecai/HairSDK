/**
 * @copyright Copyright (c) 2021, Haier.Co, Ltd.
 * @file uh_ble_gatt_server.c
 * @author maaiguo (maaiguo@haier.com)
 * @brief 基于ASR5882s硬件模块的BLE GATT层server端相关功能函数实现
 * @date 2022-02-24
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>version <th>Author  <th>Description
 * <tr><td>2022-02-24   <td>1.0     <td>maaiguo <td>
 * </table>
 */

#define LOG_TAG "ble-s"

/**************************************************************************************************/
/*                           #include (依次为标准头文件、非标准头文件)                            */
/**************************************************************************************************/
#include "esp_mac.h"
#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_gattc_api.h"
#include "esp_gatt_defs.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_gatt_common_api.h"
#include "esp_bt_device.h"
#include "esp_system.h"

#include "uh_types.h"
#include "uh_libc.h"
#include "uh_osal.h"
#include "uh_log.h"

#include "uh_ble.h"
#include "uh_ble_common.h"


/**************************************************************************************************/
/*                                          外部引用声明                                          */
/**************************************************************************************************/


/**************************************************************************************************/
/*                                           内部宏定义                                           */
/**************************************************************************************************/
/**
 * @brief gatts characteristic
 */
#define UHOS_BLE_HAL_CHR_BROADCAST          0x01
#define UHOS_BLE_HAL_CHR_READ               0x02
#define UHOS_BLE_HAL_CHR_WRITE_WITHOUT_RESP 0x04
#define UHOS_BLE_HAL_CHR_WRITE              0x08
#define UHOS_BLE_HAL_CHR_NOTIFY             0x10
#define UHOS_BLE_HAL_CHR_INDICATE           0x20
#define UHOS_BLE_HAL_CHR_AUTH               0x40
#define UHOS_BLE_HAL_CHR_EXT_PROP           0x80
#define UHOS_BLE_HAL_CHR_PERM_READABLE      0x01
#define UHOS_BLE_HAL_CHR_PERM_WRITABLE      0x02

#define UHOS_BLE_HAL_PROFILE_NUM 2
#define UHOS_BLE_HAL_PROFILE_A_APP_ID 0
#define UHOS_BLE_HAL_PROFILE_B_APP_ID 1

#define GATTS_SERVICE_UUID_TEST_A   0x00FF
#define GATTS_CHAR_UUID_TEST_A      0xFF01
#define GATTS_DESCR_UUID_TEST_A     0x3333
#define GATTS_NUM_HANDLE_TEST_A     4

#define GATTS_SERVICE_UUID_TEST_B   0x00EE
#define GATTS_CHAR_UUID_TEST_B      0xEE01
#define GATTS_DESCR_UUID_TEST_B     0x2222
#define GATTS_NUM_HANDLE_TEST_B     4

#define TEST_DEVICE_NAME            "ESP_GATTS_DEMO"
#define TEST_MANUFACTURER_DATA_LEN  17

#define GATTS_DEMO_CHAR_VAL_LEN_MAX 0x40

#define PREPARE_BUF_MAX_SIZE 1024

static uint8_t char1_str[] = {0x11,0x22,0x33};
static esp_gatt_char_prop_t a_property = 0;
static esp_gatt_char_prop_t b_property = 0;

static esp_attr_value_t gatts_demo_char1_val =
{
    .attr_max_len = GATTS_DEMO_CHAR_VAL_LEN_MAX,
    .attr_len     = sizeof(char1_str),
    .attr_value   = char1_str,
};

static uint8_t adv_config_done = 0;
#define adv_config_flag      (1 << 0)
#define scan_rsp_config_flag (1 << 1)

/**************************************************************************************************/
/*                                        内部数据类型定义                                        */
/**************************************************************************************************/
typedef struct uhos_ble_gatts_profile_inst {
    esp_gatts_cb_t gatts_cb;
    uint16_t gatts_if;
    uint16_t app_id;
    uint16_t conn_id;
    uint16_t service_handle;
    esp_gatt_srvc_id_t service_id;
    uint16_t char_handle;
    esp_bt_uuid_t char_uuid;
    esp_gatt_perm_t perm;
    esp_gatt_char_prop_t property;
    uint16_t descr_handle;
    esp_bt_uuid_t descr_uuid;
}uhos_ble_gatts_profile_inst_t;

///Declare the static function
static void gatts_profile_a_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);
static void gatts_profile_b_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);

static uhos_ble_gatts_profile_inst_t uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_NUM] = {
    [UHOS_BLE_HAL_PROFILE_A_APP_ID] = {
        .gatts_cb = gatts_profile_a_event_handler,
        .gatts_if = ESP_GATT_IF_NONE,       /* Not get the gatt_if, so initial is ESP_GATT_IF_NONE */
    },
    [UHOS_BLE_HAL_PROFILE_B_APP_ID] = {
        .gatts_cb = gatts_profile_b_event_handler,                   /* This demo does not implement, similar as profile A */
        .gatts_if = ESP_GATT_IF_NONE,       /* Not get the gatt_if, so initial is ESP_GATT_IF_NONE */
    },
};

typedef struct {
    uint8_t                 *prepare_buf;
    int                     prepare_len;
} prepare_type_env_t;

static prepare_type_env_t a_prepare_write_env;
static prepare_type_env_t b_prepare_write_env;

/**************************************************************************************************/
/*                                        全局(静态)变量                                          */
/**************************************************************************************************/
uhos_ble_gatts_cb_t g_uhos_ble_pal_gatts_user_cb = UHOS_NULL;   //<! GATT层用户设置的server端回调函数


/**************************************************************************************************/
/*                                          内部函数原型                                          */
/**************************************************************************************************/
void example_write_event_env(esp_gatt_if_t gatts_if, prepare_type_env_t *prepare_write_env, esp_ble_gatts_cb_param_t *param);
void example_exec_write_event_env(prepare_type_env_t *prepare_write_env, esp_ble_gatts_cb_param_t *param);


/**************************************************************************************************/
/*                                          内部函数实现                                          */
/**************************************************************************************************/
void example_write_event_env(esp_gatt_if_t gatts_if, prepare_type_env_t *prepare_write_env, esp_ble_gatts_cb_param_t *param){
    esp_gatt_status_t status = ESP_GATT_OK;
    if (param->write.need_rsp){
        if (param->write.is_prep){
            if (prepare_write_env->prepare_buf == NULL) {
                prepare_write_env->prepare_buf = (uint8_t *)malloc(PREPARE_BUF_MAX_SIZE*sizeof(uint8_t));
                prepare_write_env->prepare_len = 0;
                if (prepare_write_env->prepare_buf == NULL) {
                    UHOS_LOGE( "Gatt_server prep no mem\n");
                    status = ESP_GATT_NO_RESOURCES;
                }
            } else {
                if(param->write.offset > PREPARE_BUF_MAX_SIZE) {
                    status = ESP_GATT_INVALID_OFFSET;
                } else if ((param->write.offset + param->write.len) > PREPARE_BUF_MAX_SIZE) {
                    status = ESP_GATT_INVALID_ATTR_LEN;
                }
            }

            esp_gatt_rsp_t *gatt_rsp = (esp_gatt_rsp_t *)malloc(sizeof(esp_gatt_rsp_t));
            gatt_rsp->attr_value.len = param->write.len;
            gatt_rsp->attr_value.handle = param->write.handle;
            gatt_rsp->attr_value.offset = param->write.offset;
            gatt_rsp->attr_value.auth_req = ESP_GATT_AUTH_REQ_NONE;
            memcpy(gatt_rsp->attr_value.value, param->write.value, param->write.len);
            esp_err_t response_err = esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, status, gatt_rsp);
            if (response_err != ESP_OK){
               UHOS_LOGE( "Send response error\n");
            }
            free(gatt_rsp);
            if (status != ESP_GATT_OK){
                return;
            }
            memcpy(prepare_write_env->prepare_buf + param->write.offset,
                   param->write.value,
                   param->write.len);
            prepare_write_env->prepare_len += param->write.len;

        }else{
            esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, status, NULL);
        }
    }
}

void example_exec_write_event_env(prepare_type_env_t *prepare_write_env, esp_ble_gatts_cb_param_t *param){
    if (param->exec_write.exec_write_flag == ESP_GATT_PREP_WRITE_EXEC){
        esp_log_buffer_hex("GATTS_TAG", prepare_write_env->prepare_buf, prepare_write_env->prepare_len);
    }else{
        UHOS_LOGI("ESP_GATT_PREP_WRITE_CANCEL");
    }
    if (prepare_write_env->prepare_buf) {
        free(prepare_write_env->prepare_buf);
        prepare_write_env->prepare_buf = NULL;
    }
    prepare_write_env->prepare_len = 0;
}

static void uhos_ble_gatts_profile_a_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) {
    switch (event) {
    case ESP_GATTS_REG_EVT:
        UHOS_LOGI("REGISTER_APP_EVT, status %d, app_id %d\n", param->reg.status, param->reg.app_id);
        uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_A_APP_ID].service_id.is_primary = true;
        uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_A_APP_ID].service_id.id.inst_id = 0x00;
        uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_A_APP_ID].service_id.id.uuid.len = ESP_UUID_LEN_16;
        uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_A_APP_ID].service_id.id.uuid.uuid.uuid16 = GATTS_SERVICE_UUID_TEST_A;

        esp_err_t set_dev_name_ret = esp_ble_gap_set_device_name(TEST_DEVICE_NAME);
        if (set_dev_name_ret){
            UHOS_LOGE("set device name failed, error code = %x", set_dev_name_ret);
        }
#ifdef CONFIG_SET_RAW_ADV_DATA
        esp_err_t raw_adv_ret = esp_ble_gap_config_adv_data_raw(raw_adv_data, sizeof(raw_adv_data));
        if (raw_adv_ret){
            UHOS_LOGE("config raw adv data failed, error code = %x ", raw_adv_ret);
        }
        adv_config_done |= adv_config_flag;
        esp_err_t raw_scan_ret = esp_ble_gap_config_scan_rsp_data_raw(raw_scan_rsp_data, sizeof(raw_scan_rsp_data));
        if (raw_scan_ret){
           UHOS_LOGE("config raw scan rsp data failed, error code = %x", raw_scan_ret);
        }
        adv_config_done |= scan_rsp_config_flag;
#else
        //config adv data
        esp_err_t ret = esp_ble_gap_config_adv_data(&adv_data);
        if (ret){
            UHOS_LOGE("config adv data failed, error code = %x", ret);
        }
        //adv_config_done |= adv_config_flag;
        //config scan response data
        ret = esp_ble_gap_config_adv_data(&scan_rsp_data);
        if (ret){
            UHOS_LOGE("config scan response data failed, error code = %x", ret);
        }
        //adv_config_done |= scan_rsp_config_flag;

#endif
        esp_ble_gatts_create_service(gatts_if, &uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_A_APP_ID].service_id, GATTS_NUM_HANDLE_TEST_A);
        break;
    case ESP_GATTS_READ_EVT: {
        UHOS_LOGI("GATT_READ_EVT, conn_id %d, trans_id %d, handle %d\n", param->read.conn_id, param->read.trans_id, param->read.handle);
        esp_gatt_rsp_t rsp;
        memset(&rsp, 0, sizeof(esp_gatt_rsp_t));
        rsp.attr_value.handle = param->read.handle;
        rsp.attr_value.len = 4;
        rsp.attr_value.value[0] = 0xde;
        rsp.attr_value.value[1] = 0xed;
        rsp.attr_value.value[2] = 0xbe;
        rsp.attr_value.value[3] = 0xef;
        esp_ble_gatts_send_response(gatts_if, param->read.conn_id, param->read.trans_id,
                                    ESP_GATT_OK, &rsp);
        break;
    }
    case ESP_GATTS_WRITE_EVT: {
        UHOS_LOGI("GATT_WRITE_EVT, conn_id %d, trans_id %d, handle %d", param->write.conn_id, param->write.trans_id, param->write.handle);
        if (!param->write.is_prep){
            UHOS_LOGI("GATT_WRITE_EVT, value len %d, value :", param->write.len);
            esp_log_buffer_hex("GATTS_TAG", param->write.value, param->write.len);
            if (uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_A_APP_ID].descr_handle == param->write.handle && param->write.len == 2){
                uint16_t descr_value = param->write.value[1]<<8 | param->write.value[0];
                if (descr_value == 0x0001){
                    if (a_property & ESP_GATT_CHAR_PROP_BIT_NOTIFY){
                        UHOS_LOGI("notify enable");
                        uint8_t notify_data[15];
                        for (int i = 0; i < sizeof(notify_data); ++i)
                        {
                            notify_data[i] = i%0xff;
                        }
                        //the size of notify_data[] need less than MTU size
                        esp_ble_gatts_send_indicate(gatts_if, param->write.conn_id, uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_A_APP_ID].char_handle,
                                                sizeof(notify_data), notify_data, false);
                    }
                }else if (descr_value == 0x0002){
                    if (a_property & ESP_GATT_CHAR_PROP_BIT_INDICATE){
                        UHOS_LOGI("indicate enable");
                        uint8_t indicate_data[15];
                        for (int i = 0; i < sizeof(indicate_data); ++i)
                        {
                            indicate_data[i] = i%0xff;
                        }
                        //the size of indicate_data[] need less than MTU size
                        esp_ble_gatts_send_indicate(gatts_if, param->write.conn_id, uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_A_APP_ID].char_handle,
                                                sizeof(indicate_data), indicate_data, true);
                    }
                }
                else if (descr_value == 0x0000){
                    UHOS_LOGI("notify/indicate disable ");
                }else{
                    UHOS_LOGE("unknown descr value");
                    esp_log_buffer_hex("GATTS_TAG", param->write.value, param->write.len);
                }

            }
        }
        example_write_event_env(gatts_if, &a_prepare_write_env, param);
        break;
    }
    case ESP_GATTS_EXEC_WRITE_EVT:
        UHOS_LOGI("ESP_GATTS_EXEC_WRITE_EVT");
        esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, ESP_GATT_OK, NULL);
        example_exec_write_event_env(&a_prepare_write_env, param);
        break;
    case ESP_GATTS_MTU_EVT:
        UHOS_LOGI("ESP_GATTS_MTU_EVT, MTU %d", param->mtu.mtu);
        break;
    case ESP_GATTS_UNREG_EVT:
        break;
    case ESP_GATTS_CREATE_EVT:
        UHOS_LOGI( "CREATE_SERVICE_EVT, status %d,  service_handle %d\n", param->create.status, param->create.service_handle);
        uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_A_APP_ID].service_handle = param->create.service_handle;
        uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_A_APP_ID].char_uuid.len = ESP_UUID_LEN_16;
        uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_A_APP_ID].char_uuid.uuid.uuid16 = GATTS_CHAR_UUID_TEST_A;

        esp_ble_gatts_start_service(uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_A_APP_ID].service_handle);
        a_property = ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_WRITE | ESP_GATT_CHAR_PROP_BIT_NOTIFY;
        esp_err_t add_char_ret = esp_ble_gatts_add_char(uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_A_APP_ID].service_handle, &uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_A_APP_ID].char_uuid,
                                                        ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
                                                        a_property,
                                                        &gatts_demo_char1_val, NULL);
        if (add_char_ret){
            UHOS_LOGIE"add char failed, error code =%x",add_char_ret);
        }
        break;
    case ESP_GATTS_ADD_INCL_SRVC_EVT:
        break;
    case ESP_GATTS_ADD_CHAR_EVT: {
        uint16_t length = 0;
        const uint8_t *prf_char;

        UHOS_LOGI( "ADD_CHAR_EVT, status %d,  attr_handle %d, service_handle %d\n",
                param->add_char.status, param->add_char.attr_handle, param->add_char.service_handle);
        uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_A_APP_ID].char_handle = param->add_char.attr_handle;
        uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_A_APP_ID].descr_uuid.len = ESP_UUID_LEN_16;
        uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_A_APP_ID].descr_uuid.uuid.uuid16 = ESP_GATT_UUID_CHAR_CLIENT_CONFIG;
        esp_err_t get_attr_ret = esp_ble_gatts_get_attr_value(param->add_char.attr_handle,  &length, &prf_char);
        if (get_attr_ret == ESP_FAIL){
            UHOS_LOGI("ILLEGAL HANDLE");
        }

        UHOS_LOGI( "the gatts demo char length = %x\n", length);
        for(int i = 0; i < length; i++){
            UHOS_LOGI("prf_char[%x] =%x\n",i,prf_char[i]);
        }
        esp_err_t add_descr_ret = esp_ble_gatts_add_char_descr(uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_A_APP_ID].service_handle, &uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_A_APP_ID].descr_uuid,
                                                                ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, NULL, NULL);
        if (add_descr_ret){
            UHOS_LOGI("add char descr failed, error code =%x", add_descr_ret);
        }
        break;
    }
    case ESP_GATTS_ADD_CHAR_DESCR_EVT:
        uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_A_APP_ID].descr_handle = param->add_char_descr.attr_handle;
        UHOS_LOGI("ADD_DESCR_EVT, status %d, attr_handle %d, service_handle %d\n",
                 param->add_char_descr.status, param->add_char_descr.attr_handle, param->add_char_descr.service_handle);
        break;
    case ESP_GATTS_DELETE_EVT:
        break;
    case ESP_GATTS_START_EVT:
        UHOS_LOGI("SERVICE_START_EVT, status %d, service_handle %d\n",
                 param->start.status, param->start.service_handle);
        break;
    case ESP_GATTS_STOP_EVT:
        break;
    case ESP_GATTS_CONNECT_EVT: {
        esp_ble_conn_update_params_t conn_params = {0};
        memcpy(conn_params.bda, param->connect.remote_bda, sizeof(esp_bd_addr_t));
        /* For the IOS system, please reference the apple official documents about the ble connection parameters restrictions. */
        conn_params.latency = 0;
        conn_params.max_int = 0x20;    // max_int = 0x20*1.25ms = 40ms
        conn_params.min_int = 0x10;    // min_int = 0x10*1.25ms = 20ms
        conn_params.timeout = 400;    // timeout = 400*10ms = 4000ms
        UHOS_LOGI("ESP_GATTS_CONNECT_EVT, conn_id %d, remote %02x:%02x:%02x:%02x:%02x:%02x:",
                 param->connect.conn_id,
                 param->connect.remote_bda[0], param->connect.remote_bda[1], param->connect.remote_bda[2],
                 param->connect.remote_bda[3], param->connect.remote_bda[4], param->connect.remote_bda[5]);
        uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_A_APP_ID].conn_id = param->connect.conn_id;
        //start sent the update connection parameters to the peer device.
        esp_ble_gap_update_conn_params(&conn_params);
        break;
    }
    case ESP_GATTS_DISCONNECT_EVT:
        UHOS_LOGI("ESP_GATTS_DISCONNECT_EVT, disconnect reason 0x%x", param->disconnect.reason);
        esp_ble_gap_start_advertising(&adv_params);
        break;
    case ESP_GATTS_CONF_EVT:
        UHOS_LOGI("ESP_GATTS_CONF_EVT, status %d attr_handle %d", param->conf.status, param->conf.handle);
        if (param->conf.status != ESP_GATT_OK){
            esp_log_buffer_hex("GATTS_TAG", param->conf.value, param->conf.len);
        }
        break;
    case ESP_GATTS_OPEN_EVT:
    case ESP_GATTS_CANCEL_OPEN_EVT:
    case ESP_GATTS_CLOSE_EVT:
    case ESP_GATTS_LISTEN_EVT:
    case ESP_GATTS_CONGEST_EVT:
    default:
        break;
    }
}

static void gatts_profile_b_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) {
    switch (event) {
    case ESP_GATTS_REG_EVT:
        UHOS_LOGI( "REGISTER_APP_EVT, status %d, app_id %d\n", param->reg.status, param->reg.app_id);
        uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_B_APP_ID].service_id.is_primary = true;
        uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_B_APP_ID].service_id.id.inst_id = 0x00;
        uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_B_APP_ID].service_id.id.uuid.len = ESP_UUID_LEN_16;
        uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_B_APP_ID].service_id.id.uuid.uuid.uuid16 = GATTS_SERVICE_UUID_TEST_B;

        esp_ble_gatts_create_service(gatts_if, &uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_B_APP_ID].service_id, GATTS_NUM_HANDLE_TEST_B);
        break;
    case ESP_GATTS_READ_EVT: {
        UHOS_LOGI( "GATT_READ_EVT, conn_id %d, trans_id %d, handle %d\n", param->read.conn_id, param->read.trans_id, param->read.handle);
        esp_gatt_rsp_t rsp;
        memset(&rsp, 0, sizeof(esp_gatt_rsp_t));
        rsp.attr_value.handle = param->read.handle;
        rsp.attr_value.len = 4;
        rsp.attr_value.value[0] = 0xde;
        rsp.attr_value.value[1] = 0xed;
        rsp.attr_value.value[2] = 0xbe;
        rsp.attr_value.value[3] = 0xef;
        esp_ble_gatts_send_response(gatts_if, param->read.conn_id, param->read.trans_id,
                                    ESP_GATT_OK, &rsp);
        break;
    }
    case ESP_GATTS_WRITE_EVT: {
        UHOS_LOGI( "GATT_WRITE_EVT, conn_id %d, trans_id %d, handle %d\n", param->write.conn_id, param->write.trans_id, param->write.handle);
        if (!param->write.is_prep){
            UHOS_LOGI( "GATT_WRITE_EVT, value len %d, value :", param->write.len);
            esp_log_buffer_hex(GATTS_TAG, param->write.value, param->write.len);
            if (uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_B_APP_ID].descr_handle == param->write.handle && param->write.len == 2){
                uint16_t descr_value= param->write.value[1]<<8 | param->write.value[0];
                if (descr_value == 0x0001){
                    if (b_property & ESP_GATT_CHAR_PROP_BIT_NOTIFY){
                        UHOS_LOGI( "notify enable");
                        uint8_t notify_data[15];
                        for (int i = 0; i < sizeof(notify_data); ++i)
                        {
                            notify_data[i] = i%0xff;
                        }
                        //the size of notify_data[] need less than MTU size
                        esp_ble_gatts_send_indicate(gatts_if, param->write.conn_id, uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_B_APP_ID].char_handle,
                                                sizeof(notify_data), notify_data, false);
                    }
                }else if (descr_value == 0x0002){
                    if (b_property & ESP_GATT_CHAR_PROP_BIT_INDICATE){
                        UHOS_LOGI( "indicate enable");
                        uint8_t indicate_data[15];
                        for (int i = 0; i < sizeof(indicate_data); ++i)
                        {
                            indicate_data[i] = i%0xff;
                        }
                        //the size of indicate_data[] need less than MTU size
                        esp_ble_gatts_send_indicate(gatts_if, param->write.conn_id, uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_B_APP_ID].char_handle,
                                                sizeof(indicate_data), indicate_data, true);
                    }
                }
                else if (descr_value == 0x0000){
                    UHOS_LOGI( "notify/indicate disable ");
                }else{
                    UHOS_LOGI("unknown value");
                }

            }
        }
        example_write_event_env(gatts_if, &b_prepare_write_env, param);
        break;
    }
    case ESP_GATTS_EXEC_WRITE_EVT:
        UHOS_LOGI("ESP_GATTS_EXEC_WRITE_EVT");
        esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, ESP_GATT_OK, NULL);
        example_exec_write_event_env(&b_prepare_write_env, param);
        break;
    case ESP_GATTS_MTU_EVT:
        UHOS_LOGI( "ESP_GATTS_MTU_EVT, MTU %d", param->mtu.mtu);
        break;
    case ESP_GATTS_UNREG_EVT:
        break;
    case ESP_GATTS_CREATE_EVT:
        UHOS_LOGI( "CREATE_SERVICE_EVT, status %d,  service_handle %d\n", param->create.status, param->create.service_handle);
        uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_B_APP_ID].service_handle = param->create.service_handle;
        uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_B_APP_ID].char_uuid.len = ESP_UUID_LEN_16;
        uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_B_APP_ID].char_uuid.uuid.uuid16 = GATTS_CHAR_UUID_TEST_B;

        esp_ble_gatts_start_service(uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_B_APP_ID].service_handle);
        b_property = ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_WRITE | ESP_GATT_CHAR_PROP_BIT_NOTIFY;
        esp_err_t add_char_ret =esp_ble_gatts_add_char( uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_B_APP_ID].service_handle, &uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_B_APP_ID].char_uuid,
                                                        ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
                                                        b_property,
                                                        NULL, NULL);
        if (add_char_ret){
            UHOS_LOGI("add char failed, error code =%x",add_char_ret);
        }
        break;
    case ESP_GATTS_ADD_INCL_SRVC_EVT:
        break;
    case ESP_GATTS_ADD_CHAR_EVT:
        UHOS_LOGI( "ADD_CHAR_EVT, status %d,  attr_handle %d, service_handle %d\n",
                 param->add_char.status, param->add_char.attr_handle, param->add_char.service_handle);

        uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_B_APP_ID].char_handle = param->add_char.attr_handle;
        uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_B_APP_ID].descr_uuid.len = ESP_UUID_LEN_16;
        uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_B_APP_ID].descr_uuid.uuid.uuid16 = ESP_GATT_UUID_CHAR_CLIENT_CONFIG;
        esp_ble_gatts_add_char_descr(uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_B_APP_ID].service_handle, &uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_B_APP_ID].descr_uuid,
                                     ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
                                     NULL, NULL);
        break;
    case ESP_GATTS_ADD_CHAR_DESCR_EVT:
        uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_B_APP_ID].descr_handle = param->add_char_descr.attr_handle;
        UHOS_LOGI( "ADD_DESCR_EVT, status %d, attr_handle %d, service_handle %d\n",
                 param->add_char_descr.status, param->add_char_descr.attr_handle, param->add_char_descr.service_handle);
        break;
    case ESP_GATTS_DELETE_EVT:
        break;
    case ESP_GATTS_START_EVT:
        UHOS_LOGI( "SERVICE_START_EVT, status %d, service_handle %d\n",
                 param->start.status, param->start.service_handle);
        break;
    case ESP_GATTS_STOP_EVT:
        break;
    case ESP_GATTS_CONNECT_EVT:
        UHOS_LOGI( "CONNECT_EVT, conn_id %d, remote %02x:%02x:%02x:%02x:%02x:%02x:",
                 param->connect.conn_id,
                 param->connect.remote_bda[0], param->connect.remote_bda[1], param->connect.remote_bda[2],
                 param->connect.remote_bda[3], param->connect.remote_bda[4], param->connect.remote_bda[5]);
        uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_B_APP_ID].conn_id = param->connect.conn_id;
        break;
    case ESP_GATTS_CONF_EVT:
        UHOS_LOGI( "ESP_GATTS_CONF_EVT status %d attr_handle %d", param->conf.status, param->conf.handle);
        if (param->conf.status != ESP_GATT_OK){
            esp_log_buffer_hex("GATTS_TAG", param->conf.value, param->conf.len);
        }
    break;
    case ESP_GATTS_DISCONNECT_EVT:
    case ESP_GATTS_OPEN_EVT:
    case ESP_GATTS_CANCEL_OPEN_EVT:
    case ESP_GATTS_CLOSE_EVT:
    case ESP_GATTS_LISTEN_EVT:
    case ESP_GATTS_CONGEST_EVT:
    default:
        break;
    }
}

static void uhos_ble_gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{
    /* If event is register event, store the gatts_if for each profile */
    if (event == ESP_GATTS_REG_EVT) {
        if (param->reg.status == ESP_GATT_OK) {
            uhos_ble_profile_tab[param->reg.app_id].gatts_if = gatts_if;
        } else {
            UHOS_LOGI("Reg app failed, app_id %04x, status %d\n",
                    param->reg.app_id,
                    param->reg.status);
            return;
        }
    }

    /* If the gatts_if equal to profile A, call profile A cb handler,
     * so here call each profile's callback */
    do {
        int idx;
        for (idx = 0; idx < UHOS_BLE_HAL_PROFILE_NUM; idx++) {
            if (gatts_if == ESP_GATT_IF_NONE || /* ESP_GATT_IF_NONE, not specify a certain gatt_if, need to call every profile cb function */
                    gatts_if == uhos_ble_profile_tab[idx].gatts_if) {
                if (uhos_ble_profile_tab[idx].gatts_cb) {
                    uhos_ble_profile_tab[idx].gatts_cb(event, gatts_if, param);
                }
            }
        }
    } while (0);
}

/**************************************************************************************************/
/*                                          全局函数实现                                          */
/**************************************************************************************************/
/**
 * @brief       GATT层Server端初始化
 */
void uhos_ble_pal_gatts_init(void)
{
    esp_err_t ret;

    ret = esp_ble_gatts_register_callback(uhos_ble_gatts_event_handler);
    if (ret){
         UHOS_LOGE("esp_ble_gatts_register_callback failed, error code = %x ", ret);
         return UHOS_BLE_ERROR;
    }

    ret = esp_ble_gatts_app_register(UHOS_BLE_HAL_PROFILE_A_APP_ID);
    if (ret){
         UHOS_LOGE("esp_ble_gatts_app_register failed, error code = %x ", ret);
         return UHOS_BLE_ERROR;
    }
    return UHOS_BLE_SUCCESS;
}

/**
 * @brief       GATT层Server端反初始化，主要时回收相关的资源
 */
void uhos_ble_pal_gatts_deinit(void)
{
    return;
}

/**
 * @brief       注册GATT层Server端用户回调函数
 * @param[in]   cb 用户回调函数
 * @return      uhos_ble_status_t 执行结果
 */
uhos_ble_status_t uhos_ble_gatts_callback_register(uhos_ble_gatts_cb_t cb)
{
    g_uhos_ble_pal_gatts_user_cb = cb;

    return UHOS_BLE_SUCCESS;
}

/**
 * @brief       注册GATT层Server端用户侧的free函数，用来free用户侧申请的堆内存
 *
 * @param[in]   cb 用户注册的free函数，用来free用户侧malloc的数据
 * @return      uhos_ble_status_t 执行结果
 */
 uhos_ble_status_t uhos_ble_gatts_register_destroy(destroy_func_t destroy)
 {
    return UHOS_BLE_SUCCESS;
 }

/**
 * @brief       设置GATT层Server端的服务框架
 * @param[in]   service_database 服务数据集合
 * @return      uhos_ble_status_t
 */
uhos_ble_status_t uhos_ble_gatts_service_set(uhos_ble_gatts_db_t *service_database)
{
    uhos_ble_gatts_srv_db_t *p_srv_db;
    uhos_u8                  srv_num = 0;
    uhos_ble_status_t        status;

    srv_num = service_database->srv_num;

    if (srv_num > APP_MAX_SERVICE_NUM || srv_num == 0)
    {
        UHOS_LOGW("srv num limited");
        return UHOS_BLE_ERROR;
    }

    for (int i = 0; i < srv_num; i++)
    {

        p_srv_db = &service_database->p_srv_db[i];
        status   = uhos_ble_pal_gatts_add_service(p_srv_db);

        if (UHOS_BLE_SUCCESS != status)
        {
            UHOS_LOGW("ble add service failed, %d", i);
            continue;
        }
    }

    return UHOS_BLE_SUCCESS;
}

/**
 * @brief       向指定的特性发送notify和indicate数据
 * @param[in]   conn_handle         连接句柄
 * @param[in]   srv_handle          服务句柄
 * @param[in]   char_value_handle   特性值句柄
 * @param[in]   offset              偏移量
 * @param[in]   p_value             发送数据
 * @param[in]   len                 发送数据的字节数
 * @return      uhos_ble_status_t
 */
uhos_ble_status_t uhos_ble_gatts_notify_or_indicate(
    uhos_u16 conn_handle,
    uhos_u16 srv_handle,
    uhos_u16 char_value_handle,
    uhos_u8  offset,
    uhos_u8 *p_value,
    uhos_u16 len)
{
    uhos_u16 conidx = uhos_ble_pal_conn_id_switch(conn_handle, UHOS_BLE_CONNECT_SWITCH_MODE_DEC);
    uhos_u16 handler = 0;
    uhos_u16 att_offset = 0;
    int      start_handler = srv_handle;

    handler    = start_handler >> 8;
    att_offset = (char_value_handle & 0xFF);

    if (offset == 0)
    {
        app_ble_gatt_data_send_notify((uhos_u8)conidx, handler, att_offset, len, p_value);
    }
    else
    {
        app_ble_gatt_data_send_indicate((uhos_u8)conidx, handler, att_offset, len, p_value);
    }

    return UHOS_BLE_SUCCESS;
}

/**
 * @brief       设置默认的MTU
 * @param[in]   mtu MTU值
 * @return      uhos_ble_status_t
 */
uhos_ble_status_t uhos_ble_gatts_mtu_default_set(uhos_u16 mtu)
{
   esp_err_t ret;

    ret = esp_ble_gatt_set_local_mtu(mtu);
    if (ret){
         UHOS_LOGE("esp_ble_gatt_set_local_mtu failed, error code = %x ", ret);
         return UHOS_BLE_ERROR;
    }

    return UHOS_BLE_SUCCESS;
}

/**
 * @brief       获取当前的MTU值
 * @param[in]   conn_handle 连接ID
 * @param[out]  mtu_size    当前的MTU值
 * @return      uhos_ble_status_t
 */
uhos_ble_status_t uhos_ble_gatts_mtu_get(uhos_u16 conn_handle, uhos_u16 *mtu_size)
{
    // uhos_u16 conidx = uhos_ble_pal_conn_id_switch(conn_handle, UHOS_BLE_CONNECT_SWITCH_MODE_DEC);
    // *mtu_size = app_ble_get_con_mtu((uhos_u8)conidx);

    return UHOS_BLE_SUCCESS;
}

