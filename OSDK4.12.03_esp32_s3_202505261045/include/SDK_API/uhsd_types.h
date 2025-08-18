/**
 *
 * @defgroup grp_uhsdtypes 类型定义
 * @{
 * @brief 类型定义
 * @version 0.1
 * @author hudx (hudingxuan.xian@haier.com)
 * @date 2022-08-24
 * @details
 * SmartDevice 基本类型定义，用户根据不通平台可进行适配。
 *
 * @copyright Copyright (c) 2021, Haier.Co, Ltd.
 * @file uhsd_types.h
 *
 */

#ifndef __UHSD_TYPES_H__
#define __UHSD_TYPES_H__

#include <stdint.h>

// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
#define UHSD_HELPER_SO_IMPORT __declspec(dllimport)
#define UHSD_HELPER_SO_EXPORT __declspec(dllexport)
#define UHSD_HELPER_SO_LOCAL
#else
#if __GNUC__ >= 4
#define UHSD_HELPER_SO_IMPORT __attribute__((visibility("default")))
#define UHSD_HELPER_SO_EXPORT __attribute__((visibility("default")))
#define UHSD_HELPER_SO_LOCAL  __attribute__((visibility("hidden")))
#else
#define UHSD_HELPER_SO_IMPORT
#define UHSD_HELPER_SO_EXPORT
#define UHSD_HELPER_SO_LOCAL
#endif
#endif

// Now we use the generic helper definitions above to define UHSD_API and UHSD_LOCAL.
// UHSD_API is used for the public API symbols. It either SO imports or SO exports (or does nothing for static build)
// UHSD_LOCAL is used for non-api symbols.

#ifdef UHSD_SO         // defined if UHSD is compiled as a SO
#ifdef UHSD_SO_EXPORTS // defined if we are building the UHSD SO (instead of using it)
#define UHSD_API UHSD_HELPER_SO_EXPORT
#else
#define UHSD_API UHSD_HELPER_SO_IMPORT
#endif // UHSD_SO_EXPORTS
#define UHSD_LOCAL UHSD_HELPER_SO_LOCAL
#else // UHSD_SO is not defined: this means UHSD is a static lib.
#define UHSD_API
#define UHSD_LOCAL
#endif // UHSD_SO

#ifdef __cplusplus
#define UHOS_NULL 0L
#else
#define UHOS_NULL ((void *)0)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define UHSD_SUCCESS 0
#define UHSD_FAILURE -1

typedef int uhsd_devHandle;
typedef int uhsd_moduleHandle;

typedef int uhepp_devHandle;
typedef int uhsd_dcc_listener_t;

typedef unsigned char uhsd_u8;
typedef signed char uhsd_s8;
typedef unsigned short uhsd_u16;
typedef signed short uhsd_s16;
typedef signed int uhsd_s32;
typedef unsigned int uhsd_u32;
typedef unsigned long long uhsd_u64;
typedef signed long long uhsd_s64;
typedef char uhsd_char;
typedef void uhsd_void;
typedef unsigned char uhsd_bool;
typedef float uhsd_float;
typedef double uhsd_double;
// typedef long int          uhsd_l32;
// typedef unsigned long      uhsd_ulong;
// typedef long               uhsd_long;
typedef intptr_t uhsd_intptr;
typedef uintptr_t uhsd_uintptr;

#define UHOS_TRUE  1
#define UHOS_FALSE 0

#define UHOS_SUCCESS 0
#define UHOS_FAILURE (-1)

typedef enum
{
    UHSD_REGION_DOMESTIC = 0, /*国内*/
    UHSD_REGION_SEA = 1       /*东南亚*/
} uhsd_domain_region_t;

typedef struct
{
    const uhsd_char *name;
    const uhsd_char *value;
} uhsd_dev_pair_t;

/**
 * @brief 事件信息结构体定义
 */
typedef struct uhsd_dev_event_context
{
    uhsd_char *name;              /* 事件name */
    uhsd_char *type;              /* 事件类型 */
    uhsd_dev_pair_t **attr_pairs; /* 事件属性键值对 */
    uhsd_u16 pair_num;            /* 事件属性键值对个数 */
} uhsd_dev_event_context_t;

typedef enum
{
    UHSD_DEV_ROLE_GENERAL = 0x01, /**< 普通设备 */
    UHSD_DEV_ROLE_GW = 0x02,      /**< 网关设备 */
    UHSD_DEV_ROLE_ANNEX = 0x03,   /**< 附件设备 */
    UHSD_DEV_ROLE_SLAVE = 0x04,   /**< 子设备 */
    UHSD_DEV_ROLE_GROUP = 0x05,   /**< 组设备 */
} uhsd_dev_role_t;

typedef enum
{
    UNDEF = 0,
    ZIGBEE3_0
} uhsd_dev_protocol_t;


typedef enum
{
    UHSD_MODULE_TYPE_NONE = 0,     // 组设备时传入此值
    UHSD_MODULE_TYPE_VOICE,        // 语音模块 module
    UHSD_MODULE_TYPE_BLEMESH,      // blemesh module
    UHSD_MODULE_TYPE_HOSTAPP,      //hostApp module
    UHSD_MODULE_TYPE_NBIOT,        // NBIOT module
    UHSD_MODULE_TYPE_4G,           // 4G module
    UHSD_MODULE_TYPE_WIFI_NETCARD, // wifi netcard module
    UHSD_MODULE_TYPE_ETH_NETCARD,  // 以太网卡module
    UHSD_MODULE_TYPE_ZIGBEE,       // ZIGBEE module
    UHSD_MODULE_TYPE_BASEBOARD,
    UHSD_MODULE_TYPE_MAX
} uhsd_module_type_e;

typedef struct
{
    uhsd_char *typeId;     /* 设备类型标识符，海极网创建设备模型时生成*/
    uhsd_char *devKey;     /* 设备密钥，海极网创建设备模型时生成*/
    uhsd_module_type_e module_type;   /*模块类型, 此处填入类型值后，后续添加moudle时需要调用相对应类型的创建接口; 组设备传入UHSD_MODULE_TYPE_NONE */
    uhsd_char *module_id;   /*模块唯一标识；当设备是组设备时，此值无效填入NULL即可*/
    uhsd_char *subId;       /* 子机设备标识，附件设备必须传入唯一值，否则传入NULL即可*/
} uhsd_dev_reg_factor_t;
/**
 * @brief 设备离线原因定义
 */
typedef enum
{
    UHSD_OFFLINE_REASON_ENTER_CONFIG = 1,                                 /* 进入配置 */
    UHSD_OFFLINE_REASON_CLR_USER_CONFIG,                                  /* 恢复出厂设置 */
    UHSD_OFFLINE_REASON_WATCHDOG,                                         /* 看门狗复位 */
    UHSD_OFFLINE_REASON_OTA_RESET,                                        /* 升级成功复位 */
    UHSD_OFFLINE_REASON_DEV_FAIL,                                         /* 串口故障引起离线 */
    UHSD_OFFLINE_REASON_RESET,                                            /*  自动重启*/
    UHSD_OFFLINE_REASON_WIFI_DOWN_TIMEOUT,                                /*Wi-Fi连接中断-模块主动断开*/
    UHSD_OFFLINE_REASON_WIFI_CHANGED,                                     /*修改Wi-Fi SSID*/
    UHSD_OFFLINE_REASON_WIFI_ACTIVE_DISCONNECT,                           /*模块主动重连WIFI*/
    UHSD_OFFLINE_REASON_WIFI_DISCONNECT = 10,                             /*主动断开Wi-Fi*/
    UHSD_OFFLINE_REASON_WIFI_DISABLE,                                     /*主动关闭Wi-Fi*/
    UHSD_OFFLINE_REASON_WIFI_DOWN_AP_DISCONNECT,                          /*Wi-Fi连接中断-AP主动断开*/
    UHSD_OFFLINE_REASON_WIFI_DOWN_AP_PAPA_CHANGE,                         /*Wi-Fi连接中断-AP参数变化*/
    UHSD_OFFLINE_REASON_WIFI_DOWN_PASSWORD_CHANGE,                        /*Wi-Fi连接中断-AP密码变化*/
    UHSD_OFFLINE_REASON_WIFI_DOWN_AP_POWER_OFF,                           /*Wi-Fi连接中断-AP断电*/
    UHSD_OFFLINE_REASON_WIFI_DOWN_IP_CHANGE,                              /*IP地址变化*/
    UHSD_OFFLINE_REASON_WIFI_DOWN_AP_RESTART,                             /*Wi-Fi连接中断-AP重启*/
    UHSD_OFFLINE_REASON_WIFI_CHANNEL_CHANGED,                             /*Wi-Fi连接中断-AP信道变化*/
    UHSD_OFFLINE_REASON_WIFI_ROUTER_COMMUNICATION_ERROR,                  /*与路由器通信错误*/
    UHSD_OFFLINE_REASON_WIFI_CONNECTIVITY_TX_ERROR = 20,                  /*WIFI连接中断-路由器连通性检测失败（发错误)*/
    UHSD_OFFLINE_REASON_WIFI_CONNECTIVITY_RX_ERROR,                       /*WIFI连接中断-路由器连通性检测失败（收错误）*/
    UHSD_OFFLINE_REASON_WIFI_CONNECTIVITY_TRX_ERROR,                      /*WIFI连接中断-路由器连通性检测失败（收发错误）*/
    UHSD_OFFLINE_REASON_WIFI_DOWN_AP_DISCONNECT_WITH_REASON,              /*WIFI连接中断-AP主动断开（错误码）*/
    UHSD_OFFLINE_REASON_AP_POWER_OR_NETWORK_DISCONNECT_AFTER_OFFLIE = 24, /*离线后AP断电或者断网*/
    UHSD_OFFLINE_REASON_AP_RESTART_AFTER_OFFLINE,                         /*离线后AP重启*/
    UHSD_OFFLINE_REASON_RESET_AFTER_OFFLINE,                              /*离线后设备复位*/
    UHSD_OFFLINE_REASON_AP_CHANGED,                                       /*AP切换*/
    UHSD_OFFLINE_REASON_AP_BSSID_UNMATCH_MAC                              /*AP的bssid和mac不匹配*/
} uhsd_dev_offline_reason_t;

typedef enum
{
    UHSD_DEV_BIND_STATUS_ERR = -1,
    UHSD_DEV_BIND_STATUS_BOUND = 12,
    UHSD_DEV_BIND_SATUS_UNBOUND = 13
} uhsd_dev_bind_status_t;

#ifdef __cplusplus
}
#endif

#endif /*__UHSD_TYPES_H__*/

/**@} grp_uhsdtypes end */