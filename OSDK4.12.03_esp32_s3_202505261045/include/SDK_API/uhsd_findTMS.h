#ifndef __uhsd_FINDTMS_H__
#define __uhsd_FINDTMS_H__
#include "uhsd_types.h"

typedef struct
{
    uhsd_u8 is_valid;
    uhsd_s32 type_main;
    uhsd_s32 type_minor;
} uhsd_ble_dev_type_info_t;

typedef struct
{
    uhsd_char device_tmpId[33]; // 设备临时id
    uhsd_char device_id[33];
    uhsd_char product_code[33];        // 产品编码
    uhsd_s32 rssi;                     // 信号强度
    uhsd_char dev_name[64];            // 设备名称
    uhsd_ble_dev_type_info_t dev_type; // 设备类型
} uhsd_findTMS_dev_attr_t;

typedef struct
{
    uhsd_findTMS_dev_attr_t dev_info;
    uhsd_s32 status;   // 配置绑定状态。必填。
    uhsd_s32 err_code; // 错误码
} uhsd_findTMS_dev_t;

/**
 * @brief:  uhsd_findTMS_find_start 后的结果返回。
 * @param: errNo 错误码，0:成功； 其他：失败。
 * @param: dev_num 扫描到的设备数量
 * @param: dev_attr_list 设备列表。。
 * @param: [in] cb_param 回传参数
 * @note:
 */
typedef uhsd_void (*findTMS_find_cb)(uhsd_s32 err_code, uhsd_u32 dev_num, uhsd_findTMS_dev_attr_t *dev_attr_list, uhsd_void *user_data);

/**
 * @brief:  找队友初始化接口。
 * @param:  返回值，0:成功； 其他：失败。
 */
UHSD_API uhsd_s32 uhsd_findTMS_init(uhsd_void);
/**
 * @brief:  找队友去初始化接口。
 * @ret:  返回值，0:成功； 其他：失败。
 */
UHSD_API uhsd_s32 uhsd_findTMS_deinit(uhsd_void);

/**
 * @brief:  创建找队友对象实例。
 * @param: clientID 用户ID。。
 * @param: appID 应用ID。
 * @param: appkey 应用密钥。
 * @param: access_token 用户token
 * @param: src_devHandle 源设备句柄
 * @param: is_sync_roomInfo 是否同步设备房间信息。
 * @ret:   成功返回对象句柄，失败返回NULL。
 */
UHSD_API uhsd_void *uhsd_findTMS_crt(uhsd_char *clientId,
                                     uhsd_char *appId,
                                     uhsd_char *appKey,
                                     uhsd_char *access_token,
                                     uhsd_s32 src_devHandle,
                                     uhsd_u8 is_sync_roomInfo);
/**
 * @brief:  销毁找队友对象实例。
 * @param: findTMS_handle uhsd_findTMS_crt返回的句柄
 * @ret:   0:成功。小于0:失败。
 */
UHSD_API uhsd_s32 uhsd_findTMS_dstr(uhsd_void *findTMS_handle);
/**
 * @brief:  开始找队友。
 * @param: findTMSHandle uhsd_findTMS_crt返回的句柄
 * @param: timeout 超时时间
 * @param: cb 搜索结果回调。时间达到timeout时触发此回调接口
 * @param: cb_param 回传参数
 * @ret:   0:成功。小于0:失败。
 */
UHSD_API uhsd_s32 uhsd_findTMS_find_start(uhsd_void *findTMSHandle, uhsd_u32 timeout, findTMS_find_cb cb, uhsd_void *cb_param);

/**
 * @brief:  停止找队友
 * @ret:   0:成功。小于0:失败。
 */
UHSD_API uhsd_s32 uhsd_findTMS_find_stop(uhsd_void);

/***********************启动批量配置接口 start***********************/

// 批量整体配置绑定状态回调
#define UHSD_FINDTMS_STATE_IDLE     (0)
#define UHSD_FINDTMS_STATE_CTRLABLE (1)
#define UHSD_FINDTMS_STATE_INCONFIG (2)
#define UHSD_FINDTMS_STATE_TRIGABLE (3)
#define UHSD_FINDTMS_STATE_CONFIGED (4)
#define UHSD_FINDTMS_STATE_UNBINDED (5)
#define UHSD_FINDTMS_STATE_BINDED   (6) // 配置绑定成功, 只有6是成功，其他都是异常
#define UHSD_FINDTMS_STATE_TIMEOUT  (7) // 配置超时

// 具体error code 数值
#define UHSD_E_FINDTMS_BASE             (-600)
#define UHSD_E_FINDTMS_NOT_ALL_CONFIGED (-601)
#define UHSD_E_FINDTMS_STOP_FROM_APP    (-650)

typedef struct findTMS_config_s
{
    uhsd_s32 state;    // @ref UHSD_FINDTMS_STATE_XXX
    uhsd_s32 err_code; // state不等于UHSD_FINDTMS_STATE_BINDED, 再看err_code，err_code值来自于蓝牙上报
    uhsd_char *device_tmpID;
    uhsd_char *device_id;
    uhsd_void *extra_data; // 暂时未用
} findTMS_config_t;
// typedef uhsd_void (
//     *findTMS_config_process_cb)(uhsd_s32 status, uhsd_s32 sn, uhsd_char *device_tmpID, uhsd_char *device_id, uhsd_void *extra_data, uhsd_void *user_data);
typedef uhsd_void (*findTMS_config_process_cb)(findTMS_config_t *single_config, uhsd_void *user_data);

// 批量配置绑定结果
typedef uhsd_void (*findTMS_batch_config_cb)(uhsd_s32 ret, uhsd_u32 dev_num, findTMS_config_t **batch_config, uhsd_void *user_data);
/**
 * @brief:  启动批量配置。
 * @param: findTMSHandle uhsd_findTMS_crt返回的句柄
 * @param: total_timeout 整个批量配置绑定的超时时间。单位：秒。
 * @param: single_timeout 每个设备的配置绑定超时时间。单位：秒。
 * @param: user_token 用户token。
 * @param: ssid 如果OSDK接管wifi,可填NULL
 * @param: pwd 如果OSDK接管wifi,可填NULL
 * @param: bssid 如果OSDK接管wifi,可填NULL
 * @param: sn 此次配置序列号。
 * @param: traceID 链式ID。
 * @param: device_tmpID_list 设备列表。
 * @param: device_num 设备数量
 * @param: user_data 用户数据指针。
 * @param: config_process_cb 配置绑定状态回调
 * @param: batch_config_cb 批量配置绑定结果
 * @ret:   0:成功。小于0:失败。
 */
UHSD_API uhsd_s32 uhsd_findTMS_batch_config_start(uhsd_void *findTMSHandle,
                                                  uhsd_u32 total_timeout,
                                                  uhsd_u32 single_timeout,
                                                  uhsd_char *user_token, // 新增
                                                  uhsd_char *ssid,       // 新增
                                                  uhsd_char *pwd,        // 新增
                                                  uhsd_char *bssid,      // 新增
                                                  uhsd_s32 sn,
                                                  uhsd_char *traceID,
                                                  uhsd_char *device_tmpID_list[33],
                                                  uhsd_u32 device_num,
                                                  uhsd_void *user_data,
                                                  findTMS_config_process_cb config_process_cb,
                                                  findTMS_batch_config_cb batch_config_cb);
/**
 * @brief:  停止批量配置。
 * @param: findTMSHandle uhsd_findTMS_crt返回的句柄
 * @ret:   0:成功。小于0:失败。
 */
UHSD_API uhsd_s32 uhsd_findTMS_batch_config_stop(uhsd_void *findTMSHandle);
/***********************启动批量配置接口 end***********************/

#endif
