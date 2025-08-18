#ifndef __UHSD_MCS_H__
#define __UHSD_MCS_H__

#include <uhsd_types.h>

#ifdef __cplusplus
extern "C" {
#endif
// client
#define UHSD_MCS_CLIENT_PARAM_ERR             -73000
#define UHSD_MCS_CLIENT_NO_INIT_ERR           -73001
#define UHSD_MCS_CLIENT_REPEAT_INIT_ERR       -73002
#define UHSD_MCS_CLIENT_NO_CONNECT_SERVER_ERR -73003
#define UHSD_MCS_CLIENT_TIMEOUT_ERR           -73004
#define UHSD_MCS_CLIENT_MSG_STATUS_ERR        -73005 // 这部分状态不对该APP开放
// server
#define UHSD_MCS_SERVER_COMMON_ERR               -73020 // 组json失败、
#define UHSD_MCS_SERVER_PARAM_ERR                -73021
#define UHSD_MCS_SERVER_MALLOC_ERR               -73022
#define UHSD_MCS_SERVER_REINIT_ERR               -73023
#define UHSD_MCS_SERVER_NO_REGISTER_ERR          -73024 // 没有注册, 没有找到映射关系,   没有找到消息.  没有找到映射关系. 可能是内部丢了
#define UHSD_MCS_SERVER_REPEATED_REGISTER_ERR    -73025 // 相同的注册, style等, 已经注册过了.
#define UHSD_MCS_SERVER_RESP_NO_FIND_SESSION_ERR -73026 // 应答找不到session
#define UHSD_MCS_SERVER_RESP_JSON_PAESE_ERR      -73027 // 解析resp json 失败.错误的resp 应答json格式. 没有映射关系..
#define UHSD_MCS_SERVER_RESP_CODE_FAILED_ERR     -73028 // iot平台的code不是“00000”. 有应答, 但是是错误的.
#define UHSD_MCS_SERVER_MSG_STATUS_ERR           -73029 // 错误的message status 类型.

/* uistyle */
#define UHSD_MCS_CLIENT_STYLE_VOICE        0x01 // voice
#define UHSD_MCS_CLIENT_STYLE_ACTION       0x02 // action
#define UHSD_MCS_CLIENT_STYLE_DIALOG       0x04 //
#define UHSD_MCS_CLIENT_STYLE_TOAST        0x08
#define UHSD_MCS_CLIENT_STYLE_NOTIFICATION 0x10 // --

/* actionType */
#define UHSD_MCS_CLIENT_ACTION_TYPE_OPENAPP       0x01  // openApp
#define UHSD_MCS_CLIENT_ACTION_TYPE_OPENPAGE      0x02  // openPage
#define UHSD_MCS_CLIENT_ACTION_TYPE_OPENLINK      0x04  // openLink
#define UHSD_MCS_CLIENT_ACTION_TYPE_CALLAPI       0x08  // callApi
#define UHSD_MCS_CLIENT_ACTION_TYPE_CONTROLDEVICE 0x10  // controlDevice
#define UHSD_MCS_CLIENT_ACTION_TYPE_CONTROLSCENE  0x20  // controlScene
#define UHSD_MCS_CLIENT_ACTION_TYPE_DOWNLOADFILE  0x40  // downloadFile
#define UHSD_MCS_CLIENT_ACTION_TYPE_ACTIVATEAPP   0x80  // activateApp
#define UHSD_MCS_CLIENT_ACTION_TYPE_DELIVERDATA   0x100 // deliverData   512
typedef enum
{
    UHSD_MCS_GATEWAY_CONNECTING = -3,    // gateway connecting, local connected
    UHSD_MCS_GATEWAY_NOT_CONNECTED = -2, // gateway not connected, local connected
    UHSD_MCS_LOCAL_NOT_CONNECTED = -1,   // local not connected
    UHSD_MCS_CONNECTED = 0               // both local and gateway connected
} uhsd_mcs_client_connect_state_e;
/**
 * @brief 初始化消息分发能力.
 * 1、先uhos初始化, 然后调用此接口, 再调用客户端初始化接口, .... 再添加设备, 连接uhos云...[防止消息丢失, 防止UMS服务是否可用状态通知不及时]
 * 2、devid是即将添加的设备. 不限制设备类型.
 * 3、将 创建(读取)uhsd_mcs.txt. !!!!! 此文件不可以手动删除、修改.
 * 4、将监听 56890 端口.
 *
 * 不支持重复调用, 再提供一个重启服务的接口.
 * @param devid 用于消息推送功能的devid.
 * @param path  存放映射关系文件「uhsd_mcs.txt」的路径, 需要有读写权限.
 */
UHSD_API uhsd_s32 uhsd_mcs_server_init(uhsd_char *devid, uhsd_char *path);

typedef uhsd_s32 (*uhsd_mcs_server_uninit_cb)(uhsd_s32 result);

/**
 * @brief server反初始化
 * @param cb 反初始化结果回调
 * @return 成功返回 0, 失败返回 < 0
 */
UHSD_API uhsd_s32 uhsd_mcs_server_uninit(uhsd_mcs_server_uninit_cb cb);

/* 设备注册参数 */
typedef struct
{
    uhsd_char traceid[32 + 1]; // 链式跟踪
    uhsd_char timezone[32];
    uhsd_char language[32];
    uhsd_char deviceid[32]; // 用于消息推送功能的devid
    uhsd_char appid[48];    // register_type=1 从4.3.4版本后不用填写 register_type=0 需要填写
    uhsd_char appver[20];   // 应用version register_type=0 需要填写
    uhsd_char appkey[48];   // 应用key,用于注册加密用 register_type=0 需要填写
    uhsd_u32 uistyle;       // 见上 uistyle
    uhsd_u32 actiontype;    // 见上 actionType
    uhsd_u32 sn;
    uhsd_s32 register_type; // 0 --- deviceApp, 1 --- device
} uhsd_mcs_client_register_device_params_t;

/**
 * @brief 消息推送 --- 设备APP接收推送消息
 * @note 设备APP 接收推送消息(平台/推送方)的回调
 * @param data message数据. 将完整数据传给APP.
 * @param pushid  client端 唯一标识.
 * @return 成功返回0，失败返回<0
 */
typedef uhsd_s32 (*uhsd_mcs_client_receive_message_cb)(uhsd_char *data, uhsd_char *msgid, uhsd_char *pushid, uhsd_char *from_pushid);

/**
 * @brief client初始化结果回调
 * @param result, 0表示成功,失败为负数
 */
typedef uhsd_s32 (*uhsd_mcs_client_init_cb)(uhsd_s32 result);

/**
 * @brief 本地client和云端连接状态回调
 * @note 本地client和云端连接成功了才能正常收发信息
 * @param conn_status,连接状态值, 见@uhsd_mcs_client_connect_state_e
 */
typedef uhsd_s32 (*uhsd_mcs_conn_status_cb)(uhsd_s32 conn_status);
/**
 * @brief 初始化 MCS Client, APP 在 MCS Client注册接收平台推送的回调.
 * @note 1、初始化消息接收能力. !!! 依赖项见 server init 接口.     「待优化」
 *       2、建立客户端[MCS Server的客户端]  重连机制 1S/一次 [无]
 *       3、连server不成功, client将一直尝试连接. [无]
 * @param
 *      local_clinetId 多个clinet时保证local_clientid 不同，多client时不能用devId
 *      receive_message_cb  接受推送消息的回调.
 *      client_init_cb      clinet端初始化回调.
 *      client_connet_state_cb  clinet端和server端连接状态回调
 * @return 成功返回 0, 失败返回 < 0;
 */
UHSD_API uhsd_s32 uhsd_mcs_client_init(uhsd_char *local_clinetId,
                                       uhsd_mcs_client_receive_message_cb receive_message_cb,
                                       uhsd_mcs_client_init_cb client_init_cb,
                                       uhsd_mcs_conn_status_cb client_connet_state_cb);

/**
 * @brief client反初始化结果回调
 * @param result, 0表示成功,失败为负数
 */
typedef uhsd_s32 (*uhsd_mcs_client_deInit_result_cb)(uhsd_s32 result);

/**
 * @brief client 反初始化
 * @param cb
 * @return uhsd_s32
 */
UHSD_API uhsd_s32 uhsd_mcs_client_deInit(uhsd_mcs_client_deInit_result_cb result_cb);

/**
 * @brief 消息推送 ---- 设备APP注册应答
 * @note 接收设备APP注册消息的应答消息中，获取注册结果.
 * @param sn      与注册时的sn相同
 * @param code    错误码
 * @param pushid  client端唯一标识
 * @return 成功返回0，失败返回<0
 */
typedef uhsd_s32 (*mcs_client_register_dev_cb)(uhsd_u32 sn, uhsd_s32 code, uhsd_char *pushid);

/**
 * @brief 消息推送 --- 设备APP注册
 * @note 向设备APP提供APP注册接口，设备APP调用该接口、传递客户端信息及能力
 * @param mcs_client_param
 * ------@register_type 注册的类型.[0 -- devApp, 1 -- device]
 *          devApp 需要appid信息. device 将devid填入appid传入.
 * ------@traceid 用于全局唯一标识该笔业务,格式为定长32字符,可使用字符集为数字与小写字母
 * ------@uistyle 消息交互方式 (notification 、toast 、dialog、voice、action)。   [UHOS全部支持, 具体支持的业务类型需咨询平台]
 * ------@actiontype  见 actionType. style == action时的一种或多种的任意组合.      [UHOS全部支持, 具体支持的业务类型需咨询平台]
 * ------@timezone 时区. eg: Asia/Shanghai 用户设置. 详细 iot平台获取
 * ------@language 语言. eg: zh-cn         用户设置. 详细 iot平台获取
 * ------@sn 是消息业务用的，是APP生成的, 标识APP的这次业务请求, 与应答时候的sn一致。
 *
 * @param register_cloud_msg_ack 注册消息应答接口
 * @return 成功返回0，失败返回<0
 */
UHSD_API uhsd_s32 uhsd_mcs_client_register_dev(uhsd_mcs_client_register_device_params_t *mcs_client_param, mcs_client_register_dev_cb register_dev_cb);

/**
 * @brief 消息推送 --- 设备APP注销应答
 * @note 接收设备APP注销消息的应答消息中，获取注册结果.
 * @param sn      与注册时的sn相同
 * @param code    错误码
 * @param pushid  client端唯一标识
 * @return 成功返回0，失败返回<0
 */
typedef uhsd_s32 (*mcs_client_unregister_dev_app_cb)(uhsd_u32 sn, uhsd_s32 code, uhsd_char *pushid);

/**
 * @brief  消息推送 --- 设备、设备APP 注销
 * @param appid 从4.3.4版本后不用填写
 * @param  unregister_cloud_msg_ack  注销应答，返回注销的结果。
 * @return 成功 0, 失败 < 0;
 */
UHSD_API uhsd_s32 uhsd_mcs_client_unregister_dev(uhsd_char *appid,
                                                 uhsd_char *appver,
                                                 uhsd_char *device_id,
                                                 mcs_client_unregister_dev_app_cb unregister_dev_app_ack);

typedef struct
{
    uhsd_char appid[48];     // 需要填写
    uhsd_char appver[20];    // 应用version register_type=0 需要填写
    uhsd_char token[96 + 1]; // 用户信息
    uhsd_u32 sn;
} uhsd_mcs_client_register_user_params_t;
typedef uhsd_s32 (*mcs_client_register_user_cb)(uhsd_u32 sn, uhsd_s32 code, uhsd_char *pushid);

UHSD_API uhsd_s32 uhsd_mcs_client_register_user(uhsd_mcs_client_register_user_params_t *mcs_client_param, mcs_client_register_user_cb register_user_cb);

UHSD_API uhsd_s32 uhsd_mcs_client_unregister_user(uhsd_mcs_client_register_user_params_t *mcs_client_param, mcs_client_register_user_cb unregister_user_cb);

typedef enum
{
    // CLIENT_MESSAGE_STATUS_DELIVERED = 1, // APP 1是已读，2才是已送达
    CLIENT_MESSAGE_STATUS_READ = 1,  // APP
    CLIENT_MESSAGE_STATUS_DELIVERED, // APP
    CLIENT_MESSAGE_STATUS_FAILED,    // APP、server、
    CLIENT_MESSAGE_STATUS_DONE,      // APP
    CLIENT_MESSAGE_STATUS_ACCEPTED,  // 已受理   // 消息中心APP
    CLIENT_MESSAGE_STATUS_CLICKED,   // 已点击   // 消息中心APP 分发.
    CLIENT_MESSAGE_STATUS_REJECTED,  // 已点击并拒绝 // 消息中心APP 分发.
    CLIENT_MESSAGE_STATUS_TOTAL
} client_message_status_e;

/**
 * @brief 消息推送 --- 上报消息状态
 * @param appid 消息标识
 * @param msgid 消息标识，从推送的消息体中获取
 * @param pushid MCS Client端唯一标识, 从消息中心注册设备信息应答中获取
 * @param from_pushid 消息来源对端的pushid
 * @param status 1 已读(read)，2 已送达(delivered), 3 失败(failed), 4 业务状态回执(done).
 * @param errstr 错误码字符串, 详见《IOT平台UMS错误码》. status == failed, 输入errstr才有意义.
 * @return 成功返回0，失败返回<0
 */
UHSD_API uhsd_s32
uhsd_mcs_client_report_msg_status(uhsd_char *msgid, uhsd_char *pushid, uhsd_char *from_pushid, client_message_status_e status, uhsd_char *errstr);

typedef struct mcs_push_message_header_params
{
    uhsd_char traceid[32 + 1]; // 链式跟踪
    uhsd_char language[32];    // 语言. eg: zh-cn
    uhsd_char timezone[32];    // 时区. eg: Asia/Shanghai
    uhsd_char clientid[48];    // 客户端ID， 主要用途为唯一标识客户端
    uhsd_char appid[48];       // 应用ID    从4.3.4版本后不用填写
    uhsd_char appver[20];      // 应用version 从4.3.4版本后不用填写
    uhsd_char appkey[48];      // 应用key,用于注册加密用 从4.3.4版本后不用填写
} uhsd_mcs_push_message_header_t;
typedef enum
{
    VIDEO_CALL_STARUS_DEFAULT = 0,  // 默认未开始状态
    VIDEO_CALL_STARUS_START = 1,    // 已呼出状态
    VIDEO_CALL_STARUSDELIVERED = 2, // 已呼叫状态 已收到消息中心应答
} uhsd_mcs_video_call_status_e;

/**
 * @brief 门铃呼叫错误码枚举定义
 */
typedef enum
{
    MCS_VIDEO_CALL_ERR_CODE_SUCCESS = 0,         // 请求发送成功。
    MCS_VIDEO_CALL_ERR_CODE_INVALID_PARAMS = -1, // 无效参数
    MCS_VIDEO_CALL_ERR_CODE_BUSY = -2,           // 呼叫服务忙，正在呼叫
    MCS_VIDEO_CALL_ERR_CODE_REQUEST_FAIL = -3,   // 请求失败
    MCS_VIDEO_CALL_ERR_CODE_TIMEOUT = -4,        // 请求超时
    MCS_VIDEO_CALL_ERR_CODE_CALL_TIMEOUT = -5,   // 无人接听呼叫超时
} uhsd_mcs_video_call_err_code_e;

/**
 * @brief 门铃呼叫状态ack回调定义
 */
typedef uhsd_s32 (*mcs_video_call_status_cb)(uhsd_mcs_video_call_err_code_e errcode, uhsd_mcs_video_call_status_e status);

/**
 * @brief 门铃呼叫请求
 * @param deviceId
 * @param header_param
 * @param templateId
 * @param templateArgs
 * @param video_call_status_ack
 * @return uhsd_s32
 */
UHSD_API uhsd_s32 uhsd_mcs_video_call(uhsd_char *deviceId,
                                      uhsd_mcs_push_message_header_t *header_param,
                                      uhsd_char *templateId,
                                      uhsd_char *templateArgs,
                                      mcs_video_call_status_cb video_call_status_ack);

////
////  呼叫消息接口
////

// Defines for call_status
#define UHSD_MCS_MC_CALLSTATUS_IGNORED  -1
#define UHSD_MCS_MC_CALLSTATUS_NONE     0
#define UHSD_MCS_MC_CALLSTATUS_REJECTED 1
#define UHSD_MCS_MC_CALLSTATUS_ANSWERED 2

// Defines for process_type
#define UHSD_MCS_MC_PROCESSTYPE_IGNORE -1
#define UHSD_MCS_MC_PROCESSTYPE_NONE   0
#define UHSD_MCS_MC_PROCESSTYPE_REJECT 1
#define UHSD_MCS_MC_PROCESSTYPE_ANSWER 2

// Defines for code in cb
#define UHSD_MCS_MC_CB_CODE_SUCC           0
#define UHSD_MCS_MC_CB_CODE_TIMEOUT        -1
#define UHSD_MCS_MC_CB_CODE_OP_FAILED      -2
#define UHSD_MCS_MC_CB_CODE_OP_RESP_FAIL   -3
#define UHSD_MCS_MC_CB_CODE_OP_SAME        -4
#define UHSD_MCS_MC_CB_CODE_OP_RPT_FAIL    -5
#define UHSD_MCS_MC_CB_CODE_OP_STATUS_ERR  -6
#define UHSD_MCS_MC_CB_CODE_OP_PROCESS_ERR -7
#define UHSD_MCS_MC_CB_CODE_OP_GEN_ERR     -8

//  呼叫消息结构
struct uhsd_mcs_msgcall_
{
    //消息id 不为空
    uhsd_char *msg_id;

    //对端发起呼叫时间，单位 秒
    uhsd_double call_time;

    //呼叫过期时长，单位 秒
    uhsd_double call_time_out;

    //可能为空
    uhsd_char *other_push_id;
    //可能为空
    uhsd_char *owner_push_id;

    //呼叫消息内容，可能为空
    uhsd_char *call_msg;
    uhsd_u32 call_msg_len;

    //操作结果消息内容，可能为空
    uhsd_char *result_msg;
    uhsd_u32 result_msg_len;

    //呼叫操作类型：0 未操作；-1 忽略； 1 拒接；  2 接听
    uhsd_s32 process_type;

    //呼叫当前状态：0 呼叫中；-1 已忽略；1 已拒接；2 已接听
    uhsd_s32 call_status;
};
typedef struct uhsd_mcs_msgcall_ uhsd_mcs_msgcall_t;

//呼叫回调类型，目前 三类：呼叫消息接收；呼叫消息状态同步；呼叫操作结果异步
typedef enum
{
    //呼叫消息接收回调
    uhsd_msgcall_cbt_call,

    //呼叫消息状态同步回调
    //是来自其他端操作结果的同步消息
    uhsd_msgcall_cbt_status_push,

    //呼叫操作结果异步回调
    //是来自当前端操作结果的回调消息
    uhsd_msgcall_cbt_result_reply,

    uhsd_msgcall_cbt_last
} uhsd_mc_cb_type_t;

//呼叫消息 接收回调，状态同步回调，操作结果异步回调，都通过这个回调
//  uhsd_mcs_msgcall_t *cmsg，消息内容，详见 struct uhsd_mcs_msgcall_
//                            需要在回调中使用该消息，如果要回调外保留该消息内容，在回调返回前拷贝
//  uhsd_s32 code，           正确为0，负数为异常
//  uhsd_mc_cb_type_t cbt     回调类型，见 uhsd_mc_cb_type_t
typedef uhsd_s32 (*uhsd_mc_cb_t)(const uhsd_mcs_msgcall_t *cmsg, uhsd_s32 code, uhsd_mc_cb_type_t cbt);

//启动消息呼叫
// 需要传入 回调函数
// 返回值 >= 0 成功
//        <  0 失败
UHSD_API uhsd_s32 uhsd_msgcall_start(uhsd_mc_cb_t msgcall_cb);

//停止消息呼叫，需要传入启动时相同的回调函数来匹配
// 返回值 >= 0 成功
//        <  0 失败
UHSD_API uhsd_s32 uhsd_msgcall_stop(uhsd_mc_cb_t msgcall_cb);

//呼叫处理函数（用户操作）
// uhsd_char *msg_id:  消息id，不空，标识要操作的消息
// uhsd_s32 process_type: -1 忽略；1拒接；2接听
// 返回值 >= 0 成功
//        <  0 失败
//错误码：-1，失败
//        -2，如参异常
//        -3，状态异常
//        -4，重复操作
//        -5，上报失败
UHSD_API uhsd_s32 uhsd_msgcall_oper(const uhsd_char *msg_id, uhsd_s32 process_type);

////  呼叫消息接口 结束

#ifdef __cplusplus
}
#endif

#endif /* __uhsd_VIDEO_INIT_H__ */
