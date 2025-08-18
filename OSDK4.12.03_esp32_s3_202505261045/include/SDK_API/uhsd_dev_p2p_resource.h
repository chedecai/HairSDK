#ifndef __UHSD_DEV_P2P_RESOURCE_H__
#define __UHSD_DEV_P2P_RESOURCE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "uhsd_types.h"

/**
 * @brief 资源通知回调
 * @param devHandle         设备句柄
 * @param rsc_name          资源名
 * @param rsc_type_cb_param 回传参数
 * @note  sdk触发此回调通知开发者APP请求的资源名，开发者根据资源名调用资源上报接口上报相应的资源
 */
typedef uhsd_void (*uhsd_dev_rsc_type_notify_cb)(uhsd_devHandle devHandle, uhsd_char *rsc_name, uhsd_void *rsc_type_cb_param);

/**
 * @brief 资源传输通道状态回调
 * @param devHandle               设备句柄
 * @param status                  通道状态，0-不可用，1-可用
 * @param channel_status_cb_param 回传参数
 * @note：sdk触发此回调通知开发者传输通道是否可用；当通道状态变化时会被触发；通道有效期为30天；
 */
typedef uhsd_void (*uhsd_dev_rsc_channel_status_notify_cb)(uhsd_devHandle devHandle, uhsd_s32 status, uhsd_void *channel_status_cb_param);

/**
 * @brief 注册资源相关回调函数
 * @param rsc_type_cb             资源通知回调函数
 * @param rsc_type_cb_param       资源通知回调回传参数
 * @param channel_status_cb       资源传输通道状态回调函数
 * @param channel_status_cb_param 资源传输通道状态回调回传参数
 * @return 成功返回0，失败返回<0
 */
UHSD_API uhsd_s32 uhsd_dev_register_rsc_related_cb(uhsd_dev_rsc_type_notify_cb rsc_type_cb,
                                                   uhsd_void *rsc_type_cb_param,
                                                   uhsd_dev_rsc_channel_status_notify_cb channel_status_cb,
                                                   uhsd_void *channel_status_cb_param);

/**
 * @brief 资源注册结果通知回调
 * @param devHandle 设备句柄
 * @param result    注册结果，成功为0,失败为<0
 * @param param     回调函数的回传参数
 */
typedef uhsd_void (*uhsd_dev_register_resource_result_cb)(uhsd_devHandle devHandle, uhsd_u32 sn, uhsd_s32 result, uhsd_void *param);

/**
 * @brief 资源注册接口
 * @param devHandle 设备句柄,目前仅支持主设备
 * @param rsc_name  资源名，长度范围[1,64], 参考设备相关文档的定义，e.g. 局部地图或者全局地图等；
 * @return 成功返回0, 失败返回<0
 */
UHSD_API uhsd_s32
uhsd_dev_register_resource(uhsd_devHandle devHandle, uhsd_char *rsc_name, uhsd_u32 sn, uhsd_dev_register_resource_result_cb cb, uhsd_void *param);

/**
 * @brief 资源注销接口,目前仅支持主设备
 * @param devHandle 设备句柄
 * @param rsc_name  资源名
 * @return 成功返回0, 失败返回<0
 */
UHSD_API uhsd_s32 uhsd_dev_unregister_resource(uhsd_devHandle devHandle, uhsd_char *rsc_name);

/**
 * @brief 资源上报接口,目前仅支持主设备
 * @param devHandle  设备句柄
 * @param rsc_name   资源名字
 * @param data       资源数据
 * @param len        资源数据长度
 * @return 成功返回0, 失败返回<0
 * @note 此接口将资源数据上报到云平台，在触发 资源通知回调 接口通知开发者
 */
UHSD_API uhsd_s32 uhsd_dev_report_resource(uhsd_devHandle devHandle, uhsd_char *rsc_name, uhsd_u8 *data, uhsd_u32 len);

#ifdef __cplusplus
}
#endif

#endif /*__UHSD_DEV_P2P_RESOURCE_H__*/