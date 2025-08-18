#ifndef __UHSD_DEVCTL_LOW_POWER_H__
#define __UHSD_DEVCTL_LOW_POWER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "uhsd_types.h"

typedef struct
{
    uhsd_char gwAddressesInfo[128]; //设备休眠网关地址
    uhsd_s32 sleep_gw_port;         //设备网关端口
    uhsd_u8 heartbeatPkg[256];      //设备休眠心跳包内容
    uhsd_s32 heartbeatPkg_len;      //设备休眠心跳包长度
    uhsd_u8 wakeUpPkg[256];         //设备休眠唤醒包内容
    uhsd_s32 wakeUpPkg_len;         //设备休眠唤醒包长度
} uhsd_lowpoer_heartbeat_info;

typedef uhsd_s32 (*uhsd_devctl_get_lowpower_info_cb)(uhsd_s32 err, uhsd_lowpoer_heartbeat_info *pstSleepInfo);
uhsd_s32 uhsd_devctl_lowpower_info_request(uhsd_s32 timeout, uhsd_devctl_get_lowpower_info_cb cb_fun);

#ifdef __cplusplus
}
#endif
#endif