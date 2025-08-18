#ifndef __UHSD_DCC_H__
#define __UHSD_DCC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "uhsd_types.h"

/**
 * @brief 分布式控制初始化
 *
 * @param 无
 * @return UHSD_API
 */
UHSD_API uhsd_s32 uhsd_dcc_init(uhsd_void);

/**
 * @brief 创建监听者
 *
 * @param 无
 * @return 成功返回非负整数的监听者句柄,失败返回<0
 * @note 目前最大支持5个监听者
 */
UHSD_API uhsd_dcc_listener_t uhsd_dcc_listener_new(void);

/**
 * @brief 删除监听者
 *
 * @param ctx 监听者句柄
 * @return 成功返回0,失败返回<0
 */
UHSD_API uhsd_s32 uhsd_dcc_listener_free(uhsd_dcc_listener_t ctx);

/**
 * @brief 订阅结果通知回调
 *
 * @param deviceId 设备唯一标识
 * @param err_code 错误码，成功为0，失败为<0
 * @param cb_param 回传参数
 * @return 无
 */
typedef uhsd_void (*uhsd_dcc_device_sub_result_cb)(const uhsd_char *deviceId, uhsd_s16 err_code, uhsd_void *cb_param);

/**
 * @brief 订阅设备
 *
 * @param ctx 监听者句柄
 * @param deviceId 设备唯一标识
 * @param cb 订阅结果通知回调
 * @param cb_param 回传参数
 * @return 成功返回0,失败返回<0
 */
UHSD_API uhsd_s32 uhsd_dcc_device_sub(uhsd_dcc_listener_t ctx, const uhsd_char *deviceId, uhsd_dcc_device_sub_result_cb cb, uhsd_void *cb_param);

/**
 * @brief 解订阅结果通知回调
 *
 * @param deviceId 设备唯一标识
 * @param err_code 错误码，成功为0，失败为<0
 * @param cb_param 回传参数
 * @return 无
 */
typedef uhsd_void (*uhsd_dcc_device_unsub_result_cb)(const uhsd_char *deviceId, uhsd_s16 err_code, uhsd_void *cb_param);

/**
 * @brief 解订阅设备
 *
 * @param ctx 监听者句柄
 * @param deviceId 设备唯一标识
 * @param cb 解订阅结果通知回调
 * @param cb_param 回传参数
 * @return 成功返回0,失败返回<0
 */
UHSD_API uhsd_s32 uhsd_dcc_device_unsub(uhsd_dcc_listener_t ctx, const uhsd_char *deviceId, uhsd_dcc_device_unsub_result_cb cb, uhsd_void *cb_param);

/**
 * @brief 属性通知回调函数
 *
 * @param deviceId 设备唯一标识
 * @param pairs 属性键值对数组
 * @param pairs_num 属性键值对个数
 * @param cb_param 回传参数
 * @return 无
 */
typedef uhsd_void (*uhsd_dcc_device_property_notify_cb)(const uhsd_char *deviceId, uhsd_dev_pair_t pairs[], uhsd_u32 pairs_num, uhsd_void *cb_param);

/**
 * @brief 设置属性通知回调函数
 *
 * @param ctx 监听者句柄
 * @param cb 属性通知回调
 * @param cb_param 回传参数
 * @return 成功返回0,失败返回<0
 */
UHSD_API uhsd_s32 uhsd_dcc_set_opt_listen_property(uhsd_dcc_listener_t ctx, uhsd_dcc_device_property_notify_cb cb, uhsd_void *cb_param);

/**
 * @brief 设备发现通知回调
 *
 * @param deviceId 设备唯一标识
 * @param status 发现状态 1:发现设备, 0：删除设备
 * @param cb_param 回传参数
 * @return 无
 */
typedef uhsd_void (*uhsd_dcc_device_found_notify_cb)(const uhsd_char *deviceId, uhsd_s32 status, uhsd_void *cb_param);

/**
 * @brief 设置设备发现通知回调
 *
 * @param ctx 监听者句柄
 * @param cb 设备发现通知回调
 * @param cb_param 回传参数
 * @return 成功返回0,失败返回<0
 */
UHSD_API uhsd_s32 uhsd_dcc_set_opt_listen_device_found(uhsd_dcc_listener_t ctx, uhsd_dcc_device_found_notify_cb cb, uhsd_void *cb_param);

/**
 * @brief 设备连接状态通知回调
 *
 * @param deviceId 设备唯一标识
 * @param status 连接状态 251:连接成功，252:连接断开
 * @param cb_param 回传参数
 * @return 无
 */
typedef uhsd_void (*uhsd_dcc_device_conn_status_notify_cb)(const uhsd_char *deviceId, uhsd_s16 status, uhsd_void *cb_param);

/**
 * @brief 设置设备连接状态通知回调
 *
 * @param ctx 监听者句柄
 * @param cb 设备连接状态通知回调
 * @param cb_param 回传参数
 * @return 成功返回0,失败返回<0
 */
UHSD_API uhsd_s32 uhsd_dcc_set_opt_listen_device_conn_status(uhsd_dcc_listener_t ctx, uhsd_dcc_device_conn_status_notify_cb cb, uhsd_void *cb_param);

/**
 * @brief 设备重启通知回调
 * @note
 * @attention
 * @param[in] err_code 重启原因，暂时可不关注
 * @return 无
 */
typedef uhsd_void (*uhsd_dcc_device_need_restart_notify_cb)(uhsd_s16 err_code);

/**
 * @brief 设备重启通知
 * @note devApp收到此通知时，需要重新启动
 * @attention 无
 * @param[in] cb 设备重启通知回调
 * @return 成功返回0,失败返回<0
 */
uhsd_s32 uhsd_dcc_device_need_restart_notify_sub(uhsd_dcc_device_need_restart_notify_cb cb);

/**
 * @brief 批控设备结果通知回调
 *
 * @param sn 请求唯一标识
 * @param err_code 错误码，成功为0,失败为非0
 * @param reason 失败原因，json字符串；err_code非0时有效
 * @param cb_param 回传参数
 * @return 无
 */
typedef uhsd_void (*uhsd_dcc_batch_device_ctrl_result_cb)(uhsd_s32 sn, uhsd_s32 err_code, const uhsd_char *reason, uhsd_void *cb_param);

/**
 * @brief 批控设备
 *
 * @param req_sn 请求唯一标识
 * @param cmd 批控命令 json字符串
 * @param result_cb 批控设备结果通知回调
 * @param cb_param 回传参数
 * @return 成功返回0,失败返回<0
 */
UHSD_API uhsd_s32 uhsd_dcc_batch_device_ctrl(uhsd_s32 req_sn, const uhsd_char *cmd, uhsd_dcc_batch_device_ctrl_result_cb result_cb, uhsd_void *cb_param);

/**
 * @brief 查询设备是否已发现
 *
 * @param deviceId 设备唯一标识
 * @return 成功返回1,失败返回0
 */
UHSD_API uhsd_s32 uhsd_dcc_is_device_found(const uhsd_char *deviceId);

#ifdef __cplusplus
}
#endif

#endif /*__UHSD_DCC_H__*/