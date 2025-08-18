/**
 *
 * @defgroup grp_net status manager软件接口
 * @{
 * @author jiguangfei (jiguangfei@haier.com)
 * @brief
 * 提供统一的网络状态接口
 * @details
 * 设备APP通过调用此组接口可获得：
 * 1. 设备wifi链接状态信息
 * 2. 连云状态信息
 * 3. ai连云状态信息
 * 4. 网络探测状态信息
 *
 * @version 0.1
 * @date 2023-09-18
 *
 * @file uhsd_net_status_manager.h
 * @copyright Copyright (c) 2021, Haier.Co, Ltd.
 *
 * @attention
 *
 */

#ifndef __UHSD_NET_STATUS_MANAGER_H__
#define __UHSD_NET_STATUS_MANAGER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "uhsd_types.h"

/**
 * @brief 统一的设备网络状态信息。
 * @attention 设备基本网络状态
 */
typedef enum
{
    /**
     * @brief STA 连接成功
     */
    UH_NET_STA_CONNCTED,

    /**
     * @brief STA连接失败
     */
    UH_NET_STA_DISCONNCTED,

    /**
     * @brief IOT连接成功
     */
    UH_IOT_CLOUD_CONNECTED,

    /**
     * @brief IOT连接失败
     */
    UH_IOT_CLOUD_DISCONNECTED,

    /**
     * @brief AI云连接成功
     */
    UH_AI_CLOUD_NET_CONNECTED,

    /**
     * @brief AI云连接失败
     */
    UH_AI_CLOUD_NET_DISCONNECTED,

} uhsd_net_status_manager_e;

/**
 * @brief 设备联网状态信息回调定义
 */
typedef uhsd_s32 (*uhsd_net_status_manager_cb)(uhsd_net_status_manager_e net_status, uhsd_void *userdata);

/**
 * @brief 	网络事件注册函数。
 * @note	注册后，将收到终端向本设备推送的网络事件。
 * @ref		uhsd_net_status_manager_e
 *
 * @param[in] func_cb		回调函数，用于接收网络事件.
 * @param[in] userdata		用户自定义数据。随回调函数传递回给用户处理。
 * @return uhsd_s32
 */
UHSD_API uhsd_s32 uhsd_net_status_register(uhsd_net_status_manager_cb func_cb, uhsd_void *userdata);

#ifdef __cplusplus
}
#endif

#endif /*__UHSD_NET_STATUS_MANAGER_H__*/

/** @}*/
