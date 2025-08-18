/**
 *
 * @defgroup 组控 设备接入以及控制接口
 * @{
 * @author jiashouqiang (jiashouqiang@haier.com)
 * @author LiJianWu (lijianwu@haier.com)
 * @brief
 * 设备接入接口集合。使用此组接口可完成用户设备注册。
 * @version 0.1
 * @date 2023-11-22
 *
 * @copyright Copyright (c) 2023, Haier.Co, Ltd.
 * @file uhsd_dev.h
 *
 */

#ifndef __UHSD_GROUP_CTRL_DEV_H__
#define __UHSD_GROUP_CTRL_DEV_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "uhsd_types.h"
#include "uhsd_add_dev.h"

// 组控相关的定义
typedef struct
{
    uhsd_char group_id[128]; // 组唯一标识
    uhsd_s8 **devid_list;    // 组内设备唯一标识符列表
    uhsd_s32 num;            // 组内设备个数
} uhsd_dev_group_v2_info_t;

typedef enum
{
    GROUP_V2_CREATE = 1,
    GROUP_V2_DELETE,
    GROUP_V2_MODIFY
} uhsd_dev_group_v2_notify_type;

typedef enum
{
    GROUP_V1_ADD = 1,
    GROUP_V1_REMOVE,
    GROUP_V1_DESTORY
} uhsd_dev_group_v1_notify_type;

typedef struct uhsd_dev_group_pair
{
    const uhsd_char *name;
    const uhsd_char *value;
} uhsd_dev_group_pair;

/**
 * @brief 组控获取组信息回调
 * @param devHdl 设备句柄
 * @param sn 指令唯一标识符
 * @param err_num 错误值
 * @param cloud_retCode 云端返回的原始错误值, 当err_num小于0时，可以辅助通过cloud_retCode进一步确定云端返回的错误原因,"130024"："设备均未加入分组"
 * @param group_info group_info 分组信息指针数组
 * @param group_info_num 分组信息中的分组个数
 * @param cb_param 回传参数
 * @return 成功返回0，失败返回<0
 */
typedef uhsd_s32 (*uhsd_group_v2_fetch_info_result_cb)(const uhsd_s32 devHdl,
                                                       uhsd_s32 sn,
                                                       uhsd_s32 err_num,
                                                       uhsd_char *cloud_retCode,
                                                       uhsd_dev_group_v2_info_t *group_info[],
                                                       uhsd_s32 group_info_num,
                                                       void *cb_param);

/**
 * @brief 组控写回调
 * @param group_id 组标识
 * @param sn 指令唯一标识符
 * @param name 属性名
 * @param value 属性值
 * @param trace_id trace id，开发者不关心的话可以忽略
 * @param cb_param 回传参数
 * @return 成功返回0，失败返回<0
 */
typedef uhsd_s32 (
    *uhsd_dev_group_v2_write_cb)(const uhsd_char *group_id, uhsd_s32 sn, const uhsd_char *name, const uhsd_char *value, uhsd_char *trace_id, void *cb_param);

/**
 * @brief 组控分组信息变化回调
 * @param sn 指令唯一标识
 * @param type 通知信息类型，
 * @param group_info 分组信息指针数组
 * @param group_info_num 分组信息中的分组个数
 * @param cb_param 回传参数
 * @return 成功返回0，失败返回<0
 */
typedef uhsd_s32 (*uhsd_dev_group_v2_notify_cb)(uhsd_s32 sn,
                                                uhsd_dev_group_v2_notify_type type,
                                                uhsd_dev_group_v2_info_t *group_info[],
                                                uhsd_s32 group_info_num,
                                                void *cb_param);

/**
 * @brief 组控操作回调
 * @param group_id 组唯一标识
 * @param sn 指令唯一标识，开发者调用读应答接口时原样传入
 * @param name 操作名
 * @param pairs 键值对数组
 * @param len 键值对数组长度
 * @param trace_id trace id，开发者不关心的话可以忽略
 * @param cb_param 回传参数
 * @return 成功返回0，失败返回<0
 */
typedef uhsd_s32 (*uhsd_dev_group_v2_op_cb)(const uhsd_char *group_id,
                                            uhsd_s32 sn,
                                            const uhsd_char *name,
                                            uhsd_dev_group_pair *pairs[],
                                            uhsd_s32 len,
                                            uhsd_char *trace_id,
                                            void *cb_param);

/**
 * @brief 组控注册相关函数和参数
 * @param write_cb 写回调函数
 * @param write_cb_param 回传参数
 * @param op_cb 操作回调函数
 * @param op_cb_param 回传参数
 * @param notify_cb 组变化回调函数
 * @param notify_cb_param 回传参数
 * @return 成功返回0，失败返回<0
 */
UHSD_API uhsd_s32 uhsd_group_ctrl_v2_register_cb(uhsd_dev_group_v2_write_cb write_cb,
                                                 void *write_cb_param,
                                                 uhsd_dev_group_v2_op_cb op_cb,
                                                 void *op_cb_param,
                                                 uhsd_dev_group_v2_notify_cb notify_cb,
                                                 void *notify_cb_param);

/**
 * @brief 组控write应答
 * @param sn 指令唯一标识，开发者调用读应答接口时原样传入
 * @param err_code 错误码
 * @return 成功返回0，失败返回<0
 */
UHSD_API uhsd_s32 uhsd_group_ctrl_v2_write_resp(uhsd_s32 sn, uhsd_s32 err_code);

/**
 * @brief 组控notify应答
 * @param sn 指令唯一标识，开发者调用读应答接口时原样传入
 * @param err_code 错误码
 * @return 成功返回0，失败返回<0
 */
UHSD_API uhsd_s32 uhsd_group_ctrl_v2_notify_resp(uhsd_s32 sn, uhsd_s32 err_code);

/**
 * @brief 组控op应答
 * @param sn 指令唯一标识，开发者调用读应答接口时原样传入
 * @param err_code 错误码
 * @param pairs 键值对数组
 * @param len 键值对数组长度
 * @return 成功返回0，失败返回<0
 */
UHSD_API uhsd_s32 uhsd_group_ctrl_v2_op_resp(uhsd_s32 sn, uhsd_s32 err_code, uhsd_dev_group_pair *pairs[], uhsd_s32 len);

/**
 * @brief 组控主动查询组信息
 * @param devHdl 设备句柄
 * @param sn 指令唯一标识
 * @param cb 获取应答函数
 * @param cb_param 应答函数回传参数
 * @return 成功返回0，失败返回<0
 */
UHSD_API uhsd_s32 uhsd_group_ctrl_v2_fetch_info(uhsd_s32 devHdl, uhsd_s32 sn, uhsd_group_v2_fetch_info_result_cb cb, void *cb_param);

// 组控V1 start

/**
 * @brief 组控V1,入组，出组，销毁组回调函数
 * @param sn 指令唯一标识
 * @param type 通知信息类型，
 * @param group_info 分组信息指针数组
 * @param group_info_num 分组信息中的分组个数
 * @param cb_param 回传参数
 * @return 成功返回0，失败返回<0
 */
typedef uhsd_s32 (
    *uhsd_dev_group_v1_notify_cb)(uhsd_s32 sn, uhsd_dev_group_v1_notify_type type, uhsd_char *group_id, uhsd_char *devid, uhsd_char *traceid, void *cb_param);

/**
 * @brief 注册组相关回调通知函数
 * @param cb 组相关请求的回调通知函数
 * @param param 回传参数
 * @return 成功返回0，失败返回<0
 */
UHSD_API uhsd_s32 uhsd_group_ctrl_v1_register_cb(uhsd_dev_group_v1_notify_cb cb, void *param);

/**
 * @brief 组控V1 notify应答
 * @param devid 设备id
 * @param sn 指令唯一标识，开发者调用读应答接口时原样传入
 * @param err_code 错误码
 * @return 成功返回0，失败返回<0
 */
UHSD_API uhsd_s32 uhsd_group_ctrl_v1_notify_resp(uhsd_char *devid, uhsd_s32 sn, uhsd_s32 err_code);

/**
 * @brief 组内的成员设备列表回调通知函数
 * @param group_devid 组设备唯一标识符
 * @param err_num 错误码
 * @param cloud_retCode 云端返回的原始错误值, 当err_num小于0时，可以辅助通过cloud_retCode进一步确定云端返回的错误原因。"1200012":"组设备已被删除不存在"
 * @param devid_list 组内的成员设备数组
 * @param len 组内的成员设备个数
 * @param param 回传参数
 * @return 成功返回0，失败返回<0
 */
typedef uhsd_s32 (*uhsd_group_v1_device_list_cb)(const uhsd_char *group_devid,
                                                 uhsd_s32 err_num,
                                                 uhsd_char *cloud_retCode,
                                                 uhsd_char *devid_list[],
                                                 uhsd_s32 len,
                                                 void *param);

/**
 * @brief 组控V1主动查询组信息
 * @param group_devid 组设备唯一标识符
 * @param cb 获取应答函数
 * @param cb_param 应答函数回传参数
 * @return 成功返回0，失败返回<0
 */
UHSD_API uhsd_s32 uhsd_group_ctrl_v1_fetch_info(const uhsd_char *group_devid, uhsd_group_v1_device_list_cb cb, void *cb_param);

//  组控V1 end

#ifdef __cplusplus
}
#endif

#endif /*__UHSD_DEV_H__*/

/**@} grp_uhsddev end*/
