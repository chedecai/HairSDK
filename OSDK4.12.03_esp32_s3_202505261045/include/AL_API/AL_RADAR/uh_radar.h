#ifndef __UH_RADAR_H__
#define __UH_RADAR_H__

#include "uh_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(1)
/**
 * @brief 雷达检测结果
 * @param somebody 0:无人，1:有人
 * @param distance_min 距离最小值（单位：cm）
 * @param distance_max 距离最大值（单位：cm）
 */
typedef struct uhos_radar_result
{
    uhos_u8 somebody;
    uhos_u32 distance_min;
    uhos_u32 distance_max;
} uhos_radar_result_t;

/**
 * @brief 雷达参数
 * @param range 判断有人的最大范围（单位：cm）
 * @param delay_time_judge_nobody 判断有人到无人的延时时间（单位：s）
 */
typedef struct uhos_radar_param
{
    uhos_u32 range;
    uhos_u32 delay_time_judge_nobody;
} uhos_radar_param_t;

#pragma pack()

/**
 * @brief 雷达检测结果变化通知接口
 * @param result 雷达检测结果
 * @return uhos_s32
 */
typedef uhos_s32 (*uhos_radar_result_cb)(uhos_radar_result_t result);

/**
 * @brief 注册雷达检测结果回调接口。
 * @param cb
 * @return uhos_s32
 */
uhos_s32 uhos_radar_register_result_cb(uhos_radar_result_cb cb);

/**
 * @brief 查询雷达状态。
 * @return uhos_s32 0：未启动；1：已启动；<0：异常停止；
 */
uhos_s32 uhos_radar_get_status(void);

/**
 * @brief 查询雷达检测结果。
 *
 * @param result
 * @return uhos_s32
 */
uhos_s32 uhos_radar_get_result(uhos_radar_result_t *result);

/**
 * @brief 启动雷达功能。
 *
 * @param param
 * @return uhos_s32
 */
uhos_s32 uhos_radar_start(uhos_radar_param_t param);

/**
 * @brief 停止雷达功能。
 *
 * @return uhos_s32
 */
uhos_s32 uhos_radar_stop(void);

#ifdef __cplusplus
}
#endif

#endif /* __UH_RADAR_H__ */
