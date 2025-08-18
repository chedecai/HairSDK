/**
 * @brief 1. 本文件中的添加设备接口暂时为线程非安全；
 *        2. 添加单设备头文件(即指需要添加一个设备)；
 *        2. 添加设备伪代码可参考https://ihaier.feishu.cn/wiki/H9YBw7HJQiLkqgkFSYOcj2tmnDf
 */

#ifndef __UHSD_ADD_DEV_H__
#define __UHSD_ADD_DEV_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "uhsd_add_dev_common.h"

/**
 * @brief 注册deviceId结果回调
 *
 * @param[in] devHandle 设备句柄
 * @param[in] req_sn 请求唯一标识,devApp调用注册接口时传入
 * @param[in] err_num 注册设备错误码，成功为0，失败<0；失败时errCode标识具体错误码
 * @param[in] deviceId 设备唯一标识
 * @param[in] errCode 具体错误码，err_num<0时有效
 * @param[in] cb_param  回调函数的回传参数
 *
 * @note 只有此回调触发且err_num为0时,才可以调用设备上线接口去上线设备
 */
typedef uhsd_s32 (*uhsd_dev_register_deviceId_result_cb)(uhsd_devHandle devHandle,
                                                         uhsd_s32 req_sn,
                                                         uhsd_s32 err_num,
                                                         const uhsd_char *deviceId,
                                                         const uhsd_char *errCode,
                                                         uhsd_void *cb_param);

/**
 * @brief 主机设备注册，调用此接口可以获取设备的唯一标识
 *
 * @param[in] devHandle 设备句柄
 * @param[in] req_sn 请求sn
 * @param[in] upgrade_status 是否在升级中 0:不在升级中，1:在升级中
 * @param[in] cb 结果回调
 * @param[in] cb_param 结果回调回传参数
 * @return 成功返回0,失败返回<0
 *
 * @note  【！！！仅用于主机设备】
 */
UHSD_API uhsd_s32
uhsd_dev_register_deviceId(uhsd_devHandle devHandle, uhsd_s32 req_sn, uhsd_s32 upgrade_status, uhsd_dev_register_deviceId_result_cb cb, uhsd_void *cb_param);

/**
 * @brief 主机设备上线，设备必须先注册成功获取到devid后，才可上线
 *
 * @param[in] devHandle 设备句柄
 * @return 成功返回0,失败返回<0
 *
 * @note  【！！！仅用于主机设备】
 */
UHSD_API uhsd_s32 uhsd_dev_device_online(uhsd_devHandle devHandle);

#ifdef __cplusplus
}
#endif

#endif
