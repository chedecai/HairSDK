/**
 * @defgroup grp_uhsdfota FOTA接口
 * @{
 * @author hudx (hudingxuan.xian@haier.com)
 * @brief FOTA功能
 * @version 0.1
 * @date 2022-08-03
 *
 * @details
 * 设备的版本信息需要通过设备管理接口进行注册。
 * 只有通过设备管理注册了设备版本后，才可以支持OTA功能。
 * @verbatim
 * 名词解释：
 * UHSD = U+ Home SmartDevice
 * DEVAPP = 设备APP 用户基于SmartDevice开发的应用程序。
 * @endverbatim
 *
 * @copyright Copyright (c) 2021, Haier.Co, Ltd.
 *
 * @file uhsd_ota.h
 *
 */

#ifndef __UHSD_OTA_H__
#define __UHSD_OTA_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "uhsd_types.h"

/**
 * @brief 通用OTA错误码
 *
 */
typedef enum
{
    /**
     * @brief 成功
     */
    UHSD_OTA_ERROR_OK = 0,
    /**
     * @brief 其它错误
     */
    UHSD_OTA_ERROR_OTHER = 99,
    /**
     * @brief 内容校验失败（如OTA升级包（整包）md5校验失败）
     */
    UHSD_OTA_ERROR_CONTENT_VALIDATION_FAILURE = 106,
    /**
     * @brief 非法的内容（如OTA升级包与目标平台、硬件不匹配；如更改远程域名时，域名或IP格式不合法）
     */
    UHSD_OTA_ERROR_ILLEGAL_CONTENT = 107,
    /**
     * @brief 请求实体过大（如OTA升级包大小）
     */
    UHSD_OTA_ERROR_ENTITY_TOO_LARGE = 108,
    /**
     * @brief 资源不可用（如OTA URL推送升级中的URL获取失败）
     */
    UHSD_OTA_ERROR_RESOURCE_NOT_AVAILABLE = 109,
    /**
     * @brief 不完整的内容（如OTA升级包接收数据长度不够或接收中断失败）
     */
    UHSD_OTA_ERROR_INCOMPLETE_CONTENT = 110,
    /**
     * @brief 设备底板升级中，无法响应请求
     */
    UHSD_OTA_ERROR_DEVICE_BASEBOARD_UPGRADING = 111,
    /**
     * @brief 设备底板出现故障，无法响应请求
     */
    UHSD_OTA_ERROR_DEVICE_BASEBORAD_NOT_IN_SERVICE = 112,
    /**
     * @brief 设备网络模块升级中，无法响应请求
     */
    UHSD_OTA_ERROR_DEVICE_MODULE_UPGRADING = 113,
    /**
     * @brief 无效升级内容请求（即请求升级内容时UPGRADE_SN错误）
     */
    UHSD_OTA_ERROR_INVALID_UPGRADE_SN = 114,
    /**
     * @brief 升级内容请求偏移量越界
     */
    UHSD_OTA_ERROR_BOUNDARY = 115,
    /**
     * @brief 当前升级中，无法再次下发升级指令
     */
    UHSD_OTA_ERROR_CURRENT_UPGRADING = 116,
    /**
     * @brief 升级数据（分片后）的校验和错误
     */
    UHSD_OTA_ERROR_FRAGMENT_VALIDATION_FAILURE = 117,
    /**
     * @brief 底板升级文件大小超出范围
     */
    UHSD_OTA_ERROR_BASEBOARD_FILE_SIZE = 119,
    /**
     * @brief 底板固件升级包摘要信息校验失败
     */
    UHSD_OTA_ERROR_DIGEST_MISMATCH = 120,
    /**
     * @brief 规定时间内升级失败（只在本地升级中出现）
     */
    UHSD_OTA_ERROR_UPDATE_TIMESUP = 121,
    /**
     * @brief 底板指示不能升级
     */
    UHSD_OTA_ERROR_BASEBOARD_REJECT = 122,
    /**
     * @brief 底板指示升级失败
     */
    UHSD_OTA_ERROR_BASEBOARD_FAIL = 123,
    /**
     * @brief BLE配置错误
     */
    UHSD_OTA_ERROR_BLE_CONFIGING = 124,
    /**
     * @brief 模块因资源不足导致本次升级中止
     */
    UHSD_OTA_ERROR_DEVICE_RESOURCE_INSUFFICIENT = 126,
    /**
     * @brief 设备执行升级时，升级超时(低功耗类设备)
     */
    UHSD_OTA_ERROR_DEVICE_UPGRADE_TIMEOUT = 127,
    /**
     * @brief 设备升级时，设备电池电量低。
     */
    UHSD_OTA_ERROR_DEVICE_LOW_BATTERY = 128,
    /**
     * @brief 固件版本相同（内部错误字），升级通道需要对齐做相应的转换
     */
    UHSD_OTA_ERROR_INTERNAL_UPGRADE_VER_SAME = -1,
    /**
     * @brief 固件版本过低（内部错误字），升级通道需要对齐做相应的转换
     */
    UHSD_OTA_ERROR_INTERNAL_UPGRADE_VER_TOO_LOW = -2
} UHSD_OTA_ERROR_E;

/** @brief 包来源 */
typedef enum
{
    UHSD_OTA_PKG_LOCAL = 1, /**< 用户本地自定义格式的固件包，通过海极网本地上传功能上传。*/
    UHSD_OTA_PKG_ONLINE,    /**< 海极网在线打包工具生成的升级固件包。*/
    UHSD_OTA_PKG_MAX
} UHSD_OTA_PKG_SRC_E;

/** @brief 升级包所对应的升级目标 */
typedef enum
{
    UHSD_OTA_UPGRADE_DST_WHOLE = 0,   /**< 升级设备整机固件。*/
    UHSD_OTA_UPGRADE_DST_WIFI_MODULE, /**< 升级WIFI模块。*/
    UHSD_OTA_UPGRADE_DST_VOICE        /**< 升级语音模块。*/
} UHSD_OTA_UPGRADE_DST_E;

typedef struct _uhsd_ota_req_
{
    uhsd_u32 upgrade_sn;     /**< 升级序列号，标识一次OTA过程 */
    uhsd_u32 timeout;        /**< 预制的OTA超时时间，来源于产品创建时的设定。 单位：秒。*/
    uhsd_s32 pkg_src;        /**< 包来源，@ref UHSD_OTA_PKG_SRC_E */
    uhsd_u32 total_len;      /**< 包总体大小*/
    uhsd_u32 upgrade_dst;    /**< 升级包所对应的升级目标，@ref UHSD_OTA_UPGRADE_DST_E*/
    uhsd_u8 clear_cfg;       /**< 是否清除配置：0=升级时不清除配置，1=升级时清除配置*/
    uhsd_u8 force_upgrade;   /**< 是否强制升级*/
    uhsd_u8 is_litte_loop;   /**< 是否小循环：0：非小循环升级，1=小循环升级*/
    uhsd_u8 verifier_method; /**< 0=使用MD5校验, 1=使用SHA256校验*/
    /**
     * @brief 设备固件文件摘要信息。
     * @details
     * MD5格式：全小写十六进制字符串。
     * SHA256格式：256位二进制数据摘要数据。
     */
    uhsd_u8 verification[32];
} uhsd_ota_evt_firm_info_t;

typedef struct
{
    uhsd_ota_evt_firm_info_t firm_info;
    /**
     * @brief 整机固件类型
     *
     * @details 整机固件类型，用于标识同一个TypeId下可相互升级固件标识；
     * 升级包的整机固件类型不同，不可相互升级；
     *
     * 本字段原名称为“整机型号编码”，现在型号编码也仅仅是整机固件类型的一种取值，
     * 对原有填充“整机型号编码”的升级包天然就是兼容的；
     *【设备升级时固件包合法性校验】需要判断升级包的整机固件类型是否与当前设备的整机固件类型一致，
     * 如不一致不可进行升级；
     */
    uhsd_s8 whole_firm_type[20];

    /**
     * @brief 整机固件版本号
     *
     * @details 整机固件版本号，可取字符集[0-9a-zA-Z\.]
     *
     * 开发者定义并输入；
     */
    uhsd_s8 whole_version[14];

    /**
     * @brief 整机固件升级文案长度
     *
     */
    uhsd_s16 desc_len;

    /**
     * @brief 整机固件升级文案，最大长度2000字节
     *
     */
    uhsd_s8 *desc;

    /**
     * @brief 整机固件升级注意事项长度
     *
     */
    uhsd_s16 note_len;

    /**
     * @brief 整机固件升级注意事项，最大长度800字节
     *
     */
    uhsd_s8 *note;
    /**
     * @brief 升级任务类型：
     *          1 - 常规升级，
     *          2 - 大固件升级
     */
    uhsd_u32 task_type;
    /**
     * @brief 整机固件唯一标识
     *
     */
    uhsd_char fwId[33];
} uhsd_ota_version_qry_ack_t;

typedef struct
{
    uhsd_u32 upgrade_sn; /**< 升级序列号，标识一次OTA过程 */
    uhsd_u32 offset;     /**< 分段下载固件起始偏移。 */
    uhsd_u32 len;        /**< 分段下载固件长度。 */
    uhsd_u32 task_type;  /*< 升级任务类型：1=常规升级，, 2=大固件升级*/
    /*
     * @brief 设备固件包下载完成后动作：
     *          true ：设备固件包下载完成后，设备自动安装更新
     *          false：设备固件包下载完成后，需要用户确认后安装更新
     */
    uhsd_u8 action_after_download;
} uhsd_ota_evt_sec_firm_finish_t;

typedef struct
{
    uhsd_u32 upgrade_sn; /**< 升级序列号，标识一次OTA过程 */
    uhsd_s32 error_num;  /**< 升级异常终止原因。 */
} uhsd_ota_evt_exception_t;

/**
 * @brief SmartDevice通知设备APP OTA事件。
 * SD_SDK => DEVAPP
 *
 */
typedef enum
{
    /**
     * @brief SmartDevice 收到了设备OTA命令，向设备APP发送请求事件。
     * 设备APP使用 @ref uhsd_ota_notify_req_result 接口通知SmartDevice当前OTA请求结果。
     */
    UHSD_OTA_EVT_REQ,

    /**
     * @brief SmartDevice 通知设备APP发送固件准备就绪，并将固件信息报给APP。
     * @ref uhsd_ota_evt_firm_info_t
     */
    UHSD_OTA_EVT_FIRM_READY,

    /**
     * @brief SmartDevice 通知设备APP固件分段请求下载完成。
     */
    UHSD_OTA_EVT_SECTION_FIRM_DOWNLOAD_FINISH,
    /**
     * @brief SmartDevice 通知设备APP大固件升级需要确认。
     */
    UHSD_OTA_EVT_UPDATE_NEED_CONFIRM_FIRM,
    /**
     * @brief SmartDevice 通知设备APP大固件确认升级有效。
     */
    UHSD_OTA_EVT_CONFIRM_FIRM_UPDATE_VALIID,
    /**
     * @brief OTA异常终止。
     * @ref uhsd_ota_evt_exception_t
     * @details
     * OTA过程中，SmartDevice如有任何OTA异常，均通过此事件告知设备APP。
     */
    UHSD_OTA_EVT_EXCEPTION_OCCURED,

} UHSD_OTA_EVT_E;

typedef struct _uhsd_evt_
{
    UHSD_OTA_EVT_E evt; /**< OTA事件。 */
    union
    {
        uhsd_ota_evt_firm_info_t *firm_info;              /**< OTA事件：升级请求信息。 */
        uhsd_ota_evt_sec_firm_finish_t *down_finish_info; /**< OTA事件：分段下载结束信息。 */
        uhsd_ota_evt_exception_t *exception_info;         /**< OTA事件：异常中止信息。 */
    } evt_data;
} uhsd_evt_t;

/**
 * @brief 函数指针，用户通过注册此函数获取OTA事件通知。
 *
 * @details SDSDK=>DEVAPP
 * @param[out]  evt OTA事件。
 * @param[in out] userdata 用户通过 @ref uhsd_ota_evt_register 注册的用户数据。
 *
 * @return signed int	true: accepted ； false： an err occured.
 * @retval 0 接受事件。
 * @retval -1 发生错误，拒绝该事件。
 *
 * @attention 当返回false后，SDK会自动停止本次升级任务。
 *
 */
typedef uhsd_s32 (*uhsd_ota_evt_func_cb)(const uhsd_evt_t *evt, void *userdata);

/**
 * @brief 	OTA事件注册函数。
 * @note	注册后，将收到终端向本设备推送的OTA事件。
 * @ref		UHSD_OTA_EVT_E
 *
 * @param[in] func_cb		回调函数，用于接收OTA事件.
 * @param[in] userdata		用户自定义数据。随回调函数传递回给用户处理。
 * @return uhsd_s32
 */
UHSD_API uhsd_s32 uhsd_ota_evt_register(uhsd_ota_evt_func_cb func_cb, void *userdata);

typedef struct _uhsd_operation_
{
    /**
     * @brief 数据下载
     * @details
     * DEVAPP实现此接口。SmartDevice下载固件后，通过此接口将数据返回给用户。
     * 对于SDR，如果升级为设备APP固件，则用户需根据对应平台分区，将固件写入到OTA分区中。
     * 对于SDL，设备APP实现此接口，将固件保存。
     *
     * @param[in]   offset 数据在固件包中的偏移地址，从0开始。
     * @param[in]   buf SmartDevice获取的数据。
     * @param[in]   len SmartDevice获取的固件包数据长度。
     *
     * @return uhsd_s32	结果.
     * @retval >0  成功写入的字节数。
     * @retval <=0 失败。
     *
     *
     */
    uhsd_s32 (*firmware_write)(uhsd_u32 offset, uhsd_u8 *buf, uhsd_u32 len);

    /**
     * @brief 获取
     * @details DEVAPP实现此接口。
     * @param[in]  offset SmartDevice需要获取的固件包偏移量，从0开始。
     * @param[in out] buf SmartDevice申请，设备APP直接按照长度填充。
     * @param[in]   len SmartDevice需要获取的固件包长度。
     *
     * @return uhsd_s32
     * @retval >0  成功读入的字节数。
     * @retval <=0 失败。
     *
     *
     */
    uhsd_s32 (*firmware_read)(uhsd_u32 offset, uhsd_u8 *buf, uhsd_u32 len);

} uhsd_ota_operation_t;

/**
 * @brief 	SmartDevice操作接口设置。
 * @note	SmartDevice可以使用用户提供的接口实现对系统的操作。
 * @details
 * @verbatim
 * opt.firmware_get_cb：OTA 固件获取接口。 设备APP实现该功能并注册至SmartDevice，SmartDevice可使用该接口获取已下载的固件包。
 * @endverbatim
 * @ref		UHSD_OTA_EVT_E
 *
 * @param[in] opt		    操作类接口集合.
 * @param[in] def_param		保留数据。SmartDeviceSDK自定义数据。
 * @return uhsd_s32
 */
UHSD_API uhsd_s32 uhsd_ota_operation_set(uhsd_ota_operation_t opt, void *def_param);

/**
 * @brief 请求结果通知类型。
 * @ref uhsd_ota_notify_req_result_t
 */
typedef enum
{
    UHSD_OTA_REQ_RESULT_NOT_ALLOW, /**< 不允许升级 */
    UHSD_OTA_REQ_RESULT_LATER,     /**< 延迟升级 */
    UHSD_OTA_REQ_RESULT_ALLOW      /**< 允许升级 */
} UHSD_OTA_REQ_RESULT_E;

/**
 * @brief 通知SDK升级请求结果
 *
 * @param result：升级请求结果
 */
UHSD_API void uhsd_ota_notify_req_result(UHSD_OTA_REQ_RESULT_E result);

typedef uhsd_void (*ota_version_qry_cb)(uhsd_s32 err_num, uhsd_ota_version_qry_ack_t *qry_ack, uhsd_void *user_data);
/**
 * @brief 查询可升级的版本信息
 * @details
 * 当设备APP在云端部署OTA任务，选择手动升级后，则可以调用此接口向云端查询是否有可升级的版本信息。
 *
 * @param[in] ota_version_qry_cb 查询OTA版本回调。
 * @param[in] 用户自定义数据。
 *
 * @return uhsd_s32
 * @retval 0 成功
 *
 * @attention 当ota_version_qry_cb的err_num为0时，qry_ack有效；否则，为空参数，不可使用。
 */
UHSD_API uhsd_s32 uhsd_ota_version_qry(ota_version_qry_cb qry, uhsd_void *user_data);

/**
 * @brief 通知SDK升级开始升级
 *
 * @param upgrade_sn 升级序列号，
 * 来源 @ref uhsd_ota_version_qry @ref uhsd_ota_version_qry_ack_t @ref uhsd_ota_evt_firm_info_t @ref upgrade_sn
 * @return uhsd_s32
 * @retval 0 成功
 */
UHSD_API uhsd_s32 uhsd_ota_notify_start(uhsd_u32 upgrade_sn);

/**
 * @brief 分段固件下载
 *
 * @param upgrade_sn
 * @param offset：在整个固件包的偏移地址
 * @param len：分段下载的固件大小
 * @param userdata：用户自定义数据，在下载结束后，事件回调参数中返回；
 * @retval 0 成功
 * @retval others 失败
 */
UHSD_API uhsd_s32 uhsd_ota_download_section_firm(uhsd_u32 upgrade_sn, uhsd_u32 offset, uhsd_u32 len);

/**
 * @brief ota升级状态定义
 */
typedef enum
{
    UHSD_OTA_UPGRADE_STATUS_IDLE_NO_DOWNLOAD = 0x01, /* 空闲升级，未下载 */
    UHSD_OTA_UPGRADE_STATUS_IDLE_DOWNLOADING = 0x02, /* 空闲升级,正在下载 */
    UHSD_OTA_UPGRADE_STATUS_ALLOW = 0x03             /* 允许升级 */
} UHSD_OTA_UPGRADE_STATUS_E;

/**
 * @brief ota升级状态上报
 * @details
 * 设备ota状态上报
 * 需要调用此接口通知SmartDevice。
 *
 * @param upgrade_sn 升级时使用的sn
 * @param status 参考枚举定义
 * @param download_complete 对于SDK来讲，暂时无特殊意义，对于底板升级，需要根据实际情况来填写
 * @return uhsd_s32
 * @attention
 * 设备APP下载固件的状态，则必须调用此接口通知SmartDevice，否则会导致升级状态异常。
 */
UHSD_API uhsd_s32 uhsd_ota_upgrade_status_rpt(uhsd_u32 upgrade_sn, UHSD_OTA_UPGRADE_STATUS_E status, uhsd_bool download_complete);

/**
 * @brief 通知SDK结束整个升级流程
 *
 * @param upgrade_sn
 * @param error：错误码
 */
UHSD_API uhsd_s32 uhsd_ota_notify_stop(uhsd_u32 upgrade_sn, uhsd_s32 error);

#pragma pack(1)
/** @brief 获取整机包信息相关接口*/
typedef struct uhsd_ota_online_whole_base_info
{
    /**
     * @brief 本版包格式类型固定为：0xFFF1，设备整机固件升级包
     *
     * @details 固定值，表示当前打包规则的类型；
     */
    uhsd_u16 format_type;
    /**
     * @brief 本版包格式版本号固定为：0x0100，1.0版本
     *
     * @details 表示当前打包规则的版本；
     * 【设备升级时固件包合法性校验】需要识别收到的升级包的“包格式类型”与“包格式版本号”是否是是自己支持的，
     * 如不支持，则不可进行升级；
     */
    uhsd_u16 format_version;
    /**
     * @brief 整包CRC校验码
     *
     * @details 本字段之后全部内容进行CRC16计算的校验码。十六进制存储。
     * 主要用于某些无法支持SHA256的设备进行整包校验。
     */
    uhsd_u16 crc;
    /**
     * @brief 整包摘要信息
     *
     * @details 本字段之后全部内容进行SHA256计算的摘要信息。十六进制存储。
     */
    uhsd_u8 summary[32];
    /**
     * @brief 固件包描述信息地址
     *
     * 本版包格式固定值为（64）；
     */
    uhsd_u32 firm_desc_addr;
    /**
     * @brief 固件包描述信息地址长度
     *
     * 本版包格式固定值为（128）；
     */
    uhsd_u32 firm_desc_len;
    /**
     * @brief 固件包描述信息CRC校验码
     */
    uhsd_u16 firm_desc_crc;
    /**
     * @brief 预留
     */
    uhsd_u8 rsvd1[12];
    /**
     * @brief 固件包内容长度
     *
     * @details 除基本信息之后的总长度
     */
    uhsd_u32 firm_content_size;
} uhsd_ota_online_whole_base_info_t;

typedef struct uhsd_ota_online_whole_desc_info
{
    /**
     * @brief 整机固件类型
     *
     * @details 整机固件类型，用于标识同一个TypeId下可相互升级固件标识；
     * 升级包的整机固件类型不同，不可相互升级；
     *
     * 本字段原名称为“整机型号编码”，现在型号编码也仅仅是整机固件类型的一种取值，
     * 对原有填充“整机型号编码”的升级包天然就是兼容的；
     *【设备升级时固件包合法性校验】需要判断升级包的整机固件类型是否与当前设备的整机固件类型一致，
     * 如不一致不可进行升级；
     */
    uhsd_s8 whole_firm_type[20];
    /**
     * @brief 预留
     */
    uhsd_u8 rsvd1[3];
    /**
     * @brief 产品标识类型
     *
     * @details 产品标识摘要信息字段使用的产品标识的类型，可取值：
     * 1 -- TypeId；
     *
     * 目前仅支持TypeId，后续可扩展出PID，PPID等标识；
     */
    uhsd_u8 product_summary_type;
    /**
     * @brief 产品信息标识摘要信息
     *
     * @details 产品标识类型不同，计算规则不同，如下：
     * ● 产品标识为TypeId时：TypeId（Byte数据，非ASCII码）进行MD5计算后的Byte数据；
     *
     * 【设备升级时固件包合法性校验】需要判断升级包的产品标识摘要信息是否与当前设备的计算的摘要信息一致，
     * 如不一致不可进行升级；
     */
    uhsd_s8 product_summary[16];
    /**
     * @brief 整机固件版本号
     *
     * @details 整机固件版本号，可取字符集[0-9a-zA-Z\.]
     *
     * 开发者定义并输入；
     */
    uhsd_s8 whole_version[14];
    /**
     * @brief 整机固件自定义信息
     *
     * @details 整机固件自定义信息，由开发者自定义内容及格式，设备按照开发者定义解析处理。
     * 如不使用，可忽略。
     *
     * 开发者定义并输入；
     */
    uhsd_s8 whole_custom_info[32];
    /**
     * @brief 预留
     */
    uhsd_u8 rsvd2[30];
    /**
     * @brief 子固件包描述信息CRC校验
     *
     * @details 所有子固件包描述信息内容进行CRC16计算的校验码。十六进制存储。
     *
     * 从第一个子固件包描述信息的“子固件包类型”字段到最后一个子固件包描述信息的“预留”字段的内容进行计算；
     */
    uhsd_u16 sub_desc_crc;
    /**
     * @brief 子固件包描述信息起始地址
     *
     * 本版固定值为（192）
     */
    uhsd_u32 sub_desc_addr;
    /**
     * @brief 单个子固件包描述信息长度
     *
     * 本版固定值为（128）
     */
    uhsd_u32 sub_desc_size;
    /**
     * @brief 子固件包数量
     *
     * @details 本整机固件包中包含的子固件包数量（n）。
     * 子固件描述信息会存在n个；
     * 子固件包内容也会存在n个；
     * 即从整个固件包初始地址0开始计算，截取地址（offset+（m-1）*size）~（offset+m*size）的内容，
     * 既是第m个子固件描述信息。
     *
     * 用户添加的子固件个数；
     * 最少1个，最大8个；
     * 每个子固件均包含下方一个“子固件包描述信息”与一个“子固件包内容”；
     */
    uhsd_u16 sub_firm_count;
} uhsd_ota_online_whole_desc_info_t;

typedef struct uhsd_ota_online_sub_firm_info
{
    /**
     * @brief 子固件包类型
     *
     * @details 可取值：
     * 1 -- 设备电脑板；
     * 2 -- 语音模块；
     * 3 -- WIFI模块；
     * 4 -- BLE Mesh模块；
     * 5 -- 自定义，即用户自定义子固件；
     * 6 -- BLE SDK模块，即BLE SDK软硬一体化模块（设备App直接集成BLE SDK，并在指定模块上开发，模块FOTA过程有BLE SDK完成）；
     * 7 -- 蓝牙模组；
     */
    uhsd_u16 firm_type;
    /**
     * @brief 子固件包软件标识
     *
     * @details
     * ● 设备电脑板 --
     * 设备电脑板固件唯一标识符，及单片机固件唯一识别标识号中，除去版本号（V）的其他部分的字符串格式内容（F+L+N）。例如单片机固件唯一识别标识号F11012345678988010273，底板固件标识符为F11012345678988。
     * ● 语音模块 -- 模块软硬件标识（例如厂商型号），待定；
     * ● WIFI模块 -- 模块软硬件标识（例如厂商型号），待定；
     * ● BLE Mesh模块 -- 模块软硬件标识（例如厂商型号），待定；
     * ● 自定义 --
     * 由设备开发者自定义，但要求同一个整机型号编码下，所有不同自定义子固件包的软件标识不同（开发者需按照自己设备的个性化部件组成，自行设计软件标识，需避免相同类型部件使用相同软件标识）；
     * ● BLE SDK模块 -- 同自定义，BLE SDK所在模块的子固件包软件标识由集成其的设备App自定义；
     */
    uhsd_s8 soft_id[20];
    /**
     * @brief 子固件包版本号
     *
     * @details 子固件包版本号，可取字符集[0-9a-zA-Z\.] 开发者定义并输入
     */
    uhsd_s8 soft_version[14];
    /**
     * @brief 子固件包地址
     *
     * @details 底板固件包在整个固件包中的偏移量地址（offset）。
     *
     * 第n个子固件值，从0到底板n的子固件包内容起始地址；
     */
    uhsd_u32 firm_addr;
    /**
     * @brief 子固件包长度
     *
     * @details 底板固件包内容长度（size）。
     * 即从整个固件包初始地址0开始计算，截取地址offset~（offset+size）的内容，既是本底板固件包内容。
     *
     * 当前子固件文件的大小；
     */
    uhsd_u32 firm_size;
    /**
     * @brief 子固件包CRC校验码
     *
     * @details CRC16计算的该“子固件包内容”校验码。十六进制存储。
     * 主要用于某些无法支持SHA256的设备进行整包校验。
     */
    uhsd_u16 firm_crc;
    /**
     * @brief 子固件包摘要信息
     *
     * @details SHA256计算的该“子固件包内容”摘要信息。十六进制存储。
     */
    uhsd_u8 summary[32];
    /**
     * @brief 子固件包自定义信息
     *
     * @details 子固件包自定义信息，由开发者自定义内容及格式，设备按照开发者定义解析处理。
     * 例如，可录入底板固件包分片大小，即底版固件包内容中每行多少字节。
     * 如不使用，可忽略。
     *
     * 用户输入，字符串，最大32字符
     */
    uhsd_s8 custom[32];
    /**
     * @brief 预留
     */
    uhsd_u8 rsvd1[18];
} uhsd_ota_online_sub_firm_info_t;

#pragma pack()

/**
 * @brief 在线打包格式包基本信息获取
 *
 * @param buff 需要解析的数据
 * @param len 需要解析的数据长度，固定值64字节
 * @param[in out] base_info 获取基本信息 用户申请
 * @return uhsd_s32
 * @retval 0 成功
 * @retval others 失败
 */
UHSD_API uhsd_s32 uhsd_ota_get_online_whole_base_info(uhsd_u8 *buff, uhsd_u16 len, uhsd_ota_online_whole_base_info_t *base_info);

/**
 * @brief 在线打包格式固件描述信息
 *
 * @param buff 需要解析的数据
 * @param len 需要解析的数据长度，固定值128字节
 * @param[in out] desc_info 获取描述信息 用户申请
 * @return uhsd_s32
 * @retval 0 成功
 * @retval others 失败
 */
UHSD_API uhsd_s32 uhsd_ota_get_online_whole_desc_info(uhsd_u8 *buff, uhsd_u16 len, uhsd_ota_online_whole_desc_info_t *desc_info);

/**
 * @brief 在线打包格式固件包，子固件描述信息列表
 *
 * @param buff 需要解析的数据
 * @param len 需要解析的数据长度，固定值 desc_info->sub_firm_count * 128字节
 * @param[in out] sub_desc_list 用户申请
 * @return uhsd_s32
 * @retval 0 成功
 * @retval others 失败
 */
UHSD_API uhsd_s32 uhsd_ota_get_online_sub_firm_info(uhsd_u8 *buff,
                                                    uhsd_u16 len,
                                                    uhsd_ota_online_whole_base_info_t *base_info,
                                                    uhsd_ota_online_whole_desc_info_t *desc_info,
                                                    uhsd_ota_online_sub_firm_info_t sub_desc_list[]);

//  进度progress的范围为0-10000; 10000代表100%
typedef uhsd_s32 (*uhsd_ota_download_progress_notify)(uhsd_s32 progress);

/**
 * @brief 设置ota下载进度通知进度回调函数
 */
UHSD_API uhsd_s32 uhsd_set_ota_download_progress_notify_cb(uhsd_ota_download_progress_notify cb);

/**
 * @brief 设置ota下载完成后是否自动更新,此接口仅在查询固件 uhsd_ota_version_qry_ack_t 中task_type=2为大固件模式下有效
 * @details 注意：此接口要在下载固件固件前调用
 * @param is_auto_update 是否自动更新，0：否，1：是
 */
UHSD_API uhsd_void uhsd_set_ota_download_auto_update(uhsd_s32 is_auto_update);

typedef uhsd_void (*ota_confirm_updae_cb)(uhsd_s32 err_num, uhsd_void *user_data);
/*
 * @brief 大固件升级时，确认升级后会到云端检查升级任务是否有效
 * @details 升级任务是否有效会通过uhsd_ota_evt_func_cb func_cb 注册的回调函数返回
 * 如果任务有效会返回 UHSD_OTA_EVT_SECTION_FIRM_DOWNLOAD_FINISH 事件，用户决定是否确认升级
 * 如果任务无效会返回 UHSD_OTA_EVT_EXCEPTION_OCCURED 事件，返回错误码是127需调用uhsd_ota_notify_stop函数将
 * 错误码127通知给云端
 */
UHSD_API uhsd_s32 uhsd_ota_big_firm_confirm_update_req(ota_confirm_updae_cb comfirm_cb, uhsd_void *user_data);
#ifdef __cplusplus
}
#endif

#endif /*__UHSD_OTA_H__*/

/** @} */
