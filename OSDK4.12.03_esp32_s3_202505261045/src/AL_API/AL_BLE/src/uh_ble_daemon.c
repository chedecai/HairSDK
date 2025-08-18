/**
 * @copyright Copyright (c) 2021, Haier.Co, Ltd.
 * @file uh_ble_daemon.c
 * @author maaiguo (maaiguo@haier.com)
 * @brief 基于ASR5882s硬件模块的BLE底层协议栈守侯线程的功能实现
 * @date 2022-02-24
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>version <th>Author  <th>Description
 * <tr><td>2022-02-24   <td>1.0     <td>maaiguo <td>
 * </table>
 */

#define LOG_TAG "ble-d"

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
#include "uh_ble_gap.h"
#include "uh_ble_daemon.h"

/**************************************************************************************************/
/*                                          外部引用声明                                          */
/**************************************************************************************************/

/**************************************************************************************************/
/*                                           内部宏定义                                           */
/**************************************************************************************************/
#define UHOS_BLE_DAEMON_TASK_NAME       "ble_daemon"
#define UHOS_BLE_DAEMON_TASK_STACK_SIZE 2 * 1024
#define UHOS_BLE_DAEMON_TASK_PRIORITY   5

#define UHOS_BLE_EVENT_SEM_TIMEOUT 50

/**************************************************************************************************/
/*                                        内部数据类型定义                                        */
/**************************************************************************************************/

/**************************************************************************************************/
/*                                        全局(静态)变量                                          */
/**************************************************************************************************/
static uhos_thread_t g_uhos_ble_pal_daemon_tid = UHOS_NULL; //<! 守护任务的句柄

/**************************************************************************************************/
/*                                          内部函数原型                                          */
/**************************************************************************************************/

/**************************************************************************************************/
/*                                          内部函数实现                                          */
/**************************************************************************************************/
/**
 * @brief       BLE守护线程的实现
 * @param[in]   p_param 任务参数
 */
static void *uhos_ble_daemon_task(void *p_param)
{
    while (1)
    {
        // 广播数据上报
        uhos_ble_pal_gap_adv_rpt_handle(UHOS_BLE_EVENT_SEM_TIMEOUT);
    }

    return UHOS_NULL;
}

/**************************************************************************************************/
/*                                          全局函数实现                                          */
/**************************************************************************************************/
/**
 * @brief       BLE守候线程初始化
 */
void uhos_ble_daemon_init(void)
{
    uhos_thread_attr_t attr = {0};

    // 检查守护任务是否已经创建
    if (UHOS_NULL != g_uhos_ble_pal_daemon_tid)
    {
        UHOS_LOGW("ble daemon already init");
        return;
    }

    // 创建守护任务
    attr.stack_size = UHOS_BLE_DAEMON_TASK_STACK_SIZE;
    attr.priority = UHOS_BLE_DAEMON_TASK_PRIORITY;
    attr.name = UHOS_BLE_DAEMON_TASK_NAME;

    if (UHOS_SUCCESS == uhos_thread_create(&g_uhos_ble_pal_daemon_tid, uhos_ble_daemon_task, UHOS_NULL, &attr))
    {
        UHOS_LOGI("ble daemon init ok");
    }
    else
    {
        UHOS_LOGE("ble daemon init failed");
    }

    return;
}

/**
 * @brief       BLE守候线程反初始化
 */
void uhos_ble_daemon_deinit(void)
{
    // 删除任务
    if (g_uhos_ble_pal_daemon_tid)
    {
        uhos_thread_delete(g_uhos_ble_pal_daemon_tid);
        UHOS_LOGI("ble daemon task is deleted");
        g_uhos_ble_pal_daemon_tid = UHOS_NULL;
    }

    return;
}
