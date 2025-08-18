/**
 * @copyright Copyright (c) 2021, Haier.Co, Ltd.
 * @file uh_sysinfo.h
 * @author  sunzhiqiang@haier.com
 * @brief linux系统配置信息
 * @date 22023-04-01
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>version <th>Author  <th>Description
 * <tr><td>2023-04-01   <td>1.0     <td>sunzhiqiang        <td>init version
 * </table>
 */

#ifndef _UH_SYSINFO_H_
#define _UH_SYSINFO_H_

#include "uh_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @enum 配置类型
 */
typedef enum
{
    //静态字符串
    UHOS_SYSINFO_UHOMEOS_FILE_PATH = 0, //存储文件路径，存储LOG, CONFIG, 等等
    UHOS_SYSINFO_EPP_SERIAL_NAME,       // E++串口配置
    UHOS_SYSINFO_ETH_DEV_NAME,          //指定网卡名称配置开关, 或者是当前使用网卡名称

    //整数值
    UHOS_SYSINFO_FLASH_SIZE_SAVE_LOG,     //为日志保留的FLASH空间，如果空间不够，业务上应保证跟OTA互斥
    UHOS_SYSINFO_DEFAULT_WIRE_IFNAME,     //有线网网卡系统名称
    UHOS_SYSINFO_DEFAULT_WIRELESS_IFNAME, //无线网网卡系统名称
} uhos_sysinfo_type_t;

/**
 * @union 配置信息
 */
typedef union uhos_sysinfo
{
    uhos_char *value_str; //静态字符串
    uhos_s32 value_int;   //整数值
} uhos_sysinfo_t;

/**
 * @brief 系统配置信息获取
 *        重新实现该接口可以自定义系统配置信息,接口中必须处理所有配置类型
 * @param type 配置类型
 * @return uhos_sysinfo_t 系统配置信息
 *
 */
uhos_sysinfo_t uhos_sysinfo_get(uhos_sysinfo_type_t type);
uhos_s32 uhos_sysinfo_set(uhos_sysinfo_type_t type, uhos_sysinfo_t info);

#ifdef __cplusplus
}
#endif

#endif // _UH_SYSINFO_H_