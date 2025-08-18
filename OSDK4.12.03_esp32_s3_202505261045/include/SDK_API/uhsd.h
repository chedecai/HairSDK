/**
 *
 * @defgroup grp_uhsd 通用接口
 * @{
 * @brief SmartDevice 通用接口
 * @version 0.1
 * @author hudx (hudingxuan.xian@haier.com)
 * @date 2022-08-14
 * @details
 * <pre>
 * @verbatim
 * 功能包括：
 * 1. SmartDevice初始化
 * 2. SmartDevice重置
 * 3. 设备APP重启通知SmartDevice功能
 * 4. SmartDevice版本查询
 * 5. SmartDevice日志控制
 * 6. 时间同步功能
 * 7. 日志上传功能
 * @endverbatim
 * </pre>
 *
 * @file uhsd.h
 *
 * @copyright Copyright (c) 2021, Haier.Co, Ltd.
 *
 */

#ifndef __UHSD_H__
#define __UHSD_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "uhsd_types.h"

/**
 * @brief smartDevice SDK 初始化
 * @details 设备APP必须调用此接口成功后，才可调用SmartDevice提供的任意接口。
 *
 *
 * @return uhsd_s32
 * @retval 0 成功
 * @retval <0 异常 @ref UHSD_ERROR_NUM
 *
 */
UHSD_API uhsd_s32 uhsd_init(uhsd_void);

/**
 * @brief smatDevice SDK设置域名区域
 * @param region 连云环境区域
 *
 * @return uhsd_s32 接口调用结果
 * @retval 0 成功
 * @retval <0 失败
 */
UHSD_API uhsd_s32 uhsd_set_domain_region(uhsd_domain_region_t region);

/**
 * @brief SmartDevice SDK 恢复出厂设置。
 * @details SmartDevice清空当前状态以及SDK存储的数据。
 *
 * @return uhsd_s32 接口调用结果
 * @retval 0 成功。
 * @retval others 失败。
 *
 * @attention
 * 1. 如用户自行重制Flash，可能会导致SmartDevice运行异常。
 * 2. 调用此接口后，建议用户自行重启设备。重启前，需要调用uhsd_reboot_notify接口以通知SmartDevice SDK释放对应资源并上报离线原因。
 * 默认重启时间5秒。
 */
UHSD_API uhsd_s32 uhsd_factory_reset(uhsd_void);

/**
 * @brief 设备重启原因定义
 *
 */
enum
{
    UHSD_REBOOT_REASON_ENTER_CONFIG = 1, /* 进入配置 */
    UHSD_REBOOT_REASON_CLR_USER_CONFIG,  /* 恢复出厂设置 */
    UHSD_REBOOT_REASON_WATCHDOG,         /* 看门狗复位 */
    UHSD_REBOOT_REASON_OTA_RESET,        /* 升级成功复位 */
    UHSD_REBOOT_REASON_DEV_FAIL,         /* 串口故障引起离线 */
    UHSD_REBOOT_REASON_RESET,            /*  自动重启*/
};

/**
 * @brief SmartDevice SDK 设备重启通知
 * @details
 * 设备APP需要进行设备重启时，需通知SmartDevice，以防出现系统异常。
 * SmartDevice系统
 * @param reason 重启原因
 * @param cb SmartDevice确认可以重启后，通过此回调反馈给设备APP。
 * 如果seconds=0，则可以立即重启。否则，需要设备APP延迟相应的秒数后重启。
 * @return uhsd_s32 接口调用结果
 * @retval 0 接口调用成功
 * @retval <0 接口调用异常
 *
 * @attention SDK不会自动重启，需要设备APP收到sd_reboot_cb回调后，自行调用系统接口重启设备。
 */
UHSD_API uhsd_s32 uhsd_reboot_notify(uhsd_s32 reason, uhsd_void (*sd_reboot_cb)(uhsd_u32 seconds));

/**
 * @brief SmartDevice版本查询
 * @details [x.x.xx]
 *
 * @return uhsd_s8*
 */
UHSD_API uhsd_s8 *uhsd_version(uhsd_void);

/**
 * @brief SmartDevice日志控制
 * @details 用户可使用此接口调节SmartDevice的日志级别。
 *
 * @param tag 如果为NULL，则为全局设置，否则为对应的组件名称。 详见 @ref 文档。
 * @param level 0 关闭日志。
 * 1-Verbose
 * 2-Debug
 * 3-Info
 * 4-Warn
 * 5-Error
 * 6-Fatal
 * @return uhsd_s32 接口调用结果
 */
#define UHSD_LOG_OFF           0
#define UHSD_LOG_LEVEL_VERBOSE 1
#define UHSD_LOG_LEVEL_DEBUG   2
#define UHSD_LOG_LEVEL_INFO    3
#define UHSD_LOG_LEVEL_WARN    4
#define UHSD_LOG_LEVEL_ERROR   5
#define UHSD_LOG_LEVEL_FATAL   6
UHSD_API uhsd_s32 uhsd_log_set(const uhsd_char *tag, uhsd_s32 level);

/**
 * @brief 调用此接口获取当前日志级别
 * @return 成功返回当前日志级别，失败返回<0 @ref UHSD_LOG_LEVEL_XXX
 */
UHSD_API uhsd_s32 uhsd_log_get();

/**
 * @brief 函数指针，用户通过注册此函数获取系统时间同步的通知。
 *
 * @details SDSDK=>DEVAPP
 * @param[in]  时间同步结果，如果sec和usec均为0，则此次时间同步失败
 *
 * @return uhsd_void
 *
 */
typedef uhsd_void (*uhsd_datetime_sync_cb)(uhsd_u32 sec, uhsd_u32 usec);

/**
 * @brief SmartDevice时间同步功能【使用wifibase协议时才有效】
 * @details
 * 设备APP通过调用此接口，SmartDevice开始进行时间同步。
 * 同步结果则通过回调通知设备APP。
 * @param[in] func_cb 时间同步回调
 *
 * @return uhsd_s32
 */
UHSD_API uhsd_s32 uhsd_datetime_sync_start(uhsd_datetime_sync_cb func_cb);

/**
 * @brief 获取当前时间
 *
 * @param secs 1970年到现在的秒数
 * @param usec
 * @return uhsd_s32 接口返回值
 * @retval 0 时间已经同步。
 * @retval -1 时间未同步。 需要用户调用 @ref uhsd_datetime_sync_start
 */
UHSD_API uhsd_s32 uhsd_datetime_get(uhsd_u32 *sec, uhsd_u32 *usec);

/**
 * @brief 设置当前时间
 * @details
 * 当用户不需要OSDK进行时间同步时，可使用此接口进行时间设置, 设置的时间为1970年到现在的秒数
 * @param secs 设置时间的秒数整数部分
 * @param usec 设置时间秒数的小数部分，以微秒为单位, 取值范围为[0, 1000000);
 * @return uhsd_s32
 *
 * @attention
 * 如果调用了此接口，则SmartDevice不会自行进行时间同步。
 */
UHSD_API uhsd_s32 uhsd_datetime_set(uhsd_u32 sec, uhsd_u32 usec);

typedef struct
{

    /**
     * @brief 蓝牙广播透传规则
     *
     */
    uhsd_u8 rule_relation; /**< 0: 条件为‘或’关系； 1: 条件为‘与’关系 */

    uhsd_u32 CID_num; /**< CID 数量 */
    uhsd_u16 *cids;

    uhsd_u32 MAC_num; /**< mac 数量*/
    struct
    {
        uhsd_u8 mac_type; /**< 类型：目前为0 */
        uhsd_u8 mac[6];
    } * macs;

    uhsd_u32 serviceUUID_num; /**< UUID数量*/
    struct
    {
        uhsd_u8 uuid_type; /**< 0x00 16bits UUID；0x01 32bits UUID；0x02 128bits UUID */
        uhsd_u8 uuid[16];
    } * uuids;

} uhsd_ble_adv_scan_passthrough_rules_t;

/**
 * @brief 蓝牙广播透传规则设置结果
 * @param[out] result 设置结果：0:成功；1:扫描规则前后矛盾；2:扫描规则解析失败；3:无可用资源；4:扫描规则已执行。
 */
typedef uhsd_s32 (*uhsd_ble_adv_scan_passthrough_rules_set_result_cb)(uhsd_s32 result, uhsd_void *user_data);

/**
 * @brief 蓝牙广播透传数据回调
 * @param[out] data 数据
 * @param[out] data_len 数据长度
 * @param[out] user_data 用户自定义数据返回 @ref uhsd_ble_adv_scan_passthrough_start
 *
 */
typedef uhsd_s32 (*uhsd_ble_adv_scan_passthrough_data_cb)(uhsd_u8 *data, uhsd_u8 *data_len, uhsd_void *user_data);

/**
 * @brief 设置蓝牙广播透传规则
 *
 * @param[in] opt 0: 清除所有规则； 1: 设置规则；
 * @param[in] rules 规则 @ref uhsd_ble_adv_scan_passthrough_rules_t
 * @param[in] result_cb 设置结果
 * @param[in] user_data 自定义数据， @ref uhsd_ble_adv_scan_passthrough_rules_set_result_cb 时返回。
 * @return UHSD_API
 */
UHSD_API uhsd_s32 uhsd_ble_adv_scan_passthrough_rule_set(uhsd_u8 opt, uhsd_ble_adv_scan_passthrough_rules_t *rules, uhsd_void *user_data);

/**
 * @brief 开启蓝牙广播透传功能
 *
 * @param opt
 * @param rules
 * @param result_cb
 * @param user_data
 * @return UHSD_API
 */
UHSD_API uhsd_s32 uhsd_ble_adv_scan_passthrough_start(uhsd_void *user_data);

/**
 * @brief 停止蓝牙广播透传功能
 *
 * @return UHSD_API
 */
UHSD_API uhsd_s32 uhsd_ble_adv_scan_passthrough_stop(uhsd_void);

/**
 * @brief 日志上传配置参数
 *
 */
typedef struct
{
    uhsd_u16 level; // 日志打印级别
    uhsd_u32 time;  // 日志上传间隔时间单位秒
    uhsd_u32 size;  // 日志上传大小单位字节
} uhsd_log_upload_config_t;

/**
 * @brief 设置日志上传参数
 * @param[in] config 日志上传的配置参数
 *
 * @return uhsd_s32
 * @retval -1 失败
 * @retval 0 成功
 */
UHSD_API uhsd_s32 uhsd_log_upload_conifg(uhsd_log_upload_config_t *config);

/**
 * @brief 日志上传状态监听回调函数
 * @param[out] state 0x00-关闭日志上传功能,0x01-打开日志上传功能
 * @param[out] user_data 用户指针
 *
 * @return uhsd_void
 *
 */
typedef uhsd_void (*uhsd_log_upload_status_cb)(uhsd_u8 state, uhsd_void *user_data);

/**
 * @brief 日志上传状态监听注册
 * @param[in] status_cb 状态回调
 * @param[in] user_data 用户数据指针
 *
 * @return uhsd_s32
 * @retval 0 成功
 */
UHSD_API uhsd_s32 uhsd_log_upload_status_register(uhsd_log_upload_status_cb status_cb, uhsd_void *user_data);

/**
 * @brief 停止日志上传
 *
 * @return uhsd_s32
 * @retval 0 成功
 */
UHSD_API uhsd_s32 uhsd_log_upload_suspend(uhsd_void);

/**
 * @brief 恢复日志上传
 *
 * @return uhsd_s32
 * @retval 0 成功
 */
UHSD_API uhsd_s32 uhsd_log_upload_resume(uhsd_void);

/**
 * @brief 开启U+BLE业务
 * @details
 * OSDK初始化后，默认开启。
 *
 * 业务包括：
 * 1. 开启快连业务
 * 2. 开启小程序业务
 * @param[in] function 0x01 开启快连； 0x02 开启小程序； 0xFFFFFFFF 开启所有业务。
 * @param[in] 返回执行结果，注意，需要等待完全执行结束后，再进行后续操作。ret: 0x00 成功 ret: <0 失败。
 * @return uhsd_s32
 * @retval 0 成功
 * @retval <0 异常 @ref UHSD_ERROR_NUM
 *
 */
UHSD_API uhsd_s32 uhsd_ble_enable_async(uhsd_u32 function, uhsd_void (*sd_ble_enable_ret_cb)(uhsd_s32 ret));

/**
 * @brief 关闭U+BLE业务
 * @details
 * 关闭后，将无法使用OSDK的蓝牙小程序直控功能。
 * 业务包括：
 * 1. 关闭快连业务
 * 2. 关闭小程序业务
 * @param[in] fucntion 0x01 关闭快连； 0x02 关闭小程序； 0xFFFFFFFF 关闭所有业务。
 * @param[in] 返回执行结果，注意，需要等待完全执行结束后，再进行后续操作。ret: 0x00 成功 ret: <0 失败。
 * @return uhsd_s32
 * @retval 0 成功
 * @retval <0 异常 @ref UHSD_ERROR_NUM
 *
 * @attention 即使设备APP关闭U+BLE业务，当设备APP调用进入配置接口时，若设备APP支持经典蓝牙，则需要设备APP关闭自己的经典蓝牙，并确保uhsd_ble_enable已经开启。
 */
UHSD_API uhsd_s32 uhsd_ble_disable_async(uhsd_u32 function, uhsd_void (*sd_ble_disable_ret_cb)(uhsd_s32 ret));

/**
 * @brief 开启Wi-Fi功能
 * @details
 * OSDK初始化后，默认开启。
 *
 * @param[in] 返回执行结果回调，注意，需要等待完全执行结束后，再进行后续操作。ret: 0x00 成功 ret: <0 失败； user_data：用户数据。
 * @param[in] user_data 用户数据，@ref sd_wifi_enable_ret_cb 返回时将带回给调用者。
 * @return uhsd_s32
 * @retval 0 成功
 * @retval UHSD_E_COMMON_RE_ENABLE  Wi-Fi重复开启
 * @retval UHSD_E_COMMON_COMM_FAIL 其他原因导致的开启失败。
 *
 */
UHSD_API uhsd_s32 uhsd_wifi_enable_async(uhsd_void (*sd_wifi_enable_ret_cb)(uhsd_s32 ret, uhsd_void *user_data), uhsd_void *user_data);

/**
 * @brief 关闭Wi-Fi功能
 * @details
 * 1. 若用户接管Wi-Fi，则
 * 用户调用此接口后，OSDK需要上报离线原因等操作
 *
 * @param[in] 返回执行结果回调，注意，需要等待完全执行结束后，再进行后续操作。ret: 0x00 成功 ret: <0 失败； user_data：用户数据。
 * @param[in] user_data 用户数据，@ref sd_wifi_disable_ret_cb 返回时将带回给调用者。
 * @return uhsd_s32
 * @retval 0 成功
 * @retval UHSD_E_COMMON_COMM_FAIL 其他原因导致的关闭失败。
 *
 */
UHSD_API uhsd_s32 uhsd_wifi_disable_async(uhsd_void (*sd_wifi_disable_ret_cb)(uhsd_s32 ret, uhsd_void *user_data), uhsd_void *user_data);

#ifdef __cplusplus
}

#endif

#endif

/** @}*/
