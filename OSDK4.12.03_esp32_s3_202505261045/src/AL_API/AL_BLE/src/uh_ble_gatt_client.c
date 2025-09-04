/**
 * @copyright Copyright (c) 2021, Haier.Co, Ltd.
 * @file uh_ble_gatt_client.c
 * @author maaiguo (maaiguo@haier.com)
 * @brief 基于HR3010硬件模块的BLE GATT层client端相关功能函数实现
 * @date 2021-10-26
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>version <th>Author  <th>Description
 * <tr><td>2021-10-26   <td>1.0     <td>maaiguo <td>
 * </table>
 */

#define LOG_TAG "ble-c"

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
/*                                          内部函数原型                                          */
/**************************************************************************************************/
void uhos_ble_pal_gattc_stack_cb(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param);

/**************************************************************************************************/
/*                                          外部引用声明                                          */
/**************************************************************************************************/


/**************************************************************************************************/
/*                                           内部宏定义                                           */
/**************************************************************************************************/
#define UHOS_BLE_GATTC_MAX_CON_IDX 5

#define UHOS_BLE_GATTC_PROFILE_NUM 1
#define UHOS_BLE_GATTC_PROFILE_A_APP_ID 0

/**************************************************************************************************/
/*                                        内部数据类型定义                                        */
/**************************************************************************************************/
struct gattc_profile_inst {
    esp_gattc_cb_t gattc_cb;
    uint16_t gattc_if;
    uint16_t app_id;
    uint16_t conn_id;
    uint16_t service_start_handle;
    uint16_t service_end_handle;
    uint16_t char_handle;
    esp_bd_addr_t remote_bda;
};

/**************************************************************************************************/
/*                                        全局(静态)变量                                          */
/**************************************************************************************************/
static uhos_u8            g_uhos_ble_pal_gattc_op[UHOS_BLE_GATTC_MAX_CON_IDX] = {0};
uhos_ble_gattc_callback_t g_uhos_ble_pal_gattc_user_cb = UHOS_NULL; //<! GATT层用户设置的Client端回调函数

static uhos_u16 g_uhos_ble_gattc_if[UHOS_BLE_GATTC_MAX_CON_IDX] = {0,};
static uhos_u16 g_uhos_ble_gattc_conn_id[UHOS_BLE_GATTC_MAX_CON_IDX] = {0,};

/* One gatt-based profile one app_id and one gattc_if, this array will store the gattc_if returned by ESP_GATTS_REG_EVT */
static struct gattc_profile_inst gl_profile_tab[UHOS_BLE_GATTC_PROFILE_NUM] = {
    [UHOS_BLE_GATTC_PROFILE_A_APP_ID] = {
        .gattc_cb = uhos_ble_pal_gattc_stack_cb,
        .gattc_if = ESP_GATT_IF_NONE,       /* Not get the gatt_if, so initial is ESP_GATT_IF_NONE */
    },
};

/**************************************************************************************************/
/*                                          内部函数实现                                          */
/**************************************************************************************************/
/**
 * @brief       协议栈gatt层的回调函数实现
 * @param[in]   event   回调事件
 * @param[in]   param   回调参数
 * @return      无
 */
static void uhos_ble_pal_gattc_stack_cb(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param)
{
    esp_ble_gattc_cb_param_t *p_data = (esp_ble_gattc_cb_param_t *)param;

    // 输入参数检查
    if (UHOS_NULL == param)
    {
        UHOS_LOGE("input param is null");
        return;
    }

    // 回调函数检查
    if (UHOS_NULL == g_uhos_ble_pal_gattc_user_cb)
    {
        UHOS_LOGE("no user gattc cb");
        return;
    }

    // 依据事件类型进行操作
    switch (event)
    {
        case ESP_GATTC_REG_EVT:  //在 ESP_GATTC_REG_EVT 中保存 gattc_if, 标志着 GATT 客户端应用程序的初始化完成
            {
                UHOS_LOGI("ESP_GATTC_REG_EVT");
            }
            break;

        case ESP_GATTC_CONNECT_EVT:
            {
                UHOS_LOGI("ESP_GATTC_CONNECT_EVT, conn_id %d, if %d", p_data->connect.conn_id, gattc_if);
                gl_profile_tab[UHOS_BLE_GATTC_PROFILE_A_APP_ID].conn_id = p_data->connect.conn_id;

                g_uhos_ble_gattc_if[UHOS_BLE_GATTC_PROFILE_A_APP_ID] = gattc_if;
                g_uhos_ble_gattc_conn_id[UHOS_BLE_GATTC_PROFILE_A_APP_ID] = p_data->connect.conn_id;
            }
            break;
        
        case ESP_GATTC_OPEN_EVT:
            {
                if (param->open.status != ESP_GATT_OK)
                {
                    UHOS_LOGE("open failed, status %d", p_data->open.status);
                    break;
                }
                UHOS_LOGI("ESP_GATTC_OPEN_EVT");
            }
            break;

        case ESP_GATTC_DIS_SRVC_CMPL_EVT:
            {
                UHOS_LOGI("ESP_GATTC_DIS_SRVC_CMPL_EVT");
            }
            break;

        case ESP_GATTC_CFG_MTU_EVT:
            {
                UHOS_LOGI("ESP_GATTC_CFG_MTU_EVT");
            }
            break;

        case ESP_GATTC_SEARCH_RES_EVT:
            {
                UHOS_LOGI("ESP_GATTC_SEARCH_RES_EVT");
            }
            break;

        case ESP_GATTC_SEARCH_CMPL_EVT:
            {
                UHOS_LOGI("ESP_GATTC_SEARCH_CMPL_EVT");
            }
            break;

        case ESP_GATTC_REG_FOR_NOTIFY_EVT:
            {
                UHOS_LOGI("ESP_GATTC_REG_FOR_NOTIFY_EVT");
            }
            break;

        case ESP_GATTC_NOTIFY_EVT:
            {
                UHOS_LOGI("ESP_GATTC_NOTIFY_EVT");
            }
            break;

        case ESP_GATTC_WRITE_DESCR_EVT:
            {
                UHOS_LOGI("ESP_GATTC_WRITE_DESCR_EVT");
            }
            break;

        case ESP_GATTC_SRVC_CHG_EVT:
            {
                UHOS_LOGI("ESP_GATTC_SRVC_CHG_EVT");
            }
            break;

        case ESP_GATTC_WRITE_CHAR_EVT:
            {
                UHOS_LOGI("ESP_GATTC_WRITE_CHAR_EVT");
            }
            break;

        case ESP_GATTC_DISCONNECT_EVT:
            {
                UHOS_LOGI("ESP_GATTC_DISCONNECT_EVT");
            }
            break;
        default:
            break;
    }

    return;
}

static void uhos_ble_pal_gattc_cb(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param)
{
    /* If event is register event, store the gattc_if for each profile */
    if (event == ESP_GATTC_REG_EVT) 
    {
        if (param->reg.status == ESP_GATT_OK) 
        {
            gl_profile_tab[param->reg.app_id].gattc_if = gattc_if;
        } 
        else 
        {
            UHOS_LOGI("reg app failed, app_id %04x, status %d", param->reg.app_id, param->reg.status);
            return;
        }
    }

    /* If the gattc_if equal to profile A, call profile A cb handler,
     * so here call each profile's callback */
    do {
        int idx;
        for (idx = 0; idx < UHOS_BLE_GATTC_PROFILE_NUM; idx++) {
            if (gattc_if == ESP_GATT_IF_NONE || /* ESP_GATT_IF_NONE, not specify a certain gatt_if, need to call every profile cb function */
                    gattc_if == gl_profile_tab[idx].gattc_if) {
                if (gl_profile_tab[idx].gattc_cb) {
                    gl_profile_tab[idx].gattc_cb(event, gattc_if, param);
                }
            }
        }
    } while (0);
}
/**************************************************************************************************/
/*                                          全局函数实现                                          */
/**************************************************************************************************/
/**
 * @brief       GATT层Client端初始化
 */
uhos_ble_status_t uhos_ble_gattc_init(void)
{
    esp_err_t ret;

    ret = esp_ble_gattc_register_callback(uhos_ble_pal_gattc_cb);
    if (ret) {
        UHOS_LOGE("gattc register error, error code = %x", ret);
        return UHOS_BLE_ERROR;
    }

    ret = esp_ble_gattc_app_register(UHOS_BLE_GATTC_PROFILE_A_APP_ID);
    if (ret) {
        UHOS_LOGE("gattc app register error, error code = %x", ret);
        return UHOS_BLE_ERROR;
    }

    return UHOS_BLE_SUCCESS;
}

/**
 * @brief       注册GATT层client端用户回调函数
 * @param[in]   cb  用户回调函数
 * @return      uplus_ble_status_t
 */
uhos_ble_status_t uhos_ble_gattc_callback_register(uhos_ble_gattc_callback_t cb)
{
    g_uhos_ble_pal_gattc_user_cb = cb;

    return UHOS_BLE_SUCCESS;
}

/**
 * @brief       首要服务发现
 * @param[in]   conn_handle     连接句柄
 * @param[in]   handle_range    服务ID的范围
 * @param[in]   p_srv_uuid      UUID的描述
 * @return      uhos_ble_status_t
 */
uhos_ble_status_t uhos_ble_gattc_primary_service_discover_all(uhos_u16 conn_handle, void    *req)
{
    esp_err_t ret;
    uhos_u8  conidx = 0;

    // 检查连接状态
    conidx = uhos_ble_pal_conn_id_switch(conn_handle, UHOS_BLE_CONNECT_SWITCH_MODE_DEC);

    if (!app_get_connect_status(conidx))
    {
        UHOS_LOGE("conn dis 0x%x", conidx);
        return UHOS_BLE_ERROR;
    }

    // 调用协议栈接口
    g_uhos_ble_pal_gattc_op[conidx] = UHOS_BLE_GATTC_EVT_PRIMARY_SERVICE_DISCOVER_RESP;

    ret = esp_ble_gattc_search_service(esp_gatt_if_t gattc_if, uint16_t conn_id, esp_bt_uuid_t *filter_uuid);

    if (ret)
    {
        UHOS_LOGE("primary service discover all fail 0x%4x", ret);
        return UHOS_BLE_ERROR;
    }

    return UHOS_BLE_SUCCESS;
}

/**
 * @brief       通过UUID启动首要服务发现
 * @param[in]   conn_handle     连接句柄
 * @param[in]   handle_range    服务ID的范围
 * @param[in]   p_srv_uuid      UUID的描述
 * @return      uhos_ble_status_t
 */
uhos_ble_status_t uhos_ble_gattc_primary_service_discover_by_uuid(
    uhos_u16                 conn_handle,
    uhos_ble_handle_range_t *handle_range,
    uhos_ble_uuid_t         *p_srv_uuid)
{
    uhos_u8  conidx   = 0;
    uhos_u8  uuid_len = 0;
    esp_err_t ret;

    // 检查连接状态
    conidx = uhos_ble_pal_conn_id_switch(conn_handle, UHOS_BLE_CONNECT_SWITCH_MODE_DEC);

    if (!app_get_connect_status(conidx))
    {
        UHOS_LOGE("conn dis 0x%x", conidx);
        return UHOS_BLE_ERROR;
    }

    // 获取UUID的长度
    if (UHOS_BLE_UUID_TYPE_16 == p_srv_uuid->type)
    {
        uuid_len = UPLUG_BLE_UUID_16_LEN;
    }
    else
    {
        uuid_len = UPLUG_BLE_UUID_128_LEN;
    }

    // 通过协议栈接口发现服务
    g_uhos_ble_pal_gattc_op[conidx] = UHOS_BLE_GATTC_EVT_PRIMARY_SERVICE_DISCOVER_RESP;

    ret = esp_ble_gattc_search_service(esp_gatt_if_t gattc_if, uint16_t conn_id, esp_bt_uuid_t *filter_uuid);//sonata_ble_gatt_disc_all_svc(conidx);

    if (ret)
    {
        UHOS_LOGE("primary service discover by uuid fail 0x%4x",ret);
        return UHOS_BLE_ERROR;
    }

    return UHOS_BLE_SUCCESS;
}

/**
 * @brief       启动服务特征发现
 * @param[in]   conn_handle     连接句柄
 * @param[in]   handle_range    句柄范围
 * @return      uhos_ble_status_t
 */
uhos_ble_status_t uhos_ble_gattc_char_discover_of_service(
    uhos_u16                 conn_handle,
    uhos_ble_handle_range_t *char_handle_range)
{
    uhos_u8  conidx = 0;
    esp_err_t ret;

    // 检查连接状态
    conidx = uhos_ble_pal_conn_id_switch(conn_handle, UHOS_BLE_CONNECT_SWITCH_MODE_DEC);

    if (!app_get_connect_status(conidx))
    {
        UHOS_LOGE("conn dis 0x%x", conidx);
        return UHOS_BLE_ERROR;
    }

    g_uhos_ble_pal_gattc_op[conidx] = UHOS_BLE_GATTC_EVT_CHAR_DISCOVER_RESP;
    ret = esp_ble_gattc_get_all_char(esp_gatt_if_t gattc_if, uint16_t conn_id, uint16_t start_handle, uint16_t end_handle);
    if (ret)
    {
        UHOS_LOGE("char discover of service fail 0x%4x",ret);
        return UHOS_BLE_ERROR;
    }

    return UHOS_BLE_SUCCESS;
}


/**
 * @brief       通过UUID启动服务特征发现
 * @param[in]   conn_handle     连接句柄
 * @param[in]   handle_range    句柄范围
 * @param[in]   p_char_uuid     特征UUID
 * @return      uhos_ble_status_t
 */
uhos_ble_status_t uhos_ble_gattc_char_discover_by_uuid(
    uhos_u16                 conn_handle,
    uhos_ble_handle_range_t *handle_range,
    uhos_ble_uuid_t *        p_char_uuid)
{
    uhos_u8  conidx   = 0;
    uhos_u8  uuid_len = 0;
    esp_err_t ret;

    // 检查连接状态
    conidx = uhos_ble_pal_conn_id_switch(conn_handle, UHOS_BLE_CONNECT_SWITCH_MODE_DEC);

    if(!app_get_connect_status(conidx))
    {
        UHOS_LOGE("conn dis 0x%x", conidx);
        return UHOS_BLE_ERROR;
    }

    // 获取UUID的长度
    if (UHOS_BLE_UUID_TYPE_16 == p_char_uuid->type)
    {
        uuid_len = UPLUG_BLE_UUID_16_LEN;
    }
    else
    {
        uuid_len = UPLUG_BLE_UUID_128_LEN;
    }

    // 调用协议栈接口发现特征
    g_uhos_ble_pal_gattc_op[conidx] = UHOS_BLE_GATTC_EVT_CHAR_DISCOVER_BY_UUID_RESP;

    ret = esp_ble_gattc_get_char_by_uuid(esp_gatt_if_t gattc_if, uint16_t conn_id, uint16_t start_handle, uint16_t end_handle, esp_bt_uuid_t char_uuid);

    if (ret)
    {
        UHOS_LOGE("gattc char discover by uuid fail 0x%4x", ret);
        return UHOS_BLE_ERROR;
    }

    return UHOS_BLE_SUCCESS;

}

/**
 * @brief       启动特征描述符发现
 * @param[in]   conn_handle     连接ID
 * @param[in]   handle_range    句柄范围
 * @return      uhos_ble_status_t
 */
uhos_ble_status_t uhos_ble_gattc_clt_cfg_descriptor_discover(
    uhos_u16                 conn_handle,
    uhos_ble_handle_range_t *handle_range)
{
    uhos_u8  conidx = 0;
    esp_err_t ret;

    // 检查连接状态
    conidx = uhos_ble_pal_conn_id_switch(conn_handle, UHOS_BLE_CONNECT_SWITCH_MODE_DEC);

    if(!app_get_connect_status(conidx))
    {
        UHOS_LOGE("conn dis 0x%x", conidx);
        return UHOS_BLE_ERROR;
    }

    // 调用协议栈接口
    g_uhos_ble_pal_gattc_op[conidx] = UHOS_BLE_GATTC_EVT_CHAR_DESC_DISCOVER_RESP;

    ret = esp_ble_gattc_get_all_descr(esp_gatt_if_t gattc_if, uint16_t conn_id, uint16_t char_handle);

    if (ret)
    {
        UHOS_LOGE("char descriptor discover all fail 0x%4x", ret);
        return UHOS_BLE_ERROR;
    }

    return UHOS_BLE_SUCCESS;
}

/**
 * @brief       读取特征值
 * @param[in]   conn_handle     连接ID
 * @param[in]   handle_range    句柄范围
 * @return      uhos_ble_status_t
 */
uhos_ble_status_t uhos_ble_gattc_read_char_value(
    uhos_u16 conn_handle,
    uhos_u16 char_value_handle)
{
    uhos_u8  conidx = 0;
    esp_err_t ret;

    // 连接状态检查
    conidx = uhos_ble_pal_conn_id_switch(conn_handle, UHOS_BLE_CONNECT_SWITCH_MODE_DEC);

    if (!app_get_connect_status(conidx))
    {
        UHOS_LOGE("conn dis 0x%x", conidx);
        return UHOS_BLE_ERROR;
    }

    // 调用协议栈接口
    g_uhos_ble_pal_gattc_op[conidx] = UHOS_BLE_GATTC_EVT_READ_CHAR_VALUE_RESP;

    ret = esp_ble_gattc_read_char(esp_gatt_if_t gattc_if, uint16_t conn_id, uint16_t char_handle, esp_gatt_auth_req_t auth_req);

    if (ret)
    {
        UHOS_LOGE("read char value fail 0x%4x", ret);

        return UHOS_BLE_ERROR;
    }

    return UHOS_BLE_SUCCESS;
}

/**
 * @brief       读取指定UUID特征的特征值
 * @param[in]   conn_handle     连接ID
 * @param[in]   handle_range    句柄范围
 * @param[in]   p_char_uuid     特征UUID
 * @return      uhos_ble_status_t
 */
uhos_ble_status_t uhos_ble_gattc_read_char_value_by_uuid(
    uhos_u16                 conn_handle,
    uhos_ble_handle_range_t *handle_range,
    uhos_ble_uuid_t         *p_char_uuid)
{
    uhos_u8  conidx   = 0;
    uhos_u8  uuid_len = 0;
    esp_err_t ret;

    // 检查连接状态
    conidx = uhos_ble_pal_conn_id_switch(conn_handle, UHOS_BLE_CONNECT_SWITCH_MODE_DEC);

    if (!app_get_connect_status(conidx))
    {
        UHOS_LOGE("conn dis 0x%x", conidx);
        return UHOS_BLE_ERROR;
    }

    // 获取UUID的长度
    if (UHOS_BLE_UUID_TYPE_16 == p_char_uuid->type)
    {
        uuid_len = UPLUG_BLE_UUID_16_LEN;
    }
    else
    {
        uuid_len = UPLUG_BLE_UUID_128_LEN;
    }

    // 调用协议栈接口
    g_uhos_ble_pal_gattc_op[conidx] = UHOS_BLE_GATTC_EVT_READ_USING_UUID_RESP;

    ret = esp_ble_gattc_read_by_type(esp_gatt_if_t gattc_if, uint16_t conn_id, uint16_t start_handle, uint16_t end_handle, esp_bt_uuid_t *uuid, esp_gatt_auth_req_t auth_req);
    if (ret)
    {
        UHOS_LOGE("read char value by uuid fail 0x%4x", ret);
        return UHOS_BLE_ERROR;
    }

    return UHOS_BLE_SUCCESS;
}

/**
 * @brief       GTAA层client写特征值（需要响应）
 * @param[in]   conn_handle 连接句柄
 * @param[in]   handle      特性句柄
 * @param[in]   p_value     写入数据
 * @param[in]   len         写入数据字节数
 * @return      uhos_ble_status_t
 */
uhos_ble_status_t uhos_ble_gattc_write_with_rsp(
    uhos_u16 conn_handle,
    uhos_u16 handle,
    uhos_u8 *p_value,
    uhos_u8  len)
{
    uhos_u8  conidx = 0;
    esp_err_t ret;

    // 检查连接状态
    conidx = uhos_ble_pal_conn_id_switch(conn_handle, UHOS_BLE_CONNECT_SWITCH_MODE_DEC);

    if (!app_get_connect_status(conidx))
    {
        UHOS_LOGE("conn dis 0x%x", conidx);
        return UHOS_BLE_ERROR;
    }

    // 调用协议栈接口
    g_uhos_ble_pal_gattc_op[conidx] = UHOS_BLE_GATTC_EVT_WRITE_RESP;
    ret = esp_ble_gattc_write_char(
        esp_gatt_if_t gattc_if,
        uint16_t conn_id,
        uint16_t handle,
        uint16_t value_len,
        uint8_t *value,
        ESP_GATT_WRITE_TYPE_RSP,
        ESP_GATT_AUTH_REQ_NONE
    );

    if (ret)
    {
        UHOS_LOGE("gattc write with rsp fail 0x%4x", ret);
        // uh_ble_asr_gattc_optimize(0);

        return UHOS_BLE_ERROR;
    }

    return UHOS_BLE_SUCCESS;
}

/**
 * @brief       GTAA层client写特征值（无响应）
 * @param[in]   conn_handle 连接句柄
 * @param[in]   handle      特性句柄
 * @param[in]   p_value     写入数据
 * @param[in]   len         写入数据字节数
 * @return      uhos_ble_status_t
 */
uhos_ble_status_t uhos_ble_gattc_write_without_rsp(
    uhos_u16 conn_handle,
    uhos_u16 char_value_handle,
    uhos_u8* p_value,
    uhos_u16 len)
{
    uhos_u8  conidx = 0;
    esp_err_t ret;

    // 检查连接状态
    conidx = uhos_ble_pal_conn_id_switch(conn_handle, UHOS_BLE_CONNECT_SWITCH_MODE_DEC);

    if (!app_get_connect_status(conidx))
    {
        UHOS_LOGE("conn dis 0x%x", conidx);
        return UHOS_BLE_ERROR;
    }

    // 调用协议栈接口
    ret = esp_ble_gattc_write_char(
        esp_gatt_if_t gattc_if,
        uint16_t conn_id,
        uint16_t handle,
        uint16_t value_len,
        uint8_t *value,
        ESP_GATT_WRITE_TYPE_NO_RSP,
        ESP_GATT_AUTH_REQ_NONE
    );

    if (ret)
    {
        UHOS_LOGE("gattc write with rsp fail 0x%4x", ret);
        return UHOS_BLE_ERROR;
    }

    return UHOS_BLE_SUCCESS;
}

/**
 * @brief       GATT层client端写命令
 * @param[in]   conn_handle 连接句柄
 * @param[in]   handle      特性句柄
 * @param[in]   p_value     命令数据
 * @param[in]   len         命令数据字节数
 * @return      uhos_ble_status_t
 */
uhos_ble_status_t uhos_ble_gattc_write_cmd(
    uhos_u16 conn_handle,
    uhos_u16 handle,
    uhos_u8 *p_value,
    uhos_u8  len)
{
    return UHOS_BLE_ERROR;
}

uhos_ble_status_t uhos_ble_gattc_exchange_mtu(
    uhos_u16 conn_handle,
    uhos_u16 mtu)
{
    esp_err_t ret;
    uhos_u8 conidx = uhos_ble_pal_conn_id_switch(conn_handle, UHOS_BLE_CONNECT_SWITCH_MODE_DEC);

    if(!app_get_connect_status(conidx))
    {
        // APP_HAL_TRC("con dis 0x%x\r\n",conidx);
        UHOS_LOGW("ble not inited");
        return UHOS_BLE_ERROR;
    }

    ret = esp_ble_gattc_send_mtu_req(esp_gatt_if_t gattc_if, uint16_t conn_id);
    
    if(ret)
    {
        UHOS_LOGW("exchange mtu fail 0x%4x\r\n", ret);
        return UHOS_BLE_ERROR;
    }
    return UHOS_BLE_SUCCESS;
}

uhos_ble_status_t uhos_ble_gattc_mtu_get(uhos_u16 conn_handle, uhos_u16 *mtu_size)
{

    uhos_u16 conidx = uhos_ble_pal_conn_id_switch(conn_handle, UHOS_BLE_CONNECT_SWITCH_MODE_DEC);

    *mtu_size = app_ble_get_con_mtu((uhos_u8)conidx);

    return UHOS_BLE_SUCCESS;
}
