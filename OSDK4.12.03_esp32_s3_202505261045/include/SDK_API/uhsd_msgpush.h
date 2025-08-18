/**
 * @defgroup grp_uhsdmsgpush 消息推送接口
 * @{
 * @author hudx (hudingxuan.xian@haier.com)
 * @brief 消息推送功能接口
 * @version 0.1
 * @date 2022-08-27
 *
 * @copyright Copyright (c) 2021, Haier.Co, Ltd.
 *
 * @file uhsd_msgpush.h
 */

#ifndef __UHSD_MSGPUSH_H
#define __UHSD_MSGPUSH_H
#include "uhsd_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief style
 *
 */
#define UHSD_MSGPUSH_STYLE_VOICE  0x01 /**< voice */
#define UHSD_MSGPUSH_STYLE_ACTION 0x02 /**< action */
#define UHSD_MSGPUSH_STYLE_DIALOG 0x03 /**< voice and action */

/**
 * @brief actionType
 *
 */
#define UHSD_MSGPUSH_ACTION_TYPE_DELIVERDATA 0x01 /**< deliverData 512 */

/**
 * @brief message status
 */
typedef enum
{
    UHSD_MSGPUSH_STATUS_DELIVERED = 1, // APP
    UHSD_MSGPUSH_STATUS_READ,          // APP
    UHSD_MSGPUSH_STATUS_FAILED,        // APP、server、
    UHSD_MSGPUSH_STATUS_DONE,          // APP
    UHSD_MSGPUSH_STATUS_ACCEPTED,      // 已受理   // 消息中心APP
    UHSD_MSGPUSH_STATUS_CLICKED,       // 已点击   // 消息中心APP 分发.
} message_status_e;                    /* referenced from MCS_CLIENT_MESSAGE_STATUS in SDK4_003_00_dev */

typedef struct
{
    uhsd_s32 style;           /**< 消息交互方式。 @ref UHSD_MSGPUSH_STYLE_XXX */
    uhsd_s32 action;          /**< 仅支持 deliverdata */
    uhsd_s8 timezone[64 + 1]; /**< 时区. eg: Asia/Shanghai 用户设置. 详细 iot平台获取 */
    uhsd_s8 language[64 + 1]; /**< 语言. eg: zh-cn         用户设置. 详细 iot平台获取 */
    uhsd_s32 sn;              /**< 序列号. 是消息业务用的，是APP生成的, 标识APP的这次业务请求, 与应答时候的sn一致。*/
} uhsd_msgpush_voice_param_t;

/**
 * @brief 注册消息回调
 * @param[in] dev_id  设备ID（DeviceID）
 * @param[in] dev_pushid 消息客户端标识.
 * @param[in] sn 应与注册时的sn相同，注册消息标识。
 * @param[in] code 错误码
 * @return 成功返回0，失败返回<0
 */
typedef uhsd_s32 (*uhsd_msgpush_register_msg_ack_cb)(uhsd_u8 *dev_id, uhsd_s32 sn, uhsd_s8 *dev_pushid, const uhsd_s8 *code);

/**
 * @brief 注销消息推送 回调
 * @param[in] dev_id  设备ID（D e v ice ID
 * @param[in] dev_pushid 消息客户端标识.
 * @param[in] sn 与注销时的sn相同，注销消息标识。
 * @param[in] code 错误码
 * @return 成功返回0，失败返回<0
 */
typedef uhsd_s32 (*uhsd_msgpush_unregister_msg_ack_cb)(uhsd_u8 *dev_id, uhsd_s32 sn, uhsd_s8 *dev_pushid, const uhsd_s8 *code);

/**
 *  @brief 消息推送下发回调
 *  @param[in] dev_pushid 消息客户端标识
 *  @param[in] server_pushid 消息中心标识
 *  @param[in] data     数据内容   [json格式] 具体的格式需要与推送方协调.
 *  @return         成功返回0, 失败返回<0
 */
typedef uhsd_s32 (*uhsd_msgpush_msg_receive_cb)(uhsd_s8 *server_pushid, uhsd_s8 *dev_pushid, uhsd_s8 *msgid, uhsd_s8 *data);

/**
 * @brief 回调接口注册 需要注册回调函数后才能使用消息服务
 * @param[in] uhsd_msgpush_register_msg_ack_cb 注册消息应答接口
 * @param[in] uhsd_msgpush_msg_receive_cb 消息推送接口
 * @param[in]  uhsd_msgpush_unregister_msg_ack_cb  注销 应答接口， 返回注销的结果。
 * @return 成功返回0，失败返回<0
 */
UHSD_API uhsd_s32 uhsd_msgpush_register_callback(uhsd_msgpush_register_msg_ack_cb register_msg_ack_cb,
                                                 uhsd_msgpush_msg_receive_cb msg_receive_cb,
                                                 uhsd_msgpush_unregister_msg_ack_cb unregister_msg_ack_cb);

/**
 * @brief 注册消息 需要注册消息后才能使用消息服务
 * @param[in] dev_id  设备ID（DeviceID）
 * @msg_param
 * ------@style 消息交互方式 (notification 、toast 、dialog、voice、action) 目前支持 voice、action。
 *           仅 voice ----   1
 *           仅 action ----  2
 *             voice + action ------- 3
 * ------@action  如果style中支持action交互方式， action功能开启。 目前支持 deliverData
 *             deliverData --------- 1
 * ------@timezone 时区. eg: Asia/Shanghai
 * ------@language 语言. eg: zh-cn
 * ------@sn 是消息业务用的，是用户生成的, 标识用户的这次业务, 与应答时候的sn一致。
 *
 * @return 成功返回0，失败返回<0
 */
UHSD_API uhsd_s32 uhsd_msgpush_register_msg(uhsd_u8 *dev_id, uhsd_msgpush_voice_param_t *msg_param);

/**
 * @brief  注销 消息推送 服务
 * @param[in]  dev_id  设备ID（DeviceID）
 * @param[in]  dev_pushid  消息推送的 端的标识。 对应注册消息推送功能返回的pushid。
 * @param[in]  sn 是消息业务用的，是用户生成的, 标识用户的这次业务, 与应答时候的sn一致。
 * @return 成功返回0，失败返回<0
 */
UHSD_API uhsd_s32 uhsd_msgpush_unregister_msg(uhsd_u8 *dev_id, const uhsd_s8 *dev_pushid, uhsd_s32 sn);

/**
 * @brief 播放语音后，调用该接口上报已读状态
 * @param[in] dev_id  设备ID（DeviceID）
 * @param[in] dev_pushid 消息推送的 端的标识。 对应注册消息推送功能返回的pushid。
 * @param[in] server_pushid 消息标识，从平台推送的消息体中获取
 * @param[in] msgid 消息标识，从平台推送的消息体中获取
 * @param[in] status 状态类型 deliver----1，read----2
 * @param[in] time yyyy-MM-dd HH:mm:SS  格式
 * @return 成功返回0，失败返回<0
 */
UHSD_API uhsd_s32
uhsd_msgpush_report_msg_status(uhsd_u8 *dev_id, const uhsd_s8 *dev_pushid, const uhsd_s8 *server_pushid, const uhsd_s8 *msgid, uhsd_s32 status, uhsd_s8 *time);

#ifdef __cplusplus
}
#endif

#endif // __UHSD_MSGPUSH_H

/**@} grp_uhsdmsgpush end */