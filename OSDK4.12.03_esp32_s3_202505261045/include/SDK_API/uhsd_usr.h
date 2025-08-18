/**
 * @copyright Copyright (c) 2022, Haier.Co, Ltd.
 * @file uhsd_demo_ota.h
 * @author yzf (yuezhifan@haier.com)
 * @brief demo设计用户app升级
 * @date 2022-11-18
 */

#ifndef __UHSD_USR_H__
#define __UHSD_USR_H__

#include "uhsd_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    UHSD_USR_STATUS_CLOUD_DISCONNECTED,
    UHSD_USR_STATUS_CLOUD_CONNECT_SUCC,
    UHSD_DEV_MAX_STATE,
} uhsd_dev_user_status;

/**
 * @brief lite用户侧连云状态监听回调函数
 *
 */
typedef uhsd_s16 (*uhsd_usr_status_cb)(uhsd_s16 status);
/**
 * @brief uhsd_usr_status_subscribe() 注册用户侧连云状态监听回调函数
 * @param status 需要监听的状态，见结构体uhsd_dev_user_status
 * @param cb 回调函数
 *
 */
#define uhsd_usr_status_subscribe(status, cb)   uhsd_usr_status_subscribe_for_##status(cb);
#define DECLARE_USR_STATUS_CALLBACK(status, cb) UHSD_API uhsd_s16 uhsd_usr_status_subscribe_for_##status(cb);

DECLARE_USR_STATUS_CALLBACK(UHSD_USR_STATUS_CLOUD_CONNECT_SUCC, uhsd_usr_status_cb cb)
DECLARE_USR_STATUS_CALLBACK(UHSD_USR_STATUS_CLOUD_DISCONNECTED, uhsd_usr_status_cb cb)

/**
 * @brief uSDK_RTOS 监听用户监听句柄定义
 *
 */
typedef uhsd_u32 uhsd_usr_listener_t;

/**
 * @brief 订阅、解订阅相关的结果键值对定义
 *
 */
typedef struct
{
    uhsd_u8 *deviceId; /* 设备deviceId */
    uhsd_s16 err_code; /* 设备订阅结果；成功为0，失败<0*/
} uhsd_usr_device_sub_unsub_result_t;

/**
 * @brief 设备状态键值对结构体定义
 *
 */
typedef struct
{
    uhsd_u8 *deviceId;
    uhsd_u16 status; // 0 不在线，1在线
} uhsd_device_status_info_t;

/**
 * @brief 设备列表信息对结构体定义
 *
 */
typedef struct
{
    uhsd_u8 *deviceId;
    uhsd_u8 *deviceName;
    uhsd_u8 *typeId;
    uhsd_u8 *deviceRole;
    uhsd_bool isOnline;
} uhsd_device_list_info_t;
/**
 * @brief 属性变化通知回调接口
 * @param ctx 监听者号
 * @param deviceId 发生属性变更的设备deviceid
 * @param pairs 变化的属性
 * @param cb_param 预留参数
 */
typedef void (*uhsd_usr_device_property_notify_cb)(uhsd_usr_listener_t ctx, const uhsd_char *deviceId, uhsd_dev_pair_t pairs[], uhsd_s16 num, void *cb_param);

/**
 * @brief 绑定状态变化通知回调接口
 * @param ctx 监听者号
 * @param deviceId 发生状态变更的设备deviceid
 * @param bind_status 1 绑定，0 解绑
 * @param cb_param 预留参数
 */
typedef void (*uhsd_usr_device_bind_status_notify_cb)(uhsd_usr_listener_t ctx, const uhsd_char *deviceId, uhsd_s16 bind_status, void *cb_param);

/**
 * @brief 上下状态变化通知回调接口
 * @param ctx 监听者号
 * @param deviceId 状态变更的设备deviceid
 * @param online_status 1 上线，0 下线
 * @param cb_param 预留参数
 */
typedef void (*uhsd_usr_device_online_status_notify_cb)(uhsd_usr_listener_t ctx, const uhsd_char *deviceId, uhsd_s16 online_status, void *cb_param);
/**
 * @brief 用户侧订阅/解订阅设备回调接口
 * @param info 订阅/解订阅设备失败列表
 * @param num 订阅/解订阅设备失败数量
 * @param result 订阅/解订阅 结果
 */
typedef void (*uhsd_usr_devices_sub_unsub_result_cb)(uhsd_usr_device_sub_unsub_result_t *info[], uhsd_s16 num, uhsd_s16 result, void *cb_param);
/**
 * @brief 写属性结果回调接口
 * @param deviceId 设备deviceid
 * @param sn 本次操作的sn
 * @param result 0 成功，<0 失败
 * @param errcode 失败时的错误码  0 成功
 * @param cb_param 预留参数
 */
typedef void (*uhsd_usr_device_write_cb)(const uhsd_char *deviceId, uhsd_u32 sn, uhsd_s16 result, uhsd_s16 err_code, void *cb_param);
/**
 * @brief 批量操作属性结果回调接口
 * @param deviceId 设备deviceid
 * @param sn 本次操作的sn
 * @param result 0 成功，<0 失败
 * @param errcode 失败时的错误码  0 成功
 * @param cb_param 预留参数
 */
typedef void (*uhsd_usr_device_op_cb)(const uhsd_char *deviceId, uhsd_u32 sn, uhsd_s16 result, uhsd_s16 err_code, void *cb_param);
/**
 * @brief 获取属性结果回调接口
 * @param deviceId 设备deviceid
 * @param result 0 成功，<0 失败
 * @param errcode 失败时的错误码  0 成功
 * @param pairs 获取到的属性结构体数组
 * @param num 获取到的属性个数
 * @param cb_param 预留参数
 */
typedef void (
    *uhsd_usr_get_device_properties_cb)(const uhsd_char *deviceId, uhsd_s16 result, uhsd_s16 err_code, uhsd_dev_pair_t pairs[], uhsd_s16 num, void *cb_param);
/**
 * @brief 获取上线状态结果回调接口
 * @param result 0 成功，<0 失败
 * @param errcode 失败时的错误码  0 成功
 * @param info_list 获取到的设备上线状态数组
 * @param num 获取到的属性个数
 * @param cb_param 预留参数
 */
typedef void (*uhsd_usr_get_devices_status_cb)(uhsd_s16 result, uhsd_s16 err_code, uhsd_device_status_info_t info_list[], uhsd_u16 num, void *cb_param);
/**
 * @brief 获取设备列表结果回调接口
 * @param result 0 成功，<0 失败
 * @param errcode 失败时的错误码  0 成功
 * @param info_list 获取到的设备列表数组
 * @param num 获取到的属性个数
 * @param cb_param 预留参数
 */
typedef void (*uhsd_usr_get_devices_list_cb)(uhsd_s16 result, uhsd_s16 err_code, uhsd_device_list_info_t info_list[], uhsd_u16 num, void *cb_param);
/**
 * @brief 获取设备支持的品类数字模型结果回调接口
 * @param deviceId 设备deviceid
 * @param result 0 成功，<0 失败
 * @param errcode 失败时的错误码  0 成功
 * @param digital_info 频率数字模型
 * @param cb_param 预留参数
 */
typedef void (
    *uhsd_usr_category_digital_model_query_cb)(const uhsd_char *deviceId, uhsd_s16 result, uhsd_s16 err_code, uhsd_char *digital_info, void *cb_param);
/**
 * @brief 获取设备支持的数字模型结果回调接口
 * @param deviceId 状态变更的设备deviceid
 * @param result 0 成功，<0 失败
 * @param errcode 失败时的错误码  0 成功
 * @param digital_info 频率数字模型
 * @param cb_param 预留参数
 */
typedef void (*uhsd_usr_support_digital_model_query_cb)(const uhsd_char *deviceId, uhsd_s16 result, uhsd_s16 err_code, uhsd_char *digital_info, void *cb_param);
/**
 * @brief lite用户侧初始化接口
 */
UHSD_API uhsd_s16 uhsd_usr_init(void);
/**
 * @brief lite用户侧设置用户侧参数接口
 */
UHSD_API uhsd_s16 uhsd_usr_set_app_info(const uhsd_char *appId, const uhsd_char *appKey, const uhsd_char *appVer);
/**
 * @brief lite用户侧启动参数接口
 * @param user_token 用户token 和clientid 相匹配
 * @param clientId
 */
UHSD_API uhsd_s16 uhsd_usr_start(const uhsd_char *user_token, const uhsd_char *clientId);
/**
 * @brief lite用户侧更新用户token接口
 * @param user_token 用户token必须要和clientid 相匹配
 */
UHSD_API uhsd_s16 uhsd_usr_update_token(const uhsd_char *user_token);
/**
 * @brief lite用户侧创建监听者接口
 */
UHSD_API uhsd_usr_listener_t uhsd_usr_listener_new(void);
/**
 * @brief lite用户侧删除监听者接口
 */
UHSD_API void uhsd_usr_listener_delete(uhsd_usr_listener_t ctx);
/**
 * @brief lite用户侧设置属性变更，绑定状态变更，上线状态变更回调接口
 * @param ctx 对应的监听者id
 * @param property_notify_cb 属性变更通知回调
 * @param property_notify_cb_param 属性变更通知回调参数
 * @param dev_bind_status_cb 绑定状态变更通知回调
 * @param dev_bind_status_cb_param 绑定状态变更通知回调参数
 * @param dev_online_status_cb 上线状态变化通知回调
 * @param dev_online_status_cb_param 上线状态变化通知回调参数
 */
UHSD_API uhsd_s16 uhsd_usr_set_listener_notify_callback(uhsd_usr_listener_t ctx,
                                                        uhsd_usr_device_property_notify_cb property_notify_cb,
                                                        void *property_notify_cb_param,
                                                        uhsd_usr_device_bind_status_notify_cb dev_bind_status_cb,
                                                        void *dev_bind_status_cb_param,
                                                        uhsd_usr_device_online_status_notify_cb dev_online_status_cb,
                                                        void *dev_online_status_cb_param);
/**
 * @brief 用户侧订阅设备接口
 * @param ctx 监听者
 * @param deviceId[] 订阅设备列表
 * @param num订阅设备数量，设备app需要根据终端设备能力进行限制最大订阅设备数量，防止订阅设备过多导致程序运行内存不足崩溃
 * @param result_cb 操订阅操作结果回调
 * @param result_cb_param 传入的回调参数
 */
UHSD_API uhsd_s16 uhsd_usr_devices_subscribe(uhsd_usr_listener_t ctx,
                                             const uhsd_char *deviceId[],
                                             int num,
                                             uhsd_usr_devices_sub_unsub_result_cb result_cb,
                                             void *result_cb_param);
/**
 * @brief 用户侧取消订阅设备接口
 * @param ctx 监听者
 * @param deviceId[] 取消订阅设备列表
 * @param num订阅设备数量，
 * @param result_cb 操订阅操作结果回调
 * @param result_cb_param 传入的回调参数
 */
UHSD_API uhsd_s16 uhsd_usr_devices_unsubscribe(uhsd_usr_listener_t ctx,
                                               const uhsd_char *deviceId[],
                                               int num,
                                               uhsd_usr_devices_sub_unsub_result_cb result_cb,
                                               void *result_cb_param);
/**
 * @brief 用户侧写属性接口
 * @param deviceId 设备deviceid
 * @param num订阅设备数量，
 * @param result_cb 操订阅操作结果回调
 * @param result_cb_param 传入的回调参数
 */
UHSD_API uhsd_s16
uhsd_usr_device_write_asyn(const uhsd_char *deviceId, uhsd_u32 sn, const uhsd_char *name, const uhsd_char *value, uhsd_usr_device_write_cb cb, void *cb_param);
/**
 * @brief 用户侧批量操作属性接口
 * @param deviceId 设备deviceid
 * @param sn 本次操作的sn 必须大于0
 * @param op_name 批操作类型 例如：getAllProperty getAllAlarm
 * @param op_args 需要操作的属性数组 可以为空
 * @param num 需要操作的属性数目 可以为0
 * @param cb 批操作结果回调
 * @param cb_param 传入的回调参数
 */
UHSD_API uhsd_s16 uhsd_usr_device_op_asyn(const uhsd_char *deviceId,
                                          uhsd_u32 sn,
                                          const uhsd_char *op_name,
                                          uhsd_dev_pair_t op_args[],
                                          uhsd_s16 num,
                                          uhsd_usr_device_op_cb cb,
                                          void *cb_param);
/**
 * @brief 用户侧获取设备属性接口
 * @param deviceId 设备deviceid
 * @param properties_name 需要获取的属性数组
 * @param num 需要获取的属性的数量，
 * @param cb 操作结果回调
 * @param cb_param 传入的回调参数
 */
UHSD_API uhsd_s16
uhsd_usr_get_device_properties(const uhsd_char *deviceId, uhsd_char *properties_name[], uhsd_s16 num, uhsd_usr_get_device_properties_cb cb, void *cb_param);
/**
 * @brief 用户侧获取设备上线状态接口
 * @param deviceId 设备deviceid列表
 * @param num 需要获取设备上线状态个数，deviceId数组的个数
 * @param cb 操作结果回调
 * @param cb_param 传入的回调参数
 */
UHSD_API uhsd_s16 uhsd_usr_get_devices_online_status(const uhsd_char *deviceIds[], uhsd_s16 num, uhsd_usr_get_devices_status_cb cb, void *cb_param);
/**
 * @brief 用户侧获取设备列表接口
 * @param num 0就是获取所有设备
 * @param deviceId 设备deviceid列表，num为0时deviceId为空
 * @param cb 操作结果回调
 * @param cb_param 传入的回调参数
 */
UHSD_API uhsd_s16 uhsd_usr_get_devices_list(uhsd_s16 num, const uhsd_char *deviceIds[], uhsd_usr_get_devices_list_cb cb, void *cb_param);
/**
 * @brief 用户侧查询支持品类数字模型接口
 * @param deviceId 徐要查询的设备deviceid
 * @param cb 操作结果回调
 * @param cb_param 传入的回调参数
 */
UHSD_API uhsd_s16 uhsd_usr_category_digital_model_query(const uhsd_char *deviceId, uhsd_usr_category_digital_model_query_cb cb, void *cb_param);
/**
 * @brief 用户侧查询支持数字模型接口
 * @param deviceId 徐要查询的设备deviceid
 * @param cb 操作结果回调
 * @param cb_param 传入的回调参数
 */
UHSD_API uhsd_s16 uhsd_usr_support_digital_model_query(const uhsd_char *deviceId, uhsd_usr_support_digital_model_query_cb cb, void *cb_param);
#ifdef __cplusplus
}
#endif

#endif