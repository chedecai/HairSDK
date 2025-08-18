/**
 * @addtogroup grp_uhosos
 * @{
 * @copyright Copyright (c) 2021, Haier.Co, Ltd.
 * @file uh_thread.h
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

#ifndef __UH_THREAD_H__
#define __UH_THREAD_H__

#include "uh_types.h"
#include "uh_time.h"
#include "uh_mutex.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 用于标识一个线程。
 *
 */
struct uhos_thread_s;
typedef struct uhos_thread_s *uhos_thread_t;

/**
 * @brief 指定线程的属性。
 *
 */
typedef struct uhos_thread_attr
{
    uhos_u32 stack_size;
    uhos_u16 priority;
    uhos_char *name;
} uhos_thread_attr_t;

/**
 * @brief 线程创建.
 *
 * @param thread        供其他函数参考的线程ID
 * @param startroutine  线程函数
 * @param arg           作为启动传递给线程函数的指针
 * argument.
 * @param attr          线程属性; NULL: 默认值.
 * @return uhos_s32     0 成功
 *                      !0 失败
 */
uhos_s32 uhos_thread_create(uhos_thread_t *thread, void *(*startroutine)(void *), void *arg, const uhos_thread_attr_t *attr);

/**
 * @brief 终止线程的执行并将其从活动线程中删除
 *
 * @param thread 通过uhos_thread_create或uhos_thread_getid获取线程ID
 * @return uhos_s32     0 成功
 *                      !0 失败
 */
uhos_s32 uhos_thread_delete(uhos_thread_t thread);

/**
 * @brief 返回当前运行线程的线程ID
 *
 * @return uhos_thread_t 线程ID
 */
uhos_thread_t uhos_thread_getid(void);

/**
 * @brief 返回当前线程的tid
 * @return uhos_u64
 */
uhos_u64 uhos_thread_gettid(void);

/**
 * @brief 获取内核及其接口中可见的线程名称
 *
 * @param thread
 * @param[out] namebuf
 * @param[in] buflen
 * @return thread name
 */
const uhos_char *uhos_thread_get_name(uhos_thread_t thread);

/**
 * @brief 导致调用线程暂停执行，直到指定的实时秒数
 * @param milliseconds
 * @return uhos_s32     0 成功
 *                      !0 失败
 */
uhos_s32 uhos_thread_sleep(uhos_u32 milliseconds);

#ifdef __cplusplus
}
#endif

#endif // __UH_THREAD_H__
       /**@}*/