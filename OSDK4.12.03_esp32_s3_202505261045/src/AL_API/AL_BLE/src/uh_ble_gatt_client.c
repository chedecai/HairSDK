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
#include "arch.h"
// #include "gap.h"
#include "sonata_gap_api.h"
#include "sonata_gatt_api.h"
#include "sonata_utils_api.h"
#include "app.h"

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


/**************************************************************************************************/
/*                                        内部数据类型定义                                        */
/**************************************************************************************************/


/**************************************************************************************************/
/*                                        全局(静态)变量                                          */
/**************************************************************************************************/
static uhos_u8            g_uhos_ble_pal_gattc_op[APP_MAX_CON_IDX] = {0};
uhos_ble_gattc_callback_t g_uhos_ble_pal_gattc_user_cb = UHOS_NULL; //<! GATT层用户设置的Client端回调函数


/**************************************************************************************************/
/*                                          内部函数原型                                          */
/**************************************************************************************************/


/**************************************************************************************************/
/*                                          内部函数实现                                          */
/**************************************************************************************************/
/**
 * @brief       协议栈gatt层的回调函数实现
 * @param[in]   event   回调事件
 * @param[in]   param   回调参数
 * @return      无
 */
static void uhos_ble_pal_gattc_stack_cb(app_ble_gatt_event_t event, app_ble_gatt_event_param_t *param)
{
    uhos_u16 conn_id = 0;
    uhos_ble_gattc_evt_param_t evt_param = {0};

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

    // 获取连接句柄
    conn_id = uhos_ble_pal_conn_id_switch(param->dis_svc.conidx, UHOS_BLE_CONNECT_SWITCH_MODE_INC);

    // 依据事件类型进行操作
    switch (event)
    {
        case APP_BLE_GATT_EVENT_DISC_SERVICE:
        {
            evt_param.conn_handle = conn_id;

            evt_param.srv_disc_rsp.primary_srv_range.begin_handle = param->dis_svc.start_hdl;
            evt_param.srv_disc_rsp.primary_srv_range.end_handle   = param->dis_svc.end_hdl;

            if (UPLUG_BLE_UUID_16_LEN == param->dis_svc.uuid_len)
            {
                evt_param.srv_disc_rsp.srv_uuid.type = UHOS_BLE_UUID_TYPE_16;
            }
            else
            {
                evt_param.srv_disc_rsp.srv_uuid.type = UHOS_BLE_UUID_TYPE_128;
            }

            uhos_libc_memcpy(evt_param.srv_disc_rsp.srv_uuid.uuid128,
                             param->dis_svc.uuid,
                             param->dis_svc.uuid_len);

            evt_param.srv_disc_rsp.succ = 1;

            g_uhos_ble_pal_gattc_user_cb(UHOS_BLE_GATTC_EVT_PRIMARY_SERVICE_DISCOVER_RESP, &evt_param);

            break;
        }

        case APP_BLE_GATT_EVENT_DISC_SERVICE_DONE:
        {
            evt_param.conn_handle = conn_id;

            evt_param.common_rsp.succ = true;

            g_uhos_ble_pal_gattc_user_cb(UHOS_BLE_GATTC_EVT_PRIMARY_SERVICE_DISCOVER_DONE, &evt_param);

            break;
        }

        case APP_BLE_GATT_EVENT_DISC_ALL_CHAR:
        case APP_BLE_GATT_EVENT_DISC_CCCD:
        case APP_BLE_GATT_EVENT_CHAR_DISCOVER:
        {
            evt_param.conn_handle = conn_id;

            evt_param.char_disc_rsp.char_handle       = param->dis_char.attr_hdl;
            evt_param.char_disc_rsp.char_properties   = param->dis_char.prop;
            evt_param.char_disc_rsp.char_value_handle = param->dis_char.pointer_hdl;

            if (UPLUG_BLE_UUID_16_LEN == param->dis_char.uuid_len)
            {
                evt_param.char_disc_rsp.char_uuid.type = UHOS_BLE_UUID_TYPE_16;
            }
            else
            {
                evt_param.char_disc_rsp.char_uuid.type = UHOS_BLE_UUID_TYPE_128;
            }

            uhos_libc_memcpy(evt_param.char_disc_rsp.char_uuid.uuid128,
                             param->dis_char.uuid,
                             param->dis_char.uuid_len);

            if ((APP_BLE_GATT_EVENT_CHAR_DISCOVER == event)
             || (APP_BLE_GATT_EVENT_DISC_ALL_CHAR == event))
            {
                if (g_uhos_ble_pal_gattc_op[param->dis_char.conidx] == UHOS_BLE_GATTC_EVT_CHAR_DISCOVER_RESP)
                {
                    g_uhos_ble_pal_gattc_user_cb(UHOS_BLE_GATTC_EVT_CHAR_DISCOVER_RESP, &evt_param);
                }
                else
                {
                    g_uhos_ble_pal_gattc_user_cb(UHOS_BLE_GATTC_EVT_READ_USING_UUID_RESP, &evt_param);
                }
            }
            else
            {
                g_uhos_ble_pal_gattc_user_cb(UHOS_BLE_GATTC_EVT_CCCD_DISCOVER_RESP, &evt_param);
            }

            break;
        }

        case APP_BLE_GATT_EVENT_CHAR_DISCOVER_DONE:
        {
            evt_param.conn_handle = conn_id;

            evt_param.common_rsp.succ  = true;

            if (g_uhos_ble_pal_gattc_op[param->dis_char.conidx] == UHOS_BLE_GATTC_EVT_CHAR_DISCOVER_RESP)
            {
                g_uhos_ble_pal_gattc_user_cb(UHOS_BLE_GATTC_EVT_CHAR_DISCOVER_DONE, &evt_param);
            }
            else
            {
                g_uhos_ble_pal_gattc_user_cb(UHOS_BLE_GATTC_EVT_READ_USING_UUID_RESP, &evt_param);
            }

            break;
        }

        case APP_BLE_GATT_EVENT_DISC_CHAR_BY_UUID_DONE:
        {
            evt_param.conn_handle = conn_id;
            evt_param.common_rsp.succ = true;

            g_uhos_ble_pal_gattc_user_cb(UHOS_BLE_GATTC_EVT_CHAR_DISCOVER_DONE, &evt_param);

            break;
        }

        case APP_BLE_GATT_EVENT_DISC_DESP:
        {
            evt_param.conn_handle = conn_id;

            evt_param.char_desc_disc_rsp.char_desc_handle = param->dis_desc.attr_hdl;

            if(param->dis_desc.uuid_len == UPLUG_BLE_UUID_16_LEN)
            {
                evt_param.char_desc_disc_rsp.char_desc_uuid.type = UHOS_BLE_UUID_TYPE_16;
            }
            else
            {
                evt_param.char_desc_disc_rsp.char_desc_uuid.type = UHOS_BLE_UUID_TYPE_128;
            }

            uhos_libc_memcpy(evt_param.char_desc_disc_rsp.char_desc_uuid.uuid128,
                             param->dis_desc.uuid,
                             param->dis_desc.uuid_len);


            if (g_uhos_ble_pal_gattc_op[param->dis_desc.conidx] == UHOS_BLE_GATTC_EVT_CHAR_DESC_DISCOVER_RESP)
            {
                g_uhos_ble_pal_gattc_user_cb(UHOS_BLE_GATTC_EVT_CHAR_DESC_DISCOVER_RESP, &evt_param);
            }

            break;
        }

        case APP_BLE_GATT_EVENT_DISC_DESP_DONE:
        {
            evt_param.conn_handle = conn_id;

            evt_param.common_rsp.succ  = true;

            if (g_uhos_ble_pal_gattc_op[param->dis_desc.conidx] == UHOS_BLE_GATTC_EVT_CHAR_DESC_DISCOVER_RESP)
            {
                g_uhos_ble_pal_gattc_user_cb(UHOS_BLE_GATTC_EVT_CHAR_DESC_DISCOVER_DONE, &evt_param);
            }

            break;
        }

        case APP_BLE_GATT_EVENT_READ_RSP:
        {
            bool flag = true;

            evt_param.conn_handle = conn_id;

            if (param->read_rsp.status !=0)
            {
                flag = false;
            }

            if (g_uhos_ble_pal_gattc_op[param->read_rsp.conidx] != UHOS_BLE_GATTC_EVT_READ_USING_UUID_RESP)
            {
                return;
            }

            evt_param.read_char_value_rsp.succ = flag;

            if (flag)
            {
                evt_param.read_char_value_rsp.data = param->read_rsp.value;
                evt_param.read_char_value_rsp.len  = param->read_rsp.length;
            }
            else
            {
                evt_param.read_char_value_rsp.data = NULL;
                evt_param.read_char_value_rsp.len  = 0;
            }

            g_uhos_ble_pal_gattc_user_cb(UHOS_BLE_GATTC_EVT_READ_CHAR_VALUE_RESP, &evt_param);

            break;
        }

        case APP_BLE_GATT_EVENT_READ_CHAR_VALUE:
        {
            evt_param.conn_handle = conn_id;

            if (g_uhos_ble_pal_gattc_op[param->read_rsp.conidx] == UHOS_BLE_GATTC_EVT_READ_CHAR_VALUE_RESP)
            {
                evt_param.read_char_value_rsp.len  = param->read_rsp.length;
                evt_param.read_char_value_rsp.data = param->read_rsp.value;
                evt_param.read_char_value_rsp.succ = 1;

                g_uhos_ble_pal_gattc_user_cb(UHOS_BLE_GATTC_EVT_READ_CHAR_VALUE_RESP, &evt_param);
            }
            else if (g_uhos_ble_pal_gattc_op[param->read_rsp.conidx] == UHOS_BLE_GATTC_EVT_READ_USING_UUID_RESP)
            {
                evt_param.read_using_uuid_rsp.char_value_handle = param->read_rsp.handle;
                evt_param.read_using_uuid_rsp.len = param->read_rsp.length;
                evt_param.read_using_uuid_rsp.data = param->read_rsp.value;

                g_uhos_ble_pal_gattc_user_cb(UHOS_BLE_GATTC_EVT_READ_USING_UUID_RESP, &evt_param);
            }
            else if (g_uhos_ble_pal_gattc_op[param->read_rsp.conidx] == UHOS_BLE_GATTC_EVT_READ_CHAR_VALUE_BY_UUID_RESP)
            {
                evt_param.read_char_value_by_uuid_rsp.char_value_handle = param->read_rsp.handle;
                evt_param.read_char_value_by_uuid_rsp.len = param->read_rsp.length;
                evt_param.read_char_value_by_uuid_rsp.data = param->read_rsp.value;
                evt_param.read_char_value_by_uuid_rsp.succ = true;

                g_uhos_ble_pal_gattc_user_cb(UHOS_BLE_GATTC_EVT_READ_CHAR_VALUE_BY_UUID_RESP, &evt_param);
            }

            break;
        }

        case APP_BLE_GATT_EVENT_READ_USING_UUID:
        {
            break;
        }

        case APP_BLE_GATT_EVENT_READ_USING_UUID_DONE:
        {
            evt_param.conn_handle = conn_id;

            evt_param.common_rsp.succ = true;

            if (g_uhos_ble_pal_gattc_op[param->read_rsp.conidx] == UHOS_BLE_GATTC_EVT_READ_USING_UUID_RESP)
            {
                g_uhos_ble_pal_gattc_user_cb(UHOS_BLE_GATTC_EVT_READ_USING_UUID_DONE, &evt_param);
            }
            else if (g_uhos_ble_pal_gattc_op[param->read_rsp.conidx] == UHOS_BLE_GATTC_EVT_READ_CHAR_VALUE_BY_UUID_RESP)
            {
                //g_uhos_ble_pal_gattc_user_cb(UPLUS_BLE_GATTC_EVT_READ_USING_UUID_DONE,&evt_param);
            }

            break;
        }

        case APP_BLE_GATT_EVENT_WRITE_RSP:
        {
            evt_param.conn_handle = conn_id;

            if (param->write_rsp.status != 0)
            {
                evt_param.write_rsp.succ = 0;
            }
            else
            {
                evt_param.write_rsp.succ = 1;
            }

            if (g_uhos_ble_pal_gattc_op[param->write_rsp.conidx] == UHOS_BLE_GATTC_EVT_WRITE_RESP)
            {
                g_uhos_ble_pal_gattc_user_cb(UHOS_BLE_GATTC_EVT_WRITE_RESP, &evt_param);
            }

            break;
        }

        case APP_BLE_GATT_EVENT_NOTIFY_IND:
        case APP_BLE_GATT_EVENT_INDICATE_IND:
        {
            evt_param.conn_handle = conn_id;

            evt_param.notification.handle = param->notify_ind.handle;
            evt_param.notification.len    = param->notify_ind.length;
            evt_param.notification.pdata  = param->notify_ind.value;

            if (event == APP_BLE_GATT_EVENT_NOTIFY_IND)
            {
                g_uhos_ble_pal_gattc_user_cb(UHOS_BLE_GATTC_EVT_NOTIFICATION, &evt_param);
            }
            else
            {
                g_uhos_ble_pal_gattc_user_cb(UHOS_BLE_GATTC_EVT_INDICATION, &evt_param);
            }

            break;
        }

        case APP_BLE_GATTC_EVT_EXCHANGE_MTU_DONE:
        {

            evt_param.conn_handle = conn_id;

            evt_param.common_rsp.succ = true;

            g_uhos_ble_pal_gattc_user_cb(UHOS_BLE_GATTC_EVT_EXCHANGE_MTU_DONE, &evt_param);

            break;
        }

        case APP_BLE_GATT_EVT_RSSI_IND:
        {
            //hal_report_gatt_connect(param->rssi_ind.rssi);
            break;
        }

        default:
        {
            break;
        }
    }

    return;
}


/**************************************************************************************************/
/*                                          全局函数实现                                          */
/**************************************************************************************************/
/**
 * @brief       注册GATT层client端用户回调函数
 * @param[in]   cb  用户回调函数
 * @return      uplus_ble_status_t
 */
uhos_ble_status_t uhos_ble_gattc_callback_register(uhos_ble_gattc_callback_t cb)
{
    g_uhos_ble_pal_gattc_user_cb = cb;
    ble_set_gatt_callback(uhos_ble_pal_gattc_stack_cb);

    return UHOS_BLE_SUCCESS;
}

/**
 * @brief       首要服务发现
 * @param[in]   conn_handle     连接句柄
 * @param[in]   handle_range    服务ID的范围
 * @param[in]   p_srv_uuid      UUID的描述
 * @return      uhos_ble_status_t
 */
uhos_ble_status_t uhos_ble_gattc_primary_service_discover_all(
    uhos_u16 conn_handle,
    void    *req)
{
    uhos_u8  conidx = 0;
    uhos_u16 retval = 0;

    // 检查连接状态
    conidx = uhos_ble_pal_conn_id_switch(conn_handle, UHOS_BLE_CONNECT_SWITCH_MODE_DEC);

    if (!app_get_connect_status(conidx))
    {
        UHOS_LOGE("conn dis 0x%x", conidx);
        return UHOS_BLE_ERROR;
    }

    // 调用协议栈接口
    g_uhos_ble_pal_gattc_op[conidx] = UHOS_BLE_GATTC_EVT_PRIMARY_SERVICE_DISCOVER_RESP;

    retval = sonata_ble_gatt_disc_all_svc(conidx);

    if (retval != API_SUCCESS)
    {
        UHOS_LOGE("primary service discover all fail 0x%4x", retval);
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
    uhos_u16 retval   = 0;

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

    retval = sonata_ble_gatt_disc_svc_by_uuid(conidx,
                                              handle_range->begin_handle,
                                              handle_range->end_handle,
                                              uuid_len,
                                              p_srv_uuid->uuid128);

    if (API_SUCCESS != retval)
    {
        UHOS_LOGE("primary service discover by uuid fail 0x%4x",retval);
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
    uhos_u16 retval = 0;

    // 检查连接状态
    conidx = uhos_ble_pal_conn_id_switch(conn_handle, UHOS_BLE_CONNECT_SWITCH_MODE_DEC);

    if (!app_get_connect_status(conidx))
    {
        UHOS_LOGE("conn dis 0x%x", conidx);
        return UHOS_BLE_ERROR;
    }

    g_uhos_ble_pal_gattc_op[conidx] = UHOS_BLE_GATTC_EVT_CHAR_DISCOVER_RESP;
    retval = sonata_ble_gatt_disc_all_characteristic(conidx,
                                                     char_handle_range->begin_handle,
                                                     char_handle_range->end_handle);
    if (API_SUCCESS != retval)
    {
        UHOS_LOGE("char discover of service fail 0x%4x",retval);
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
    uhos_u16 retval   = 0;

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

    retval  = sonata_ble_gatt_disc_characteristic_by_uuid(conidx,
                                                          handle_range->begin_handle,
                                                          handle_range->end_handle,
                                                          uuid_len,
                                                          p_char_uuid->uuid128);

    if (API_SUCCESS != retval)
    {
        UHOS_LOGE("gattc char discover by uuid fail 0x%4x", retval);
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
    uhos_u16 retval = 0;

    // 检查连接状态
    conidx = uhos_ble_pal_conn_id_switch(conn_handle, UHOS_BLE_CONNECT_SWITCH_MODE_DEC);

    if(!app_get_connect_status(conidx))
    {
        UHOS_LOGE("conn dis 0x%x", conidx);
        return UHOS_BLE_ERROR;
    }

    // 调用协议栈接口
    g_uhos_ble_pal_gattc_op[conidx] = UHOS_BLE_GATTC_EVT_CHAR_DESC_DISCOVER_RESP;

    retval = sonata_ble_gatt_disc_all_descriptor(conidx,
                                                 handle_range->begin_handle,
                                                 handle_range->end_handle);

    if (API_SUCCESS != retval)
    {
        UHOS_LOGE("char descriptor discover all fail 0x%4x", retval);
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
    uhos_u16 retval = 0;

    // 连接状态检查
    conidx = uhos_ble_pal_conn_id_switch(conn_handle, UHOS_BLE_CONNECT_SWITCH_MODE_DEC);

    if (!app_get_connect_status(conidx))
    {
        UHOS_LOGE("conn dis 0x%x", conidx);
        return UHOS_BLE_ERROR;
    }

    // 调用协议栈接口
    g_uhos_ble_pal_gattc_op[conidx] = UHOS_BLE_GATTC_EVT_READ_CHAR_VALUE_RESP;

    retval = sonata_ble_gatt_read_by_handle(conidx, char_value_handle);

    if (API_SUCCESS != retval)
    {
        UHOS_LOGE("read char value fail 0x%4x", retval);

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
    uhos_u16 retval   = 0;

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

    retval = sonata_ble_gatt_read_by_uuid(conidx,
                                          0,
                                          handle_range->begin_handle,
                                          handle_range->end_handle,
                                          uuid_len,
                                          p_char_uuid->uuid128);

    if (API_SUCCESS != retval)
    {
        UHOS_LOGE("read char value by uuid fail 0x%4x", retval);
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
    uhos_u16 retval = 0;

    // 检查连接状态
    conidx = uhos_ble_pal_conn_id_switch(conn_handle, UHOS_BLE_CONNECT_SWITCH_MODE_DEC);

    if (!app_get_connect_status(conidx))
    {
        UHOS_LOGE("conn dis 0x%x", conidx);
        return UHOS_BLE_ERROR;
    }

    // 调用协议栈接口
    g_uhos_ble_pal_gattc_op[conidx] = UHOS_BLE_GATTC_EVT_WRITE_RESP;

    retval = sonata_ble_gatt_write(conidx,handle, 0, 0, len, p_value);

    if (API_SUCCESS != retval)
    {
        UHOS_LOGE("gattc write with rsp fail 0x%4x", retval);
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
    uhos_u16 retval = 0;

    // 检查连接状态
    conidx = uhos_ble_pal_conn_id_switch(conn_handle, UHOS_BLE_CONNECT_SWITCH_MODE_DEC);

    if (!app_get_connect_status(conidx))
    {
        UHOS_LOGE("conn dis 0x%x", conidx);
        return UHOS_BLE_ERROR;
    }

    // 调用协议栈接口
    retval = sonata_ble_gatt_write_no_response(conidx, char_value_handle, 0, 0, len, p_value);

    if (API_SUCCESS != retval)
    {
        UHOS_LOGE("gattc write with rsp fail 0x%4x", retval);
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
    uhos_u8 conidx = uhos_ble_pal_conn_id_switch(conn_handle, UHOS_BLE_CONNECT_SWITCH_MODE_DEC);

    if(!app_get_connect_status(conidx))
    {
        // APP_HAL_TRC("con dis 0x%x\r\n",conidx);
        UHOS_LOGW("ble not inited");
        return UHOS_BLE_ERROR;
    }

    uhos_u16 retval = sonata_ble_gatt_exchange_mtu(conidx);
    if(retval != API_SUCCESS)
    {
        UHOS_LOGW("exchange mtu fail 0x%4x\r\n", retval);
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
