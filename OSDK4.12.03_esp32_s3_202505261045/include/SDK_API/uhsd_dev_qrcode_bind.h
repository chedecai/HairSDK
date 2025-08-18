/*
 * @Author: Carl wangkaifan@haier.com
 * @Date: 2023-04-25 14:09:23
 * @LastEditors: Carl wangkaifan@haier.com
 * @LastEditTime: 2023-04-28 09:59:16
 * @FilePath: /jzt40/UHOS/interface/SDK_API/include/uhsd_dev_qrcode_bind.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __UHSD_DEV__QRCODE_BIND_H__
#define __UHSD_DEV__QRCODE_BIND_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "uhsd_types.h"
typedef struct
{
    uhsd_char token[48];
    uhsd_u32 bindCode;
    uhsd_char passwd[48];
    uhsd_char ssid[48];
    uhsd_char bssid[48];
    uhsd_char traceid[32 + 1];
    uhsd_s32 flag_network_ok;
} uhsd_qrcode_bind_data_t;
/**
 *  @brief （绑定设备接口回调）
 *  @param[out] dev_id 绑定设备的deviceid
 *  @param[out] 此次绑定的sn
 *  @param[err] 此次绑定的结果，成功为0.否则失败
 *  @attention 绑定时token或者bindcode必须有一个是正确有效的才能绑定成功
 */
typedef uhsd_s32 (*uhsd_dev_bind_gw_cb)(const uhsd_char *dev_id, uhsd_s32 sn, uhsd_s32 err);
/**
 *  @brief （扫码绑配置接口，不需要osdk接管WiFi时调用此接口）
 *  @param[in] data 摄像头识别二维码后的字符串
 *  @param[out] out 二维码解析后的结构体
 *  @return      成功返回0, 失败返回负数
 *  @attention 无
 */
UHSD_API uhsd_s32 uhsd_video_parse_qr_code_data(uhsd_char *data, uhsd_qrcode_bind_data_t **out);
/**
 *  @brief （扫码绑配置接口，需要osdk接管WiFi时调用此接口）
 *  @param[in] data 摄像头识别二维码后的字符串
 *  @return      成功返回0, 失败返回负数
 *  @attention 无
 */
UHSD_API uhsd_s32 uhsd_dev_config_qr_code_data_set(uhsd_char *data);
/**
 *  @brief （绑定设备接口）
 *  @param[in] dName 设备名称
 *  @param[in] token 设备绑定所需的token
 *  @param[in] bindcode 设备绑定所需的bindcode
 *  @param[out] bind_cb 绑定通知
 *  @return      成功返回0, 失败返回负数
 *  @attention 绑定时token或者bindcode必须有一个是正确有效的才能绑定成功
 */
UHSD_API uhsd_s32 uhsd_dev_bind_gw_dev(uhsd_char *dName, uhsd_char *token, uhsd_u32 bindcode, uhsd_dev_bind_gw_cb bind_cb);

#ifdef __cplusplus
}
#endif

#endif