/**
 * @defgroup grp_uhsdcfg 设备配置功能接口
 * @{
 * @author hudx (hudingxuan.xian@haier.com)
 * @author jiashouqiang(jiashouqiang@haier.com)
 * @brief SDR/L设备配置功能
 * @version 0.1
 * @date 2022-08-12
 *
 * @attention 设备APP必须注册设备后，才可使用配置功能。
 * @copyright Copyright (c) 2021, Haier.Co, Ltd.
 *
 * @file uhsd_cfg.h
 *
 */

#ifndef __UHSD_CFG_H__
#define __UHSD_CFG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "uhsd_types.h"

/**
 * @brief 配置启动
 * @details
 * 开始配置，具体使用的配置模式（如smartlink或者smartap4.0、Mesh入网、BLE入网）由SDK配置决定，具体应从整体的配置文件进行定义。
 * @param trigger_type 0 -- epp trigger(E++触发)   1 -- user app trigger
 * @return 配置是否正常开启
 * @retval 0 成功
 * @retval others 失败 错误码 @ref uhsd_error.h
 *
 * @attention
 * 此接口不仅仅包括配置WiFi能力，还需要支持BLE/Mesh入网能力。
 * 针对于配对业务，需要设计单独的接口进行支撑，此接口不包含此部分功能。
 */
UHSD_API uhsd_s32 uhsd_dev_config_start(uhsd_u8 trigger_type);

/**
 * @brief 配置停止
 * @details 配置过程中，用户可以主动停止配置服务。
 * @return 配置是否停止成功
 * @retval 0 成功
 * @retval others 失败 @ref uhsd_error.h
 *
 */
UHSD_API uhsd_s32 uhsd_dev_config_stop(uhsd_u8 trigger_type);

/**
 * @brief 配置状态获取接口
 *
 * @return 配置状态, 异常为负值，正常值参考 UHSD_DEV_STATUS_E
 */
UHSD_API uhsd_s32 uhsd_dev_config_status_get(uhsd_void);

/**
 * @brief 是否含有配置信息
 * @retval ture 有配置信息，false，无配置信息
 */
UHSD_API uhsd_bool uhsd_dev_config_is_exist(uhsd_void);

/**
 * @brief 清除配置信息ssid
 * @return 0:成功； 其他:失败
 */
UHSD_API uhsd_s32 uhsd_dev_config_ssid_clear(uhsd_void);

/**
 * @brief 开启绑定时间窗
 * @details
 * 当设备APP不使用海尔自动绑定功能（已嵌入到配置接口中）时，
 * 需要调用此接口进行开启绑定时间窗操作。
 * 使用场景：
 * 1. 用户侧绑定。
 * 2. 蜂窝类设备绑定。
 *
 * @return uhsd_s32
 * @retval 0 开启成功。
 *
 * @attention
 * 此种绑定方式，为手机主动绑定行为，SmartDevice无事件通知设备APP。
 */
UHSD_API uhsd_s32 uhsd_dev_config_open_bind_win(uhsd_void);

/**
 * @enum UHSD_LOCAL_BIND_STATUS_UNBIND 未绑定，用于本地存储
 * @enum UHSD_LOCAL_BIND_STATUS_BINDED 已绑定，用于本地存储
 * @enum UHSD_REMOTE_BIND_STATUS_FAIL 用于获取远程绑定状态时，由于网络等原因无法获取返回失败
 * @enum UHSD_REMOTE_BIND_STATUS_OK   用于获取远程绑定状态时,请求发送成功
 */
// typedef enum
// {
//     UHSD_LOCAL_BIND_STATUS_UNBIND = 0x00,
//     UHSD_LOCAL_BIND_STATUS_BINDED,
//     UHSD_REMOTE_BIND_STATUS_FAIL,
//     UHSD_REMOTE_BIND_STATUS_OK
// } uhsd_dev_bind_status_t;

/**
 * @brief 绑定主设备状态获取
 *
 * @param 无
 * @return 绑定状态，值为UHSD_LOCAL_BIND_STATUS_UNBIND 或 UHSD_LOCAL_BIND_STATUS_BINDED
 *
 * @note 1. 本接口获取的是flash中本地存储的设备绑定状态；
 *       2. 本接口目前仅支持主设备的绑定状态获取；
 */
UHSD_API uhsd_dev_bind_status_t uhsd_dev_bind_status_get(uhsd_void);

/**
 * @brief  设备APP控制OSDK 重新连接路由器，用于[二次配网]需求
 *
 * @param[in]   bssid       站点mac
 * @param[in]   bssid_len   bssid长度
 * @param[in]   ssid        wifi标识
 * @param[in]   pwd         wifi密码
 *
 * @return 0: 成功；< 0：失败
 *
 */

UHSD_API uhsd_s32 uhsd_dev_config_reconn_wifi_router(const uhsd_u8 *bssid, const uhsd_s16 bssid_len, const uhsd_char *ssid, const uhsd_char *pwd);

typedef struct
{
    uhsd_char ssid[32 + 1]; /* ssid */
    uhsd_char pwd[64 + 1];  /* passwrd */
    uhsd_u8 bssid[6];       /* bssid */
    uhsd_u16 channel;
} uhsd_dev_config_wifi_info_st;

/**
 * @brief 用户设置配置信息,调用此接口前必须先调用进配置接口
 * @param [IN]usr_config_info
 * @return uhsd_s32,成功返回0，失败返回其它值，
              若不支持用户直接配置，则返回失败
 */
UHSD_API uhsd_s32 uhsd_dev_config_wifi_info_set(uhsd_dev_config_wifi_info_st *usr_config_info);

typedef struct
{
    uhsd_u8 ssid[32 + 1]; /*!<SSID。*/
    uhsd_u16 ssid_len;    /*!<SSID长度。*/
    uhsd_u8 bssid[6];     /*!<BSSID。*/
    uhsd_u8 bssid_len;    /*!<BSSID长度。*/
    uhsd_s32 rssi;        /*!<信号强度。*/
    uhsd_u16 channel;     /*!<信道。*/
    uhsd_s32 security;    /*!<安全信息。*/
} uhsd_dev_config_wifi_scan_info_st;

/**
 * @typedef wifimgr_scan_cb_func
 * @brief 扫描完成后的回调通知。
 * @param ap_list ap列表指针，ap_num 列表数量
 * @return N/A。
 */
typedef void (*uhsd_dev_config_wifimgr_scan_cb_func)(uhsd_dev_config_wifi_scan_info_st *ap_list, uhsd_s32 ap_num);

/**
 * @brief 用户打开wifi扫描，并返回扫描到的wifi信息
 * @param [IN]scan_cb:扫描结束的回调，
 * @param [IN]ap_num:、表示用户期望的扫描数,根据设备的资源设置，建议不大于32
 * @return uhsd_s32,成功返回0，失败返回其它值，
              若不支持用户直接配置，则返回失败
              返回非0时不会填充或改变输入参数
 */
UHSD_API uhsd_s32 uhsd_dev_config_start_wifi_scan(uhsd_dev_config_wifimgr_scan_cb_func scan_cb, uhsd_s32 ap_num);

/**
 * @brief 新直连绑定过程状态枚举定义
 */
typedef enum
{
    UHSD_DEV_DIRECT_BIND_STATE_STOP,              // 停止
    UHSD_DEV_DIRECT_BIND_STATE_STARTING,          // 正在启动
    UHSD_DEV_DIRECT_BIND_STATE_WAITING_BIND_INFO, // 等待绑定信息
    UHSD_DEV_DIRECT_BIND_STATE_VERIFYING,         // 正在验证
    UHSD_DEV_DIRECT_BIND_STATE_BINDING,           // 正在绑定
    UHSD_DEV_DIRECT_BIND_STATE_FINISH             // 完成
} uhsd_dev_direct_bind_process_state_e;

/**
 * @brief 新直连绑定验证方式枚举定义
 */
typedef enum
{
    UHSD_DEV_DIRECT_BIND_VERIFY_METHOD_CODE,
    UHSD_DEV_DIRECT_BIND_VERIFY_METHOD_KEY,
} uhsd_dev_direct_bind_verify_method_e;

/**
 * @brief 新直连绑定过程状态通知回调函数定义
 *
 * @param state 绑定过程状态
 * @param user_data 用户数据
 *
 * @note 开发者只有在启动新直连绑定时，设置该回调函数，才能收到绑定过程状态通知。
 *       该回调函数在绑定过程中会被多次调用，开发者可以根据状态进行相应的处理。
 */
typedef uhsd_void (*uhsd_dev_direct_bind_process_notify_cb)(uhsd_dev_direct_bind_process_state_e state, uhsd_void *user_data);

/**
 * @brief 新直连绑定验证请求通知回调函数定义
 *
 * @param method 绑定验证方式
 * @param code 绑定验证码，当method为UHSD_DEV_DIRECT_BIND_VERIFY_METHOD_CODE时该参数才有效。
 *                       当method为UHSD_DEV_DIRECT_BIND_VERIFY_METHOD_KEY时该参数为NULL。
 * @param user_data 用户数据
 *
 * @note 1. 开发者只有在启动新直连绑定时，必须设置该回调函数。
 *       2. 开发者收到该回调通知后，启动验证功能，将验证结果通过uhsd_dev_direct_bind_verify_result_set设置给SDK.
 */
typedef uhsd_void (*uhsd_dev_direct_bind_verify_req_notify_cb)(uhsd_dev_direct_bind_verify_method_e method, uhsd_char *code, uhsd_void *user_data);

/**
 * @brief 新直连绑定结果通知回调函数定义
 *
 * @param result 绑定结果， 0表示成功，< 0 表示失败
 * @param user_data 用户数据
 *
 * @note 开发者只有在启动新直连绑定时，设置该回调函数，才能收到绑定结果通知。
 *
 * @attention 该回调函数在绑定完成后会被调用一次，开发者可以根据结果进行相应的处理。
 */
typedef uhsd_void (*uhsd_dev_direct_bind_result_notify_cb)(uhsd_s32 result, uhsd_void *user_data);

/**
 * @brief 设置新直连绑定验证结果
 *
 * @param is_success 绑定结果，UHOS_TRUE表示成功，UHOS_FALSE表示失败
 *
 * @return 0 成功，< 0 失败
 *
 * @note 1. 开发者收到uhsd_dev_direct_bind_verify_req_notify_cb回调通知后，
 *       启动验证功能，将验证结果通过该接口设置给SDK，SDK会根据结果进行相应的处理。
 *
 *       2. 该接口只有启动新直连绑定时才有效，其他情况下调用该接口返回失败。
 *
 */
UHSD_API uhsd_s32 uhsd_dev_direct_bind_verify_result_set(uhsd_bool is_success);

/**
 * * @brief 启动新直连绑定
 *
 * @param method 绑定验证方式
 * @param process_cb 绑定过程状态通知回调函数, 如果开发者不关心绑定过程状态，可以设置为NULL
 * @param verify_req_cb 绑定验证请求通知回调函数， 该回调函数必须设置，开发者收到该回调通知后，
 *                      启动验证功能，将验证结果通过uhsd_dev_direct_bind_verify_result_set设置给SDK.
 * @param result_cb 绑定结果通知回调函数，该回调函数必须设置，开发者可以根据结果进行相应的处理。
 * @param user_data 用户数据，开发者可以在回调函数中使用该参数传递用户数据，process_cb、verify_req_cb、result_cb
 *                  回调函数中都会通知该参数。
 *
 * @return 0 成功，< 0 失败
 *
 * @note 1. 新直连功能与softAP配置绑定功能互斥，不能同时使用。
 *       2. 新直连绑定功能与BLE配置绑定功能互斥，不能同时使用。
 *       3. 新直连绑定服务默认超时为30分钟，超时后SDK会自动停止绑定服务，触发结果回调通知。
 *
 */
UHSD_API uhsd_s32 uhsd_dev_direct_bind_start(uhsd_dev_direct_bind_verify_method_e method,
                                             uhsd_dev_direct_bind_process_notify_cb process_cb,
                                             uhsd_dev_direct_bind_verify_req_notify_cb verify_req_cb,
                                             uhsd_dev_direct_bind_result_notify_cb result_cb,
                                             uhsd_void *user_data);

/**
 * @brief 停止新直连绑定
 *
 * @param 无
 *
 * @return 0 成功，< 0 失败
 *
 * @note 1. 新直连绑定完成后，SDK会自动停止绑定服务，触发结果回调通知。设备APP不需要再次调用该接口。
 *       2. 新直连绑定过程中，如果设备APP需要停止绑定服务，可以调用该接口。
 *
 */
UHSD_API uhsd_s32 uhsd_dev_direct_bind_stop(uhsd_void);

/**
 * @brief 设备配网信息通用结构体
 * 字符串默认 UTF-8 格式
 */
typedef struct
{
    uhsd_char ssid[32 + 1]; /* ssid 字符串类型 */
    uhsd_u8 bssid[6];       /* bssid 固定6字节,非字符串 */
    uhsd_char pwd[64 + 1];  /* password 字符串类型 */
} uhsd_dev_cfg_info;

/**
 * @brief wifi连接状态故障码枚举
 */
typedef enum
{
    UHSD_WIFI_NORMAL = 1000,     /* 连接正常 */
    UHSD_WIFI_NOROUTE = 1001,    /* 找不到路由器SSID*/
    UHSD_WIFI_ERRCFG = 1002,     /* 连接路由器密码错误*/
    UHSD_WIFI_FUDCFGINFO = 1003, /* 疑似密码错误*/
    UHSD_WIFI_NOIP = 1005,       /* 未获取到IP */
    UHSD_WIFI_GENERAL = 1007,    /* 未知错误 */
    UHSD_WIFI_WEAKSIGNAL = 1008, /* 信号弱 */
} UHSD_DEVAPP_WIFI_STATUS_E;

/**
 * @brief 设备配网状态通枚举
 */
typedef enum
{
    UHSD_DEV_CFG_STATUS_DFT = 0,

    /**
     * @brief osdk查询配网信息
     * @details
     *  1. uSDK通过WiFi小循环查询配网信息的时候，OSDK通过此接口向设备app查询WiFi配网信息
     *  2. 设备app需要提供当前连接的路由器SSID和密码
     *  3. 配网信息通过回调函数 uhsd_dev_cfg_status_notify() 中的 devapp_data 参数进行回传, 传参类型为: uhsd_dev_cfg_info
     */
    UHSD_DEV_WIFI_CFG_INFO_GET,

    /**
     * @brief osdk接收到配网信息
     * @details
     * 1. 当OSDK通过BLE接收到wifi配网信息以后通知设备app ，设备app自行完成wifi联网
     * 2. 配网信息通过回调函数 uhsd_dev_cfg_status_notify() 中的 osdk_data 参数进行输入, 传参类型为: uhsd_dev_cfg_info
     */
    UHSD_DEV_CFG_INFO_SET,

    /**
     * @brief osdk查询故障码
     * @details 设备APP通过 uhsd_dev_wifi_status_report 接口上报当前wifi连接状态对应的故障码
     */
    UHSD_DEV_WIFI_STATUS_GET,

} UHSD_DEV_CFG_STATUS_E;

/**
 * @brief 配网状态回调函数指针
 * @details
 * 设备APP实现此接口并通过 @ref uhsd_dev_config_status_register 接口注册至SmartDevice。
 * 当对应事件触发时，通过此回调接口通知设备APP
 * @param[in] status 设备配网状态
 * @param[in] osdkdata 对应'设备配网状态'osdk通知给设备APP的参数
 * @param[out] devapp_data 对应'设备配网状态'设备APP需要返回给osdk的参数
 *
 */
typedef uhsd_s32 (*uhsd_dev_cfg_status_notify)(UHSD_DEV_CFG_STATUS_E status, void *osdk_data, void *devapp_data);

/**
 * @brief 设备配网状态通知注册。
 * @details
 * 设备APP调用此接口，注册设备配网状态回调，用来获取设备状态变更通知。
 *
 * @param[in] cfg_status_cb @ref uhsd_dev_cfg_status_notify 用户实现函数。
 * @param[in] user_data 用户自定义数据。会通过 @ref uhsd_dev_cfg_status_notify 接口返回。
 * @return uhsd_s32 注册结果。
 * @retval 0 成功。
 * @retval others 失败。
 */
UHSD_API uhsd_s32 uhsd_dev_config_status_register(uhsd_dev_cfg_status_notify cfg_status_cb);

/**
 * @brief 设备APP通知配网结果状态
 * @details
 * 设备APP调用此接口，将wifi配网状态通知给osdk。
 *
 * @param[in] status wifi连接状态故障码。
 * @retval 0 成功。
 * @retval others 失败。
 */
UHSD_API uhsd_s32 uhsd_dev_wifi_status_report(UHSD_DEVAPP_WIFI_STATUS_E status);

#ifdef __cplusplus
}
#endif

#endif /*__UHSD_CFG_H__*/

/** @} grp_uhsdcfg end */