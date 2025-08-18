/**
 * @addtogroup grp_uhosos
 * @{
 * @copyright Copyright (c) 2021, Haier.Co, Ltd.
 * @file uh_semaphore.h
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
#ifndef __UH_SEMAPHORE_H__
#define __UH_SEMAPHORE_H__

#include "uh_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define UHOS_TEST_SEM         0
#define UHOS_SEM_WAIT_FOREVER 0xfffffffful
#define UHOS_SEM_WAIT_NONE    0u

#define UHOS_SEM_TIME_OUT 110

struct uhos_sem_s;
typedef struct uhos_sem_s *uhos_sem_t;

/**
 * @brief 创建并初始化用于管理资源的信号量对象
 * @param sem               信号量ID供其他函数参考
 * @param initial_count     可用标记的初始数量
 * @return uhos_s32         0 : 成功
 *                          !0: 失败
 */
uhos_s32 uhos_sem_create(uhos_sem_t *sem, uhos_u32 initial_count);

/**
 * @brief 删除由 uhos_sem_creat 创建的信号量
 *
 * @param sem
 * @return uhos_s32         0 : 成功
 *                          !0: 失败
 */
uhos_s32 uhos_sem_delete(uhos_sem_t sem);

/**
 * @brief 信号量对象的可用标记数
 * @param sem uhos_sem_creat 引用的信号量对象
 * @return uhos_u32 可用标记数量
 */
uhos_u32 uhos_sem_count(uhos_sem_t sem);

/**
 * @brief
 *
 * @param sem uhos_sem_creat 引用的信号量对象
 * @param millisec 超时值
 *                      or
 *                      0xffffffff 在没有超时的情况下
 *                      0 如果是非阻塞的情况
 * @return uhos_s32         0 : 成功
 *                          -1: 失败
 *                          110: 超时
 */
uhos_s32 uhos_sem_wait(uhos_sem_t sem, uhos_u32 millisec);

/**
 * @brief 释放信号量令牌 uhos_sem_wait
 *
 * @param sem uhos_sem_creat 引用的信号量对象
 * @return uhos_s32         0 : 成功
 *                          -1: 失败
 */
uhos_s32 uhos_sem_release(uhos_sem_t sem);

#ifdef __cplusplus
}
#endif

#endif // __UH_SEMAPHORE_H__
       /**@}*/