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

#define APP_MAX_SERVICE_NUM                 10

#define UHOS_BLE_HAL_PROFILE_NUM 2
#define UHOS_BLE_HAL_PROFILE_A_APP_ID 0
#define UHOS_BLE_HAL_PROFILE_B_APP_ID 1

#define GATTS_SERVICE_UUID_TEST_A   0x00FF
#define GATTS_CHAR_UUID_TEST_A      0xFF01
#define GATTS_DESCR_UUID_TEST_A     0x3333
#define GATTS_NUM_HANDLE_TEST_A     4

static esp_attr_value_t heart_rate_attr = {
    .attr_max_len = 2,
    .attr_len     = sizeof(heart_rate_val),
    .attr_value   = heart_rate_val,
};

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

static uhos_ble_gatts_profile_inst_t uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_NUM] = {
    [UHOS_BLE_HAL_PROFILE_A_APP_ID] = {
        .gatts_cb = gatts_profile_a_event_handler,
        .gatts_if = ESP_GATT_IF_NONE,       /* Not get the gatt_if, so initial is ESP_GATT_IF_NONE */
    },
    [UHOS_BLE_HAL_PROFILE_B_APP_ID] = {
        .gatts_cb = NULL,                   /* This demo does not implement, similar as profile A */
        .gatts_if = ESP_GATT_IF_NONE,       /* Not get the gatt_if, so initial is ESP_GATT_IF_NONE */
    },
};

/**************************************************************************************************/
/*                                        全局(静态)变量                                          */
/**************************************************************************************************/
uhos_ble_gatts_cb_t g_uhos_ble_pal_gatts_user_cb = UHOS_NULL;   //<! GATT层用户设置的server端回调函数


/**************************************************************************************************/
/*                                          内部函数原型                                          */
/**************************************************************************************************/


/**************************************************************************************************/
/*                                          内部函数实现                                          */
/**************************************************************************************************/
/**
 * @brief       向协议栈添加一个服务
 * @param[in]   p_srv_db    蓝牙服务描述数据
 * @return      uhos_ble_status_t
 */
static uhos_ble_status_t uhos_ble_pal_gatts_add_service(uhos_ble_gatts_srv_db_t *p_srv_db, uhos_u8 id)
{
    // uhos_ble_profile_tab[id].gatts_cb = NULL;
    // uhos_ble_profile_tab[id].gatts_if = ESP_GATT_IF_NONE;

    uhos_ble_profile_tab[id].app_id = id;

    // uhos_ble_profile_tab[id].conn_id = id;
    // uhos_ble_profile_tab[id].service_handle = id;

    if(p_srv_db->srv_type == UHOS_BLE_PRIMARY_SERVICE)
    {
        uhos_ble_profile_tab[id].service_id.is_primary = UHOS_TRUE;
    }
    else if(p_srv_db->srv_type == UHOS_BLE_SECONDARY_SERVICE)
    {
        uhos_ble_profile_tab[id].service_id.is_primary = UHOS_FALSE;
    }

    uhos_ble_profile_tab[id].service_id.id.inst_id = 0x00;

    if(p_srv_db->srv_uuid.type == UHOS_BLE_UUID_TYPE_16)
    {
        uhos_ble_profile_tab[id].service_id.id.uuid.len = ESP_UUID_LEN_16;
        uhos_ble_profile_tab[id].service_id.id.uuid.uuid.uuid16 = p_srv_db->srv_uuid.uuid16;
    }
    else if(p_srv_db->srv_uuid.type == UHOS_BLE_UUID_TYPE_128)
    {
        uhos_ble_profile_tab[id].service_id.id.uuid.len = ESP_UUID_LEN_128;
        uhos_libc_memcpy(uhos_ble_profile_tab[id].service_id.id.uuid.uuid.uuid16
                        ,p_srv_db->srv_uuid.uuid128
                        ,ESP_UUID_LEN_128);
    }

    // uhos_ble_profile_tab[id].char_handle = id;

    if(p_srv_db->p_char_db->char_uuid.type == UHOS_BLE_UUID_TYPE_16)
    {
        uhos_ble_profile_tab[id].char_uuid.len = ESP_UUID_LEN_16;
        uhos_ble_profile_tab[id].char_uuid.uuid.uuid16 = p_srv_db->p_char_db->char_uuid.uuid16;
    }
    else if(p_srv_db->p_char_db->char_uuid.type == UHOS_BLE_UUID_TYPE_128)
    {
        uhos_ble_profile_tab[id].char_uuid.len = ESP_UUID_LEN_128;
        uhos_libc_memcpy(uhos_ble_profile_tab[id].char_uuid.uuid.uuid16
                        ,p_srv_db->p_char_db->char_uuid.uuid128
                        ,ESP_UUID_LEN_128);
    }

    uhos_ble_profile_tab[id].perm = ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE;

    if(p_srv_db->p_char_db->char_property & UHOS_BLE_CHAR_PROP_BROADCAST)
    {
        uhos_ble_profile_tab[id].property |= ESP_GATT_CHAR_PROP_BIT_BROADCAST; 
    }
    if(p_srv_db->p_char_db->char_property & UHOS_BLE_CHAR_PROP_READ)
    {
        uhos_ble_profile_tab[id].property |= ESP_GATT_CHAR_PROP_BIT_READ; 
    }
    if(p_srv_db->p_char_db->char_property & UHOS_BLE_CHAR_PROP_WRITE_WITHOUT_RESP)
    {
        uhos_ble_profile_tab[id].property |= ESP_GATT_CHAR_PROP_BIT_WRITE_NR; 
    }
    if(p_srv_db->p_char_db->char_property & UHOS_BLE_CHAR_PROP_WRITE)
    {
        uhos_ble_profile_tab[id].property |= ESP_GATT_CHAR_PROP_BIT_WRITE; 
    }
    if(p_srv_db->p_char_db->char_property & UHOS_BLE_CHAR_PROP_NOTIFY)
    {
        uhos_ble_profile_tab[id].property |= ESP_GATT_CHAR_PROP_BIT_NOTIFY; 
    }
    if(p_srv_db->p_char_db->char_property & UHOS_BLE_CHAR_PROP_INDICATE)
    {
        uhos_ble_profile_tab[id].property |= ESP_GATT_CHAR_PROP_BIT_INDICATE; 
    }
    if(p_srv_db->p_char_db->char_property & UHOS_BLE_CHAR_PROP_AUTH_SIGNED_WRITE)
    {
        uhos_ble_profile_tab[id].property |= ESP_GATT_CHAR_PROP_BIT_AUTH; 
    }
    if(p_srv_db->p_char_db->char_property & UHOS_BLE_CHAR_PROP_EXTENDED_PROPERTIES)
    {
        uhos_ble_profile_tab[id].property |= ESP_GATT_CHAR_PROP_BIT_EXT_PROP; 
    }

    // uhos_ble_profile_tab[id].descr_handle = id;
    // uhos_ble_profile_tab[id].descr_uuid.len = id;
    // uhos_ble_profile_tab[id].descr_uuid.uuid.uuid16 = id;
}

static void uhos_ble_gatts_profile_a_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) 
{
    esp_err_t ret;

    switch (event) 
    {
    case ESP_GATTS_REG_EVT:
        UHOS_LOGI("REGISTER_APP_EVT, status %d, app_id %d\n", param->reg.status, param->reg.app_id);

        esp_ble_gatts_create_service(gatts_if, &uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_A_APP_ID].service_id, GATTS_NUM_HANDLE_TEST_A);
        break;
    case ESP_GATTS_CREATE_EVT:
        //service has been created, now add characteristic declaration
        ret = esp_ble_gatts_add_char(gl_profile_tab[HEART_PROFILE_APP_ID].service_handle, &gl_profile_tab[HEART_PROFILE_APP_ID].char_uuid,
                            ESP_GATT_PERM_READ,
                            ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_INDICATE,
                            &heart_rate_attr, NULL);
        if (ret) {
            ESP_LOGE(GATTS_TAG, "add char failed, error code = %x", ret);
        }
        break;
    case ESP_GATTS_ADD_CHAR_EVT: {
        uint16_t length = 0;
        const uint8_t *prf_char;

        UHOS_LOGI( "ADD_CHAR_EVT, status %d,  attr_handle %d, service_handle %d\n",
                param->add_char.status, param->add_char.attr_handle, param->add_char.service_handle);

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
        uhos_ble_profile_tab[UHOS_BLE_HAL_PROFILE_A_APP_ID].conn_id = param->connect.conn_id;
        //start sent the update connection parameters to the peer device.
        esp_ble_gap_update_conn_params(&conn_params);
        break;
    }
    case ESP_GATTS_DISCONNECT_EVT:
        UHOS_LOGI("ESP_GATTS_DISCONNECT_EVT, disconnect reason 0x%x", param->disconnect.reason);
        break;
    case ESP_GATTS_CONF_EVT:
        UHOS_LOGI("ESP_GATTS_CONF_EVT, status %d attr_handle %d", param->conf.status, param->conf.handle);
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
        status   = uhos_ble_pal_gatts_add_service(p_srv_db, i);

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
    // uhos_u16 conidx = uhos_ble_pal_conn_id_switch(conn_handle, UHOS_BLE_CONNECT_SWITCH_MODE_DEC);
    // uhos_u16 handler = 0;
    // uhos_u16 att_offset = 0;
    // int      start_handler = srv_handle;

    // handler    = start_handler >> 8;
    // att_offset = (char_value_handle & 0xFF);

    if (offset == 0)
    {
        //app_ble_gatt_data_send_notify((uhos_u8)conidx, handler, att_offset, len, p_value);
        esp_ble_gatts_send_indicate(gatts_if, param->write.conn_id, gl_profile_tab[HEART_PROFILE_APP_ID].char_handle,
                                        len, p_value, false);
    }
    else
    {
        //app_ble_gatt_data_send_indicate((uhos_u8)conidx, handler, att_offset, len, p_value);
        esp_ble_gatts_send_indicate(gatts_if, param->write.conn_id, gl_profile_tab[HEART_PROFILE_APP_ID].char_handle,
                                        len, p_value, true);
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
    uhos_u16 conidx = uhos_ble_pal_conn_id_switch(conn_handle, UHOS_BLE_CONNECT_SWITCH_MODE_DEC);
    *mtu_size = app_ble_get_con_mtu((uhos_u8)conidx);

    return UHOS_BLE_SUCCESS;
}

