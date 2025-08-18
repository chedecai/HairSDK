/**
 * @addtogroup grp_uhosos
 * @{
 * @copyright Copyright (c) 2021, Haier.Co, Ltd.
 * @file uh_time.h
 * @author shiqingpeng (shiqingpeng@haier.com)
 * @brief
 * @date 2021-10-18
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>version <th>Author  <th>Description
 * <tr><td>2021-10-18   <td>1.0     <td>        <td>
 * <tr><td>2021-10-18   <td>1.1     <td>chowhan <td>timer接口移动到uh_os_timer.h
 * </table>
 */
#ifndef __UH_TIME_H__
#define __UH_TIME_H__

#include "uh_types.h"
//#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

#if 0
typedef uhos_u32 uhos_time_t;
#else
typedef uhos_u64 uhos_time_t;
#endif

struct uhos_tm
{
    uhos_s32 tm_sec;   /* 秒，范围从 0 到 59 */
    uhos_s32 tm_min;   /* 分，范围从 0 到 59 */
    uhos_s32 tm_hour;  /* 小时，范围从 0 到 23 */
    uhos_s32 tm_mday;  /* 一月中的第几天，范围从 1 到 31 */
    uhos_s32 tm_mon;   /* 月，范围从 0 到 11(注意) */
    uhos_s32 tm_year;  /* 自 1900 年起的年数 */
    uhos_s32 tm_wday;  /* 一周中的第几天，范围从 0 到 6 */
    uhos_s32 tm_yday;  /* 一年中的第几天，范围从 0 到 365 */
    uhos_s32 tm_isdst; /* 夏令时 */
};

struct uhos_timespec
{
    uhos_s32 tv_sec;  /* seconds */
    uhos_s32 tv_nsec; /* nanoseconds */
};

struct uhos_timeval
{
    uhos_s32 tv_sec;  /* seconds */
    uhos_s32 tv_usec; /* and microseconds */
};

struct uhos_timezone
{
    uhos_s32 tz_minuteswest; /* (minutes west of Greenwich) */
    uhos_s32 tz_dsttime;     /* (type of DST correction)*/
};

#if defined(__IAR_SYSTEMS_ICC__)
typedef void *timer_t;

struct itimerspec
{
    struct uhos_timespec it_interval; //首次超时后，每隔it_interval超时一次
    struct uhos_timespec it_value;    //首次超时时间
};
#endif

/*time def*/
#define TIME_MILLISECOND (1)
#define TIME_SECOND      (1000 * (TIME_MILLISECOND))
#define TIME_MINUTE      (60 * (TIME_SECOND))
#define TIME_HOUR        (60 * (TIME_MINUTE))
#define TIME_DAY         (24 * (TIME_HOUR))

#define MILLISECONDS_PER_SECOND (1000L)
#define MICROSECONDS_PER_SECOND (1000000LL)    /**< Microseconds per second. */
#define NANOSECONDS_PER_SECOND  (1000000000LL) /**< Nanoseconds per second. */

/** @def TIME_NO_WAIT/TIME_WAIT_FOREVER
 *
 * @brief 获取资源时的等待操作，用于信号量和队列操作。
 * TIME_NO_WAIT 如果资源不能获得，不等待。
 * TIME_WAIT_FOREVER 一直等待，直到获取到资源。
 *
 */
#define TIME_NO_WAIT      0
#define TIME_WAIT_FOREVER (-1)

#define TIME_MAX_PROCESS_ONCE (1000)

uhos_time_t uhos_time(uhos_time_t *in_time);

struct uhos_tm *uhos_gmtime(const uhos_time_t *in_time);

struct uhos_tm *uhos_localtime(const uhos_time_t *in_time);

uhos_s32 uhos_gettimeofday(struct uhos_timeval *tv, struct uhos_timezone *tz);

/**
 * @brief 当前硬件启动后的计数毫秒数， 此数值在启动后持续增长，不受授时等操作影响
 * @return 自启动的毫秒数
 */
uhos_u32 uhos_current_time_get(void);

uhos_s32 uhos_real_time_get(struct uhos_timespec *real_time);

#ifdef __cplusplus
}
#endif

#endif // __UH_TIME_H__
       /**@}*/