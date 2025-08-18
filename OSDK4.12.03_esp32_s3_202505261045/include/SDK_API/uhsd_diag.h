
#ifndef __UHSD_DIAG_H__
#define __UHSD_DIAG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "uhsd_types.h"

/**
 * @brief 获取wifi信号强度回调
 * @param [in] wifi状态，0：wifi已关闭， 1：wifi已开启
 * @param [in] wifi信号强度, 取值范围【-96, 0】，单位db; wifi关闭时传入-96即可；
 * @return 成功返回0,失败返回<0
 * @note 若为有线连接，则固定传入-96即可；
 */
typedef uhsd_s32 (*uhsd_wifi_rssi_cb)(uhsd_u8 *state, uhsd_s8 *rssi);

/**
 * @brief 获取信噪比回调
 * @param [in] snr 路由器信噪比 范围0-100
 * @return 成功返回0,失败返回<0
 */
typedef uhsd_s32 (*uhsd_wifi_snr_cb)(uhsd_u8 *snr);

/**
 * @brief 设置路由器信息【OSDK不接管wifi时可以使用】
 * @param [in] ssid wifi路由器ssid信息
 * @param [in] bssid wifi路由器bssid信息
 * @return 成功返回0,失败返回<0
 * @note 1. wifi路由器信息有变化时需要调用此接口
 *       2. bssid不可为NULL或者空字符串
 */
UHSD_API uhsd_s32 uhsd_diag_set_router_info(uhsd_char *ssid, uhsd_char *bssid);

/**
 * @brief 设置获取wifi信号强度回调函数【OSDK不接管wifi时可以使用】
 * @param [in] cb wifi信号强度回调
 * @return void
 */
UHSD_API uhsd_s32 uhsd_diag_set_wifi_rssi_cb(uhsd_wifi_rssi_cb cb);

/**
 * @brief 设置获取wifi信噪比回调函数【暂不支持】
 * @param [in] cb 信噪比回调
 * @return void
 */
UHSD_API uhsd_s32 uhsd_diag_set_snr_cb(uhsd_wifi_snr_cb cb);

/**
 * @brief 设备离线原因主动上报。
 * @param devHandle 设备唯一标识符
 * @param reason_code 设备离线原因码
 * @return uhsd_s32
 *      0: 成功
 *    < 0: 失败
 * @note 1. 此接口只针对主设备；
 *       2. 此接口必须要在连云断开时才能调用，否则无效；
 */
UHSD_API uhsd_s32 uhsd_dev_offline_reason_report(uhsd_devHandle devHandle, uhsd_dev_offline_reason_t reason_code);

/*
 * @brief 获取当前存储的wifi ssid和密码
 * @param [out] ssid wifi ssid 最大长度为32字节
 * @param [out] passwd wifi passwd 最大长度为64字节
 * @return void
 */
UHSD_API uhsd_void uhsd_diag_get_wifi_ssid_passwd(uhsd_char *ssid, uhsd_char *passwd);

/**
 * @brief 获取路由器wifi状态和wifi信号强度   【OSDK接管wifi时可以调用】
 * @param[out] status 路由器wifi状态，0:WIFI不可用, 1:wifi可用
 * @param[out] rssi 路由器wifi信号强度(0-100)
 * @return 成功返回0，失败返回<0
 */
UHSD_API uhsd_s32 uhsd_diag_router_status_get(uhos_u8 *status, uhos_u8 *rssi);

#ifdef __cplusplus
}
#endif

#endif /*__UHSD_DIAG_H__*/