/**
 *
 * @defgroup grp_uhsd_ft 产测（工厂）功能集合
 * @{
 * @brief SmartDevice 产测（工厂）接口
 * @version 0.1
 * @author hudx (hudingxuan.xian@haier.com)
 * @date 2024-03-02
 * @details
 * <pre>
 * @verbatim
 * 功能包括：
 * 1. 模组产测能力；
 * 2. 设备认证功能；
 * 3. 检测模块与待测家电设备功能；
 * @endverbatim
 * </pre>
 *
 * @file uhsd_ft.h
 *
 * @copyright Copyright (c) 2021, Haier.Co, Ltd.
 *
 */

#ifndef __UHSD_FT_H__
#define __UHSD_FT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "uhsd_types.h"

typedef enum
{
    /**
     * @brief 启动工厂模式。
     * @details
     * 此功能当前主要针对E++设备+OSDK的应用场景，
     *
     * 通过 @ref uhsd_ft_channel_up_callback 收到此类型后，
     * 设备APP可直接使用 @ref uhepp_ft_channel_down (类型： @ref UHEPP_FT_TYPE_START)发送给E++底板。
     *
     */
    UHSD_FT_TYPE_START = 101,

    /**
     * @brief 启动工厂模式响应。
     * @details
     * 此功能当前主要针对E++设备+OSDK的应用场景，
     *
     * 设备APP通过 @ref uhepp_ft_channel_up_callback 收到类型( @ref UHEPP_FT_TYPE_START_ACK)，可直接使用 @ref uhsd_ft_channel_down 发送给OSDK。
     *
     */
    UHSD_FT_TYPE_START_ACK = 102,

    /**
     * @brief 工厂模式下行数据透传。
     * @details
     * 此功能当前主要针对E++设备+OSDK的应用场景，
     *
     * 通过 @ref uhsd_ft_channel_up_callback 收到此类型后，
     * 设备APP可直接使用 @ref uhepp_ft_channel_down （类型： @ref UHEPP_FT_TYPE_TRANS_DOWN）发送给E++底板。
     *
     */
    UHSD_FT_TYPE_TRANS_DOWN = 103,

    /**
     * @brief 工厂模式上行数据透传。
     * @details
     * 此功能当前主要针对E++设备+OSDK的应用场景，
     *
     * 设备APP通过 @ref uhepp_ft_channel_up_callback 收到类型（ @ref UHEPP_FT_TYPE_TRANS_UP）后，可直接使用 @ref uhsd_ft_channel_down 发送给OSDK。
     *
     */
    UHSD_FT_TYPE_TRANS_UP = 104,

    /**
     * @brief 停止工厂模式。
     * @details
     * 此功能当前主要针对E++设备+OSDK的应用场景，
     *
     * 通过 @ref uhsd_ft_channel_up_callback 收到此类型后，
     * 设备APP可直接使用 @ref uhepp_ft_channel_down （类型： @ref UHEPP_FT_TYPE_STOP）发送给E++底板。
     *
     */
    UHSD_FT_TYPE_STOP = 105,

    /**
     * @brief 停止工厂模式响应。
     * @details
     * 此功能当前主要针对E++设备+OSDK的应用场景，
     *
     * 通过 @ref uhepp_ft_channel_up_callback 收到类型（ @ref UHEPP_FT_TYPE_STOP_ACK）后，设备APP可直接使用 @ref uhsd_ft_channel_down 发送给OSDK。
     *
     */
    UHSD_FT_TYPE_STOP_ACK = 106,

    /**
     * @brief 设备认证通信密钥请求。
     * @details
     * 此功能当前主要针对E++设备+OSDK的应用场景，
     *
     * 通过 @ref uhsd_ft_channel_up_callback 收到此类型后，
     * 设备APP可直接使用 @ref uhepp_ft_channel_down （类型： @ref UHEPP_FT_TYPE_AUTH_COMM_KEY_REQ）发送给E++底板。
     *
     */
    UHSD_FT_TYPE_AUTH_COMM_KEY_REQ = 301,

    /**
     * @brief 设备认证通信密钥响应。
     * @details
     * 此功能当前主要针对E++设备+OSDK的应用场景，
     *
     * 通过 @ref uhepp_ft_channel_up_callback 收到类型（ @ref UHEPP_FT_TYPE_AUTH_COMM_KEY_RSP）后，设备APP可直接使用 @ref uhsd_ft_channel_down 发送给OSDK。
     *
     */
    UHSD_FT_TYPE_AUTH_COMM_KEY_RSP = 302,

    /**
     * @brief 设备认证签名请求。
     * @details
     * 此功能当前主要针对E++设备+OSDK的应用场景，
     *
     * 通过 @ref uhsd_ft_channel_up_callback 收到此类型后，
     * 设备APP可直接使用 @ref uhepp_ft_channel_down （类型： @ref UHEPP_FT_TYPE_AUTH_SIGN_REQ）发送给E++底板。
     *
     */
    UHSD_FT_TYPE_AUTH_SIGN_REQ = 303,

    /**
     * @brief 设备认证签名响应。
     * @details
     * 此功能当前主要针对E++设备+OSDK的应用场景，
     *
     * 通过 @ref uhepp_ft_channel_up_callback 收到类型（ @ref UHEPP_FT_TYPE_AUTH_SIGN_RSP）后，设备APP可直接使用 @ref uhsd_ft_channel_down 发送给OSDK。
     *
     */
    UHSD_FT_TYPE_AUTH_SIGN_RSP = 304,

    /**
     * @brief 设备认证成功。
     * @details
     * 此功能当前主要针对E++设备+OSDK的应用场景，
     *
     * 通过 @ref uhsd_ft_channel_up_callback 此类型后，表示设备认证成功。
     *
     */
    UHSD_FT_TYPE_AUTH_SUCCESS = 305,

    /**
     * @brief 设备认证失败。
     * @details
     * 此功能当前主要针对E++设备+OSDK的应用场景，
     *
     * 通过 @ref uhsd_ft_channel_up_callback 此类型后，表示设备认证失败。
     *
     */
    UHSD_FT_TYPE_AUTH_FAILURE = 306,

} UHSD_FT_TYPE_E;

/**
 * @brief 启动工厂模式响应数据
 * @details
 * 对应 @ref UHSD_FT_TYPE_START_ACK
 */
typedef struct
{
    uhsd_s16 result; /**< 0：允许进入测试； 1：不允许进入测试。*/
} uhsd_ft_type_start_ack_data_t;

/**
 * @brief 上行数据通道注册回调
 * @details 接收来自OSDK的对应的产测数据。
 * @param[in] sn 序列号。
 * @param[in] type 数据类型， @ref UHSD_FT_TYPE_E 。
 * @param[in] data 具体的type对应的数据。
 * @param[in] data_len 具体的type对应的数据长度。
 * @param[in] traceID 链式跟踪ID。
 * @return uhsd_u32 发送结果。
 * @retval 0 成功。
 * @retval <0 失败。
 */
typedef uhsd_s32 (*uhsd_ft_channel_up_callback)(uhsd_s32 sn, uhsd_u32 type, uhsd_u8 *data, uhsd_u32 data_len, const uhsd_char *traceId, uhsd_void *user_data);

/**
 * @brief 产测上行数据通道注册
 * @details
 * 设备APP通过注册此接口，来获取到OSDK->devAPP的产测相关数据。
 *
 * @param[in] ft_chl_up_cb 上行数据回调。
 * @param[in] user_data 用户数据指针。 @ref uhsd_ft_channel_up_callback 回调时返回。
 * @return uhsd_s32 注册结果。
 * @retval 0 成功
 * @retval <0 异常 @ref UHSD_ERROR_NUM
 *
 */
UHSD_API uhsd_s32 uhsd_ft_channel_up_register(uhsd_ft_channel_up_callback ft_chl_up_cb, uhsd_void *user_data);

/**
 * @brief 发送产测下行数据
 * @param[in] sn 序列号。
 * @param[in] type 数据类型， @ref UHSD_FT_TYPE_E 。
 * @param[in] data 具体的type对应的数据。
 * @param[in] data_len 具体的type对应的数据长度。
 * @param[in] traceID 链式跟踪ID。
 * @return uhsd_u32 发送结果。
 * @retval 0 成功。
 * @retval <0 失败。
 */
UHSD_API uhsd_s32 uhsd_ft_channel_down(uhsd_s32 sn, uhsd_u32 type, uhsd_u8 *data, uhsd_u32 data_len, const uhsd_char *traceID);

#ifdef __cplusplus
}
#endif

#endif

/** @}*/
