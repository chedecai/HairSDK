/**
 *
 * @defgroup grp_uhsd_dev_data_cvt 设备数据转换
 * @{
 * @brief SmartDevice 数据转换
 *
 * @version 0.1
 * @author hudx (hudingxuan.xian@haier.com)
 * @date 2022-08-14
 * @details
 * <pre>
 * @verbatim
 * 功能包括：
 * 1. binary数据格式转换为标准设备数据格式
 * 2. 标准设备数据格式转换为binary数据格式
 * @endverbatim
 * </pre>
 *
 * @file uhsd_dev_data_cvt.h
 *
 * @copyright Copyright (c) 2021, Haier.Co, Ltd.
 *
 */
#ifndef __UHSD_DEV_DATA_CVT_H__
#define __UHSD_DEV_DATA_CVT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "uhsd_types.h"

typedef uhsd_void *uhsd_ddc_mem_ctx_t;

/**
 * @brief 数据转换状态通知回调
 * @details
 * 由用户实现，用于通知当前数据转换功能的状态。
 * @param[out] devHandle 设备句柄
 * @param[out] status 状态
 * @param[out] user_param 用户参数， 来源： @ref uhsd_dev_data_cvt_params_set
 * @return uhsd_void
 *
 */
typedef uhsd_void (*uhsd_dev_data_cvt_status_notify_cb)(uhsd_devHandle devHandle, uhsd_s32 status, uhsd_void *user_param);

/**
 * @brief 创建数据转换功能句柄
 * @details 通过ID、KEY可从云端拉去设备配置信息
 *
 * @param[in] devHandle 设备句柄
 * @param[in] appID 应用ID，海极网颁发
 * @param[in] appKey 应用KEY，海极网颁发
 * @param[in] clientID 客户端ID，海极网颁发
 * @param[in] appVersion 用户应用版本
 * @return uhsd_void * 数据转换功能句柄，可创建多个。
 */
UHSD_API uhsd_void *uhsd_dev_data_cvt_crt(uhsd_char *appID, uhsd_char *appKey, uhsd_char *clientID, uhsd_char *appVersion);

/**
 * @brief 删除数据转换功能
 *
 * @param[in] dataCvtHandle 数据转换功能句柄，来源： @ref uhsd_dev_data_cvt_crt
 * @return uhsd_s32 删除结果
 * @retval 0 删除成功
 */
UHSD_API uhsd_s32 uhsd_dev_data_cvt_del(uhsd_void *dataCvtHandle);

/**
 * @brief smartDevice 设置需要转换的设备信息
 *
 * @details 设备APP必须调用此接口成功后，后续的转换接口才可以正常工作。
 * @param[in] devHandle 数据转换句柄，来源: @ref uhsd_dev_data_cvt_crt
 * @param[in] devHandle 设备句柄，来源: @ref uhsd_dev_new_devHandle
 * @param[in] dc_status_cb 回调状态，当转换器状态变更时通过此回调通知调用者。当前状态包括：新数据下载成功(用户可正常使用转换接口)；
 * @param[in] user_param 用户设置的回调数据指针， 会在 @ref uhsd_dev_data_cvt_status_notify_cb 回调时返回给调用者。
 * @return uhsd_s32
 * @retval 0 成功
 * @retval <0 异常 @ref UHSD_ERROR_NUM
 * @retval 无预置设备数据，需要等待网络连接且数据下载成功后，才可使用转换接口。
 * @retval 预制数据异常，需要等待网络连接且数据下载成功后，才可使用转换接口。
 *
 */
UHSD_API uhsd_s32 uhsd_dev_data_cvt_params_set(uhsd_void *dataCvtHandle,
                                               uhsd_devHandle devHandle,
                                               uhsd_dev_data_cvt_status_notify_cb dc_status_cb,
                                               uhsd_void *user_param);

/**
 * @brief smartDevice 设置配置文件数据
 *
 * @details 设备APP必须调用此接口成功后，后续的转换接口才可以正常工作。
 * @param[in] dataCvtHandle 数据转换句柄，来源: @ref uhsd_dev_data_cvt_crt
 * @param[in] devHandle 设备句柄，来源: @ref uhsd_dev_new_devHandle
 * @param[in] file_data_json E++配置文件数据JSON格式；
 * @return uhsd_s32
 * @retval 0 成功
 * @retval <0 异常 @ref UHSD_ERROR_NUM
 *
 * @attention file_data_json
 *
 */
UHSD_API uhsd_s32 uhsd_dev_data_cvt_cfgFileJsonData_set(uhsd_void *dataCvtHandle, uhsd_devHandle devHandle, uhsd_char *file_data_json);

/**
 * @brief OP binary数据转标准数据
 * @details
 * 将E++软件上报的响应（事件/属性）的binary数据模型转换为标准设备数据模型
 * 数据来源： @ref uhepp_op_binary_callback uhsd_dev_device_op_binary_resp uhsd_dev_op_binary_report
 * @param[in]   dataCvtHandle 句柄，来源： @ref uhsd_dev_data_cvt_crt 的返回值
 * @param[in]   devHandle 句柄，来源： @ref uhsd_dev_new_devHandle 的返回值
 * @param[in]   in_op_name 操作类型， 包括1. 全属性查询；2.单属性查询；3. 告警查询。对应 @ref uhsd_dev_op_binary_cb op_name
 * @param[in]   in_pairs 数据对，对应 @ref uhsd_dev_op_binary_cb pairs[]
 * @param[in]   in_pairs_num 数据对个数，对应 @ref uhsd_dev_op_binary_cb pairs_num
 * @param[out]  out_pairs 对应 @ref uhsd_dev_device_op_binary_resp @ref uhsd_dev_op_binary_report
 * @param[out]  out_pairs_num 对应 @ref uhsd_dev_device_op_binary_resp @ref uhsd_dev_op_binary_report pairs_num
 * @param[out]  out_data_mem_ctx 内存释放句柄
 * @return uhsd_s32
 * @retval 0 成功
 * @retval others 失败
 *
 * @attention
 * 使用完成out_pairs之后，出参必须使用 @ref uhsd_dev_data_cvt_pairs_free 进行释放
 */
UHSD_API uhsd_s32 uhsd_dev_data_cvt_op_binary_to_standard(uhsd_void *dataCvtHandle,
                                                          uhsd_devHandle devHandle,
                                                          const uhsd_char *in_op_name,
                                                          uhsd_dev_pair_t in_pairs[],
                                                          uhsd_s32 in_pairs_num,
                                                          uhsd_dev_pair_t *out_pairs[],
                                                          uhsd_s32 *out_pairs_num,
                                                          uhsd_ddc_mem_ctx_t *out_data_mem_ctx);

/**
 * @brief OP 标准数据转binary数据
 * @details
 * 数据来源： @ref uhsd_dev_op_cb
 * @param[in]   dataCvtHandle 句柄，来源： @ref uhsd_dev_data_cvt_crt 的返回值
 * @param[in]   devHandle 句柄，来源： @ref uhsd_dev_new_devHandle 的返回值
 * @param[in]   in_op_name 操作类型， 包括1. 全属性查询；2.单属性查询；3. 告警查询。对应 @ref uhsd_dev_op_cb op_name
 * @param[in]   in_pairs 数据对，对应 @ref uhsd_dev_op_cb pairs[]
 * @param[in]   in_pairs_num 数据对个数，对应 @ref uhsd_dev_op_cb pairs_num
 * @param[out]  out_op_name 操作类型，包括1. 全属性查询；2.单属性查询；3. 告警查询。对应 @ref uhsd_dev_op_binary_cb op_name
 * @param[out]  out_pairs 对应 @ref uhsd_dev_op_binary_cb pairs[]
 * @param[out]  out_pairs_num 对应 @ref uhsd_dev_op_binary_cb pairs_num
 * @return uhsd_s32
 * @attention
 * 出参必须使用 @ref uhsd_dev_data_cvt_pairs_free 进行释放
 */
UHSD_API uhsd_s32 uhsd_dev_data_cvt_op_standard_to_binary(uhsd_void *dataCvtHandle,
                                                          uhsd_devHandle devHandle,
                                                          const uhsd_char *in_op_name,
                                                          uhsd_dev_pair_t in_pairs[],
                                                          uhsd_s32 in_pairs_num,
                                                          uhsd_char **out_op_name,
                                                          uhsd_dev_pair_t *out_pairs[],
                                                          uhsd_s32 *out_pairs_num,
                                                          uhsd_ddc_mem_ctx_t *out_data_mem_ctx);

/**
 * @brief WRITE 标准数据转binary数据
 * @details
 * 数据来源： @ref uhsd_dev_write_cb
 * @param[in]   dataCvtHandle 句柄，来源： @ref uhsd_dev_data_cvt_crt 的返回值
 * @param[in]   devHandle 句柄，来源： @ref uhsd_dev_new_devHandle 的返回值
 * @param[in]   in_pairs 数据对，对应 @ref uhsd_dev_op_cb pairs[]
 * @param[in]   in_pairs_num 数据对个数，对应 @ref uhsd_dev_op_cb pairs_num
 * @param[out]  out_op_name 操作类型，包括1. 全属性查询；2.单属性查询；3. 告警查询。对应 @ref uhsd_dev_op_binary_cb op_name
 * @param[out]  out_pairs 对应 @ref uhsd_dev_op_binary_cb pairs[]
 * @param[out]  out_pairs_num 对应 @ref uhsd_dev_op_binary_cb pairs_num
 * @return UHSD_API
 * @attention
 * 出参必须使用 @ref uhsd_dev_data_cvt_pairs_free 进行释放
 */
UHSD_API uhsd_s32 uhsd_dev_data_cvt_write_standard_to_binary(uhsd_void *dataCvtHandle,
                                                             uhsd_devHandle devHandle,
                                                             const uhsd_char *property_name,
                                                             const uhsd_char *property_value,
                                                             uhsd_char **out_op_name,
                                                             uhsd_dev_pair_t *out_pairs[],
                                                             uhsd_s32 *out_pairs_num,
                                                             uhsd_ddc_mem_ctx_t *out_data_mem_ctx);

/**
 * @brief E++属性、告警、事件数据转换为标准数据
 * @param[in]   dataCvtHandle 句柄，来源： @ref uhsd_dev_data_cvt_crt 的返回值
 * @param[in]   devHandle 句柄，来源： @ref uhsd_dev_new_devHandle 的返回值
 * @param[in]   epp 完整E++数据
 * @param[in]   epp_len E++数据长度
 * @param[out]  out_pairs 标准数据对
 * @param[out]  out_pairs_num 标准数据对个数
 * @return UHSD_API
 * @attention
 * 出参必须使用 @ref uhsd_dev_data_cvt_pairs_free 进行释放
 */
UHSD_API uhsd_s32 uhsd_dev_data_cvt_rawEpp_to_standard(uhsd_void *dataCvtHandle,
                                                       uhsd_devHandle devHandle,
                                                       const uhsd_u8 *epp,
                                                       const uhsd_u16 epp_len,
                                                       uhsd_dev_pair_t *out_pairs[],
                                                       uhsd_s32 *out_pairs_num,
                                                       uhsd_ddc_mem_ctx_t *out_data_mem_ctx);

/**
 * @brief 标准下行控制数据转换为E++完整数据
 * @param[in]   dataCvtHandle 句柄，来源： @ref uhsd_dev_data_cvt_crt 的返回值
 * @param[in]   devHandle 句柄，来源： @ref uhsd_dev_new_devHandle 的返回值
 * @param[in]   in_op_name 操作类型， 包括1. 全属性查询；2.单属性查询；3. 告警查询。对应 @ref uhsd_dev_op_binary_cb op_name
 * @param[in]   in_pairs 数据对，对应 @ref uhsd_dev_op_cb pairs[]
 * @param[in]   in_pairs_num 数据对个数，对应 @ref uhsd_dev_op_cb pairs_num
 * @param[out]  out_epp 完整E++数据
 * @param[out]  out_epp_len 完整E++数据长度
 * @return uhsd_s32
 * @attention
 * 出参 out_epp 必须使用 @ref uhsd_dev_data_cvt_rawEpp_free 进行释放
 */
UHSD_API uhsd_s32 uhsd_dev_data_cvt_standard_to_rawEpp(uhsd_void *dataCvtHandle,
                                                       uhsd_devHandle devHandle,
                                                       const uhsd_char *in_op_name,
                                                       uhsd_dev_pair_t in_pairs[],
                                                       uhsd_s32 in_pairs_num,
                                                       uhsd_u8 **out_epp,
                                                       uhsd_u16 *out_epp_len,
                                                       uhsd_ddc_mem_ctx_t *out_data_mem_ctx);

/**
 * @brief 释放转换后的数据内存
 *
 * @param[in] ctx 释放内存上下文
 * @return uhsd_s32
 * @retval 0 成功
 * @retval -1 失败
 */
UHSD_API uhsd_s32 uhsd_dev_data_cvt_mem_free(uhsd_ddc_mem_ctx_t ctx);

#ifdef __cplusplus
}
#endif

#endif

/** @}*/
