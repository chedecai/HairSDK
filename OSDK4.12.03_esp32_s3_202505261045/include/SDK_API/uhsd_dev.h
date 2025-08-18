/**
 *
 * @defgroup grp_uhsddev 设备接入以及控制接口
 * @{
 * @author jiashouqiang (jiashouqiang@haier.com)
 * @author hudx (hudingxuan.xian@haier.com)
 * @author zhoudong (zhoudong@haier.com)
 * @brief
 * 设备接入接口集合。使用此组接口可完成用户设备注册。
 * @version 0.1
 * @date 2022-08-06
 *
 * @copyright Copyright (c) 2021, Haier.Co, Ltd.
 * @file uhsd_dev.h
 *
 */

#ifndef __UHSD_DEV_H__
#define __UHSD_DEV_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "uhsd_types.h"

typedef enum
{
    /**
     * @brief 设备进入配置状态。
     * @details
     * 当设备通过：
     * 1. 接口触发进开启配置；
     * 2. 手机端触发进配置；
     * 3. E++设备触发进配置；
     * SmartDevice会通过 @ref uhsd_status_notify_pfunc 通知设备APP。
     *
     */
    UHSD_DEV_STATUS_ENTER_CFG = 0,

    /**
     * @brief 设备退出配置状态。
     * @details
     * 当设备通过：
     * 1. 接口触发退出配置；
     * 2. E++设备触发退出配置；
     * SmartDevice会通过 @ref uhsd_status_notify_pfunc 通知设备APP。
     *
     */
    UHSD_DEV_STATUS_EXIT_CFG = 1,

    /**
     * @brief 设备接收到配网信息。
     * @details
     * 当设备开始配置，SmartDevice接收到配置数据信息后，
     * 会通过 @ref uhsd_status_notify_pfunc 通知设备APP。
     * 配置信息主要指以下几种：
     * 1. 路由器信息，SSID、密码等；
     * 2. 海尔自定义配置业务数据；
     */
    UHSD_DEV_STATUS_CFG_DATA_RCVD = 2,

    /**
     * @brief 设备配置成功消息。
     * @details 收到此状态变更，说明SmartDevice已保存配置信息。
     *
     * @attention
     * 1. 如果设备APP主动停止配置，则不会有任何配置结果事件通知。
     * 2. 设备未收到此事件前，可以收到
     * @ref UHSD_DEV_STATUS_ROUTER_CONNECTING
     * @ref UHSD_DEV_STATUS_ROUTER_CONNECTED
     */
    UHSD_DEV_STATUS_CFG_OK = 3,

    /**
     * @brief 配置超时
     * @details
     * 当设备
     * 配置超时后，SmartDevice会通过 @ref uhsd_status_notify_pfunc 通知设备APP。
     *
     * @attention
     * 1. 如果设备APP主动停止配置，则不会有任何配置结果事件通知。
     * 2. 设备未收到此事件前，可以收到
     * @ref UHSD_DEV_STATUS_ROUTER_CONNECTING
     * @ref UHSD_DEV_STATUS_ROUTER_CONNECTED
     */
    UHSD_DEV_STATUS_CFG_TIMEOUT = 4,

    /**
     * @brief 路由器连接中
     * @attention
     * 如设备APP已收到了 @ref UHSD_DEV_STATUS_ENTER_CFG 事件，则说明
     * SmartDevice已经进入到了配置模式，此时也可以收到连接路由的事件。
     *
     */
    UHSD_DEV_STATUS_ROUTER_CONNECTING = 5,

    /**
     * @brief 路由器连接失败
     * @attention
     * 如设备APP已收到了 @ref UHSD_DEV_STATUS_ENTER_CFG 事件，则说明
     * SmartDevice已经进入到了配置模式，此时收到 @ref UHSD_DEV_STATUS_ROUTER_CONNECT_FAILURE
     * 事件后，SmartDevice会尝试重新连接，设备APP会收到 @ref UHSD_DEV_STATUS_ROUTER_CONNECTING
     * 整个过程，SmartDevice未退出配置。只有收到 @ref UHSD_DEV_STATUS_CFG_OK，或
     * @ref UHSD_DEV_STATUS_CFG_TIMEOUT SmartDevice才最终退出配置。
     *
     */
    UHSD_DEV_STATUS_ROUTER_CONNECT_FAILURE = 6,

    /**
     * @brief 路由器连接成功
     *
     */
    UHSD_DEV_STATUS_ROUTER_CONNECTED = 7,

    /**
     * @brief 路由器断开
     *
     */
    UHSD_DEV_STATUS_ROUTER_DISCONNECTED = 8,

    /**
     * @brief 海尔云连接中
     * 设备通过路由器连接到网络。
     */
    UHSD_DEV_STATUS_CLOUD_CONNECTING = 9,

    /**
     * @brief 海尔云连接成功
     *
     */
    UHSD_DEV_STATUS_CLOUD_CONNECTED = 10,

    /**
     * @brief 海尔云异常断开
     *
     */
    UHSD_DEV_STATUS_CLOUD_DISCONNECTED = 11,

    /**
     * @brief 设备绑定成功
     *
     */
    UHSD_DEV_STATUS_BIND_OK = 12,

    /**
     * @brief 设备解绑
     * 如设备通过手机解绑，则触发此事件。
     * @attention
     * SmartDevice 会自动清除配置。
     * 如需要网器入云，则必须重新进行配置。
     *
     */
    UHSD_DEV_STATUS_UNBOUND = 13,
    /**
     * @brief 进入场景配对
     *
     */
    UHSD_DEV_STATUS_SCENE_ENTER_PAIR = 14,
    /**
     * @brief 退出场景配对
     *
     */

    UHSD_DEV_STATUS_SCENE_EXIT_PAIR = 15,
    /**
     * @brief 启用场景
     *
     */
    UHSD_DEV_STATUS_SCENE_ENABLE = 16,
    /**
     * @brief 关闭场景
     *
     */
    UHSD_DEV_STATUS_SCENE_DISABLE = 17,
    /**
     * @brief 场景配对成功
     *
     */
    UHSD_DEV_STATUS_SCENE_PAIR_OK = 18,
    /**
     * @brief 场景配对超时
     *
     */
    UHSD_DEV_STATUS_SCENE_PAIR_TIMEOUT = 19,
    /**
     * @brief 场景解配对
     *
     */
    UHSD_DEV_STATUS_SCENE_UNPAIRED = 20,

    /**
     * @brief 新增本地连接
     *
     */
    UHSD_DEV_STATUS_LOCAL_CONNECT = 21,
    /**
     * @brief 本地连接断开
     *
     */
    UHSD_DEV_STATUS_LOCAL_DISCONNECT = 22,
    /**
     * @brief 所有本地连接都已经断开
     *
     */
    UHSD_DEV_STATUS_LOCAL_ALL_DISCONNECTED = 23,

    /**
     * @brief ble建立连接
     *
     */
    UHSD_DEV_STATUS_BLE_CONNECT = 24,
    /**
     * @brief ble建立安全连接
     *
     */
    UHSD_DEV_STATUS_BLE_CONNECT_SECURITY = 25,
    /**
     * @brief ble建立连接
     *
     */
    UHSD_DEV_STATUS_BLE_DISCONNECT = 26,

    /**
     * @brief 系统时间更新完成
     *
     */
    UHSD_DEV_STATUS_SYSTIME_UPDATED = 27,

    /**
     * @brief 进入产测模式
     *
     */
    UHSD_DEV_STATUS_ENTER_MFT = 28,

    /**
     * @brief 退出产测模式
     *
     */
    UHSD_DEV_STATUS_EXIT_MFT = 29,

    /**
     * @brief 设备绑定失败
     *
     */
    UHSD_DEV_STATUS_BIND_FAIL = 30,

} UHSD_DEV_STATUS_E;

/**
 * @brief    随机生成32字节的字符串,可用作uuid, traceid来使用
 * @param[out] uuid 数组内存起始地址。size需要大于等于33字节，且全部为0
 * @note
 */
UHSD_API uhsd_void uhsd_str_uuid_string(uhsd_char *uuid);

/**
 * @brief 状态函数指针
 * @details
 * 设备APP实现此接口并通过 @ref uhsd_status_register 接口注册至SmartDevice。
 * 当对应事件触发时，通过此回调接口通知设备APP
 * @param[out] devHandle 设备句柄
 * @param[out] status 设备状态
 * @param[out] params 对应状态参数，预留。（当status为UHSD_DEV_STATUS_BIND_FAIL时，params 对应的是绑定失败错误码）
 * @param[out] userdata 设备APP在调用 @ref uhsd_dev_status_register 接口时注册的自定义数据。
 *
 */
typedef uhsd_s32 (*uhsd_dev_status_notify_pfunc)(uhsd_devHandle devHandle, UHSD_DEV_STATUS_E status, uhsd_void *params, uhsd_void *userdata);

/**
 * @brief 设备状态通知注册。
 * @details
 * 设备APP调用此接口，注册设备状态回调，已获取设备状态变更通知。
 *
 * @param[in] sdk_status_cb @ref uhsd_dev_status_notify_pfunc 用户实现函数。
 * @param[in] user_data 用户自定义数据。会通过 @ref uhsd_dev_status_notify_pfunc 接口返回。
 * @return uhsd_s32 注册结果。
 * @retval 0 成功。
 * @retval others 失败。
 */
UHSD_API uhsd_s32 uhsd_dev_status_register(uhsd_dev_status_notify_pfunc dev_status_cb, uhsd_void *user_data);

/**
 * @brief 启动配对通知参数结构体定义
 */
typedef struct
{
    uhsd_char *uplusId; /* 设备类型 */
    uhsd_char *upCodeT; /* 成品编码*/
    uhsd_char *bindSn;  /* 绑定流程sn */
    uhsd_char *extend;  /* 用户自定义数据 */
} uhsd_dev_start_pairing_params_t;

/**
 * @brief 启动配对通知回调定义
 */
typedef uhsd_s32 (*uhsd_dev_start_pairing_notify_cb)(uhsd_devHandle devHandle,
                                                     uhsd_char *traceId,
                                                     uhsd_u32 req_sn,
                                                     uhsd_dev_start_pairing_params_t *params,
                                                     uhsd_void *user_data);

/**
 * @brief 设置启动设备配对通知回调，开发者只有调用该接口后，才能够收到云端下发的[启动配对]通知消息
 * @param cb            通知回调
 * @param user_data     回传参数
 * @return 0: 成功； < 0 : 失败
 */
UHSD_API uhsd_s32 uhsd_dev_set_start_pairing_notify_cb(uhsd_dev_start_pairing_notify_cb cb, uhsd_void *user_data);

/**
 * @brief 启动配对响应上报，开发者收到启动配对通知后，必须要上报响应消息到云端，否则云端会判定本次配对流程异常。
 * @param devHandle     设备句柄
 * @param req_sn        请求sn，来源于请求通知的参数req_sn
 * @param err_num       错误码,详见<uhsd_error.h>
 * @return uhsd_s32
 */
UHSD_API uhsd_s32 uhsd_dev_start_pairing_resp(uhsd_devHandle devHandle, uhsd_u32 req_sn, uhsd_s32 err_num);

/**
 * @brief 启动解除配对回调定义
 */
typedef uhsd_s32 (*uhsd_dev_start_depairing_notify_cb)(uhsd_devHandle devHandle, uhsd_char *traceId, uhsd_u32 req_sn, uhsd_void *user_data);

/**
 * @brief 设置启动设备解除配对回调，开发者只有调用该接口后，才能够收到云端下发的[启动解除配对]通知消息
 * @param cb
 * @param user_data
 * @return uhsd_s32
 */
UHSD_API uhsd_s32 uhsd_dev_set_start_depairing_notify_cb(uhsd_dev_start_depairing_notify_cb cb, uhsd_void *user_data);

/**
 * @brief 启动解除配对响应上报，开发者收到启动配对通知后，须要上报响应消息到云端.
 * @param devHandle     设备句柄
 * @param req_sn        请求sn，来源于请求通知的参数req_sn
 * @param err_num       错误码
 * @return uhsd_s32
 */
UHSD_API uhsd_s32 uhsd_dev_start_depairing_resp(uhsd_devHandle devHandle, uhsd_u32 req_sn, uhsd_s32 err_num);

/**
 * @brief 更新设备信息
 * @details 当设备需要变更设备信息时，设置成功后，需要调用此接口生效修改。
 *
 * @param[in] devHandle 设备句柄
 * @return uhsd_s32
 * @retval 0 成功
 * @attention 此接口目前使用场景为升级底板模块，升级成功后，主程序不需要重新启动，更新设备版本即可。
 */
UHSD_API uhsd_s32 uhsd_dev_device_update(uhsd_devHandle devHandle);

/**
 * @brief 由于注册deviceId信息会存储到本地，所以需要提供删除历史文件接口，目前暂定删除全部。
 *
 * @return uhsd_s32
 */
UHSD_API uhsd_s32 uhsd_dev_delete_deviceId_register_histroy_file(uhsd_void);

/**
 * @brief 使用（EPP透传相关接口），则该接口无效
 * @brief 读设备单个属性回调通知，SDK收到读属性指令后触发该回调，开发者实现。
 * @details
 * eg.
 * 1. 查询指定参数:     op_name="014C01"    pairs[0].name="07eb" pairs[1].name="17c5"
 * 2. 全量属性查询：    op_name="014D01"    pairs=NULL
 * 3. 大数据属性查询:   op_name="014DFE"    pairs=NULL
 * 4. 全量告警查询：    op_name="73"        pairs=NULL
 * 5. 单属性控制:      op_name="015D"    pairs[0].name="01" pairs[0].value="xxxx"
 * 6. 设置指定参数：    op_name="015C01"    pairs[0].name="07eb" pairs[0].value="xxxx" pairs[1].name="17c5" pairs[1].value="xxxx"
 * 7. 组命令主从:      op_name="01"    pairs[0].name="6001" pairs[0].value="xxxx"
 * 7. 组命令交互:       op_name="60"    pairs[0].name="0001" pairs[0].value="xxxx"
 * 8. 告警解除:         op_name="09"        pairs=NULL
 *
 *
 * @param[in] devHandle     设备句柄
 * @param[in] req_sn        请求sn，开发者调用read_resp时需要传回
 * @param[in] property_name 属性名称
 * @param[in] traceId       链式跟踪标识
 * @attention 标准E++设备二进制通用接口，支持Wifibase、uwt通信。
 *
 */
typedef uhsd_s32 (*uhsd_dev_op_binary_cb)(uhsd_devHandle devHandle,
                                          uhsd_s32 req_sn,
                                          const uhsd_char *op_name,
                                          uhsd_dev_pair_t pairs[],
                                          uhsd_s32 pairs_num,
                                          const uhsd_char *traceId);

/**
 * @brief 使用（EPP透传相关接口），则该接口无效
 * @brief 设定读设备属性通知回调,开发者只有设定该回调，才能收到设备读属性通知回调。
 *
 * @param[in] cb
 * @return uhsd_s32
 */
UHSD_API uhsd_s32 uhsd_dev_set_device_op_binary_cb(uhsd_dev_op_binary_cb cb);

/**
 * @brief 使用（EPP透传相关接口），则该接口无效
 * @brief 响应设备属性
 * @details
 * 设备APP通过 @ref uhsd_dev_set_device_read_cb 收到后，通过此接口
 * 返回对应的数据。
 * eg.
 * 1. 指定参数查询响应:     op_name="026C01"    pairs[0].name="07eb" paris[0].value="xxxx"
 * 2. 全量属性查询响应：    op_name="026D01"     pairs=[0].name="" pairs[0].value="016400640000000101000000"
 * [开机-100%亮度-100%电池等共12字节，使用字符串表示为24字符]
 * 3. 大数据属性查询响应:   op_name="027D01"    pairs=[0].name="" pairs[0].value="xxxxxxxx"
 * 4. 全量告警查询响应：    op_name="740F5A"    pairs=[0].name="" pairs[0].value="0001"
 * 5. 单属性控制响应:      op_name="026D01"     pairs[0].name="" pairs[0].value="016400640000000101000000"
 * [开机-100%亮度-100%电池等共12字节，使用字符串表示为24字符]
 * 6. 设置指定参数响应：     op_name="026C01"    pairs[0].name="07eb" pairs[0].value="xxxx"
 * 7. 组命令响应:          op_name="026D01"     pairs[0].name="" pairs[0].value="016400640000000101000000"
 * [开机-100%亮度-100%电池等共12字节，使用字符串表示为24字符]
 * 7. 组命令交互:          op_name="05"         pairs=NULL
 *
 *
 * @param[in] devHandle 设备句柄
 * @param[in] req_sn 请求时的sn
 * @param[in] op_name 操作名称
 * @param[in] value   操作数据
 * @param[in] traceId 链式跟踪标识
 * @return uhsd_s32
 * @retval 0 成功；
 * @retval <0 失败；
 * @attention 标准E++设备二进制通用接口，支持Wifibase、uwt通信。
 */
UHSD_API uhsd_s32 uhsd_dev_device_op_binary_resp(uhsd_devHandle devHandle,
                                                 uhsd_s32 req_sn,
                                                 const uhsd_char *op_name,
                                                 uhsd_dev_pair_t pairs[],
                                                 uhsd_s32 pairs_num,
                                                 const uhsd_s32 result,
                                                 const uhsd_char *traceId);

/**
 *  @brief 使用（EPP透传相关接口），则该接口无效
 *  @brief       设备状态上报
 *  @details
 * <pre>
 *  @verbatim
 *  1. 全量属性上报：       op_name="066D01"    pairs[0].name=NULL pairs[0].value="016400640000000101000000"
 * [开机-100%亮度-100%电池等共12字节，使用字符串表示为24字符]
 *  2. 参数状态上报：       op_name="066C01"    pairs[0].name="07eb" pairs[0].value="xxxx" ; pairs[1].name="07ex" pairs[1].value="xxxx"
 *  3. 事件上报：           op_name="066E01"    pairs[0].name="07eb" pairs[0].value="xxxx" 只有一个。
 *  4. 大数据属性上报:      op_name="067D01"    pairs[0].name=NULL, pairs[0].value="xxxxxxxx"
 *  5. 全量告警上报：       op_name="040F5A"    pairs[0].name=NULL, pairs[0].value="0001"
 *  6. 大数据属性上报补充:  op_name="067D02"    pairs[0].name=NULL, pairs[0].value="xxxxxxxx"
 *  @endverbatim
 * </pre>
 *  @param[in] devHandle 设备唯一标识符
 *  @param[in] pairs 属性键值对数组
 *  @param[in] len   属性键值对数组长度
 *  @return      成功返回0, 失败返回
 *  @attention 标准E++设备二进制通用接口，支持Wifibase、uwt通信。
 */
UHSD_API uhsd_s32 uhsd_dev_op_binary_report(uhsd_devHandle devHandle, const uhsd_char *op_name, uhsd_dev_pair_t pairs[], uhsd_s32 pairs_num);

/**
 * @brief （EPP透传相关接口）
 * @brief epp 二进制数据透传应答
 * @param[in] devHandle     设备句柄
 * @param[in] req_sn        请求时的sn
 * @param[in] epp_data      epp数据
 * @param[in] epp_data_len  epp数据长度
 * @param[in] result        应答结果错误码
 * @param[in] traceId       链式跟踪标识
 * @return uhsd_s32
 * @retval 0 成功；
 * @retval <0 失败；
 * @attention e++二进制，支持Wifibase、uwt通信。
 */
UHSD_API uhsd_s32 uhsd_dev_device_epp_binary_transparent_resp(uhsd_devHandle devHandle,
                                                              uhsd_u32 req_sn,
                                                              const uhsd_char *epp_data,
                                                              const uhsd_u32 epp_data_len,
                                                              const uhsd_s32 result,
                                                              const uhsd_char *traceId);

/**
 * @brief （EPP透传相关接口）
 *  @brief epp 二进制数据透传上报(状态)
 *  @param[in] devHandle 设备唯一标识符
 *  @param[in] dst_devHdl    目标设备句柄，0:上报给所有联动设备、云、小循环所有连接（本地、ble）,
 *                                       非0:上报给指定联动设备
 *  @param[in] op_name       = "066D"、"066E"、"067D"、"04"、"06"
 *  @param[in] epp_data  epp数据
 *  @param[in] epp_data_len   epp数据长度
 *  @return      成功返回0, 失败返回
 *  @attention 标准E++设备二进制通用接口，支持Wifibase、uwt通信。
 */
UHSD_API uhsd_s32 uhsd_dev_device_epp_binary_transparent_rpt(uhsd_devHandle devHandle,
                                                             uhsd_devHandle dst_devHdl,
                                                             const uhsd_char *op_name,
                                                             const uhsd_char *epp_data,
                                                             const uhsd_u32 epp_data_len);

/**
 * @brief （EPP透传相关接口）
 * @brief 底板设备E++数据回调通知，SDK收到读属性指令后触发该回调，开发者实现。
 * @param[in] devHandle     设备句柄
 * @param[in] src_devHdl    源设备句柄，0:数据来源为云、小循环（本地、ble）,
 *                                    非0:数据来源设备为联动设备
 * @param[in] req_sn        请求sn，开发者调用read_resp时需要传回
 * @param[in] epp_data      epp原始数据
 * @param[in] epp_data_len  epp原始数据长度
 * @param[in] traceId       链式跟踪标识
 * @attention 标准E++设备二进制通用接口，支持Wifibase、uwt通信。
 *
 */
typedef uhsd_s32 (*uhsd_dev_epp_transparent_req_cb)(uhsd_devHandle devHandle,
                                                    uhsd_s32 src_devHdl,
                                                    uhsd_u32 req_sn,
                                                    uhsd_char *epp_data,
                                                    uhsd_u32 epp_data_len,
                                                    const uhsd_char *traceId);

/**
 * @brief （EPP透传相关接口）
 * @brief 设置epp透传数据回调, 可以回调透传所有的epp数据
 *
 * @param[in] cb @ref uhsd_dev_epp_transparent_all_cb
 * @return uhsd_s32
 * @retval UHSD_E_COMMON_OK 成功。
 */
UHSD_API uhsd_s32 uhsd_dev_set_device_epp_transparent_req_cb(uhsd_dev_epp_transparent_req_cb cb);

/**
 * @brief 读设备单个属性回调通知，SDK收到读属性指令后触发该回调，开发者实现。
 * @details
 *
 * @param devHandle     设备句柄
 * @param req_sn        请求sn，开发者调用read_resp时需要传回
 * @param property_name 属性名称
 * @param traceId       链式跟踪标识
 * @attention 标准设备模型接口
 *
 */
typedef uhsd_s32 (*uhsd_dev_read_cb)(uhsd_devHandle devHandle, uhsd_s32 req_sn, const uhsd_char *property_name, const uhsd_char *traceId);

/**
 * @brief 设定读设备属性通知回调,开发者只有设定该回调，才能收到设备读属性通知回调。
 *
 * @param cb
 * @return uhsd_s32
 * @attention 标准设备模型接口
 */
UHSD_API uhsd_s32 uhsd_dev_set_device_read_cb(uhsd_dev_read_cb cb);

/**
 * @brief 响应设备属性
 * @details
 * 设备APP通过 @ref uhsd_dev_set_device_read_cb 收到后，通过此接口
 * 返回对应的数据。
 *
 * @param[in] devHandle 设备句柄
 * @param[in] req_sn 请求时的sn
 * @param[in] property_value 属性值
 * @param[in] result
 * @param[in] traceId 链式跟踪标识
 * @return uhsd_s32
 * @retval 0 成功；
 * @retval <0 失败；
 * @attention 标准设备模型接口
 */
UHSD_API uhsd_s32 uhsd_dev_device_read_resp(uhsd_devHandle devHandle,
                                            uhsd_s32 req_sn,
                                            const uhsd_char *property_value,
                                            uhsd_s32 result,
                                            uhsd_s32 invalid_code,
                                            const uhsd_char *traceId);

/**
 * @brief 单属性设置。
 * @details
 * 设备APP通过此接口进行设备单属性设置。
 * SDR 不支持单属性查询
 *
 * @param[out] devHandle     设备句柄
 * @param[out] req_sn        请求sn，开发者调用read_resp时需要传回
 * @param[out] property_name 属性名称
 * @param[out] property_value 属性值
 * @param[out] traceId       链式跟踪标识
 *
 * @attention 标准设备模型接口
 *
 */
typedef uhsd_s32 (
    *uhsd_dev_write_cb)(uhsd_devHandle devHandle, uhsd_s32 req_sn, const uhsd_char *property_name, const uhsd_char *property_value, const uhsd_char *traceId);

/**
 * @brief 设定写设备属性通知回调,开发者只有设定该回调，才能收到写设备属性通知回调。
 *
 * @param cb
 * @return uhsd_s32
 * @attention 标准设备模型接口
 */
UHSD_API uhsd_s32 uhsd_dev_set_write_cb(uhsd_dev_write_cb cb);

/**
 * @brief 响应设备属性设置
 * @details
 * 设备APP通过 @ref uhsd_dev_write_cb 收到后，通过此接口
 * 返回对应的数据。
 *
 * @param[in] devHandle 设备句柄
 * @param[in] req_sn 请求时的sn
 * @param[in] result 应答码, 根据指令执行情况传入相应的已定义的应答码,参见<uhsd_error.h>
 * @param[in] invalid_code 无效命令编码，海极网创建设备时生成；
 *                      在result为UGW_RSP_INVALID_CMD时传入响应的无效命令编码，否则传入0即可；
 * @param[in] traceId 链式跟踪标识
 * @return uhsd_s32
 * @retval 0 成功；
 * @retval <0 失败；
 * @attention 标准设备模型接口
 */
UHSD_API uhsd_s32 uhsd_dev_write_resp(uhsd_devHandle devHandle, uhsd_s32 req_sn, uhsd_s32 result, uhsd_s32 invalid_code, const uhsd_char *traceId);

/**
 * @brief 属性设置。
 * @details
 * <pre>
 * @verbatim
 * 设备APP通过此接口进行设备属性设置。
 * 操作类型包括：
 * 1. 全属性查询
 * 2. 单属性控制
 * 3. 告警查询
 * @endverbatim
 * </pre>
 *
 * @param[out] devHandle     设备句柄
 * @param[out] req_sn        请求sn，开发者调用read_resp时需要传回
 * @param[out] op_name       操作类型
 * @param[out] pairs         属性集合
 * @param[out] traceId       链式跟踪标识
 *
 * @attention 标准设备模型接口
 *
 */
typedef uhsd_s32 (*uhsd_dev_op_cb)(uhsd_devHandle devHandle,
                                   uhsd_s32 req_sn,
                                   const uhsd_char *op_name,
                                   uhsd_dev_pair_t pairs[],
                                   uhsd_s32 pairs_num,
                                   const uhsd_char *traceId);
/**
 * @brief 设定设备操作通知回调,开发者只有设定该回调，才能收到操作设备通知回调。
 *
 * @param cb
 * @return uhsd_s32
 * @attention 标准设备模型接口
 */
UHSD_API uhsd_s32 uhsd_dev_set_op_cb(uhsd_dev_op_cb cb);

/**
 * @brief 响应设备属性设置
 * @details
 * 设备APP通过 @ref uhsd_dev_set_op_cb 收到数据后，
 * 通过此接口返回对应的数据。
 *
 *
 * @param[in] devHandle 设备句柄
 * @param[in] req_sn 请求时的sn
 * @param[in] op_name       操作类型
 * @param[in] pairs         属性集合
 * @param[in] pairs_num
 * @param[in] result 应答码, 根据指令执行情况传入相应的已定义的应答码
 * @param[in] invalid_code 无效命令编码，海极网创建设备时生成；
 *                      在result为UGW_RSP_INVALID_CMD时传入响应的无效命令编码，否则传入0即可；
 * @param[in] traceId 链式跟踪标识
 * @return uhsd_s32
 * @retval 0 成功；
 * @retval <0 失败；
 * @attention 标准设备模型接口
 */
UHSD_API uhsd_s32 uhsd_dev_op_resp(uhsd_devHandle devHandle,
                                   uhsd_s32 req_sn,
                                   const uhsd_char *op_name,
                                   uhsd_dev_pair_t pairs[],
                                   uhsd_s32 pairs_num,
                                   uhsd_s32 result,
                                   uhsd_s32 invalid_code,
                                   const uhsd_char *traceId);

/**
 * @brief 批控通知回调 【目前仅支持DCC(分布式控制)模块】
 *
 * @param req_sn 请求唯一标识
 * @param cmd 批控命令
 * @return 成功返回0,失败返回<0
 */
typedef uhsd_s32 (*uhsd_dev_batch_ctrl_cb)(uhsd_u32 req_sn, const uhsd_char *cmd);

/**
 * @brief 设置批控通知回调 【目前仅支持DCC(分布式控制)模块】
 *
 * @param cb 批控通知回调
 * @return 成功返回0,失败返回<0
 */
UHSD_API uhsd_s32 uhsd_dev_set_batch_ctrl_cb(uhsd_dev_batch_ctrl_cb cb);

/**
 * @brief 批控应答 【目前仅支持DCC(分布式控制)模块】
 *
 * @param sn 请求唯一标识
 * @param errCode 错误码，成功为0,失败为非0
 * @return 成功返回0,失败返回<0
 */
UHSD_API uhsd_s32 uhsd_dev_batch_ctrl_resp(uhsd_u32 sn, uhsd_s32 errCode);

/**
 *  @brief       设备状态上报
 *  @details
 *  @param devHandle 设备唯一标识符
 *  @param pairs 属性键值对数组
 *  @param len   属性键值对数组长度
 *  @return      成功返回0, 失败返回
 *  @attention 标准设备模型接口
 */
UHSD_API uhsd_s32 uhsd_dev_status_report(uhsd_devHandle devHandle, uhsd_dev_pair_t pairs[], uhsd_s32 len);

/**
 *  @brief       设备警告上报
 *  @param devid 设备唯一标识符
 *  @param pairs 警告键值对数组
 *  @param len   警告键值对数组长度
 *  @return      成功返回0, 失败返回
 *  @attention 标准设备模型接口
 */
UHSD_API uhsd_s32 uhsd_dev_alarm_report(uhsd_devHandle devHandle, uhsd_dev_pair_t pairs[], uhsd_s32 len);

/**
 * @brief 申请1块事件参数结构体内存
 * @param name  事件名称-标准定义
 * @param type  事件类型，目前仅支持message
 * @param attr_pair 事件参数键值队数组
 * @param attr_pair_num 事件参数键值对数组长度
 * @return 成功: 事件参数结构体指针；失败: NULL
 *
 * @note 上报事件完成后，需要使用uhsd_dev_event_context_free 释放该事件参数内存
 *
 */
UHSD_API uhsd_dev_event_context_t *uhsd_dev_event_context_new(const uhsd_char *name,
                                                              const uhsd_char *type,
                                                              uhsd_dev_pair_t *attr_pair[],
                                                              uhsd_u16 attr_pair_num);

/**
 * @brief 释放事件参数内存
 * @param ctx 将要释放的事件参数结构体内存地址
 * @return uhsd_void
 */
UHSD_API uhsd_void uhsd_dev_event_context_free(uhsd_dev_event_context_t *ctx);

/**
 * @brief 设备事件信息上报
 * @param devHandle     设备句柄
 * @param ts            事件发生时间戳，单位：毫秒
 * @param uniqueId      事件唯一序列号
 * @param data_type     事件数据类型：0-实时数据；1-历史数据
 * @param events        事件键值对数组
 * @param event_num     事件键值对数组长度
 * @return 成功:0; 失败:<0
 */
UHSD_API uhsd_s32
uhsd_dev_event_report(uhsd_devHandle devHandle, uhsd_u64 ts, uhsd_s64 uniqueId, uhsd_s32 data_type, uhsd_dev_event_context_t *events[], uhsd_s32 event_num);

/**
 * @brief 获取当前连云状态
 * @return uhsd_s32
 *  251：当前已连云
 *  252：当前连云已断开
 *
 */
UHSD_API uhsd_s32 uhsd_dev_get_cloud_conn_status(uhsd_void);

/**
 * @brief 设置软件短ID
 *
 * @param devHandle 设备句柄
 * @param swShortID 软件短ID
 * @return 成功返回0，失败返回<0
 */
UHSD_API uhsd_s32 uhsd_dev_set_devHandle_swShortID(uhsd_devHandle devHandle, const uhsd_char *swShortID);

/**
 * @brief 设置模组KEY
 *
 * @param devHandle 设备句柄
 * @param moduleKey 设备KEY
 * @return 成功返回0，失败返回<0
 */
UHSD_API uhsd_s32 uhsd_dev_set_devHandle_moduleKey(uhsd_devHandle devHandle, const uhsd_char *moduleKey);

/**
 * @brief 设置模组是否支持设备模型2.1能力
 *
 * @param devHandle 设备句柄
 * @param tlvAbility 是否支持设备模型2.1能力
 * @return 成功返回0，失败返回<0
 */

UHSD_API uhsd_s32 uhsd_dev_set_epptlv_ability(uhsd_devHandle devHandle, uhsd_u8 *tlvAbility);

/**
 * @brief 上报绑定进度
 * @param devHandle     设备句柄
 * @param err_num       错误码
 * @param process       绑定进度; 1-配对完成 2-绑定完成.
 * @param bindSn        绑定流程sn
 * @return uhsd_s32     0: 成功； < 0 : 失败
 * @note 绑定进度 process = 0-进配对完成。可以不上报进度0，原因有2: 1. 不会影响手机APP的显示。
 *       2.上报0的时候，云端颁发devid，有可能还没获取到devid，上报会失败。
 *
 * @note 若在组网配对过程中，在绑定从设备时sdl返回错误码301，
 *       则开发者在调用此接口上报绑定完成状态时将err_num传入301错误码。
 */
UHSD_API uhsd_s32 uhsd_dev_bind_process_report(uhsd_devHandle devHandle, uhsd_s32 err_num, uhsd_s32 process, uhsd_char *bindSn);

/**
 * @brief 批量删除设备主从关系结果回调
 */
typedef uhsd_s32 (*uhsd_dev_batch_del_dev_relation_result_cb)(uhsd_devHandle devHandle, uhsd_s32 err_num, uhsd_void *cb_param);

/**
 * @brief 批量删除主从关系
 * @param devHandle
 * @param traceId
 * @param cb
 * @param cb_param
 * @return uhsd_s32
 *
 * @note 删除主设备与其下的一级从设备的主从关系(不递归删除)，若删除的一级从设备有附件设备，则同时解绑此一级附件设备
 *       因为不递归删除，故如果需要删除多级关系，则需要多次调用接口，且必须先删除底层关系；
 *       如：网关设备->子设备->附件设备这种关系，则先调用接口删除子设备与附件设备的关系，后调用接口删除网关设备与子设备的关系
 */
UHSD_API uhsd_s32 uhsd_dev_batch_del_dev_relation(uhsd_devHandle devHandle,
                                                uhsd_char *traceId,
                                                uhsd_dev_batch_del_dev_relation_result_cb cb,
                                                uhsd_void *cb_param);

/**
 * @brief 设备恢复出厂设置通知回调函数
 * @param devHandle 设备唯一标识符
 * @param result  设备恢复出厂设置结果；0: 恢复出厂设置成功；非0: 恢复出厂设置失败；
 * @return 无
 */
typedef uhsd_void (*uhsd_dev_reset_notify_cb)(uhsd_devHandle devHandle, uhsd_s32 result);

/**
 * @brief 设备会出出厂设置功能API
 * @param devHandle 设备唯一标识符
 * @param cb 设备恢复出厂设置执行结果通知回调
 * @return 成功返回0,失败返回<0, 具体如下：
 *               0 -- 请求成功，（请求成功只代表该接口调用成功，不代表执行恢复出厂设置结果成功，恢复出厂设置结果通过回调进行通知）；
 *              -1 -- 参数非法；（格式错误，未曾添加上线）
 *              -2 -- 该devID请求正忙
 *              -3 -- 当前连云状态断开
 */
UHSD_API uhsd_s32 uhsd_dev_reset(uhsd_devHandle devHandle, uhsd_dev_reset_notify_cb cb);

/**
 * @brief 查询主从设备结果回调定义
 */
/**
 * @brief 主动查询主从设备信息
 * @param master_devHandle 主设备句柄
 * @param err_num 错误码
 * @param slave_json  云端返回的子设备的json字符串
 * @param slave_json_len  子设备的json字符串长度
 * @param cb_param      回传参数
 * @note json字符串的格式为
 * []
 * [{"role":4,"dev":"B6CF5E9436"},{"role":4,"dev":"1B6FB74EA4"}];
 * 当从设备下还有附件设别，字符串格式为
 * [{"role":3,"dev":"3C997EFA46"},{"slaves":[{"role":3,"dev":"F33343CDD1"},{"role":3,"dev":"A69932B367"},{"role":3,"dev":"6BDE4B7B4B"}],"role":4,"dev":"91DA59C9CB"},{"role":4,"dev":"4E4BC87A64"},{"role":3,"dev":"18055AED4F"},{"role":3,"dev":"3786C5EBF1"},{"role":4,"dev":"3094B91280"}]
 * @return uhsd_s32
 */
typedef uhsd_s32 (*uhsd_dev_get_relation_slave_devs_result_cb)(uhsd_devHandle master_devHandle,
                                                               uhsd_s32 err_num,
                                                               uhsd_char *slave_json,
                                                               uhsd_s32 slave_json_len,
                                                               uhsd_void *cb_param);

/**
 * @brief 主动查询主从设备信息
 * @param master_devHandle 主设备句柄
 * @param traceId
 * @param cb            结果回调
 * @param cb_param      回传参数
 * @return uhsd_s32
 */
UHSD_API uhsd_s32 uhsd_dev_get_relation_slave_devs(uhsd_devHandle master_devHandle,
                                                   uhsd_char *traceId,
                                                   uhsd_dev_get_relation_slave_devs_result_cb cb,
                                                   uhsd_void *cb_param);

/**
 * @brief: 文件上传结果通知回调。
 * @param: [in] errNo 错误码，0:成功； 其他：失败。
 * @param: [in] file_name 上传的文件名称，必须包含后缀
 * @param: [in] file_index 文件唯一索引值，用于主动查询playURL，预留。
 * @param: [in] playUrl 文件存放地址
 * @param: [in] cb_param 回传参数
 * @note:
 */
typedef uhsd_void (*push_file_result_cb)(uhsd_s32 errNo, uhsd_char *file_name, uhsd_char *file_index, uhsd_char *playUrl, uhsd_void *cb_param);
/**
 * @brief: 通用文件上传，开发者调用该接口后，SDK将开发者的指定路径的文件上传至资源服务器，上传结果通过回调通知。
 * @param: [in] appId 应用ID， 海极网申请。
 * @param: [in] file_name 上传的文件名称， 文件名称不需要包含文件路径。
 * @param: [in] file_path 要上传的文件绝对路径；目前文件大小限制为5M。
 * @param: [in] expire_time 文件存储过期时间，单位：天，范围[1, 30。
 * @param: [in] cb 文件上传结果回调
 * @param: [in] cb_param 文件上传结果回传参数
 * @return {*} 0:成功 只代表接口调用成功，不代表文件是否上传成功，文件上传结果通过回调通知。<0: 失败
 * @note:
 */
UHSD_API uhsd_s32
uhsd_push_file_to_oss(uhsd_char *appId, uhsd_char *file_name, uhsd_char *file_path, uhsd_u16 expire_time, push_file_result_cb cb, uhsd_void *cb_param);

typedef struct
{
    uhsd_char *appid;
    uhsd_char *appkey;
    uhsd_char *file_index[5];
    uhsd_u32 file_index_num;

} uhsd_fs_del_file_info_t;
/*
 * @brief: 删除指定文件回调
 * @param: [in] ret 删除结果，0:成功，其他失败
 */
typedef uhsd_void (*uhsd_fs_del_remote_file_cb)(uhsd_s32 ret, uhsd_void *cb_param);

/**
 * @brief: 删除指定文件
 * @param: [in] del_info 删除文件信息,file_index_num 最大数值为5
 * @param: [in] cb 回调函数
 * @param: [in] cb_param 回调函数参数
 * @return {*} 0:成功 只代表接口调用成功，不代表文件是否删除成功，文件删除结果通过回调通知。<0: 失败
 * @note:
 */
UHSD_API uhsd_s32 uhsd_file_push_del_remote_file(uhsd_fs_del_file_info_t *del_info, uhsd_fs_del_remote_file_cb cb, uhsd_void *cb_param);
/**
 * @brief 清空SDK保存的文件、flash数据
 * @return 成功返回0，失败返回<0
 */
UHSD_API uhsd_s32 uhsd_dev_sdk_data_clear();

/**
 * @brief  开启/关闭httpDNS域名解析功能;
 * @param  enable 使能标志，0:关闭，1:开启
 * @return 成功返回0，失败返回<0
 * @brief  1. 开启此功能后，设备连接的IOT云平台固定为生产环境；
 *         2. OSDK默认开启此功能；
 *         3. 此接口需要在添加设备之前调用;
 */
UHSD_API uhsd_s32 uhsd_dev_httpDNS_enable_set(uhsd_s32 enable);
typedef struct uhsd_httpGW_data_report_info
{
    uhsd_char app_id[48];      // 应用ID，40位以内字符,Haier U+ 云平台全局唯一
    uhsd_char app_key[128];    // 生成签名需要, 在海极网给应用申请的appKey，不能明文发送
    uhsd_char app_version[32]; // 应用版本最多32 位字符,应用版本标识
    uhsd_char api_version[32]; // 接口版本 v1
    uhsd_char client_id[32]; // 客户端ID， 主要用途为唯一标识客户端 (例如,手机)。可调用usdk得到客户端ID的值。 取mac地址,保证唯一
    uhsd_char language[16]; // 国际化标识，代表客户端使用的语言
    uhsd_char trace[36]; // 链式跟踪TraceId，用于全局唯一标识该笔业务。格式为定长32字符UUID，可使用字符集为数字与小写字母
    uhsd_s32 time_zone;  // 代表客户端使用的时区, 8
    uhsd_s32 data_fmt;   // 数据格apiVersion式 4bytes json:0  自定义/tlv：1 probuf：2
    uhsd_char *data;
    uhsd_u32 data_len;
} uhsd_httpGW_data_report_info_t;

typedef uhsd_void (*uhsd_httpGW_data_report_result_cb)(uhsd_s32 result, uhsd_void *cb_param);

UHSD_API uhsd_s32 uhsd_sweeper_history_data_report(uhsd_httpGW_data_report_info_t *info, uhsd_httpGW_data_report_result_cb cb, uhsd_void *cb_param);

typedef struct
{
    uhsd_char app_id[48];      // 应用ID，40位以内字符,Haier U+ 云平台全局唯一
    uhsd_char app_key[128];    // 生成签名需要, 在海极网给应用申请的appKey，不能明文发送
    uhsd_char app_version[32]; // 应用版本最多32 位字符,应用版本标识
    uhsd_char api_version[32]; // 接口版本 v1
    uhsd_char client_id[32]; // 客户端ID， 主要用途为唯一标识客户端 (例如,手机)。可调用usdk得到客户端ID的值。 取mac地址,保证唯一
    uhsd_char language[16]; // 国际化标识，代表客户端使用的语言
    uhsd_char trace[36]; // 链式跟踪TraceId，用于全局唯一标识该笔业务。格式为定长32字符UUID，可使用字符集为数字与小写字母
    uhsd_s32 time_zone;  // 代表客户端使用的时区, 8
} uhsd_app_params_t;
// 扫地机实时地图信息更新结果回调
typedef uhsd_void (*sweeper_rt_map_info_update_result_cb)(uhsd_s32 result, uhsd_void *cb_param);
// 扫地机实时地图更新接口
uhsd_s32 uhsd_sweeper_rt_map_info_update(uhsd_s32 devHandle,
                                         uhsd_app_params_t *app_params,
                                         const uhsd_char *map_url,
                                         sweeper_rt_map_info_update_result_cb cb,
                                         uhsd_void *cb_param);
/**
 * @brief   请求授权码通知回调函数
 *
 * @param   result  获取授权码结果；0 -- 请求成功；非0： 失败
 * @param   sn  请求授权code时的sn
 * @param   code 授权code
 */
typedef uhsd_s32 (*uhsd_get_authorization_code_cb)(uhsd_s32 result, uhsd_u32 sn, uhsd_char *code);

UHSD_API uhsd_s32 uhsd_get_authorization_code_req(uhsd_char *devid, uhsd_char *appId, uhsd_get_authorization_code_cb cb);

/**
 * @brief 查询设备绑定状态的回调函数
 *
 * @param devHandle 设备句柄
 * @param sn 请求标识
 * @param err_num 错误码，0为成功，非0为失败
 * @param status 1. 设备绑定状态，只有在err_num为0时有意义, err_num为非0时，此值为垃圾值;
 *               2. 已绑定:UHSD_DEV_STATUS_BIND_OK; 未绑定:UHSD_DEV_STATUS_UNBOUND，详见UHSD_DEV_STATUS_E定义
 *               3. 此参数的值在err_num为0时,是且仅是UHSD_DEV_STATUS_BIND_OK 或 UHSD_DEV_STATUS_UNBOUND;
 * @param cb_param 回传参数
 * @return 成功返回0，失败返回<0
 *
 * @note !!!!!!!! 一定要先判断err_num的值是否为0，为0时才去判断status的值；
 */
typedef uhsd_void (*uhsd_dev_query_bind_status_cb)(uhsd_devHandle devHandle, uhsd_u32 sn, uhsd_s32 err_num, UHSD_DEV_STATUS_E status, uhsd_void *cb_param);

/**
 * @brief 查询设备是否被绑定 【已有新的接口，此接口会逐步废弃，建议不再使用】
 *
 * @param devHandle 设备句柄
 * @param sn 请求标识
 * @param timeout 超时时间，单位为秒，取值范围[5,30], 若传入0则使用OSDK默认的15s,建议使用默认值
 * @param cb 查询结果通知回调函数
 * @param cb_param 回调函数的回传参数
 * @return 成功返回0,失败返回<0
 */
UHSD_API uhsd_s32 uhsd_dev_query_bind_status(uhsd_s32 devHandle, uhsd_u32 sn, uhsd_u32 timeout, uhsd_dev_query_bind_status_cb cb, uhsd_void *cb_param);

/**
 * @brief 获取设备绑定状态

 * @param [IN]  devHandle 设备句柄
 * @param [OUT] curr_status 当前设备绑定状态
 * @param [OUT] flash_stored_status flash中保存的绑定状态
 * @return 成功返回0,失败返回<0
 *
 * @note:
 *      1. 当 curr_status == UHSD_DEV_BIND_STATUS_ERR 代表OSDK当前暂未查询到设备最新的绑定状态，此时返回的flash_stored_status代表 断电前flash中存储的绑定状态
 *      2. 当 curr_status != UHSD_DEV_BIND_STATUS_ERR 时，curr_status == flash_stored_status，均为设备当前的绑定状态
 */
UHSD_API uhsd_s32 uhsd_dev_get_device_bind_status(uhsd_devHandle devHandle, uhsd_dev_bind_status_t *curr_status, uhsd_dev_bind_status_t *flash_stored_status);

/**
 * 用户token信息结构体
 * 用户侧登录时，需要保证clientId和access_token成对使用
 *
 */
typedef struct
{
    uhsd_char *clientId;      // clientId
    uhsd_char *access_token;  // 用户token
    uhsd_char *refresh_token; // 刷新token
    uhsd_u32 expire_ts;       // token有效期(秒数)
} uhsd_user_token_info_t;

/**
 * @brief 用户token通知回调函数
 *
 * @param sn 请求标识
 * @param err_num 错误码
 * @param token_info 设备绑定状态，只有在err_num为0时有意义；
 * @param cb_param 回传参数
 * @return 成功返回0，失败返回<0
 */
typedef uhsd_void (*uhsd_dev_user_token_update_cb)(uhsd_u32 sn, uhsd_s32 err_num, uhsd_user_token_info_t *token_info, uhsd_void *cb_param);

/**
 * @brief 获取用户token
 * 如果回调函数返回错误，需要设备APP重试
 *
 * @param devHandle 设备句柄
 * @param sn 请求标识
 * @param clientId 客户端ID，标识设备的唯一性
 *		  如果此参数不为空，调用者需要保证唯一性
 * @param appId 应用ID
 * @param appKey 应key
 * @param cb 获取用户结果通知回调函数
 * @param cb_param 回调函数的回传参数
 * @return 成功返回0,失败返回<0
 */
UHSD_API uhsd_s32 uhsd_dev_get_user_token(uhsd_devHandle devHandle,
                                          uhsd_u32 sn,
                                          uhsd_char *clientId,
                                          uhsd_char *appId,
                                          uhsd_char *appKey,
                                          uhsd_dev_user_token_update_cb cb,
                                          uhsd_void *cb_param);

/**
 * @brief 刷新用户token
 *
 * @param devHandle 设备句柄
 * @param sn 请求标识
 * @param appId 应用ID
 * @param appKey 应key
 * @param refresh_token 刷新token
 * @param cb 获取用户结果通知回调函数
 * @param cb_param 回调函数的回传参数
 * @return 成功返回0,失败返回<0
 */
UHSD_API uhsd_s32 uhsd_dev_update_user_token(uhsd_devHandle devHandle,
                                             uhsd_u32 sn,
                                             uhsd_char *appId,
                                             uhsd_char *appKey,
                                             uhsd_char *refresh_token,
                                             uhsd_dev_user_token_update_cb cb,
                                             uhsd_void *cb_param);
/**
 * @brief 设备APP(不限于门锁)打点接口
 *
 * @param utraceId 打点trace id，要求: 32位随机字符串, [a-z,0-9]
 * @param uspanId 字符串,建议使用数字[0-9]来进行区分
 * @param code 设备APP自定义的错误码
 * @return 成功返回0,失败返回<0
 */
uhsd_s32 uhsd_dev_app_trace_set_point(uhsd_char *utraceId, uhsd_char *uspanId, uhsd_s32 code);

/**
 * @brief SDL反初始化异步回调结果
 * @param result 0 表示 成功  <0 表示失败
 */
typedef uhsd_void (*uhsd_dev_deInit_cb)(uhsd_s32 result, uhsd_void *cb_param);

/**
 * @brief  反初始化接口(重要提示：执行此接口操作后，必须重启sdk才能重新使用设备)
 * 注意：   本接口做的工作：1. 删除本地存储的设备侧相关文件
 * @param cb    反初始化异步结果
 * @return  0 表示该函数调用成功，<0 表示 调用失败
 */
UHSD_API uhsd_s32 uhsd_dev_deInit(uhsd_dev_deInit_cb cb, uhsd_void *cb_param);

/**
 * @brief 替换附件设备信息结构
 * @param uplusId
 * @param oldDeviceId 旧设备ID
 * @param newDeviceId 新设备ID
 * @param subId
 */
typedef struct __uhsd_dev_replace_annex_s
{
    uhsd_char *uplusId;
    uhsd_char *oldDeviceId;
    uhsd_char *newDeviceId;
    uhsd_char *subId;
} uhsd_dev_replace_annex_s;

/**
 * @brief 设备替换通知回调
 * @param err     错误码 0 没有错误；非0 有错误
 * @param deviceId 替换后设备id
 * @param retcode  "00000" 替换成功；非0 替换出错
 * @param ReplaceCode 传入的ReplaceCode
 * @param cb_param
 */
typedef uhsd_void (*uhsd_dev_replace_notify_cb)(uhsd_s32 err, uhsd_char *retcode, uhsd_char *deviceId, uhsd_char *ReplaceCode, uhsd_void *cb_param);
/**
 * @brief 设备替换接口
 * @param  ReplaceCode 即产业申请设备替换业务时，在IoT云端申请的ReplaceCode，在手机上发起设备替换的时候携带该参数并发给新网关
 * @param  uplusId    填入新网关在海极网生成的typeId
 * @param  deviceKey  填入新网关在海极网生成的typeId的Key
 * @param  moduleType 填入新网关注册时上报的moduleType（wifinetcard或thnetcard），如果是子设备是zigbee设备固定填写zigbeemodule
 * @param  moduleId   填入新网关注册时上报的moduleId（网关mac地址）
 * @param  subId      子机标识，网关和子设备都需要填写
 * @param  replace_annex_arr[] 附件设备列表，可为空
 * @param  annex_num 附件设备列表元素个数
 * @param  cb         注册设备替换通知回调函数
 * @param  cb_param   回调函数参数
 * @return 0：成功 其他：失败
 */
UHSD_API uhsd_s32 uhsd_dev_cloud_dev_replace(uhsd_char *ReplaceCode,
                                             uhsd_char *uplusId,
                                             uhsd_char *deviceKey,
                                             uhsd_module_type_e moduleType,
                                             uhsd_char *moduleId,
                                             uhsd_char *subId,
                                             uhsd_dev_replace_annex_s replace_annex_arr[],
                                             uhsd_s32 annex_num,
                                             uhsd_dev_replace_notify_cb cb,
                                             uhsd_void *cb_param);

/**
 * @brief familyId发生变化回调
 * @param event: 0-从无到有 1-从有到变化； 2-从有到无
 * @param familyid 当前新的familyid，最大长度32
 */
typedef uhsd_s32 (*uhsd_devcloud_familyId_change_event)(uhsd_s32 event, uhsd_char *familyid);

/**
 * @brief 设定设familyId变化通知回调,开发者只有设定该回调，才能收到familyId变化通知回调。
 *
 * @param cb
 * @return uhsd_s32
 */
UHSD_API uhsd_s32 uhsd_dev_set_familyId_change_cb(uhsd_devcloud_familyId_change_event cb);
/**
 * @brief 获取familyId接口
 * @param familyid
 * @param familyid_len 最大长度32
 * @return 成功返回0,失败返回<0
 */
UHSD_API uhsd_s32 uhsd_dev_get_familyId(uhsd_char *familyid, uhsd_s32 *familyid_len);

/**
 * @brief 大数据上报
 *
 * @param devHandle 设备句柄
 * @param type 大数据类型, 标准数据类型为stdBigdata
 * @param data 大数据内容, 数据协议格式为如下json字符串, {"data" : [ {"name" : "***"}, ... ]}
 * @param len 大数据长度
 * @return 成功返回0,失败返回<0
 */
UHSD_API uhsd_s32 uhsd_dev_bigdata_report(uhsd_s32 devHandle, const uhsd_char *type, uhsd_void *data, uhsd_u32 len);

/**
 * @brief 通用打点功能
 * @param json_trace log_trace_notify/log_trace_list_notify格式
 * @return 成功返回0,失败返回<0
 */
UHSD_API uhsd_s32 uhsd_dev_trace_v2_send(const uhsd_char *json_trace);

/**
 * @brief 开启设备绑定时间窗结果通知回调函数
 * @param err 结果错误码: 0 没有错误；非0 有错误
 */
typedef uhsd_void (*uhsd_dev_bind_window_notify_cb)(uhsd_s32 err);

/**
 *  @brief 开启设备绑定时间窗
 *  @param cb 开启设备绑定时间窗结果通知回调函数
 *  @return 成功返回0, 失败返回-1
 * 注意：
 *  1. 使用此接口前需要在初始化阶段调用"设置主设备mac"接口: uhsd_dev_set_devHandle_dMac() 设置设备mac ;
 *  2. 开启设备绑定时间窗后才可以绑定设备, 时间窗有效期为20分钟, 过期后需要重新开启;
 */
UHSD_API uhsd_s32 uhsd_dev_bind_window(uhsd_dev_bind_window_notify_cb cb);

/**
 * @brief 获取一个osdk内部维护的全局递增seq number
 * @return u32
 */
UHSD_API uhsd_u32 uhsd_request_global_seq_number(uhsd_void);

#ifdef __cplusplus
}
#endif

#endif /*__UHSD_DEV_H__*/

/**@} grp_uhsddev end*/
