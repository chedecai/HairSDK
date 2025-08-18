/**
 *
 * @defgroup grp_uhsd 错误码定义
 * @{
 * @brief SmartDevice 错误码
 * @version 0.1
 * @author zhoudong (zhoudong@haier.com)
 * @date 2022-08-31
 *
 * @file uhsd_error.h
 *
 * @copyright Copyright (c) 2021, Haier.Co, Ltd.
 *
 */

#ifndef __UHSD_ERROR_H__
#define __UHSD_ERROR_H__

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief UHSD错误码定义
 * 一般错误                     -1 ～ -99
 * 设备接入错误                 -100 ～ -199
 * 设备配置错误                 -200 ～ -249
 * 设备绑定错误                 -250 ～ -299
 * 设备OTA错误                  -300 ～ -349
 * 消息推送错误                 -350 ～ -399
 * 数据转换功能错误             -400 ～ -450
 * 用户token错误                -451 ～ -499
 * video错误                   -500 ～ -599
 */

/*!
 * 成功
 */
#define UHSD_E_COMMON_OK (0)

/*!
 * 一般错误                  -1 ～ -99
 */
#define UHSD_E_COMMON_COMM_FAIL             (-1)
#define UHSD_E_COMMON_INVALID_PARAM         (-2)
#define UHSD_E_COMMON_NO_ENOUGH_MEMORY      (-3)
#define UHSD_E_COMMON_NOT_STARTED           (-4)
#define UHSD_E_COMMON_RE_START              (-5)
#define UHSD_E_COMMON_NOT_REGISTERED        (-6)
#define UHSD_E_COMMON_RE_REGISTER           (-7)
#define UHSD_E_COMMON_NOT_INITIALIZED       (-8)
#define UHSD_E_COMMON_RE_INIT               (-9)
#define UHSD_E_COMMON_INVALID_FIX_LEN       (-10)
#define UHSD_E_COMMON_INVALID_RANGE_LEN     (-11)
#define UHSD_E_COMMON_INVALID_CONTENT       (-12)
#define UHSD_E_COMMON_INVALID_VER           (-13)
#define UHSD_E_COMMON_INVALID_MODULE_TYPE   (-14)
#define UHSD_E_COMMON_TIMEOUT               (-15)
#define UHSD_E_COMMON_INVALID_CMD           (-16)
#define UHSD_E_COMMON_NOT_ALLOW             (-17)
#define UHSD_E_COMMON_RESET_BUSYING         (-18)
#define UHSD_E_COMMON_CLOUD_NOT_CONNECTED   (-19)
#define UHSD_E_COMMON_CREATE_HTTPREQ        (-20)
#define UHSD_E_COMMON_SEND_HTTPREQ          (-21)
#define UHSD_E_COMMON_RE_ENABLE             (-22)
#define UHSD_E_COMMON_RE_DISABLE            (-23)
#define UHOS_E_COMMON_OVER_MAX_NUM          (-24)
#define UHSD_E_COMMON_NOT_FIND_DEV          (-25)
#define UHSD_E_COMMON_NOT_FIND_NODE         (-26)
#define UHOSD_E_COMMON_DEV_NOT_ONLINE       (-27)
#define UHSD_E_COMMON_FATAL2REBOOT          (-28)
#define UHSD_E_COMMON_NO_MASTER_HANDLE      (-29)
#define UHSD_E_COMMON_CLOUD_CONNECTED       (-30)
#define UHSD_E_COMMON_NOT_FOUND_MATSTER_DEV (-31)
#define UHSD_E_COMMON_REPEAT_ADD_DEV        (-32)

/*!
 * 设备接入错误              -100 ～ -199
 */
#define UHSD_E_DEV_NOT_FIND_DEV                       (-100)
#define UHSD_E_DEV_NOT_FIND_MODULE                    (-101)
#define UHSD_E_DEV_INVALID_DEVINFO                    (-102)
#define UHSD_E_DEV_EPP_TO_PAIR                        (-103)
#define UHSD_E_DEV_PAIR_TO_EPP                        (-104)
#define UHSD_E_DEV_REGISTER_FAIL                      (-105)
#define UHSD_E_DEV_SET_ATTR                           (-106)
#define UHSD_E_DEV_GET_DEV_OBJECT                     (-107)
#define UHSD_E_DEV_INVALID_PKT                        (-108)
#define UHSD_E_DEV_INVALID_SESSION                    (-109)
#define UHSD_E_DEV_ADD_MODULE                         (-110)
#define UHSD_E_DEV_GETTING_DEVID                      (-111)
#define UHSD_E_DEV_NOT_PHY_MODULE                     (-112)
#define UHSD_E_DEV_NOT_BASEBOARD_INFO                 (-113)
#define UHSD_E_DEV_SEND_FOTA_INFO                     (-115)
#define UHSD_E_DEV_SEND_TRANSPARENT_ERR               (-116)
#define UHSD_E_DEV_INVALID_FW_INFO                    (-118)
#define UHSD_E_DEV_INVALID_ROLE                       (-119)
#define UHSD_E_DEV_ATTR_INFO_EXPIRED                  (-120)
#define UHSD_E_DEV_ALREADY_REG_PHYMODULE              (-121)
#define UHSD_E_DEV_PARSE_CERT_INFO                    (-122)
#define UHSD_E_DEV_NOT_ALLOW_OFFLINE_MASTER_DEV       (-123)
#define UHSD_E_DEV_NOT_NTP                            (-124)
#define UHSD_E_DEV_MUTUAL_AUTH_DEV_ONLY_ALLOW_ONE_DEV (-125)
#define UHSD_E_DEV_NOT_ALLOW_WHEN_DEV_ONLINE          (-126)
#define UHSD_E_DEV_MASTER_NOT_ONLINE                  (-127)
#define UHSD_E_DEV_OVER_MAX_NUM                       (-128)
#define UHSD_E_DEV_NOT_ALLOW_WHEN_DEV_NOT_OFFLINE     (-129)
#define UHSD_E_DEV_IN_FT_MODE                         (-130)
#define UHSD_E_DEV_NOT_ALLOW                          (-131)
/*!
 * 设备配置错误              -200 ～ -249
 */
#define UHSD_E_CFG_USER_CONFIGURING (-200)
#define UHSD_E_CFG_NOT_REGISTER_DEV (-201)

/*!
 * 设备绑定错误              -250 ～ -299
 */

/*!
 * 设备OTA错误              -300 ～ -349
 */
#define UHSD_E_OTA_ALREADY_UPGRADING (-300)
#define UHSD_E_OTA_ALREADY_QUERYING  (-301)
#define UHSD_E_OTA_QUERY_TIMEOUT     (-302)
#define UHSD_E_OTA_NO_UPGRADE_FW     (-303)

/*!
 * 消息推送错误              -350 ～ -399
 */

/*!
 * 数据转换接错误              -400 ～ -450
 */
#define UHSD_E_DATACVT_UNKNOWN            (-400)
#define UHSD_E_DATACVT_PRESET_DATA_MISSED (-401) /**< 没有预置文件，当下载成功后，会通过相应的回调接口通知设备APP*/

/*!
 * 用户token相关错误              -451 ～ -499
 */
#define UHSD_E_USER_TOKEN_SYSTEM_ERROR    (-451)
#define UHSD_E_USER_TOKEN_INVALID_CONTENT (-452)
#define UHSD_E_USER_TOKEN_REFRESH_REJECT  (-453)
#define UHSD_E_USER_TOKEN_ACCESS_EXPIRED  (-454)
#define UHSD_E_USER_TOKEN_REFRESH_EXPIRED (-455)

/*!
 * P2P资源上报错误              -400 ～ -430
 */
#define UHSD_E_P2P_RSC_CHANNEL_DISCONNECT (-400)
#define UHSD_E_P2P_RSC_NOT_REGISTER       (-401)
#define UHSD_E_P2P_RSC_NOT_SUBSCRIBE      (-402)
#define UHSD_E_P2P_RSC_NOT_REG_CB         (-403)

/*!
 * 用户token相关错误              -451 ～ -499
 */
#define UHSD_E_USER_TOKEN_SYSTEM_ERROR    (-451)
#define UHSD_E_USER_TOKEN_INVALID_CONTENT (-452)
#define UHSD_E_USER_TOKEN_REFRESH_REJECT  (-453)
#define UHSD_E_USER_TOKEN_ACCESS_EXPIRED  (-454)
#define UHSD_E_USER_TOKEN_REFRESH_EXPIRED (-455)
/*!
 * video错误码              -400 ～ -430
 */
#define UHSD_E_VIDEO_AUTH_ONLINE      (-500)
#define UHSD_E_VIDEO_SECRET_KEY       (-501)
#define UHSD_E_VIDEO_PUSH_STREAM      (-502)
#define UHSD_E_VIDEO_UPLOAD_CS_STREAM (-503)

/*!
 * DCC错误码              -600 ～ -699
 */
#define UHSD_E_DCC_INVALID_LISTENER (-600)

/**
 * 应答码，devApp调用应答接口时使用
 */
#define UHSD_R_RSP_OK              (0)
#define UHSD_R_COMM_ERROR          (1)  /**< 通用错误应答码 */
#define UHSD_R_PARAM_ERROR         (4)  /**< 参数错误,缺少部分必填字段*/
#define UHSD_R_CMD_TIMEOUT         (11) /**< 命令执行超时，主要指设备底板执行命令超时 */
#define UHSD_R_INVALID_CMD         (12) /**< 无效命令，主要指命令执行逻辑错误,例如关机状态下再关机等 */
#define UHSD_R_ILLEGAL_VALUE       (13) /**< 非法值，主要指设备操作的操作值非法，例如温度设置的温度值越界 */
#define UHSD_R_UNSUPPORTED_CMD     (17) /**< 不支持的命令,主要指读属性或写属性请求时，设备无此属性，或者操作时，设备不支持此操作 */
#define UHSD_R_UPGRADING           (501) /**< 设备当前正在升级，不支持控制 */
#define UHSD_R_UNKNOWN_DEVICE      (503) /**< 未知设备，指向设备发送指令时，设备已注销或不存在 */
#define UHSD_R_STARTPAIR_FAILED    (504) /**< 配对失败，启动配对阶段使用 */
#define UHSD_R_INVALID_DEVICE_TYPE (505) /**< 无效设备类型，启动配对阶段使用 */
#define UHSD_R_REPEAT_STARTPAIR    (506) /**< 重复启动配对，启动配对阶段使用 */
#define UHSD_R_PAIR_DEV_MAX        (507) /**< 配对设备个数达上线,启动配对阶段使用 */
#define UHSD_R_PAIR_TIMEOUT        (508) /**< 配对超时，配对过程中使用 */
#define UHSD_R_BIND_ERROR          (509) /**< 绑定失败，绑定中使用 */
#define UHSD_R_EXIT_PAIR           (510) /**< 退出配对，配对过程中使用 */

#ifdef __cplusplus
}
#endif

#endif /*__UHSD_ERROR_H__*/

/** @}*/
