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


/**************************************************************************************************/
/*                                        内部数据类型定义                                        */
/**************************************************************************************************/


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
 * @brief       特征值权限转换
 * @param[in]   user_perm   用户配置的权限（暂未使用）
 * @param[in]   user_prop   用户配置的特性
 * @param[out]  p_perm      转换后的权限      
 */
static void uhos_ble_pal_gatts_perm_2_perm(uhos_u8 user_perm, uhos_u8 user_prop,uhos_u16 *p_perm)
{
    if (UHOS_BLE_HAL_CHR_READ == (user_prop & UHOS_BLE_HAL_CHR_READ))
    {
        *p_perm |= PRD_NA;
    }

    if (UHOS_BLE_HAL_CHR_WRITE == (user_prop & UHOS_BLE_HAL_CHR_WRITE))
    {
       *p_perm |= PWR_NA;
    }

    if (UHOS_BLE_HAL_CHR_NOTIFY == (user_prop & UHOS_BLE_HAL_CHR_NOTIFY))
    {
       *p_perm |= PNTF_NA;
    }

    if (UHOS_BLE_HAL_CHR_INDICATE == (user_prop & UHOS_BLE_HAL_CHR_INDICATE))
    {
       *p_perm |= PIND_NA;
    }

    return;
}

/**
 * @brief       获取属性数量（依据特征值数量）
 * @param[in]   attr_num    特征值数量
 * @return      属性数量
 */
static uhos_u32 uhos_ble_pal_gatts_get_att_num(uhos_u8 attr_num)
{
     uhos_u32 num = 3 * attr_num + 1 ;

     return num;
}

static uhos_u16 uhos_ble_pal_gatts_get_char_handle(uhos_u16 srv_handle, uhos_u16 handle)
{
    uhos_u16 char_handle = 0;

    char_handle = (srv_handle << 8) + handle;

    return char_handle;
}

void uhos_ble_pal_gatts_att_read_cb(
    uhos_u16  conn_handle,
    uhos_u16  srv_handle,
    uhos_u16  handle,
    uhos_u8 **data,
    uhos_u16 *size)
{
    uhos_ble_gatts_evt_param_t gatts_evt;
    uhos_ble_status_t status = UHOS_BLE_SUCCESS;
    gatts_evt.conn_handle       = conn_handle;
    gatts_evt.read.value_handle = uhos_ble_pal_gatts_get_char_handle(srv_handle, handle);
    gatts_evt.read.len          = size;
    gatts_evt.read.data         = data;
    gatts_evt.read.offset       = 0;

    if (UHOS_NULL != g_uhos_ble_pal_gatts_user_cb)
    {
        status = g_uhos_ble_pal_gatts_user_cb(UHOS_BLE_GATTS_EVT_READ, &gatts_evt);
    }

    uhos_u16 db_handle = app_ble_get_handle_by_local_handle(srv_handle, handle);

    if(status == UHOS_BLE_SUCCESS)
    {
        sonata_ble_gatt_send_read_confirm(conn_handle, db_handle, SONATA_HL_GAP_ERR_NO_ERROR, *size, *data);
    }
    else
    {
        sonata_ble_gatt_send_read_confirm(conn_handle, db_handle, SONATA_HL_ATT_ERR_INVALID_ATTRIBUTE_VAL_LEN, 0, NULL);
    }
    return;
}

void uhos_ble_pal_gatts_att_write_cb(
    uhos_u16 conn_handle,
    uhos_u16 srv_handle,
    uhos_u16 handle,
    uhos_u8 *data,
    uhos_u16 size)
{
    uhos_ble_gatts_evt_param_t gatts_evt;
    uhos_ble_status_t status = UHOS_BLE_SUCCESS;

    gatts_evt.conn_handle        = conn_handle;
    gatts_evt.write.value_handle = uhos_ble_pal_gatts_get_char_handle(srv_handle, handle);
    gatts_evt.write.data         = data;
    gatts_evt.write.len          = size;
    gatts_evt.write.offset       = 0;

    if (UHOS_NULL != g_uhos_ble_pal_gatts_user_cb)
    {
        status = g_uhos_ble_pal_gatts_user_cb(UHOS_BLE_GATTS_EVT_WRITE, &gatts_evt);
    }

    uhos_u16 db_handle = app_ble_get_handle_by_local_handle(srv_handle, handle);

    if(status == UHOS_BLE_SUCCESS)
    {
        sonata_ble_gatt_send_write_confirm(conn_handle, db_handle, SONATA_HL_GAP_ERR_NO_ERROR);
    }
    else
    {
        sonata_ble_gatt_send_write_confirm(conn_handle, db_handle, SONATA_HL_ATT_ERR_INVALID_ATTRIBUTE_VAL_LEN);
    }
    return;
}

void uhos_ble_pal_gatts_att_ind_cb(uhos_u16 conn_handle, uhos_u16 srv_handle, uhos_u16 handle, uhos_u8 *data, uhos_u16 size)
{
    uhos_ble_gatts_evt_param_t gatts_evt;
    // uhos_u8 *                  p_value = UHOS_NULL;
    // uhos_u16                   length  = 0;
    uhos_ble_status_t status = UHOS_BLE_SUCCESS;
    /*
        p_value = uhos_libc_zalloc(8);
        if (!p_value)
        {
            UHOS_LOG_MEM_ALLOC_FAIL();
            return;
        }
    */
    // gatts_evt.cccd = (uhos_s32)*data;
    UHOS_LOGI("write data:%d", (uhos_s32)*data);
    gatts_evt.conn_handle        = conn_handle;
    gatts_evt.write.data         = data;
    gatts_evt.write.len          = size;
    gatts_evt.write.value_handle = uhos_ble_pal_gatts_get_char_handle(srv_handle, handle);

    // gatts_evt.read.data = &p_value;
    // gatts_evt.read.len  = &length;

    if (UHOS_NULL != g_uhos_ble_pal_gatts_user_cb)
    {
        status = g_uhos_ble_pal_gatts_user_cb(UHOS_BLE_GATTS_EVT_CCCD_UPDATE, &gatts_evt);
    }

    uhos_u16 db_handle = app_ble_get_handle_by_local_handle(srv_handle, handle);

    if (status == UHOS_BLE_SUCCESS)
    {
        sonata_ble_gatt_send_write_confirm(conn_handle, db_handle, SONATA_HL_GAP_ERR_NO_ERROR);
    }
    else
    {
        sonata_ble_gatt_send_write_confirm(conn_handle, db_handle, SONATA_HL_ATT_ERR_INVALID_ATTRIBUTE_VAL_LEN);
    }
    /*
    if (length > 0)
    {
        app_ble_gatt_data_send_notify(conn_handle, srv_handle, handle, length, p_value);
    }

    if (UHOS_NULL != p_value)
        uhos_libc_free(p_value);
*/
    return;
}

/**
 * @brief       向协议栈添加一个服务
 * @param[in]   p_srv_db    蓝牙服务描述数据
 * @return      uhos_ble_status_t
 */
static uhos_ble_status_t uhos_ble_pal_gatts_add_service(uhos_ble_gatts_srv_db_t *p_srv_db)
{
    uhos_u8             char_num      = 0;
    uhos_u8             att_nb        = 0;
    uhos_u16            handle_offset = 0;
    ble_gatt_att_reg_t *att_list      = UHOS_NULL;
    ble_gatt_att_reg_t *att_temp      = UHOS_NULL;
    uhos_u32            len           = 0;

    // 输入参数检查
    if (UHOS_NULL == p_srv_db)
    {
        UHOS_LOGE("gatt service is empty");
        return UHOS_BLE_ERROR;
    }

    // 获取特征值数量
    char_num = p_srv_db->char_num;

    if (0 == char_num)
    {
        UHOS_LOGE("gatt service 0x%04x have no characteristics", p_srv_db->srv_uuid.uuid16);
        return UHOS_BLE_ERROR;
    }

    // 获取属性数量，并为全部属性分配内存
    att_nb = uhos_ble_pal_gatts_get_att_num(char_num);
    len    = (att_nb ) * sizeof(ble_gatt_att_reg_t);

    att_list = (ble_gatt_att_reg_t *)uhos_libc_malloc(len);

    if (UHOS_NULL == att_list)
    {
        UHOS_LOGE("malloc failed, %d", len);
        return UHOS_BLE_ERROR;
    }

    uhos_libc_memset(att_list, 0,len);

    att_temp = att_list;

    // 构造服务属性
    if (UHOS_BLE_UUID_TYPE_16 == p_srv_db->srv_uuid.type)
    {
        uhos_libc_memcpy(att_temp->att_desc.uuid, &p_srv_db->srv_uuid.uuid16, 2);
    }
    else if (UHOS_BLE_UUID_TYPE_128 == p_srv_db->srv_uuid.type)
    {
        uhos_libc_memcpy(att_temp->att_desc.uuid, p_srv_db->srv_uuid.uuid128, 16);
        SONATA_PERM_SET(att_temp->att_desc.ext_perm, UUID_LEN, 2);
        SONATA_PERM_SET(att_temp->att_desc.perm, SVC_UUID_LEN, 2);
    }

    att_temp++;
    handle_offset++;

    for (int i = 0; i < char_num; i++)
    {
        // 构造特征值的声明属性
        uint8_t char_uuid[UPLUG_BLE_UUID_128_LEN] = {0x03,0x28,0x0};
        uhos_libc_memcpy(att_temp->att_desc.uuid, char_uuid, UPLUG_BLE_UUID_128_LEN);

        att_temp->att_desc.perm     = PRD_NA;
        att_temp->att_desc.max_len  = 0;
        att_temp->att_desc.ext_perm = 0;

        att_temp++;
        handle_offset++;

        // 构造特征值的值属性（可读可写）
        switch (0x0F & p_srv_db->p_char_db[i].char_property)
        {
            case UHOS_BLE_HAL_CHR_READ:
            case UHOS_BLE_HAL_CHR_WRITE:
            {
                if (UHOS_BLE_UUID_TYPE_128 == p_srv_db->p_char_db[i].char_uuid.type)
                {
                    uhos_libc_memcpy(att_temp->att_desc.uuid,
                                     p_srv_db->p_char_db[i].char_uuid.uuid128,
                                     16);
                    SONATA_PERM_SET(att_temp->att_desc.ext_perm, UUID_LEN, 2);
                }
                else if (UHOS_BLE_UUID_TYPE_16 == p_srv_db->p_char_db[i].char_uuid.type)
                {
                    uhos_libc_memcpy(att_temp->att_desc.uuid,
                                     &p_srv_db->p_char_db[i].char_uuid.uuid16,
                                     2);
                }

                if (UHOS_BLE_HAL_CHR_READ == (0x0F & p_srv_db->p_char_db[i].char_property))
                {
                    att_temp->att_opr.read_request = uhos_ble_pal_gatts_att_read_cb;
                }

                if (UHOS_BLE_HAL_CHR_WRITE == (0x0F & p_srv_db->p_char_db[i].char_property))
                {
                    att_temp->att_opr.write_request = uhos_ble_pal_gatts_att_write_cb;
                }

                uhos_ble_pal_gatts_perm_2_perm(0,
                                               0x0F & p_srv_db->p_char_db[i].char_property,
                                               &att_temp->att_desc.perm);

                att_temp->att_desc.ext_perm |= PRI;
                att_temp->att_desc.max_len   = 512;
                att_temp++;

                p_srv_db->p_char_db[i].char_value_handle = handle_offset;

                handle_offset++;

                break;
            }

            default:
            {
                break;
            }
        }

        // 构造特征值的值属性（通知/指示）
        switch (0xF0 & p_srv_db->p_char_db[i].char_property)
        {
            case UHOS_BLE_HAL_CHR_NOTIFY:
            case UHOS_BLE_HAL_CHR_INDICATE:
            {
                if (UHOS_BLE_UUID_TYPE_128 == p_srv_db->p_char_db[i].char_uuid.type)
                {
                    uhos_libc_memcpy(att_temp->att_desc.uuid,
                                     p_srv_db->p_char_db[i].char_uuid.uuid128,
                                     16);
                    SONATA_PERM_SET(att_temp->att_desc.ext_perm, UUID_LEN, 2);
                }
                else if (UHOS_BLE_UUID_TYPE_16 == p_srv_db->p_char_db[i].char_uuid.type)
                {
                    uhos_libc_memcpy(att_temp->att_desc.uuid, 
                                     &p_srv_db->p_char_db[i].char_uuid.uuid16,
                                     2);
                }

                uhos_ble_pal_gatts_perm_2_perm(0,
                                               0xF0 & p_srv_db->p_char_db[i].char_property,
                                               &att_temp->att_desc.perm);

                att_temp->att_desc.ext_perm |= PRI;
                att_temp->att_desc.max_len   = p_srv_db->p_char_db[i].char_value_len;
                att_temp++;

                p_srv_db->p_char_db[i].char_value_handle = handle_offset;


                // 构造特征值的客户配置属性（CCCD）
                handle_offset++;

                uhos_u8 char_uuid2[UPLUG_BLE_UUID_128_LEN] = {0x02,0x29,0x0};
                uhos_libc_memcpy(att_temp->att_desc.uuid, char_uuid2, UPLUG_BLE_UUID_128_LEN);

                att_temp->att_desc.perm      = PRD_NA|PWR_NA;
                att_temp->att_desc.max_len   = 2;
                att_temp->att_desc.ext_perm |= PRI;
                att_temp->att_opr.ind_cb     = uhos_ble_pal_gatts_att_ind_cb;
            att_temp->att_opr.read_request = uhos_ble_pal_gatts_att_read_cb; // TODO for notify switch
                att_temp++;

                handle_offset++;

                break;
            }

            default:
            {
                break;
            }
        }
    }

    uint16_t start_handle = 0;

    app_ble_gatt_add_svc_helper(&start_handle, handle_offset, att_list);
    p_srv_db->srv_handle = start_handle << 8; // TODO

    for (int i = 0; i < char_num; i++)
    {
        p_srv_db->p_char_db[i].char_value_handle = p_srv_db->srv_handle
                                                 + p_srv_db->p_char_db[i].char_value_handle;
    }

    uhos_libc_free(att_list);

    return UHOS_BLE_SUCCESS;
}


/**************************************************************************************************/
/*                                          全局函数实现                                          */
/**************************************************************************************************/
/**
 * @brief       GATT层Server端初始化
 */
void uhos_ble_pal_gatts_init(void)
{
    return;
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

