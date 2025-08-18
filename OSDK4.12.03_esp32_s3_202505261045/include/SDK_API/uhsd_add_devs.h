#ifndef __UHSD_ADD_SLAVE_DEV_H__
#define __UHSD_ADD_SLAVE_DEV_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "uhsd_add_dev_common.h"

typedef enum
{
    UHSD_DEV_OFFLINE = 0,
    UHSD_DEV_ONLINE = 1
} uhsd_dev_online_status_t;

typedef struct
{
    uhsd_s32 err_num;
    uhsd_char *err_code;
    uhos_char *devid;
} uhsd_dev_result_info_t;

/**
 * @brief 添加设备结果通知
 *
 * @param[in] devHandle 设备句柄
 * @param[in] sn 请求唯一标识
 * @param[in] result 添加结果信息
 * @param[in] param 回传参数
 */
typedef uhsd_void (*uhsd_dev_add_dev_result_cb)(uhsd_devHandle devHandle, uhsd_u32 sn, uhsd_dev_result_info_t *result, uhsd_void *param);

/**
 * @brief 添加设备
 *
 * @param[in] devHandle 设备句柄
 * @param[in] sn 请求唯一标识
 * @param[in] cb 添加结果通知回调
 * @param[in] param 回调函数的回传参数
 * @return 成功返回0, 失败返回<0
 *
 * @note 1. 必须先添加主机设备(即第一个创建的设备)；
 *       2. 添加子机设备前，主机设备必须已经被绑定；
 */
UHSD_API uhsd_s32 uhsd_dev_add_dev(uhsd_devHandle devHandle, uhsd_u32 sn, uhsd_dev_add_dev_result_cb cb, uhsd_void *param);

/**
 * @brief 删除设备结果通知
 *
 * @param[in] devHandle 设备句柄
 * @param[in] sn 请求唯一标识
 * @param[in] result 删除结果信息
 * @param[in] param 回调函数的回传参数
 */
typedef uhsd_void (*uhsd_dev_del_dev_result_cb)(uhsd_devHandle devHandle, uhsd_u32 sn, uhsd_dev_result_info_t *result, uhsd_void *param);

/**
 * @brief 删除设备【子机设备允许调用】
 *
 * @param[in] devHandle 设备句柄
 * @param[in] sn 请求唯一标识
 * @param[in] cb 删除结果通知回调
 * @param[in] param 回传参数
 * @result 成功返回0, 失败返回<0
 */
UHSD_API uhsd_s32 uhsd_dev_del_dev(uhsd_devHandle devHandle, uhsd_u32 sn, uhsd_dev_del_dev_result_cb cb, uhsd_void *param);

/**
 * @brief 设置设备的在线状态【子机设备允许调用】
 *
 * @param[in] devHandle 设备句柄
 * @param[in] status 设备在线状态
 * @return 成功返回0, 失败返回<0
 *
 * @note 1. 调用此接口后，OSDK会根据传入的status让设备上线或下线
 *       2. 调用添加从机设备后，OSDK会自动让设备上线，无需调用此接口上线设备；
 *       3. TODO
 */
UHSD_API uhsd_s32 uhsd_dev_set_dev_online_status(uhsd_devHandle devHandle, uhsd_dev_online_status_t status);

/**
 * @brief 获取设备的在线状态
 *
 * @param[in] devHandle 设备句柄
 * @param[out] status 设备的在线状态
 * @return 成功返回0, 失败返回<0
 */
UHSD_API uhsd_s32 uhsd_dev_get_dev_online_status(uhsd_devHandle devHandle, uhsd_dev_online_status_t *status);

#ifdef __cplusplus
}
#endif

#endif /*__UHSD_ADD_SLAVE_DEV_H__*/