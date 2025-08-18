/**
 *
 * @defgroup grp_uhepp E++软件接口
 * @{
 * @author hudx (hudingxuan.xian@haier.com)
 * @brief
 * E++设备接入接口集合
 * @details
 * E++设备SDK。
 * 设备APP通过调用此组接口可实现：
 * 1. 获取E++设备信息
 * 2. 获取E++设备状态
 * 3. 控制E++设备
 * 4. 查询E++设备属性
 * 5. 查询E++设备告警
 * 6. 底板OTA
 *
 * @version 0.1
 * @date 2022-08-10
 *
 * @file uhepp.h
 * @copyright Copyright (c) 2021, Haier.Co, Ltd.
 *
 * @attention
 *
 */

#ifndef __UHEPP_H__
#define __UHEPP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "uhepp_types.h"
#include "uhsd_types.h"

/**
 * @brief E++设备事件。
 * @attention 设备基本状态，设备APP需要通过 ref uhepp_status_register 注册。
 */
typedef enum
{
    /**
     * @brief E++设备握手成功。
     * @details
     * SmartDevice会通过 @ref uhepp_dev_ready_cb 通知设备APP。
     *
     */
    UHEPP_DEVICE_READY,

    /**
     * @brief E++获取子板信息成功通知
     * @details
     * SmartDevice会通过 @ref uhepp_subboard_info_cb 通知设备APP。
     *
     */
    UHEPP_SUBBOARD_INFO_RPT,

    /**
     * @brief E++收到进入配置=通知。
     * @details
     * SmartDevice会通过 @ref uhepp_enter_cfg_cb 通知设备APP。
     *
     */
    UHEPP_ENTER_CFG,

    /**
     * @brief E++收到退出配置通知。
     * @details
     * SmartDevice会通过 @ref uhepp_exit_cfg_cb 通知设备APP。
     *
     */
    UHEPP_EXIT_CFG,

    /**
     * @brief E++收到清配置命令通知。
     * @details
     * SmartDevice会通过 @ref uhepp_factory_reset_cb 通知设备APP。
     *
     */
    UHEPP_FACTORY_RESET,

    /**
     * @brief E++收到A3开关日志
     * @details
     * 设备握手成功后，SmartDevice会通过 @ref uhepp_syslog_config_cb 通知设备APP。
     *
     */
    UHEPP_SYSLOG_CONFIG,

    /**
     * @brief E++获取wifi网络状态
     * @details
     * SmartDevice会通过 @ref uhepp_net_wifi_status_get_cb 通知设备APP。
     *
     */
    UHEPP_WIFI_STATUS_GET,

    /**
     * @brief E++获取BLE网络状态
     * @details
     * SmartDevice会通过 @ref uhepp_net_ble_status_get_cb 通知设备APP。
     */
    UHEPP_BLE_STATUS_GET,

    /**
     * @brief E++获取ota分区长度
     * @details
     * SmartDevice会通过 @ref uhepp_ota_bank_size_get_cb 通知设备APP。
     *
     */
    UHEPP_OTA_BANK_SIZE_GET,

    /**
     * @brief E++ 底板ota开始响应
     * @details
     * SmartDevice会通过 @ref uhepp_ota_start_rsp_cb 通知设备APP。
     *
     */
    UHEPP_OTA_START_RSP,

    /**
     * @brief E++ 请求ota数据
     * @details
     * SmartDevice会通过 @ref uhepp_ota_data_req_cb 通知设备APP。
     *
     */
    UHEPP_OTA_DATA_REQ,

    /**
     * @brief E++ ota停止通知
     * @details
     * SmartDevice会通过 @ref uhepp_syslog_config_cb 通知设备APP。
     *
     */
    UHEPP_OTA_STOP_NOTIFY,

    /**
     * @brief E++ 写OTA固件通知
     * @details
     * SmartDevice会通过 @ref uhepp_ota_write_fw_cb 通知设备APP。
     *
     */
    UHEPP_OTA_WRITE_FW,

    /**
     * @brief E++读取固件通知
     * @details
     * SmartDevice会通过 @ref uhepp_ota_read_fw_cb 通知设备APP。
     *
     */
    UHEPP_OTA_READ_FW,

    /**
     * @brief E++ OTA固件下载结束通知
     * @details
     * SmartDevice会通过 @ref uhepp_ota_download_complete_cb 通知设备APP。
     *
     */
    UHEPP_OTA_DOWNLOAD_COMPLETE,

    /**
     * @brief E++ 是否支持ota通知
     * @details
     * SmartDevice会通过 @ref uhepp_ota_support_notify_cb 通知设备APP。
     *
     */
    UHEPP_OTA_SUPPORT_NOTIFY,

    /**
     * @brief E++内置文件通知
     * @details
     * 当底板设备有内置文件时，会主动通知监听者内置文件的数量、每个文件的信息标识列表。
     * 监听此事件的用户，可根据需求，进行文件信息读取。
     *
     */
    UHEPP_FILE_DESC_NOTIFY,

    /**
     * @brief 获取模组版本
     * @details
     *
     */
    UHEPP_MODULE_VER_GET,

    /**
     * @brief 获取专用号
     * @details
     *
     */
    UHEPP_DEV_NUM_GET,

    /**
     * @brief E++蓝牙ADV透传通道设置请求
     * @ref uhepp_ble_adv_scan_passthrough_rules_set_req_cb
     * @ref uhepp_ble_adv_scan_passthrough_rules_t
     *
     */
    UHEPP_BLE_ADV_SCAN_PASSTHROUGH_RULES_SET_REQ,

    /**
     * @brief E++收到进入配对通知。
     * @details
     * SmartDevice会通过 @ref uhepp_enter_scene_pair_cb 通知设备APP。
     *
     */
    UHEPP_ENTER_SCENE_PAIR,

    /**
     * @brief E++收到退出配对通知。
     * @details
     * SmartDevice会通过 @ref uhepp_exit_scene_pair_cb 通知设备APP。
     *
     */
    UHEPP_EXIT_SCENE_PAIR,

    /**
     * @brief E++收到场景控制通知。
     * @details
     * SmartDevice会通过 @ref uhepp_op_scene_pair_cb 通知设备APP。
     * @note 当前仅支持场景的启停。
     *
     */
    UHEPP_OP_SCENE_PAIR,

    /**
     * @brief E++设备异常。
     * @details
     * @verbatim
     * 当E++设备出现异常 @ref UHEPP_ERROR_NUM 时，
     * SmartDevice会通过 @ref hepp_dev_exception_cb 通知设备APP。
     * @endverbatim
     */
    UHEPP_DEVICE_EXCEPTION,

    /**
     * @brief E++获取感知状态
     * @details
     * SmartDevice会通过 @ref uhepp_net_radar_status_get_cb 通知设备APP。
     */
    UHEPP_RADAR_STATUS_GET,

    /**
     * @brief E++收到人感开关状态切换命令通知。
     * @details
     * SmartDevice会通过 @ref uhepp_radar_status_switch_cb 通知设备APP。
     *
     */
    UHEPP_RADAR_STATUS_SWITCH,

    /**
     * @brief E++收到设置人感有人到无人的延时时间通知。
     * @details
     * SmartDevice会通过 @ref uhepp_radar_set_delay_time_cb 通知设备APP。
     *
     */
    UHEPP_RADAR_SET_DELAY_TIME,

    /**
     * @brief EPP收到底板进低功耗指令后，通知给设备app。
     * @details
     *
     */
    UHEPP_ENTER_LOW_POWER,

    /**
     * @brief EPP收到底板退低功耗指令后，通知给设备app。
     * @details
     *
     */
    UHEPP_EXIT_LOW_POWER,

} UHEPP_DEVICE_EVT_E;

/**
 * @brief 子板信息数据结构
 *
 */
typedef struct
{
    uhepp_u8 id;
    uhepp_u8 sw_id[20 + 1];
    uhepp_u8 sw_ver[14 + 1];
    uhepp_u8 supp_ota;
} uhepp_subboard_info_t;

/**
 * @brief 设备识别信息数据结构
 *
 */
typedef struct
{
    uhepp_u8 dev_role; /**< 设备角色。 [bit0:1-普通设备]；[bit1:1-网关设备]；[bit2:1-附件设备] */
    union
    {
        struct
        {
            uhepp_u8 ppid[32]; /**< 产品电脑板功能标识，PPID，由海极网自动生成。 */
            uhepp_u8 ppver[2]; /**< 产品电脑板功能版本号：PPVER，由海极网自动生成。*/
            uhepp_u8 pid[12];  /**< 产品功能标识：PID，由海极网自动生成。*/
            uhepp_u8 pver[2];  /**< 产品功能版本号：PVER，由海极网自动生成。 */
            uhepp_u8 product_type[4];
            uhepp_u8 sub_ptype[4];
        } supp_smart_board;
        struct
        {
            uhepp_u8 type_id[32]; /**< 产品电脑板功能标识，PPID，由海极网自动生成。 */
        } unsupp_smart_board;
    } u;
} uhepp_dev_ic_t; // 设备识别码

/**
 * @brief 整机信息数据结构
 *
 */
typedef struct
{
    uhepp_u8 up_fw_type[20 + 1]; /**< 整机固件标识，少于20bytes最右侧以0x00填充。*/
    uhepp_u8 up_fw_ver[14 + 1];  /**< 整机固件版本。*/
    uhepp_u8 up_fw_pkg_ver[2];   /**< 升级包版本号 */
    uhepp_u8 sub_board_num;
    uhepp_subboard_info_t subboards[]; /* 子板信息结构体数组 */
} up_dev_info_t;

/**
 * @brief 设备信息数据结构
 *
 */
typedef struct
{
    uhepp_dev_ic_t ic;
    up_dev_info_t info;
} uhepp_dev_t; // 设备识别码

/**
 * @brief epp信息数据结构
 *
 */
typedef struct
{
    uhepp_u8 epp_ver[8 + 1]; /**< 设备协议版本。示例："E++2.19" */
    uhepp_u8 sw_ver[8 + 1];  /**< 设备软件版本。示例："20190101" */
    uhepp_u8 hw_ver[8 + 1];  /**< 设备硬件版本。示例："20190101" */

    uhepp_u8 dev_name[8 + 1]; /**< 设备名称。示例："AC" */
    uhepp_u8 dev_direct_addr; /**< 设备直连子机地址 */

    uhepp_u8 product_code[14 + 1];
    uhepp_u8 short_product_code[4 + 1];

    uhepp_u8 dev_key[32 + 1]; /**< 设备名称，字符串。*/

    /**
     * @brief 设备能力
     *
     */
    uhepp_u8 role_supp;        /**< 是否支持设备角色。 */
    uhepp_u8 smart_board_supp; /**< 是否支持电脑版功能标识。 1: uhepp_dev_ic_t中supp_smart_board有效；0:unsupp_smart_board有效。 */
    uhepp_u8 devinfo_qry_supp; /**< 是否支持设备信息查询。1:支持，可获取子板信息；2:支持，尚未获取到设备信息。0:不支持。*/

    uhepp_dev_t direct_dev;
    uhepp_u8 tlvAbility; //底板是否支持设备模型2.1。1：表示支持，2：不支持
} uhepp_info_t;

/**
 * @brief 设备信息通知回调定义
 */
typedef uhepp_s32 (*uhepp_dev_ready_cb)(uhepp_info_t *epp, uhepp_void *userdata);

/**
 * @brief 设备子板信息通知回调定义
 */
typedef uhepp_s32 (*uhepp_subboard_info_cb)(uhepp_u8 num, up_dev_info_t *sub_board, uhepp_void *userdata);

/**
 * @brief 设备进入配置回调定义
 */
typedef uhepp_s32 (*uhepp_enter_cfg_cb)(uhepp_u8 type, uhepp_void *userdata);

/**
 * @brief 设备退出配置回调定义
 */
typedef uhepp_s32 (*uhepp_exit_cfg_cb)(uhepp_void *userdata);

/**
 * @brief 设备回复出厂设置回调定义
 */
typedef uhepp_s32 (*uhepp_factory_reset_cb)(uhepp_void *userdata);

/**
 * @brief 设备异常回调定义
 */
typedef uhepp_s32 (*uhepp_dev_exception_cb)(const uhepp_s32 err_num);

/**
 * @brief 设备日志开关回调u8
 */
typedef uhepp_s32 (*uhepp_syslog_config_cb)(const uhepp_u16 config_val);

/**
 * @brief WIFI网络信息结构体
 */
typedef struct
{
    /**
     * @brief
     * 0x00->通信正常, 0x01->连接AP失败, 0x02->连接服务器失败, 03->设备进入配置
     */
    uhepp_u8 net_status;
    /**
     * @brief
     * 此字段只有在net_status为0x00，0x02才有意义
     */
    uhepp_u8 rssi;
    /**
     * @brief
     * 0x01->softap, 0x02->smartlink, 0x03->WPS, 03->BT
     */
    uhepp_u8 config_type;
    /**
     * @brief
     * 0x00->未配置, 0x01->配置中, 0x02->已经配置
     */
    uhepp_u16 config_status;
    /**
     * @brief
     * 0x00->未配对, 0x01->配对中, 0x02->已配对
     */
    uhepp_u16 linkagePair_status;
} uhepp_net_wifi_status_info_t;

/**
 * @brief 获取WIFI网络信息回调函数
 * @return 0表示获取成功，其他值表示执行失败
 */
typedef uhepp_s32 (*uhepp_net_wifi_status_get_cb)(uhepp_net_wifi_status_info_t *status);

/**
 * @brief BLE网络信息结构体
 */
typedef struct
{
    /**
     * @brief
     * 0x00-未连接, 0x01->已连接
     */
    uhepp_u8 net_status;
    /**
     * @brief
     * 0x00->未配置, 0x01->配置中, 0x02->已经配置
     */
    uhepp_u16 config_status;
    /**
     * @brief
     * 0x00->未配对, 0x01->配对中, 0x02->已配对
     */
    uhepp_u16 linkagePair_status;
} uhepp_net_ble_status_info_t;

/**
 * @brief 获取BLE网络信息回调函数
 * @return 0表示获取成功，其他值表示执行失败
 */
typedef uhepp_s32 (*uhepp_net_ble_status_get_cb)(uhepp_net_ble_status_info_t *status);

/**
 * @brief 感知信息结构体
 */
typedef struct
{
    /**
     * @brief 人感功能开关状态
     * 0x00-关, 0x01->开
     */
    uhepp_u8 status;
    /**
     * @brief
     * 0x00-无人, 0x01->有人
     */
    uhepp_u8 somebody;
    /**
     * @brief
     */
    uhepp_u32 distance_min;
    /**
     * @brief
     */
    uhepp_u32 distance_max;
} uhepp_net_radar_status_info_t;

/**
 * @brief 获取雷达信息回调函数
 * @return 0表示获取成功，其他值表示执行失败
 */
typedef uhepp_s32 (*uhepp_net_radar_status_get_cb)(uhepp_net_radar_status_info_t *status);

/**
 * @brief 人感开关状态切换回调定义
 */
typedef uhepp_s32 (*uhepp_radar_status_switch_cb)(uhepp_u8 radar_switch);

/**
 * @brief 设置人感有人到无人的延时时间
 */
typedef uhepp_s32 (*uhepp_radar_set_delay_time_cb)(uhepp_u32 delay_time);

/**
 * @brief E++ ota备份分区的大小
 * @details
 * @return 实际ota备份分区的大小
 */
typedef uhepp_u32 (*uhepp_ota_bank_size_get_cb)(uhepp_void);

/**
 * @brief E++ OTA请求结果
 *
 */
enum
{
    UHEPP_OTA_STSRT_RESULT_ALLOWED = 0x00,
    UHEPP_OTA_STSRT_RESULT_REJECTED,
    UHEPP_OTA_STSRT_RESULT_IDLE,
};

/**
 * @brief E++ ota开始响应
 * @details 此状态可能会上报多次
 * @param progress 0x00 - 未下载, 0x64 下载完成
 * @param result  0x00 - 允许升级,设备APP需等待ota超时或者底通知停止ota
 *                0x01 - 拒绝升级,设备APP停止ota,上报错误码122
 *                0x02 - 空闲升级, 设备APP需等待ota超时或者底通知停止ota
 * @return 0表示执行成功，其他值表示执行失败
 */
typedef uhepp_s32 (*uhepp_ota_start_rsp_cb)(uhepp_u8 progress, uhepp_u8 result);

/**
 * @brief E++ ota请求数据回调
 * @details
 * @param offset     相对于整个固件请求偏移地址
 * @param size       请求数据长度
 * @return 0表示执行成功，其他值表示执行失败
 */
typedef uhepp_s32 (*uhepp_ota_data_req_cb)(uhepp_u32 offset, uhepp_u32 size);

/**
 * @brief E++ ota升级结果通知
 * @param err 0表示升级成功，其他值升级失败
 * @return 0表示执行成功，其他值表示执行失败
 */
typedef uhepp_s32 (*uhepp_ota_stop_notify_cb)(uhepp_s32 err);

typedef enum
{
    UHEPP_OTA_ZONE_TYPE_IMAGE,
    UHEPP_OTA_ZONE_TYPE_CONFIG,
} uhepp_ota_zone_type_e;
/**
 * @brief  E++ ota写固件数据回调接口
 * @param  zone_type
 * @param  offset 相对于flash或文件的存储偏移地址
 * @param  buf    缓存数据指针
 * @param  size   缓存数据长度
 * @return 大于0  表示实际写的数据长度，其他值表示读取错误
 */
typedef uhepp_s32 (*uhepp_ota_write_fw_cb)(uhepp_ota_zone_type_e type, uhepp_u32 offset, uhepp_u8 buf[], uhepp_u32 size);

/**
 * @brief  E++ ot读固件数据回调接口
 * @param  zone_type
 * @param  offset 相对于flash或文件的存储偏移地址
 * @param  buf    缓存数据指针
 * @param  size   缓存数据长度
 * @return 大于0  表示实际读取的数据长度，其他值表示读取错误
 */
typedef uhepp_s32 (*uhepp_ota_read_fw_cb)(uhepp_ota_zone_type_e type, uhepp_u32 offset, uhepp_u8 buf[], uhepp_u32 size);

/**
 * @brief E++ ota 组件下载结束通知
 * @return 0表示执行成功，其他值表示执行失败
 */
typedef uhepp_s32 (*uhepp_ota_download_complete_cb)(uhepp_void);

/**
 * @brief E++ 是否支持ota通知
 * @param  flag  0x00不支持,0x01支持ota
 * @return 0表示执行成功，其他值表示执行失败
 */
typedef uhepp_s32 (*uhepp_ota_support_notify_cb)(uhepp_u8 flag);

typedef struct
{
    /**
     * @brief
     * 文件标识，用于标识交互中的文件
     */
    uhepp_u16 id;

    /**
     * @brief 文件的描述信息
     *
     */
    struct
    {
        uhepp_u16 file_type; /**< 文件类型 */
        uhepp_u32 file_size; /**< 文件大小 */
        uhepp_u8 url[128];   /**< url 最大128字节，字符串 */
        uhepp_u8 check_alg;  /**< 校验算法 */
        uhepp_u8 degist[32]; /**< 摘要信息 */
        uhepp_u8 auth[32];   /**< 认证信息 */
    } desc;

} uhepp_file_desc_t;

/**
 * @brief E++内置文件通知。
 * @param[out] 文件个数
 * @param[out] 文件描述信息数组 @ref uhepp_file_desc_t
 * @return 0表示执行成功，其他值表示执行失败
 *
 */
typedef uhepp_s32 (*uhepp_file_desc_notify_cb)(uhepp_u8 file_num, uhepp_file_desc_t file_descs[]);

typedef struct
{

    /**
     * @brief 蓝牙广播透传规则
     *
     */
    uhepp_u8 rule_relation; /**< 0: 条件为‘或’关系； 1: 条件为‘与’关系 */

    uhepp_u32 CID_num; /**< CID 数量 */
    uhepp_u16 *cids;

    uhepp_u32 MAC_num; /**< mac 数量*/
    struct
    {
        uhepp_u8 mac_type; /**< 类型：目前为0 */
        uhepp_u8 mac[6];
    } * macs;

    uhepp_u32 serviceUUID_num; /**< UUID数量*/
    struct
    {
        uhepp_u8 uuid_type; /**< 0x00 16bits UUID；0x01 32bits UUID；0x02 128bits UUID */
        uhepp_u8 uuid[16];
    } * uuids;

} uhepp_ble_adv_scan_passthrough_rules_t;

typedef struct
{
    uhepp_u8 opt;         /* A0061617 */
    uhepp_u8 is_flag_end; /* A0061617 */
    uhepp_ble_adv_scan_passthrough_rules_t *rules;
} uhepp_ble_adv_scan_passthrough_set_t;

/**
 * @brief E++蓝牙透传规则设置请求。
 * @param[in] opt 0: 清除所有规则； 1: 设置规则；
 * @param[in] seqNo 设置序号，收到响应时，用此需要判定消息对；0-65535。用户填入
 * @param[in] rules 规则信息，若opt=0，则填空；若opt=1，则填充 @ref uhepp_ble_adv_scan_passthrough_rules_t
 * @return 0表示执行成功，其他值表示执行失败
 *
 */
typedef uhepp_s32 (*uhepp_ble_adv_scan_passthrough_rules_set_req_cb)(uhepp_ble_adv_scan_passthrough_set_t *rules);

typedef struct uhepp_module_ver_info
{
    uhepp_s8 hw_type[8];
    uhepp_s8 hw_ver[8];
    uhepp_s8 sw_type[16];
    uhepp_s8 sw_ver[8];
    uhepp_s8 short_ver[4];
    uhepp_s8 platform[32];
} uhepp_module_ver_info_t;

/**
 * @brief 获取模组版本回调函数
 * @return 0表示获取成功，其他值表示执行失败
 */
typedef uhepp_s32 (*uhepp_module_ver_get_cb)(uhepp_module_ver_info_t *ver);

#define MAX_DEVNUM_LEN (20)
typedef struct uhepp_usr_data_devnum
{
    unsigned char devnum_flags; /*CONF_FLAG_XXX*/
    unsigned char devnum_len;   /*sizeof device_num*/
    unsigned char devnum[MAX_DEVNUM_LEN];
} uhepp_usr_data_devnum_t;

/**
 * @brief 获取模组版本回调函数
 * @return 0表示获取成功，其他值表示执行失败
 */
typedef uhepp_s32 (*uhepp_dev_num_get_cb)(uhepp_usr_data_devnum_t *devnum);

/**
 * @brief 进入场景配对回调
 * @return 0表示获取成功，其他值表示执行失败
 */
typedef uhepp_s32 (*uhepp_enter_scene_pair_cb)(uhepp_void *params);

/**
 * @brief 退出场景配对回调
 * @return 0表示获取成功，其他值表示执行失败
 */
typedef uhepp_s32 (*uhepp_exit_scene_pair_cb)(uhepp_void *param);

/**
 * @brief 场景操作，目前支持场景的启停
 * @return 0表示获取成功，其他值表示执行失败
 */
typedef uhepp_s32 (*uhepp_op_scene_cb)(uhepp_void *param);

/**
 * @brief 通知给设备APP进低功耗
 * @param[in] lpower_mode  取值范围如下
 *				WAIT1:0x0001,
 *				WAIT2:0x0002,
 *				WAIT3:0x0003/0x0004,
 *				SLEEP:0x0005/0x0006,
 *				SLEEP2:0x0014,
 *				SLEEP3:0x0013
 * @return 0表示获取成功，其他值表示执行失败
 */
typedef uhepp_s32 (*uhepp_enter_low_power_cb)(uhepp_u16 lpower_mode);

/**
 * @brief 通知给设备APP退出低功耗
 * @return 0表示获取成功，其他值表示执行失败
 */
typedef uhepp_s32 (*uhepp_exit_low_power_cb)(uhepp_void);

#define uhepp_evt_subscribe(event, cb) uhepp_evt_subscribe_for_##event(cb);

#define DECLARE_DEV_EPP_EVT_CALLBACK(event, cb) UHSD_API uhepp_s32 uhepp_evt_subscribe_for_##event(cb);
DECLARE_DEV_EPP_EVT_CALLBACK(UHEPP_DEVICE_READY, uhepp_dev_ready_cb cb)
DECLARE_DEV_EPP_EVT_CALLBACK(UHEPP_SUBBOARD_INFO_RPT, uhepp_subboard_info_cb cb)
DECLARE_DEV_EPP_EVT_CALLBACK(UHEPP_ENTER_CFG, uhepp_enter_cfg_cb cb)
DECLARE_DEV_EPP_EVT_CALLBACK(UHEPP_EXIT_CFG, uhepp_exit_cfg_cb cb)
DECLARE_DEV_EPP_EVT_CALLBACK(UHEPP_FACTORY_RESET, uhepp_factory_reset_cb cb)
DECLARE_DEV_EPP_EVT_CALLBACK(UHEPP_DEVICE_EXCEPTION, uhepp_dev_exception_cb cb)
DECLARE_DEV_EPP_EVT_CALLBACK(UHEPP_SYSLOG_CONFIG, uhepp_syslog_config_cb cb)
DECLARE_DEV_EPP_EVT_CALLBACK(UHEPP_WIFI_STATUS_GET, uhepp_net_wifi_status_get_cb cb)
DECLARE_DEV_EPP_EVT_CALLBACK(UHEPP_BLE_STATUS_GET, uhepp_net_ble_status_get_cb cb)
DECLARE_DEV_EPP_EVT_CALLBACK(UHEPP_OTA_BANK_SIZE_GET, uhepp_ota_bank_size_get_cb cb)
DECLARE_DEV_EPP_EVT_CALLBACK(UHEPP_OTA_START_RSP, uhepp_ota_start_rsp_cb cb)
DECLARE_DEV_EPP_EVT_CALLBACK(UHEPP_OTA_DATA_REQ, uhepp_ota_data_req_cb cb)
DECLARE_DEV_EPP_EVT_CALLBACK(UHEPP_OTA_STOP_NOTIFY, uhepp_ota_stop_notify_cb cb)
DECLARE_DEV_EPP_EVT_CALLBACK(UHEPP_OTA_WRITE_FW, uhepp_ota_write_fw_cb cb)
DECLARE_DEV_EPP_EVT_CALLBACK(UHEPP_OTA_READ_FW, uhepp_ota_read_fw_cb cb)
DECLARE_DEV_EPP_EVT_CALLBACK(UHEPP_OTA_DOWNLOAD_COMPLETE, uhepp_ota_download_complete_cb cb)
DECLARE_DEV_EPP_EVT_CALLBACK(UHEPP_OTA_SUPPORT_NOTIFY, uhepp_ota_support_notify_cb cb)
DECLARE_DEV_EPP_EVT_CALLBACK(UHEPP_FILE_DESC_NOTIFY, uhepp_file_desc_notify_cb cb)
DECLARE_DEV_EPP_EVT_CALLBACK(UHEPP_MODULE_VER_GET, uhepp_module_ver_get_cb cb)
DECLARE_DEV_EPP_EVT_CALLBACK(UHEPP_DEV_NUM_GET, uhepp_dev_num_get_cb cb)
DECLARE_DEV_EPP_EVT_CALLBACK(UHEPP_BLE_ADV_SCAN_PASSTHROUGH_RULES_SET_REQ, uhepp_ble_adv_scan_passthrough_rules_set_req_cb cb)
DECLARE_DEV_EPP_EVT_CALLBACK(UHEPP_RADAR_STATUS_GET, uhepp_net_radar_status_get_cb cb)
DECLARE_DEV_EPP_EVT_CALLBACK(UHEPP_RADAR_STATUS_SWITCH, uhepp_radar_status_switch_cb cb)
DECLARE_DEV_EPP_EVT_CALLBACK(UHEPP_RADAR_SET_DELAY_TIME, uhepp_radar_set_delay_time_cb cb)

DECLARE_DEV_EPP_EVT_CALLBACK(UHEPP_ENTER_SCENE_PAIR, uhepp_enter_scene_pair_cb cb)
DECLARE_DEV_EPP_EVT_CALLBACK(UHEPP_EXIT_SCENE_PAIR, uhepp_exit_scene_pair_cb cb)
DECLARE_DEV_EPP_EVT_CALLBACK(UHEPP_OP_SCENE_PAIR, uhepp_op_scene_cb cb)

DECLARE_DEV_EPP_EVT_CALLBACK(UHEPP_ENTER_LOW_POWER, uhepp_enter_low_power_cb cb)
DECLARE_DEV_EPP_EVT_CALLBACK(UHEPP_EXIT_LOW_POWER, uhepp_exit_low_power_cb cb)

/**
 * @brief E++设备控制响应。
 * @details
 * @verbatim
 * [SDR 不支持标准模型]
 * eg.
 * 1. 指定参数查询响应:     op_name="026C01"    pairs[0].name="07eb" paris[0].value="xxxx"
 * 2. 全量属性查询响应：    op_name="026D01"    pairs=[0].name="" pairs[0].value="016400640000000101000000"
 * [开机-100%亮度-100%电池等共12字节，使用字符串表示为24字符]
 * 3. 大数据属性查询响应:   op_name="027D01"    pairs=[0].name="" pairs[0].value="xxxxxxxx"
 * 4. 全量告警查询响应：    op_name="740F5A"    pairs=[0].name="" pairs[0].value="0001"
 * 5. 单属性控制响应:      op_name="026D01"    pairs[0].name="" pairs[0].value="016400640000000101000000"
 * [开机-100%亮度-100%电池等共12字节，使用字符串表示为24字符]
 * 6. 设置指定参数：        op_name="026C01"    pairs[0].name="07eb" pairs[0].value="xxxx"
 * 6. 组命令响应:          op_name="026D01"     pairs[0].name="" pairs[0].value="016400640000000101000000"
 * [开机-100%亮度-100%电池等共12字节，使用字符串表示为24字符]
 * 7. 组命令交互:          op_name="05"    pairs=NULL
 * 8. 全量属性上报：        op_name="066D01"    pairs[0].name="" pairs[0].value="xxxx"
 * 9. 全量告警上报：        op_name="040F5A"    pairs[0].name="" pairs[0].value="xxxx"
 * 10. 大数据主动上报       op_name="067D01"    pairs[0].name="" pairs[0].value="016400640000000101000000"
 * 11. 大数据补充主动上报:  op_name="067D02"    pairs[0].name="" pairs[0].value="016400640000000101000000"
 *
 * @endverbatim
 *
 * @param[out] devHandle     设备句柄
 * @param[out] req_sn        数据响应sn， 如果为数据上报，则上报时为0
 * @param[out] pairs         属性集合
 * @param[out] result        操作类型
 * @param[out] traceId       链式跟踪标识
 *
 * @attention
 * SDR与SDL的 接口参数说明不同，请参考具体用户手册。SDR仅支持全量属性上报/响应。
 *
 *
 */
typedef uhepp_s32 (*uhepp_op_binary_callback)(uhepp_u8 devAddr,
                                              uhepp_s32 req_sn,
                                              const uhepp_u8 *op_name,
                                              uhepp_pair_t pairs[],
                                              uhepp_s32 pairs_num,
                                              const uhepp_s32 result,
                                              const uhepp_u8 *traceId);

/**
 * @brief
 *
 * @param rx_cb
 * @return uhepp_s32
 */
UHSD_API uhepp_s32 uhepp_device_op_binary_register(uhepp_op_binary_callback op_cb);

/**
 * @brief 向E++设备发送命令
 * @details
 * 支持：
 * 1. 全量属性查询 "014D01"
 * 2. 单属性控制 "015D"
 * 3. 全量告警查询 “730F5A"
 * @verbatim
 * eg.
 * 1. 查询指定参数:     op_name="014C01"    pairs[0].name="07eb" pairs[1].name="17c5"
 * 2. 全量属性响应：    op_name="014D01"    pairs=NULL
 * 3. 大数据属性查询:   op_name="014DFE"    pairs=NULL
 * 4. 全量告警查询：    op_name="73"        pairs=NULL
 * 5. 单属性控制:      op_name="015D"       pairs[0].name="01" pairs[0].value="xxxx"
 * 6. 设置指定参数：    op_name="015C01"    pairs[0].name="07eb" pairs[0].value="xxxx" pairs[1].name="17c5" pairs[1].value="xxxx"
 * 7. 组命令主从:      op_name="01"         pairs[0].name="6001" pairs[0].value="xxxx"
 * 7. 组命令交互:       op_name="60"        pairs[0].name="0001" pairs[0].value="xxxx"
 * 8. 告警解除:         op_name="09"        pairs=NULL
 *
 * @endverbatim
 *
 * @param devAddr
 * @param req_sn
 * @param op_name 操作类型
 * @param pairs
 * @param pairs_num
 * @param traceId U+提供接口 @ref xxx 用户生成后，填入。
 * @return uhepp_s32
 */
UHSD_API uhepp_s32
uhepp_device_op_binary(uhepp_u8 devAddr, uhepp_s32 req_sn, const uhepp_u8 *op_name, uhepp_pair_t pairs[], uhepp_s32 pairs_num, const uhepp_u8 *traceId);

/**
 * @brief 数据透传。
 * @details
 *
 *
 * @param[out] req_sn        请求sn，开发者调用read_resp时需要传回
 * @param[out] op_name       操作类型
 * @param[out] pairs         属性集合
 * @param[out] traceId       链式跟踪标识
 *
 * @attention SDR与SDL的 property_name 说明不同，请参考具体用户手册。
 *
 */
typedef uhepp_s32 (*uhepp_transparent_cb)(uhepp_u8 devAddr, uhepp_s32 req_sn, uhepp_u8 *value, uhepp_u8 value_len, const uhepp_u8 *traceId);

/**
 * @brief E++透传数据注册
 * @details
 * 设备APP通过此接口
 * @param rx_cb
 * @return uhepp_s32
 */
UHSD_API uhepp_s32 uhepp_transparent_register(uhepp_transparent_cb op_cb);

/**
 * @brief
 *
 * @param devHandle
 * @param req_sn
 * @param op_name
 * @param pairs
 * @param pairs_num
 * @param traceId U+提供接口 @ref xxx 用户生成后，填入。
 * @return uhepp_s32
 */
UHSD_API uhepp_s32 uhepp_transparent_tx(uhepp_u8 devAddr, uhepp_s32 req_sn, const uhepp_u8 *value, uhepp_u8 value_len, const uhepp_u8 *traceId);

enum
{
    UHEPP_CONFIG_TYPE_POWER_ON_ENTER_CONIFG = 0x01,
    UHEPP_CONFIG_TYPE_ENTER_CONIFG,
    UHEPP_CONFIG_TYPE_EXIT_CONIFG,
};

/**
 * @brief e++设备类型设置
 * @details
 * 调用此接口后，SDR-E++插件会向E++底板发送F7命令，告知底板模块已经进入配置。
 *
 * @param type 1 进入配置模式， 2 退出配置模式
 * @return uhepp_s32
 * @retval 0 成功
 */
UHSD_API uhepp_s32 uhepp_config_type_set(uhepp_u16 type);

/**
 * @brief E++设备网络授时使能
 * @details
 * 默认开启。
 * 如果用户不需要使能，则调用此接口关闭。
 * 开启后，如果模块有时间同步，则E++组件会主动向底板发送给时间同步消息。
 *
 * E++SDK订阅wifi/ble组件实现。不需要设备APP参与。
 *
 * @param enable
 * @return uhsd_s32
 */
UHSD_API uhepp_s32 uhepp_epp_datetime_sync_enable(uhepp_u8 enable);

/**
 * @brief 成品编码查询回调定义
 */
typedef uhepp_s32 (*uhepp_model_num_resp_cb)(const uhepp_u8 *modelnum);

/**
 * @brief 成品编码查询
 *
 * @param model_num_resp_cb
 * @return uhepp_s32
 */
UHSD_API uhepp_s32 uhepp_modelnum_query(uhepp_model_num_resp_cb cb);

/**
 * @brief 子板信息查询查询回调定义
 */
typedef uhepp_s32 (*uhepp_subboard_info_resp_cb)(const up_dev_info_t *subboard_info);

/**
 * @brief 子板信息查询
 *
 * @param subboard_info_resp_cb
 * @return uhepp_s32
 */
UHSD_API uhepp_s32 uhepp_subboard_info_query(uhepp_subboard_info_resp_cb cb);

/**
 * @brief F7定时网络状态上报功能
 * @details 如果开启，则E++自动进行10s网络状态变更。
 * E++SDK订阅wifi/ble组件实现。不需要设备APP参与。
 *
 * @param enable
 * @return uhsd_s32
 */
UHSD_API uhepp_s32 uhepp_netinfo_rpt_enable(uhepp_u8 enable);

/**
 * @brief  连接类型定义
 */
typedef enum
{
    UHEPP_CONNECT_TYPE_CLOUD,
    UHEPP_CONNECT_TYPE_BLE,
    UHEPP_CONNECT_TYPE_MAX,
} uhepp_connet_type_e;

/**
 * @brief  连接状态上报
 * @param type  连接类型 定义如上
 * @param status 连接状态 1 连接成功， 0， 连接失败
 * @return uhsd_s32
 */
UHSD_API uhepp_s32 uhepp_netinfo_status_rpt(uhepp_connet_type_e type, uhepp_u8 status);

/**
 * @brief  模块状态上报F7
 * @return uhsd_s32
 */
UHSD_API uhepp_s32 uhepp_module_state_notify(uhepp_void);

/**
 * @brief 时间同步通知
 * @return uhsd_s32
 */
UHSD_API uhepp_s32 uhepp_time_sync_notify(uhepp_void);

/**
 * @brief 子固件描述信息结构体
 * @return uhsd_s32
 */
typedef struct
{
    uhepp_u32 offset;   /* 子固件包地址 底板固件包在整个固件包中的偏移量地址（offset）*/
    uhepp_u32 data_len; /* 子固件包长度 底板固件包内容长度（size）即从整个固件包初始地址0开始计算，截取地址offset~（offset+size）的内容，既是本底板固件包内容 */
    uhepp_u8 digest_alg; /* 整机固件摘要算法 0x00 SHA256, 0x01 MD5 */
    uhepp_u8 digest[32]; /* SHA256计算的该“子固件包内容”摘要信息 十六进制存储 */
} uhepp_ota_fw_desc_t;

/**
 * @brief epp 固件描述信息定义
 */
typedef struct
{
    /*
     * 整机固件长度
     */
    uhepp_u32 fw_len;
    /*
     * 总固件个数包含设备APP和底板固
     */
    uhepp_u8 fw_num;
    /*
     * 底板固件描述信息个数,如果本地打包，此值是1；
     */
    uhepp_u8 fw_desc_num;
    /*
     * 底板固件描述信息,如果本地打包，此数组只有一个描述信息。
     * 其中描述信息中offset为0，data_len等于fw_len, digest就是整机固件的摘要信息
     */
    uhepp_ota_fw_desc_t *fw_descs;
} uhepp_ota_fw_desc_info_t;

/**
 * @brief 注册E++请求数据回调函数
 * @param sn        OTA session信息
 * @param desc_info 底板固件描述信息，不包含设备APP固件信息
 * @param pkt_type  固件打包类型  0x00 在线打包,0x01 本地打包
 * @return 成功返回0，失败返回其他值
 */
UHSD_API uhepp_s32 uhepp_ota_set_fw_desc(uhepp_u32 upgrade_sn, uhepp_ota_fw_desc_info_t *desc_info, uhepp_u8 pkt_type);

/**
 * @brief E++ ota开始请求
 * @details 开始ota的结果见回调通知
 * @return 成功返回0，失败返回其他值
 */
UHSD_API uhepp_s32 uhepp_ota_start_req(uhepp_void);

/**
 * @brief E++ ota停止
 * @details  设备APP OTA超时主动停止OTA
 * @param reason 停止原因
 * @return 成功返回0，失败返回其他值
 */
UHSD_API uhepp_s32 uhepp_ota_stop(uhepp_s32 reason);

/**
 * @brief E++ ota请求数据响应
 * @details
 * @param offset 响应offset
 * @param buf    响应数据缓存指针
 * @param size   响应数据长度
 * @return 成功返回0，失败返回其他值
 */
UHSD_API uhepp_s32 uhepp_ota_data_rsp(uhepp_u32 offset, uhepp_u8 buf[], uhepp_u32 size);

/**
 * @brief E++ 文件数据回复
 */
typedef uhepp_s32 (*uhepp_file_data_read_resp_cb)(uhepp_u32 offset, uhepp_u8 buf[], uhepp_u32 size);

/**
 * @brief E++ 读取E++设备内置的文件
 * @details
 * 当收到 @ref UHEPP_FILE_DESC_NOTIFY 事件通知后，调用者可使用此接口读取E++设备的内置文件数据。
 * @param id     文件标识
 * @param offset 响应offset
 * @param buf    响应数据缓存指针
 * @param size   响应数据长度
 * @return 成功返回0，失败返回其他值
 */
UHSD_API uhepp_s32 uhepp_file_data_read(uhepp_u16 id, uhepp_u32 offset, uhepp_u32 size, uhepp_file_data_read_resp_cb read_resp_cb);

/**
 * @brief 设置响应
 *
 * @param seqNo 设置序号，对应 @ref uhepp_ble_adv_scan_passthrough_rules_req_cb 中的seqID
 * @param result 设置结果：0:成功；1:扫描规则前后矛盾；2:扫描规则解析失败；3:无可用资源；4:扫描规则已执行。
 * @return UHSD_API
 */
UHSD_API uhepp_s32 uhepp_ble_adv_scan_passthrough_rules_set_rsp(uhepp_u16 seqNo, uhepp_s32 result);

/**
 * @brief
 *
 * @param seqNo
 * @param result
 * @return UHSD_API
 */
UHSD_API uhepp_s32 uhepp_ble_adv_scan_passthrough_data(uhepp_u8 *data, uhepp_u16 data_len);

#define UHEPP_SCENE_PAIR_RESULT_SUCCESS 0
#define UHEPP_SCENE_PAIR_RESULT_FAILURE -1

#define UHEPP_SENCE_PAIR_RESULT_ERRORNUM_NONE           0
#define UHEPP_SENCE_PAIR_RESULT_ERRORNUM_USER_CANCEL    1
#define UHEPP_SENCE_PAIR_RESULT_ERRORNUM_PAIRDEV_EXCEED 2
#define UHEPP_SENCE_PAIR_RESULT_ERRORNUM_TIMEOUT        3
#define UHEPP_SENCE_PAIR_RESULT_ERRORNUM_NO_PRESET_DATA 4
#define UHEPP_SENCE_PAIR_RESULT_ERRORNUM_UNSUPPORT      5
#define UHEPP_SENCE_PAIR_RESULT_ERRORNUM_OTHER          128

/**
 * @brief e++配对结果通知
 * @details
 * 当。
 *
 * @param result      -1 配对失败 0 配对成功
 * @param error_num 错误码
 * @return uhepp_s32
 * @retval 0 成功
 */
UHSD_API uhepp_s32 uhepp_scene_pair_result_notify(uhepp_s32 result, uhepp_s32 error_num);

/**
 * @brief e++场景状态通知
 * @details
 * 当。
 *
 * @param pair_state      0x0400 未配对； 0x0401 配对中； 0x0402 已配对
 * @param onoff_state     0x0500 已停止； 0x0501 启停中； 0x0502 已启用
 * @return uhepp_s32
 * @retval 0 成功
 */
UHSD_API uhepp_s32 uhepp_scene_state_notify(uhepp_u16 pair_state, uhepp_u16 onoff_state);

/**
 * @brief 进低功耗结果通知
 * @param result ：0:进低功耗完成, 1:进低功耗超时
 * @return uhepp_s32
 * @retval 0 成功
 */
UHSD_API uhepp_s32 uhepp_lpower_enter_result_notify(uhepp_s32 result);

/**
 * @brief E++软件初始化
 * @return uhepp_s32
 */
UHSD_API uhepp_s32 uhepp_init(uhepp_void);

/**
 * @brief E++软件使能
 * @return uhepp_s32
 */
UHSD_API UHSD_API uhepp_s32 uhepp_run(uhepp_void);

#ifdef __cplusplus
}
#endif

#endif /*__UHEPP_SDK_H__*/

/** @}*/
