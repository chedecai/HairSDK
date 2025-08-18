/**
 * @addtogroup grp_uhosos
 * @{
 * @copyright Copyright (c) 2021, Haier.Co, Ltd.
 * @file uh_mutex.h
 * @author shiqingpeng (shiqingpeng@haier.com)
 * @brief
 * @date 2021-10-18
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>version <th>Author  <th>Description
 * <tr><td>2021-10-18   <td>1.0     <td>        <td>
 * </table>
 */
#ifndef __UH_MUTEX_H__
#define __UH_MUTEX_H__

#include "uh_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define UHOS_MUTEX_WAIT_FOREVER 0xfffffffful
#define UHOS_MUTEX_WAIT_NONE    0u

struct uhos_mutex_s;
typedef struct uhos_mutex_s *uhos_mutex_t;

/**
 * @brief 创建并初始化互斥对象
 *
 * @param [out] mutex 供其他函数参考的互斥 ID
 * @return uhos_s32     0 成功
 *                      !0 失败
 */
uhos_s32 uhos_mutex_create(uhos_mutex_t *mutex);

/**
 * @brief 删除由 uhos_mutex_creat 创建的互斥体
 *
 * @param [in] mutex uhos_mutex_creat获得的互斥体ID
 * @return uhos_s32     0 成功
 *                      !0 失败
 */
uhos_s32 uhos_mutex_delete(uhos_mutex_t mutex);

/**
 * @brief 等待直到互斥体变得可用。
 *
 * @param mutex     uhos_mutex_creat获得的互斥体ID.
 * @param millisec  超时值
 *                  or
 *                      UHOS_MUTEX_WAIT_FOREVER 没有超时的情况
 *                      UHOS_MUTEX_WAIT_NONE 非阻塞情况
 * @return uhos_s32     0 成功
 *                      !0 失败
 */
uhos_s32 uhos_mutex_wait(uhos_mutex_t mutex, uhos_u32 millisec);

/**
 * @brief 释放由 uhos_mutex_wait 获得的互斥体
 *
 * @param mutex uhos_mutex_creat获得的互斥体ID.
 * @return uhos_s32     0 成功
 *                      !0 失败
 */
uhos_s32 uhos_mutex_release(uhos_mutex_t mutex);
#ifdef __cplusplus
}
#endif

#endif // __UH_MUTEX_H__
       /**@}*/