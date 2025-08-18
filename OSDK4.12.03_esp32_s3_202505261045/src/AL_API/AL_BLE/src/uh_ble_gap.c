/**
 * @copyright Copyright (c) 2021, Haier.Co, Ltd.
 * @file uh_ble_gap.c
 * @author maaiguo (maaiguo@haier.com)
 * @brief 基于ASR5882s硬件模块的BLE GAP层相关功能函数实现
 * @date 2022-02-24
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>version <th>Author  <th>Description
 * <tr><td>2022-02-24   <td>1.0     <td>maaiguo <td>
 * </table>
 */

#define LOG_TAG "ble-g"

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
#include "uh_osal.h"
#include "uh_log.h"
#include "uh_libc.h"

#include "uh_ble.h"
#include "uh_ble_common.h"


/**************************************************************************************************/
/*                                          外部引用声明                                          */
/**************************************************************************************************/
extern ble_adv_param_t ble_adv_param[APP_MAX_ADV_IDX];


/**************************************************************************************************/
/*                                           内部宏定义                                           */
/**************************************************************************************************/
#define UHOS_BLE_ADV_RPT_BUF_NUM            20                  //<! 广播上报事件缓存数组大小
#define UHOS_BLE_MAC_REVERSE_ENABLE         1

/**************************************************************************************************/
/*                                        内部数据类型定义                                        */
/**************************************************************************************************/
/**
 * @enum        GAP层白名单操作类型枚举定义
 */
typedef enum uhos_ble_pal_gap_white_list_op
{
    UHOS_BLE_GAP_WHITE_LIST_ADD = 1,                            //<! 添加白名单
    UHOS_BLE_GAP_WHITE_LIST_DEL,                                //<! 删除白名单
    UHOS_BLE_GAP_WHITE_LIST_CLEAR                               //<! 清楚白名单
} uhos_ble_pal_gap_white_list_op_t;

/**
 * @struct      GAP层白名单地址结构定义
 */
typedef struct uhos_ble_pal_white_list_addr
{
    uhos_ble_addr_type_t type;                                  //<! 地址类型
    uhos_ble_addr_t      addr;                                  //<! 地址
} uhos_ble_pal_white_list_addr_t;

/**
 * @struct      GAP层广播上报事件控制块结构
 */
typedef struct uhos_ble_pal_gap_adv_rpt_ctl
{
    uhos_u8                  head;                              //<! 缓存数组头索引
    uhos_u8                  tail;                              //<! 缓存数组尾索引
    uhos_u16                 count;                             //<! 缓存计数
    uhos_sem_t               event_sem_id;                      //<! 缓存同步信号量
    uhos_ble_gap_evt_param_t item[UHOS_BLE_ADV_RPT_BUF_NUM];    //<! 广播事件缓存数组
} uhos_ble_pal_gap_adv_rpt_ctl_t;

/**
 * @struct      GAP层连接信息结构
 */
typedef struct uhos_ble_pal_gap_conn_info
{
    uhos_ble_addr_t           peer_addr;                        //<! 对端地址
    uhos_ble_addr_type_t      type;                             //<! 地址类型
    uhos_ble_gap_role_t       role;                             //<! 设备角色
    uhos_ble_gap_conn_param_t param;                            //<! 连接参数
} uhos_ble_pal_gap_conn_info_t;

/**
 * @struct      GAP层的全局控制数据结构
 */
typedef struct uhos_ble_pal_gap_ctl
{
    uhos_u8                        adv_idx;                     //<! 广播索引，可连接/不可连接共2组
    uhos_u8                        adv_flag;                    //<! 广播标志；1-开启，0-关闭
    uhos_ble_gap_adv_param_t       adv_param;                   //<! 广播参数
    app_ble_adv_data_t             adv_data;                    //<! 广播数据， 需要实现
    app_ble_scan_data_t            scan_rsp_data;               //<! 扫描响应数据， 需要实现
    uhos_ble_pal_gap_adv_rpt_ctl_t adv_rpt_ctl;                 //<! 广播上报事件控制块
    uhos_u16                       conn_handle;                 //<! 连接句柄
    uhos_u16                       conn_flag;                   //<! 连接标志；1-连接，0-断开
    uhos_ble_pal_gap_conn_info_t   conn_info;                   //<! 连接信息
    uhos_u16                       peer_mtu;                    //<! 对端的MTU

} uhos_ble_pal_gap_ctl_t;


/**************************************************************************************************/
/*                                        全局(静态)变量                                          */
/**************************************************************************************************/
uhos_ble_pal_gap_ctl_t g_uhos_ble_pal_gap_ctl     = {0};        //<! GAP层全局控制数据
uhos_ble_gap_cb_t      g_uhos_ble_pal_gap_user_cb = UHOS_NULL;  //<! GAP层应用设置的回调函数   


/**************************************************************************************************/
/*                                          内部函数原型                                          */
/**************************************************************************************************/
uhos_ble_status_t uhos_ble_adv_start(void);

static void uhos_ble_mac_reverse(uhos_u8 addr[], uhos_u8 size)
{
#if UHOS_BLE_MAC_REVERSE_ENABLE == 1
    uhos_u8 temp = 0, i = 0;
    for (i = 0; i < size / 2; i++) {
        temp = addr[i];
        addr[i] = addr[size - i - 1];
        addr[size - i - 1] = temp;
    }
#endif
    return;
}

/**************************************************************************************************/
/*                                          内部函数实现                                          */
/**************************************************************************************************/
/**
 * @brief       将广播上报事件插入到缓存数组中
 * @param[in]   item    广播上报事件
 * @return      执行结果；0-失败，1-成功
 */
static uhos_s32 uhos_ble_pal_gap_adv_rpt_add(uhos_ble_gap_evt_param_t *item)
{
    uhos_ble_pal_gap_adv_rpt_ctl_t *adv_rpt_ctl = &g_uhos_ble_pal_gap_ctl.adv_rpt_ctl;

    // 输入参数检查
    if (UHOS_NULL == item)
    {
        return (0);
    }

    // 检查缓存数据是否溢出
    if (adv_rpt_ctl->count >= UHOS_BLE_ADV_RPT_BUF_NUM)
    {
        return (0);
    }

    // 保存广播上报事件数据
    uhos_libc_memcpy(&adv_rpt_ctl->item[adv_rpt_ctl->tail++],
                     item,
                     sizeof(uhos_ble_gap_evt_param_t));

    // 调整缓存数组的控制信息
    adv_rpt_ctl->tail = (adv_rpt_ctl->tail) % UHOS_BLE_ADV_RPT_BUF_NUM;
    adv_rpt_ctl->count++;

    // 释放同步信号量
    uhos_sem_release(adv_rpt_ctl->event_sem_id);

    return (1);
}

/**
 * @brief       从缓存池中获取一条GAP层的事件数据，该事件是扫描报告数据
 * @param[out]  item    gap层事件数据
 * @return      0-获取失败，1-获取成功
 */
static uhos_s32 uhos_ble_pal_gap_adv_rpt_get(uhos_ble_gap_evt_param_t *item)
{
    uhos_ble_pal_gap_adv_rpt_ctl_t *adv_rpt_ctl = &g_uhos_ble_pal_gap_ctl.adv_rpt_ctl;

    if (UHOS_NULL == item)
    {
        return (0);
    }

    if (0 == adv_rpt_ctl->count)
    {
        return (0);
    }

    //uh_k_irq_lock(0);

    uhos_libc_memcpy(item,
                     &adv_rpt_ctl->item[adv_rpt_ctl->head++],
                     sizeof(uhos_ble_gap_evt_param_t));

    adv_rpt_ctl->head = (adv_rpt_ctl->head) % UHOS_BLE_ADV_RPT_BUF_NUM;
    adv_rpt_ctl->count--;

    //uh_k_irq_unlock(0, 0);

    return (1);
}


/**
 * @brief       GAP扫描事件的回调函数实现，用于生成广播上报事件，并进行缓存
 * @param[in]   adv_ind     广播上报指示数据
 * @param[in]   rsp_ind     广播扫描响应指示数据（未使用）
 */
static void uhos_ble_pal_gap_scan_cb(void *adv_ind, void *rsp_ind)
{
    uhos_ble_gap_evt_param_t         evt_param      = {0};
    esp_ble_gap_cb_param_t *adv_report_src = UHOS_NULL;
    uhos_u8                          type           = 0;

    // 获取广播上报信息
    adv_report_src = (esp_ble_gap_cb_param_t *)adv_ind;
    type           = adv_report_src->scan_rst.search_evt & ESP_GAP_BLE_SCAN_RESULT_EVT;

    // 生成广播上报事件
    uhos_libc_memcpy(&evt_param.report.peer_addr,
                     adv_report_src->scan_rst.bda,
                     ESP_BD_ADDR_LEN);

    uhos_ble_mac_reverse(&evt_param.report.peer_addr, ESP_BD_ADDR_LEN);

    evt_param.report.addr_type = adv_report_src->scan_rst.ble_addr_type;

    if (type == ESP_GAP_BLE_SCAN_RESULT_EVT)
    {
        evt_param.report.adv_type = ADV_DATA;
    }
    else
    {
        evt_param.report.adv_type = SCAN_RSP_DATA;
    }

    evt_param.report.rssi     = adv_report_src->scan_rst.rssi;
    evt_param.report.data_len = adv_report_src->scan_rst.adv_data_len;

    uhos_libc_memcpy(evt_param.report.data, adv_report_src->scan_rst.ble_adv, adv_report_src->scan_rst.adv_data_len);

    // 将广播上报事件放入缓存
    uhos_ble_pal_gap_adv_rpt_add(&evt_param);

    return;
}

/**
 * @brief       GAP层白名单操作的实现
 * @param[in]   op      操作类型
 * @param[in]   address 白名单地址
 * @return      操作结果
 */
static uhos_ble_status_t uhos_ble_pal_gap_set_white_list(
    uhos_ble_pal_gap_white_list_op_t      op,
    const uhos_ble_pal_white_list_addr_t *address)
{
    if (false == app_ble_is_inited())
    {
        UHOS_LOGW("ble not inited");
        return UHOS_BLE_ERROR;
    }

    switch (op)
    {
        case UHOS_BLE_GAP_WHITE_LIST_ADD:
        {
            uint16_t          retval;
            struct sonata_gap_bdaddr addr;

            addr.addr_type = address->type;
            uhos_libc_memcpy(addr.addr.addr, address->addr, 6);

            retval = app_ble_set_add_whilte_list(1, &addr);

            if (API_SUCCESS != retval)
            {
                UHOS_LOGE("ble white list add fail, 0x%4x", retval);
                return UHOS_BLE_ERROR;
            }

            break;
        }

        case UHOS_BLE_GAP_WHITE_LIST_DEL:
        {
            uint16_t          retval;
            struct sonata_gap_bdaddr addr;

            addr.addr_type = address->type;
            uhos_libc_memcpy(addr.addr.addr, address->addr, 6);

            retval  = app_ble_set_del_whilte_list(1,&addr);

            if (API_SUCCESS != retval)
            {
                UHOS_LOGE("ble white list del fail, 0x%4x", retval);
                return UHOS_BLE_ERROR;
            }

            break;
        }

        case UHOS_BLE_GAP_WHITE_LIST_CLEAR:
        {
            uint16_t retval = app_ble_set_clear_whilte_list();

            if (API_SUCCESS != retval)
            {
                UHOS_LOGE("ble white list clear fail, 0x%4x", retval);
                return UHOS_BLE_ERROR;
            }

            break;
        }

        default:
        {
            UHOS_LOGW("ble white list op 0x%4x unknown", op);
            break;
        }
    }

    return UHOS_BLE_SUCCESS;
}
    
/**
 * @brief       断连事件的处理实现
 * @param[in]   param   事件参数
 * @return      uhos_ble_status_t
 */
static uhos_ble_status_t uhos_ble_pal_gap_disconn_evt_handle(uhos_ble_gap_evt_param_t *param)
{
    uhos_ble_pal_gap_ctl_t *gap_ctl = &g_uhos_ble_pal_gap_ctl;
    // uhos_u16            conidx  = 0;
    // uplus_ble_hal_conn_param_t conn_param = {0};

    // 输入参数检查
    if (UHOS_NULL == param)
    {
        UHOS_LOGE("pointer is null");
        return UHOS_BLE_ERROR;
    }

    // 清除全局连接相关数据
    gap_ctl->conn_flag   = 0;
    gap_ctl->conn_handle = 0;
    uhos_libc_memset(&gap_ctl->conn_info, 0, sizeof(uhos_ble_pal_gap_conn_info_t));

    // 调用回调
#if 0
    condix = param->conn_handle;
    conn_param.status = 2;

    if (svc->conn_cb)
    {
        svc->conn_cb(conidx, &conn_param);
    }

    hal_need_report_connect = 0;
#else
    if (g_uhos_ble_pal_gap_user_cb)
    {
        g_uhos_ble_pal_gap_user_cb(UHOS_BLE_GAP_EVT_DISCONNET, param);
    }
#endif

    // 开启广播
    uhos_ble_adv_start();

    return UHOS_BLE_SUCCESS;
}

#if 0
/**
 * @brief       连接事件的处理实现
 * @param[in]   param   事件参数
 * @return      uhos_ble_status_t
 */
static uhos_ble_status_t uhos_ble_pal_gap_connected_evt_handle(uhos_ble_gap_evt_param_t *param)
{
    uhos_ble_pal_gap_ctl_t *gap_ctl = &g_uhos_ble_pal_gap_ctl;
    uhos_u16            conidx  = 0;

    // 检查是否已经处于连接状态
    if (gap_ctl->conn_flag)
    {
        UHOS_LOGW("ble already connected");
        return UHOS_BLE_ERROR;
    }

    // 更新连接相关数据
    gap_ctl->conn_flag = 1;
    gap_ctl->conn_handle = param->conn_handle;

    uhos_libc_memset(&gap_ctl->conn_info, 0, sizeof(uhos_ble_pal_gap_conn_info_t));
    uhos_libc_memcpy(&gap_ctl->conn_info, &param->connect, sizeof(uhos_ble_pal_gap_conn_info_t));

    // hal_need_report_connect = 1;

    // 配置协议栈
    conidx = uhos_ble_pal_conn_id_switch(param->conn_handle, UHOS_BLE_CONNECT_SWITCH_MODE_DEC);

    sonata_api_app_timer_set((conidx + APP_INTERVAL_TIMER_OFFSET), 50);
    sonata_api_app_timer_active(conidx + APP_INTERVAL_TIMER_OFFSET);

    // 获取对端的MTU值
    uhos_ble_gatts_mtu_get(gap_ctl->conn_handle, &gap_ctl->peer_mtu);

    return UHOS_BLE_SUCCESS;
}
#endif

/**
 * @brief       协议栈事件回调函数的实现
 * @param[in]   event   回调事件
 * @param[in]   param   回调参数
 * @return      无
 */
void uhos_ble_pal_gap_stack_cb(app_ble_stack_event_t event, app_ble_stack_event_param_t *param)
{
    uhos_ble_pal_gap_ctl_t *gap_ctl = &g_uhos_ble_pal_gap_ctl;

    switch (event)
    {
        // 协议栈就绪
        case APP_BLE_STACK_EVENT_STACK_READY:
        {
            UHOS_LOGI("ble stack ready");
            break;
        }

        // 协议栈失败
        case APP_BLE_STACK_EVENT_STACK_FAIL:
        {
            UHOS_LOGE("ble stack failed");
            break;
        }

        // 广播开启
        case APP_BLE_STACK_EVENT_ADV_ON:
        {
            UHOS_LOGI("ble adv on");
            gap_ctl->adv_flag = 1;
            break;
        }

        // 广播关闭
        case APP_BLE_STACK_EVENT_ADV_OFF:
        {
            UHOS_LOGI("ble adv off");
            gap_ctl->adv_flag = 0;
            break;
        }

        // 断连
        case APP_BLE_STACK_EVENT_DISCONNECTED:
        {
            if (UHOS_NULL == param)
            {
                UHOS_LOGE("ble disconn cb exit");
                return;
            }

            UHOS_LOGI("ble disconn");

            uhos_ble_gap_evt_param_t evt_param;

            evt_param.conn_handle = uhos_ble_pal_conn_id_switch(param->disconnect.conidx,
                                                                  UHOS_BLE_CONNECT_SWITCH_MODE_INC);
            evt_param.disconnect.reason = param->disconnect.reason;

            if ((1 == gap_ctl->conn_flag) && (gap_ctl->conn_handle == evt_param.conn_handle))
            {
                uhos_ble_pal_gap_disconn_evt_handle(&evt_param);
            }
            else
            {
                if (g_uhos_ble_pal_gap_user_cb)
                {
                    g_uhos_ble_pal_gap_user_cb(UHOS_BLE_GAP_EVT_DISCONNET, &evt_param);
                }
            }

            break;
        }

        // 外围设备已连接
        case APP_BLE_STACK_EVENT_PERIPHERAL_CONNECTED:
        {
            uhos_ble_gap_evt_param_t evt_param;

            if (UHOS_NULL == param)
            {
                UHOS_LOGE("ble peripheral conn cb exit");
                return;
            }

            UHOS_LOGI("ble peripheral conn");

            evt_param.conn_handle = uhos_ble_pal_conn_id_switch(param->conn_param.conidx,
                                                            UHOS_BLE_CONNECT_SWITCH_MODE_INC);
            
            evt_param.connect.conn_param.conn_sup_timeout  = param->conn_param.connect_ind.sup_to;
            evt_param.connect.conn_param.min_conn_interval = param->conn_param.connect_ind.con_interval;
            evt_param.connect.conn_param.max_conn_interval = param->conn_param.connect_ind.con_interval;
            evt_param.connect.conn_param.slave_latency     = param->conn_param.connect_ind.con_latency;

            evt_param.connect.role = UHOS_BLE_GAP_PERIPHERAL;
            evt_param.connect.type = param->conn_param.connect_ind.peer_addr_type;

            uhos_libc_memcpy(evt_param.connect.peer_addr,
                             param->conn_param.connect_ind.peer_addr.addr,
                             SONATA_GAP_BD_ADDR_LEN);

            uhos_ble_mac_reverse(evt_param.connect.peer_addr, SONATA_GAP_BD_ADDR_LEN);

#if 0
            uhos_ble_pal_gap_connected_evt_handle(&evt_param);
#else
            g_uhos_ble_pal_gap_user_cb(UHOS_BLE_GAP_EVT_CONNECTED, &evt_param);

#endif

            break;
        }

        // 中心设备已连接
        case APP_BLE_STACK_EVENT_CENTRAL_CONNECTED:
        {
            if ((NULL == param) || (UHOS_NULL == g_uhos_ble_pal_gap_user_cb))
            {
                UHOS_LOGE("ble central conn cb exit");
                return;
            }

            UHOS_LOGI("ble central conn");

            uhos_ble_gap_evt_param_t evt_param;

            evt_param.conn_handle = uhos_ble_pal_conn_id_switch(param->conn_param.conidx,
                                                              UHOS_BLE_CONNECT_SWITCH_MODE_INC);

            evt_param.connect.conn_param.conn_sup_timeout  = param->conn_param.connect_ind.sup_to;
            evt_param.connect.conn_param.min_conn_interval = param->conn_param.connect_ind.con_interval;
            evt_param.connect.conn_param.max_conn_interval = param->conn_param.connect_ind.con_interval;
            evt_param.connect.conn_param.slave_latency     = param->conn_param.connect_ind.con_latency;

            evt_param.connect.role = UHOS_BLE_GAP_CENTRAL;
            evt_param.connect.type = param->conn_param.connect_ind.peer_addr_type;

            uhos_libc_memcpy(&evt_param.connect.peer_addr,
                             param->conn_param.connect_ind.peer_addr.addr,
                             SONATA_GAP_BD_ADDR_LEN);

            uhos_ble_mac_reverse(&evt_param.connect.peer_addr, SONATA_GAP_BD_ADDR_LEN);
            g_uhos_ble_pal_gap_user_cb(UHOS_BLE_GAP_EVT_CONNECTED, &evt_param);

            break;
        }

        // 扫描启动
        case APP_BLE_STACK_EVENT_SCAN_ON:
        {
            UHOS_LOGI("ble scan on");
            break;
        }

        // 扫描停止
        case APP_BLE_STACK_EVENT_SCAN_OFF:
        {
            UHOS_LOGI("ble scan off");
            break;
        }

        // 广播启动（不可连接）
        case APP_BLE_STACK_EVENT_NONCONN_ADV_ON:
        {
            UHOS_LOGI("ble non conn adv on");
            break;
        }

        // 广播停止（不可连接）
        case APP_BLE_STACK_EVENT_NONCONN_ADV_OFF:
        {
            UHOS_LOGI("ble non conn adv off");
            break;
        }

        // 连接更新
        case APP_BLE_STACK_EVENT_CONNECTE_UPDATE:
        {
            if ((NULL == param) || (UHOS_NULL == g_uhos_ble_pal_gap_user_cb))
            {
                UHOS_LOGE("ble conn update cb exit");
                return;
            }

            UHOS_LOGI("ble conn update");

            uhos_ble_gap_evt_param_t evt_param;

            evt_param.conn_handle = uhos_ble_pal_conn_id_switch(param->conn_param.conidx,
                                                                  UHOS_BLE_CONNECT_SWITCH_MODE_INC);
            
            evt_param.update_conn.conn_param.conn_sup_timeout  = param->update_conn.sup_to;
            evt_param.update_conn.conn_param.min_conn_interval = param->update_conn.con_interval;
            evt_param.update_conn.conn_param.max_conn_interval = param->update_conn.con_interval;
            evt_param.update_conn.conn_param.slave_latency     = param->update_conn.con_latency;
            
            g_uhos_ble_pal_gap_user_cb(UHOS_BLE_GAP_EVT_CONN_PARAM_UPDATED, &evt_param);

            break;
        }

        default:
        {
            UHOS_LOGW("ble stack ev %d no handler", event);
            break;
        }
    }

    return;
}


/**************************************************************************************************/
/*                                          全局函数实现                                          */
/**************************************************************************************************/
/**
 * @brief       GAP层初始化
 */
void uhos_ble_pal_gap_init(void)
{
    uhos_libc_memset(&g_uhos_ble_pal_gap_ctl, 0, sizeof(uhos_ble_pal_gap_ctl_t));

    // 默认使用可连接广播的索引
    g_uhos_ble_pal_gap_ctl.adv_idx = APP_CONN_ADV_IDX;

    // 创建广播上报事件的同步信号量
    if (UHOS_SUCCESS != uhos_sem_create(&g_uhos_ble_pal_gap_ctl.adv_rpt_ctl.event_sem_id, 0))
    {
        UHOS_LOGE("create sem err");
    }

    // 设置协议栈回调函数
    ble_set_callback(uhos_ble_pal_gap_stack_cb);

    return;
}

/**
 * @brief       GAP层的反初始化
 * @note        主要是全局数据的重置
 */
void uhos_ble_pal_gap_deinit(void)
{
    ble_set_callback(UHOS_NULL);
    return;
}

/**
 * @brief       处理扫描事件数据
 */
void uhos_ble_pal_gap_adv_rpt_handle(uhos_u32 timeout)
{
    uhos_ble_gap_evt_param_t item = {0};

    // 信号量未就绪，等待
    if (UHOS_NULL == g_uhos_ble_pal_gap_ctl.adv_rpt_ctl.event_sem_id)
    {
        UHOS_LOGW("ble adv rpt sem is null");
        uhos_thread_sleep(timeout);
        return;
    }

    // 阻塞在同步信号量上，等待广播上报数据
    if (UHOS_SUCCESS == uhos_sem_wait(g_uhos_ble_pal_gap_ctl.adv_rpt_ctl.event_sem_id, timeout))
    {
        if (UHOS_NULL == g_uhos_ble_pal_gap_user_cb)
        {
            UHOS_LOGW("user gap cb is null");
            return;
        }

        if (uhos_ble_pal_gap_adv_rpt_get(&item))
        {
            g_uhos_ble_pal_gap_user_cb(UHOS_BLE_GAP_EVT_ADV_REPORT, &item);
        }
    }

    return;
}

/**
 * @brief       设置广播相关的数据
 * @param[in]   p_data     广播数据
 * @param[in]   dlen       广播数据长度
 * @param[in]   p_sr_data  扫描响应数据
 * @param[in]   srdlen     扫描响应数据长度
 * @return      uplus_ble_status_t 执行结果
 */
uhos_ble_status_t uhos_ble_gap_adv_data_set(
    uhos_u8 const *p_data,
    uhos_u8        dlen,
    uhos_u8 const *p_sr_data,
    uhos_u8        srdlen)
{
    // 局部变量
    app_ble_adv_data_t  *adv_data_ptr      = &g_uhos_ble_pal_gap_ctl.adv_data;
    app_ble_scan_data_t *scan_rsp_data_ptr = &g_uhos_ble_pal_gap_ctl.scan_rsp_data;
    uhos_u8              adv_idx           = g_uhos_ble_pal_gap_ctl.adv_idx;

    // 输入参数检查
    if ((UHOS_NULL == p_data) || (dlen < 3))
    {
        UHOS_LOGE("adv data invalid");
        return UHOS_BLE_ERROR;
    }

    // 拷贝广播数据（去除头3个字节）  
    adv_data_ptr->ble_advdataLen  = dlen - 3;
    uhos_libc_memcpy(adv_data_ptr->ble_advdata, p_data + 3, dlen - 3);

    // 拷贝扫描响应数据（如果有）
    scan_rsp_data_ptr->ble_respdataLen = srdlen;

    if (0 != srdlen)
    {
        uhos_libc_memcpy(scan_rsp_data_ptr->ble_respdata, p_sr_data, srdlen);
    }
#if 0
    // 广播数据已经生效，则需要重新启动广播
    if (app_get_adv_status(adv_idx))
    {
        // 注意：这个接口也会检查广播状态
        app_ble_advertising_start(adv_idx, adv_data_ptr, scan_rsp_data_ptr);
    }
#endif
    return UHOS_BLE_SUCCESS;
}

/**
 * @brief       开启广播
 * @param[in]   p_adv_param 广播参数
 * @return      uhos_ble_status_t 执行结果
 */
uhos_ble_status_t uhos_ble_gap_adv_start(uhos_ble_gap_adv_param_t *p_adv_param)
{
    // 局部变量
    app_ble_adv_data_t  *adv_data_ptr      = &g_uhos_ble_pal_gap_ctl.adv_data;
    app_ble_scan_data_t *scan_rsp_data_ptr = &g_uhos_ble_pal_gap_ctl.scan_rsp_data;
    uhos_u8             *adv_idx_ptr       = &g_uhos_ble_pal_gap_ctl.adv_idx;

    // 依据广播类型配置广播参数属性
    if (p_adv_param->adv_type == UHOS_BLE_ADV_TYPE_CONNECTABLE_UNDIRECTED)
    {
        *adv_idx_ptr                     = APP_CONN_ADV_IDX;
        ble_adv_param[*adv_idx_ptr].prop = SONATA_GAP_ADV_PROP_UNDIR_CONN_MASK;
    }
    else if (p_adv_param->adv_type == UHOS_BLE_ADV_TYPE_NON_CONNECTABLE_UNDIRECTED)
    {
        *adv_idx_ptr                     = APP_NON_CONN_ADV_IDX;
        ble_adv_param[*adv_idx_ptr].prop = SONATA_GAP_ADV_PROP_NON_CONN_SCAN_MASK;
    }
    else if (p_adv_param->adv_type == UHOS_BLE_ADV_TYPE_CONNECTABLE_DIRECTED_HDC)
    {
       *adv_idx_ptr = APP_CONN_ADV_IDX;
       ble_adv_param[*adv_idx_ptr].prop = SONATA_GAP_ADV_PROP_DIR_CONN_HDC_MASK;
    }
    else if (p_adv_param->adv_type == UHOS_BLE_ADV_TYPE_CONNECTABLE_DIRECTED_LDC)
    {
        *adv_idx_ptr = APP_CONN_ADV_IDX;
        ble_adv_param[*adv_idx_ptr].prop = SONATA_GAP_ADV_PROP_DIR_CONN_LDC_MASK;
    }
    else if (p_adv_param->adv_type == UHOS_BLE_ADV_TYPE_SCANNABLE_UNDIRECTED)
    {
        *adv_idx_ptr = APP_NON_CONN_ADV_IDX;
        ble_adv_param[*adv_idx_ptr].prop = SONATA_GAP_ADV_PROP_NON_CONN_SCAN_MASK;
    }
    else
    {
       *adv_idx_ptr = APP_NON_CONN_ADV_IDX;
       ble_adv_param[*adv_idx_ptr].prop = SONATA_GAP_ADV_PROP_NON_CONN_NON_SCAN_MASK;
    }

    // 设置地址类型
    if (p_adv_param->direct_addr_type == UHOS_BLE_ADDRESS_TYPE_RANDOM)
    {
       ble_adv_param[*adv_idx_ptr].own_address_type = SONATA_GAP_STATIC_ADDR;
    }
    else
    {
       ble_adv_param[*adv_idx_ptr].own_address_type = SONATA_GAP_STATIC_ADDR; 
    }

    // 设置广播间隔
    ble_adv_param[*adv_idx_ptr].advertising_interval_min = p_adv_param->adv_interval_min;
    ble_adv_param[*adv_idx_ptr].advertising_interval_max = p_adv_param->adv_interval_max;

    // 设置广播通道
    ble_adv_param[*adv_idx_ptr].advertising_channel_map = 0x1 | 0x2 | 0x4;

    if (p_adv_param->ch_mask.ch_37_off)
    {
        //ble_adv_param[*adv_idx_ptr].advertising_channel_map &= ~0x1;
    }

    if (p_adv_param->ch_mask.ch_38_off)
    {
        //ble_adv_param[*adv_idx_ptr].advertising_channel_map &= ~0x2;
    }

    if (p_adv_param->ch_mask.ch_39_off)
    {
        //ble_adv_param[*adv_idx_ptr].advertising_channel_map &= ~0x4;
    }

    UHOS_LOGD("adv channel map is 0x%02x", ble_adv_param[*adv_idx_ptr].advertising_channel_map);

    // 开启广播
    app_ble_advertising_start(*adv_idx_ptr, adv_data_ptr, scan_rsp_data_ptr);

    return UHOS_BLE_SUCCESS;
}

/**
 * @brief       设置广播参数
 * @note        设置的参数会被保存，这些参数将作为uhos_ble_adv_start接口开启广播时的参数
 *              uhos_ble_adv_param_set
 *              uhos_ble_adv_start
 *              依次使用上述2个接口，可以替代uhos_ble_gap_adv_start接口
 * @param[in]   adv_param   广播参数
 * @return      UHOS_BLE_SUCCESS    成功
 */
uhos_ble_status_t uhos_ble_adv_param_set(uhos_ble_gap_adv_param_t *adv_param)
{
    uhos_ble_gap_adv_param_t *adv_param_ptr = &g_uhos_ble_pal_gap_ctl.adv_param;

    adv_param_ptr->adv_interval_max = adv_param->adv_interval_max;
    adv_param_ptr->adv_interval_min = adv_param->adv_interval_min;
    adv_param_ptr->adv_type         = adv_param->adv_type;
    adv_param_ptr->direct_addr_type = adv_param->direct_addr_type;
    adv_param_ptr->ch_mask          = adv_param->ch_mask;

    return UHOS_BLE_SUCCESS;
}

/**
 * @brief       开启广播（采用预先设置的广播参数）
 * @return      UHOS_BLE_SUCCESS    成功
 *              其他                失败
 */
uhos_ble_status_t uhos_ble_adv_start(void)
{
    uhos_ble_gap_adv_param_t *adv_param_ptr = &g_uhos_ble_pal_gap_ctl.adv_param;
    
    return uhos_ble_gap_adv_start(adv_param_ptr);
}

/**
 * @brief       采用全局数据中的广播参数重新启动广播
 * @return      uhos_ble_status_t 执行结果
 */
uhos_ble_status_t uhos_ble_gap_reset_adv_start(void)
{
    return uhos_ble_adv_start();
}

/**
 * @brief       关闭广播
 * @return      uhos_ble_status_t 执行结果
 */
uhos_ble_status_t uhos_ble_gap_adv_stop(void)
{
    uhos_u8 adv_idx = g_uhos_ble_pal_gap_ctl.adv_idx;

    app_ble_advertising_stop(adv_idx);
    
    return UHOS_BLE_SUCCESS;
}

/**
 * @brief       关闭广播
 * @author      李诗龙
 * @return      uhos_ble_status_t 执行结果
 */
uhos_ble_status_t uhos_ble_gap_non_connectable_stop(void)
{
    uhos_u16 res     = 0;
    uhos_u8  adv_idx = APP_NON_CONN_ADV_IDX;
    // 关闭不可连接广播
    res = app_ble_advertising_stop(adv_idx);

    return res == 0 ? UHOS_BLE_SUCCESS : UHOS_BLE_ERROR;
}

/**
 * @brief       开启扫描功能
 * @param[in]   scan_type  扫描类型
 * @param[in]   scan_param 扫描参数
 * @return      uhos_ble_status_t 执行结果
 */
uhos_ble_status_t uhos_ble_gap_scan_start(uhos_ble_gap_scan_type_t scan_type, uhos_ble_gap_scan_param_t scan_param)
{
    app_ble_scan_param_t hal_scan_param = {0};

    if (false == app_ble_is_inited())
    {
        UHOS_LOGW("ble not inited");
        return UHOS_BLE_ERROR;
    }

    hal_scan_param.scan_param.prop |= SONATA_GAP_SCAN_PROP_PHY_1M_BIT;
    hal_scan_param.own_addr_type    = SONATA_GAP_STATIC_ADDR;

    if (UHOS_BLE_SCAN_TYPE_ACTIVE == scan_type)
    {
        hal_scan_param.scan_param.prop |= SONATA_GAP_SCAN_PROP_ACTIVE_1M_BIT;
    }

    hal_scan_param.scan_param.dup_filt_pol =SONATA_GAP_DUP_FILT_DIS;

    if (app_ble_get_whilte_list_count())
    {
        hal_scan_param.scan_param.type = SONATA_GAP_SCAN_TYPE_SEL_OBSERVER;
    }
    else
    {
        hal_scan_param.scan_param.type = SONATA_GAP_SCAN_TYPE_OBSERVER;
    }

    scan_param.scan_interval = 0x002f;
    scan_param.scan_window   = 0x001c;
    
    hal_scan_param.scan_param.scan_param_1m.scan_intv = scan_param.scan_interval;
    hal_scan_param.scan_param.scan_param_1m.scan_wd   = scan_param.scan_window;
    hal_scan_param.scan_param.period = 0;

    app_gap_set_scan_cb(uhos_ble_pal_gap_scan_cb);
    app_ble_start_scan_with_param(&hal_scan_param);

    return UHOS_BLE_SUCCESS;
}

/**
 * @brief       关闭扫描
 * @return      uhos_ble_status_t 执行结果
 */
uhos_ble_status_t uhos_ble_gap_scan_stop(void)
{
    if (false == app_ble_is_inited())
    {
        UHOS_LOGW("ble not inited");
        return UHOS_BLE_ERROR;
    }

    app_ble_stop_scanning();
    app_gap_set_scan_cb(UHOS_NULL);

    return UHOS_BLE_SUCCESS;
}

/**
 * @brief       更新连接参数
 * @note        注意：这个接口配置的连接参数并未保存到本地全局数据
 * @param[in]   conn_handle 连接句柄
 * @param[in]   conn_params 连接参数
 * @return      uhos_ble_status_t 执行结果
 */
uhos_ble_status_t uhos_ble_gap_update_conn_params(
    uhos_u16                  conn_handle,
    uhos_ble_gap_conn_param_t conn_params)
{
        
    uhos_u16 conidx = 0;
    uhos_u16 retval = 0;

    conidx = uhos_ble_pal_conn_id_switch(conn_handle, UHOS_BLE_CONNECT_SWITCH_MODE_DEC);
    retval = sonata_ble_gap_update_connection_params(conidx,
                                                     0,
                                                     conn_params.min_conn_interval,
                                                     conn_params.max_conn_interval,
                                                     conn_params.slave_latency,
                                                     conn_params.conn_sup_timeout,
                                                     0xFFFF,
                                                     0xFFFF);

    if (API_SUCCESS != retval)
    {
        UHOS_LOGE("ble update conn params failed 0x%4x", retval);
        return UHOS_BLE_ERROR;
    }

    return UHOS_BLE_SUCCESS;
}

/**
 * @brief       断开连接
 * @param[in]   conn_handle 连接句柄
 * @return      uhos_ble_status_t 执行结果
 */
uhos_ble_status_t uhos_ble_gap_disconnect(uhos_u16 conn_handle)
{
    uhos_u16 conidx = 0;

    conidx = uhos_ble_pal_conn_id_switch(conn_handle, UHOS_BLE_CONNECT_SWITCH_MODE_DEC);

    if (conidx < APP_MAX_CON_IDX)
    {
        app_ble_disconnect(conidx);
    }
    else
    {
        #if ( !defined(CFG_BLE_NO_CENTRAL) )

        app_ble_stop_initiating(); // maybe not!!!
        #endif
        
    }

    return UHOS_BLE_SUCCESS;
}

/**
 * @brief       连接设备
 * @param[in]   scan_param
 * @param[in]   conn_param
 * @return      uhos_ble_status_t
 */
uhos_ble_status_t uhos_ble_gap_connect(
    uhos_ble_gap_scan_param_t scan_param,
    uhos_ble_gap_connect_t    conn_param)
{
    sonata_gap_init_param_t app_hal_connect_param = {0};

    app_hal_connect_param.type = SONATA_GAP_INIT_TYPE_DIRECT_CONN_EST;
    app_hal_connect_param.prop = SONATA_GAP_INIT_PROP_1M_BIT ;
    app_hal_connect_param.conn_to = 0;
    app_hal_connect_param.peer_addr.addr_type = SONATA_GAP_STATIC_ADDR;

    app_hal_connect_param.scan_param_1m.scan_intv      = 0x0200;
    app_hal_connect_param.scan_param_1m.scan_wd        = 0x0100;
    app_hal_connect_param.conn_param_1m.conn_intv_min  = 0x0028;
    app_hal_connect_param.conn_param_1m.conn_intv_max  = 0x0028;
    app_hal_connect_param.conn_param_1m.conn_latency   = 0;
    app_hal_connect_param.conn_param_1m.supervision_to = 0x02BC;
    app_hal_connect_param.conn_param_1m.ce_len_min     = 0x0008;
    app_hal_connect_param.conn_param_1m.ce_len_max     = 0x0008;

    uhos_libc_memcpy(app_hal_connect_param.peer_addr.addr.addr,
                     &conn_param.peer_addr,
                     SONATA_GAP_BD_ADDR_LEN);

    uhos_ble_mac_reverse(app_hal_connect_param.peer_addr.addr.addr, SONATA_GAP_BD_ADDR_LEN);

    if (app_hal_connect_param.peer_addr.addr_type == UHOS_BLE_ADDRESS_TYPE_PUBLIC)
    {
        app_hal_connect_param.peer_addr.addr_type = SONATA_GAP_STATIC_ADDR;
    }
    else 
    {
        app_hal_connect_param.peer_addr.addr_type = SONATA_GAP_STATIC_ADDR;
    }

    return app_ble_connect(&app_hal_connect_param);
}

/**
 * @brief       取消连接
 * @return      uhos_ble_status_t
 */
uhos_ble_status_t uhos_ble_gap_cancel_connection(void)
{
    app_ble_stop_initiating();
    return UHOS_BLE_SUCCESS;
}

/**
 * @brief       注册GAP层用户回调函数
 * @brief       同一用户只能调用一次本接口。每次调用，系统会自动增加用户编号。
 * @param[in]   cb 回调函数
 * @return      uhos_ble_status_t 执行结果
 * @retval      UHOS_BLE_SUCCESS  成功
 * @retval      UHOS_BLE_ERROR    错误
 */
uhos_ble_status_t uhos_ble_gap_callback_register(uhos_ble_gap_cb_t cb)
{
    g_uhos_ble_pal_gap_user_cb = cb;
    ble_set_callback(uhos_ble_pal_gap_stack_cb);

    return UHOS_BLE_SUCCESS;
}

/**
 * @brief       GAP层添加白名单
 * @param[in]   mac     ble的mac地址，16进制格式，6字节
 * @return      操作结果
 */
uhos_ble_status_t uhos_ble_gap_white_list_add(uhos_u8 *mac)
{
    uhos_ble_pal_white_list_addr_t address;

    if (false == app_ble_is_inited())
    {
        UHOS_LOGW("ble not inited");
        return UHOS_BLE_ERROR;
    }

    if (NULL == mac)
    {
        return UHOS_BLE_ERROR;
    }

    address.type = UHOS_BLE_ADDRESS_TYPE_PUBLIC;
    uhos_libc_memcpy(address.addr, mac, 6);

    uhos_ble_mac_reverse(address.addr, 6);

    return uhos_ble_pal_gap_set_white_list(UHOS_BLE_GAP_WHITE_LIST_ADD, &address);
}

/**
 * @brief       GAP层删除白名单
 * @param[in]   mac     ble的mac地址，16进制格式，6字节
 * @return      操作结果
 */
uhos_ble_status_t uhos_ble_gap_white_list_remove(uhos_u8 *mac)
{
    uhos_ble_pal_white_list_addr_t address;

    if (false == app_ble_is_inited())
    {
        UHOS_LOGW("ble not inited");
        return UHOS_BLE_ERROR;
    }

    if (NULL == mac)
    {
        return UHOS_BLE_ERROR;
    }

    address.type = UHOS_BLE_ADDRESS_TYPE_PUBLIC;
    uhos_libc_memcpy(address.addr, mac, 6);

    uhos_ble_mac_reverse(address.addr, 6);

    return uhos_ble_pal_gap_set_white_list(UHOS_BLE_GAP_WHITE_LIST_DEL, &address);
}

/**
 * @brief       GAP层清除白名单
 * @return      操作结果
 */
uhos_ble_status_t uhos_ble_gap_white_list_clear(void)
{
    if (false == app_ble_is_inited())
    {
        UHOS_LOGW("ble not inited");
        return UHOS_BLE_ERROR;
    }

    return uhos_ble_pal_gap_set_white_list(UHOS_BLE_GAP_WHITE_LIST_CLEAR, UHOS_NULL);
}

