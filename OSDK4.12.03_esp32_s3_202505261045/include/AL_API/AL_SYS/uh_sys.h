/**
 * @defgroup grp_uhossys 系统适配层接口定义
 * @{
 * @copyright Copyright (c) 2021, Haier.Co, Ltd.
 * @file uh_sys.h
 * @author tangjinqi@haier.com
 * @brief
 * @date 2021-09-29
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>version <th>Author  <th>Description
 * <tr><td>2021-09-29   <td>1.0     <td>tangjinqi    <td>init version
 * </table>
 */

#ifndef __UHOS_SYS_H__
#define __UHOS_SYS_H__

#include "uh_types.h"

// typedef unsigned int uhos_size_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 打开串行设备（与底板通信）。有海尔EPP组件，才需要适配，有参考代码提供
 * @details
 打开串行设备。串行设备硬件参数根据硬件平台和协议自行确定。串行设备发送缓存至少256字节，接收缓存至少1K字节。串行设备读数据需支持select机制。
 如果设备的读写是同一个描述符，则返回一个描述符；否则，需要返回两个描述符，第一个是用于读，第二个用于写，两个描述符都必须合法有效。
 * @param fd 设备描述符，如果成功返回设备描述符；否则不填写。
 * @return 成功返回0，失败返回-1。
 */
uhos_s32 uhos_sys_serial_open(uhos_s32 *fd, uhos_u32 baudrate);

/**
 * @brief 关闭串行设备（与底板通信）。打开串行设备（与底板通信）。有海尔EPP组件，才需要适配，有参考代码提供
 * @details
 * 关闭串行设备。如果打开串行设备返回了两个描述符，则uplugSDK会主动调用两次本接口以关闭串行设备。
 * @param fd 设备描述符。
 * @return 成功返回0，失败返回-1。
 */
uhos_s32 uhos_sys_serial_close(uhos_s32 fd);

/**
 * @brief 从串行设备读取数据（与底板通信）。打开串行设备（与底板通信）。有海尔EPP组件，才需要适配，有参考代码提供
 * @details
 * 从串行设备读取数据。如果接收缓存中数据的长度大于len，返回len长度数据；否则返回实际数据长度。
 * @param fd 设备描述符。
 * @param buf 数据缓存。
 * @param len 数据缓存大小（期望读取的数据长度）。
 * @return 成功返回实际读取的数据长度，失败返回-1。
 */
uhos_s32 uhos_sys_serial_read(uhos_s32 fd, uhos_u8 *buf, uhos_size_t len);

/**
 * @brief 向串行设备写入数据（与底板通信）。
 * @param fd 设备描述符。
 * @param buf 数据缓存。
 * @param len 数据缓存大小（期望写入的数据长度）。
 * @return 成功返回实际写入的数据长度，失败返回-1。
 */
uhos_s32 uhos_sys_serial_write(uhos_s32 fd, uhos_u8 *buf, uhos_size_t len);

/**
 * @brief       用于日志的输出， 如果支持SHELL组件，不需要实现
 * @param log   日志缓存
 * @param size  日志字符串大小
 * @return
 */
UHSD_API void uhos_log_output(uhos_u8 *log, uhos_size_t size);

/**
 * @brief       用于shell的串口的初始化， 如果不支持SHELL组件，不需要实现
 * @return      执行结果；0-成功，-1-失败
 */
uhos_s32 uhos_sys_shell_serial_init(void);

/**
 * @brief       用于shell的串口读数据实现，如果不支持SHELL组件，不需要实现
 * @param[out]  buf 指向读取缓存区的指针
 * @param[in]   len 读取缓存区的大小
 * @return      读取数据的长度
 */
uhos_s32 uhos_sys_shell_serial_read(uhos_u8 *buf, uhos_size_t len);

/**
 * @brief       用于shell的串口写数据实现，如果不支持SHELL组件，不需要实现
 * @param[in]   buf 指向写入数据的指针
 * @param[in]   len 写入数据长度
 * @return      写入数据的长度
 */
uhos_s32 uhos_sys_shell_serial_write(uhos_u8 *buf, uhos_size_t len);

/**
 * @brief 系统复位。
 * @return N/A。
 */
UHSD_API void uhos_sys_system_reset(void);

/**
 * @brief 低功耗接口，暂无应用，可直接返回
 * @return N/A。
 */
void uhos_sys_chippin_set(uhos_u8 onoff);

/**
 * @brief 读取配置，从非易失性存储器中读取配置到内存中。
 * @param config_zone 配置区域，ZONE_1/ZONE_2。
 * @param conf 内存指针，存放读取的配置数据。
 * @param len 长度，读取的配置数据长度。
 * @return 成功返回0，失败返回-1。
 */
uhos_s32 uhos_sys_config_read(uhos_u8 config_zone, uhos_u8 *conf, uhos_size_t len);

/**
 * @brief 写配置，将配置写入非易失性存储器中。
 * @param config_zone 配置区域，ZONE_1/ZONE_2。
 * @param conf 内存指针，存放待写入的配置数据。
 * @param len 长度，待写入的配置数据长度。
 * @return 成功返回0，失败返回-1。
 */
uhos_s32 uhos_sys_config_write(uhos_u8 config_zone, uhos_u8 *conf, uhos_size_t len);

/** @def ZONE_1
 *
 * @brief 代码区1镜像（A区） 4096
 *  */
/** @def ZONE_2
 *
 * @brief 代码区2镜像（B区） 4096
 */
/** @def ZONE_BOARD
 *
 * @brief 底板OTA镜像
 */
/** @def ZONE_3
 *
 * @brief 多子机数据存储 子机数量 * 1024
 */
/** @def ZONE_4
 *
 * @brief 消息传递信息 1024
 */
/** @def ZONE_5                    有场景联动功能时，必须配置
 *
 * @brief 场景模板信息 4096
 */
/** @def ZONE_6                  有场景联动功能时，必须配置
 *
 * @brief 场景实例信息 4096
 * @brief 场景模板信息 4096
 */
/** @def ZONE_7
 *
 * @brief OTA状态  大小 4096   有OTA组件时，必须配置
 *
 */
/** @def ZONE_8
 *
 * @brief 附件设备  未启用
 *
 */
/** @def ZONE_9
 *
 * @brief 多子机绑定状态  大小 6k
 *
 */
#define ZONE_1       1
#define ZONE_2       2
#define ZONE_BOARD   3
#define ZONE_3       4
#define ZONE_4       5
#define ZONE_5       6
#define ZONE_6       7
#define ZONE_7       8
#define ZONE_LICENSE 9
#define ZONE_8       10
#define ZONE_9       11

#define ZONE_OTHER 0

/**
 * @brief 获取当前运行镜像区域。
 * @param
 * image_zone，当前运行的版本镜像区，ZONE_1/ZONE_2/ZONE_OTHER。如果成功，返回当前运行的镜像区域；否则不填写。如果当前运行版本既不是镜像1，也不是镜像2，返回ZONE_OTHER。
 * @return 成功返回0，失败返回-1。
 */
UHSD_API uhos_s32 uhos_sys_run_image_get(uhos_u8 *image_zone);

/**
 * @brief 读取镜像数据。
 * @param image_zone，镜像区域，ZONE_XXX。
 * @param offset 偏移量。
 * @param buf 内存指针。
 * @param len 读取的长度。
 * @running_image_zone :only image_zone=ZONE_BOARD can use running_image_zone
 * @return 成功返回0，失败返回-1。
 */
UHSD_API uhos_s32 uhos_sys_image_read(uhos_u8 image_zone, uhos_u32 offset, uhos_u8 *buf, uhos_size_t len);

/**
 * @brief 写入镜像数据。
 * @param image_zone，镜像区域，ZONE_XXX。
 * @param offset 偏移量。
 * @param buf 待写入数据的内存指针。
 * @param len 写入的长度。
 * @running_image_zone :only image_zone=ZONE_BOARD can use running_image_zone
 * @return 成功返回0，失败返回-1。
 */
UHSD_API uhos_s32 uhos_sys_image_write(uhos_u8 image_zone, uhos_u32 offset, uhos_u8 *buf, uhos_size_t len);

/**
 * @brief 取得A/B区 预留镜像的大小
 *
 * @param image_zone
 * @return uhos_s32
 */
UHSD_API uhos_s32 uhos_get_sys_image_size(uhos_u8 image_zone);
/**
 * @brief 准备写入镜像数据。
 * @param image_zone，镜像区域，ZONE_XXX。
 * @return 成功返回0，失败返回-1。
 */
UHSD_API uhos_s32 uhos_sys_image_start(uhos_u8 image_zone);

/** @def OTA_IMAGE_OK
 *  @def OTA_IMAGE_FAIL
 *
 * @brief 写入镜像数据的结果。
 */
#define OTA_IMAGE_OK   1
#define OTA_IMAGE_FAIL 0

/**
 * @brief 完成写入镜像数据。
 * @param image_zone，镜像区域，ZONE_XXX。
 * @param result 写入镜像数据的结果，OTA_IMAGE_OK/OTA_IMAGE_FAIL
 * @return 成功返回0，失败返回-1。
 */
UHSD_API uhos_s32 uhos_sys_image_finish(uhos_u8 image_zone, uhos_s32 result);

/**
 * @brief 镜像版本切换
 * @param image_zone 镜像区域，ZONE_1/ZONE_2。下一次启动的版本区域是image_zone。
 * @return 成功返回0，失败返回-1。
 */
UHSD_API uhos_s32 uhos_sys_image_switch(uhos_u8 image_zone);

/**
 * @brief 显示剩余可分配内存大小
 * @details 用于调试，可以不实现
 * @return N/A。
 */
void uhos_display_freeheap_size(void);

/** @def SYSTEM_RESET_TYPE_
 * @brief 系统复位类型。
 * UNDEF 未定义。
 * POWER_ON 上电。
 * WATCHDOG 狗复位。
 * RESET 主动复位。
 * PIN   设备外部管脚复位。
 * PROC_RESET 进程主动重启。
 * PROC_ABNORMAL 进程异常退出。
 * PROC_WATCHDOG 进程看门狗复位。
 * PROC_MEM_OVERFLOW 进程内存溢出
 */
#define SYSTEM_RESET_TYPE_UNDEF             -1
#define SYSTEM_RESET_TYPE_POWER_ON          0
#define SYSTEM_RESET_TYPE_WATCHDOG          1
#define SYSTEM_RESET_TYPE_RESET             2
#define SYSTEM_RESET_TYPE_PIN               3
#define SYSTEM_RESET_TYPE_PROC_RESET        4
#define SYSTEM_RESET_TYPE_PROC_ABNORMAL     5
#define SYSTEM_RESET_TYPE_PROC_WATCHDOG     6
#define SYSTEM_RESET_TYPE_PROC_MEM_OVERFLOW 7

/**
 * @brief 获取系统复位类型。
 * @return 系统复位类型，如果不支持，返回SYSTEM_RESET_TYPE_UNDEF。
 */
UHSD_API uhos_s32 uhos_sys_reset_type_get(void);

/**
 * @brief 获取系统保留区域大小。
 * @details 系统保留区域是软件重启不丢失数据的区域。
 * 如果不支持，返回0。
 * 如果支持，必须是256字节的整倍数。
 * param res_area 如果支持直接访问，返回直接访问指针；否则返回NULL。
 * @return 系统保留区域大小。
 */
uhos_u16 uhos_sys_res_area_get_size(void **mem);

/**
 * @brief 获取系统保留区域的数据。
 * @details 如果可以直接访问保留区域，实现空功能即可。
 * @param offset 偏移量。
 * @param buf 内存指针。
 * @param len 读取的长度。
 * @return 成功返回0，失败返回-1。
 */
uhos_s32 uhos_sys_res_area_read(uhos_u32 offset, uhos_u8 *buf, uhos_size_t len);

/**
 * @brief 向系统保留区域写入数据。
 * @details 如果可以直接访问保留区域，实现空功能即可。
 * @param offset 偏移量。
 * @param buf 待写入数据的内存指针。
 * @param len 写入的长度。
 * @return 成功返回0，失败返回-1。
 */
uhos_s32 uhos_sys_res_area_write(uhos_u32 offset, uhos_u8 *buf, uhos_size_t len);

/**
 * @brief 进入低功耗模式。 有海尔EPP组件且支持低功耗功能时才需要适配
 * @details 进入低功耗后该接口不会返回，直到被唤醒。
 * @return N/A。
 */
void uhos_sys_lower_power_enter(void);

/**
 * @brief 退出低功耗模式。 有海尔EPP组件且支持低功耗功能时才需要适配
 * @details
 * @return N/A。
 */
void uhos_sys_lower_power_exit(void);

/**
 * @brief 启动看门狗。
 * @details 看门狗用于系统崩溃或者系统不响应时的恢复。
 * @param timeout 看门狗超时时间，单位毫秒。
 * @return 成功返回0，失败返回-1。
 */
uhos_s32 uhos_sys_watchdog_start(uhos_u32 timeout);

/**
 * @brief 喂狗。
 * @return N/A。
 */
void uhos_sys_watchdog_feed(void);

/**
 * @brief 硬件平台必要初始化入口;
 * @return int
 */
UHSD_API uhos_s32 uh_system_init(void);

/**
 * @brief 硬件平台必要反初始化入口;
 * @return int
 */
UHSD_API uhos_s32 uh_system_uninit(void);
UHSD_API uhos_s32 uhos_sys_config_app_read(uhos_u8 config_zone, uhos_u8 *conf, uhos_size_t len);
UHSD_API uhos_s32 uhos_sys_config_app_write(uhos_u8 config_zone, uhos_u8 *conf, uhos_size_t len);

#ifdef __cplusplus
}
#endif

#endif // _UH_SYS_H
       /**@} grp_uhostypes end */