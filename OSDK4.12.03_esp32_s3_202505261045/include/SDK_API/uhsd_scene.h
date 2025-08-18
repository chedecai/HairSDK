/**
 *
 * @defgroup grp_uhsd 场景功能
 * @{
 * @brief SmartDevice 场景功能接口
 * @version 0.1
 * @author hudx (hudingxuan.xian@haier.com)
 * @date 2023-06-12
 * @details
 * <pre>
 * @verbatim
 * @endverbatim
 * </pre>
 *
 * @file uhsd_scene.h
 *
 * @copyright Copyright (c) 2021, Haier.Co, Ltd.
 *
 */

#ifndef __UHSD_SCENE_H__
#define __UHSD_SCENE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "uhsd_types.h"

typedef uhsd_s32 (*uhsd_scene_async_read_notify_pfunc)(uhsd_u32 id, uhsd_u32 offset, uhsd_u32 len, uhsd_u32 buffer_size, uhsd_void *user_data);

typedef struct _uhsd_scene_data_t_
{
    uhsd_u32 id;
    uhsd_u16 type;
    uhsd_u32 size;
    uhsd_u32 alg_type;
    uhsd_u8 digest[32];
    uhsd_void *user_data;
    uhsd_scene_async_read_notify_pfunc async_read_notify_fn;
} uhsd_scene_data_t;

/**
 * @brief 联动配对业务开始
 * @details
 */
uhsd_s32 uhsd_scene_pair_start(uhsd_void);

/**
 * @brief 联动配对业务停止
 */
uhsd_s32 uhsd_scene_pair_stop(uhsd_s32 reason);

/**
 * @brief 解除配对
 * @details
 * 支持多
 */
uhsd_s32 uhsd_scene_unpair(uhsd_void);

/**
 * @brief 开启场景
 * @details 如果配对成功，需要使用者进行开启场景。
 */
uhsd_s32 uhsd_scene_enable(uhsd_void);

/**
 * @brief 关闭场景
 * @details
 * 如关闭场景，所有完成过配对的设备都会停止场景联动。
 */
uhsd_s32 uhsd_scene_disable(uhsd_void);

/**
 * @brief 获取场景状态
 * @param [out] pair_status 大于0表示已经配对，反之代表未配对
 * @param [out] scene_status 大于0表示场景已经使能，反之代表场景已经关闭
 * @details
 */
uhsd_s32 uhsd_scene_status_get(uhsd_u8 *pair_status, uhsd_u8 *scene_status);

/**
 * @brief 预置模版数据
 * 设置结束后，使用 @ref uhsd_scene_data_update 接口触发数据更新。
 * @note
 * 如果有多个文件，则需要多次调用。
 *
 *
 * @param scene_data 模板文件描述信息
 * @return uhsd_s32
 */
uhsd_s32 uhsd_scene_data_set(uhsd_scene_data_t *scene_data);

/**
 * @brief 触发预置模版数据更新
 *
 *
 * @return uhsd_s32
 */
uhsd_s32 uhsd_scene_data_update(uhsd_void);

/**
 * @brief
 *
 * @param data_id 数据标识（或是文件标识）
 * @param offset 返回的数据偏移地址
 * @param buf 数据
 * @param len 此次返回的数据大小
 * @return uhsd_s32
 * @retval 0 成功
 * @retval others 失败
 */
uhsd_s32 uhsd_scene_data_async_read_resp(uhsd_u32 data_id, uhsd_u32 offset, uhsd_char *buf, uhsd_u32 len);

#ifdef __cplusplus
}
#endif

#endif

/** @}*/
