/**
 * @defgroup grp_uhosos 操作系统适配接口定义
 * @{
 * @copyright Copyright (c) 2021, Haier.Co, Ltd.
 * @file uh_osal.h
 * @author your name (you@haier.com)
 * @brief
 * @date 2021-10-18
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>version <th>Author  <th>Description
 * <tr><td>2021-10-18   <td>1.0     <td>        <td>
 * </table>
 */

#ifndef __UH_OSAL_H__
#define __UH_OSAL_H__

#include "uh_mutex.h"
#include "uh_semaphore.h"
#include "uh_thread.h"
#include "uh_time.h"

#define ARCH_OS_PRIORITY_DEFAULT 			(-1)
#define ARCH_OS_NATIVE_PRIORITY_DEFAULT		(10)

#define ARCH_OS_WAIT_FOREVER				(0xFFFFFFFF)
#define ARCH_OS_NO_WAIT 					(0)
#define ARCH_OS_WAIT_MS2TICK(ms)			\
	( ((ms) == ARCH_OS_WAIT_FOREVER) ? ARCH_OS_WAIT_FOREVER : (((ms)/portTICK_PERIOD_MS)+((((ms)%portTICK_PERIOD_MS)+portTICK_PERIOD_MS-1)/portTICK_PERIOD_MS)) )

typedef void *								arch_os_semaphore_handle_t;
typedef void * 								arch_os_mutex_handle_t;
typedef void *								arch_os_mbox_handle_t;
typedef void * 								arch_os_thread_handle_t;
typedef void *								arch_os_queue_handle_t;
typedef void *		 						arch_os_timer_handle_t;
typedef void *								arch_os_function_return_t;
#define ARCH_OS_FUNCTION_RETURN(v)  		                     ((arch_os_function_return_t)(v))

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif // __UH_OSAL_H__
       /**@} grp_uhosos end */