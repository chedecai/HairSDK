#include "freertos/FreeRTOS.h"//must ahead of timers.h
#include "freertos/timers.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/portmacro.h"
#include "freertos/portable.h"
#include "freertos/FreeRTOSConfig.h"
#include "esp_err.h"

#include "uh_osal.h"
#include "uh_log.h"

#include "time.h"


#undef CARELINE_LOG_TAG
#define CARELINE_LOG_TAG			"esp32_os"

#define ARCH_OS_MS2TICK(ms)      ((ms)/portTICK_PERIOD_MS)
#define ARCH_OS_TICK2MS(tick)    ((tick)*portTICK_PERIOD_MS)

#define ARCH_OS_TIMER_OPT_BLOCK_MS_MAX		10*1000

#define portEND_SWITCHING_ISR( xSwitchRequired ) 	\
	do{	if( xSwitchRequired == pdTRUE) 	portYIELD_FROM_ISR();}while(0)

#define portIsInIsr()            xPortInIsrContext()

static portMUX_TYPE port_mux = portMUX_INITIALIZER_UNLOCKED;  //自旋锁

uhos_time_t uhos_time(uhos_time_t *in_time)
{
    return time(in_time);
}

struct uhos_tm *uhos_gmtime(const uhos_time_t *in_time)
{
    return gmtime(time);
}

struct uhos_tm *uhos_localtime(const uhos_time_t *in_time)
{
    return localtime(time);
}

extern uhos_s32 uhos_get_system_time(struct uhos_timespec *tp);
uhos_s32 uhos_gettimeofday(struct uhos_timeval *tv, struct uhos_timezone *tz)
{
    struct uhos_timespec ts;

    if (tv)
    {
        uhos_get_system_time(&ts);
    
        // 将秒数和微秒数分别存储在tv结构体中
        tv->tv_sec = ts.tv_sec;
        tv->tv_usec = ts.tv_nsec / 1000;
        return 0;
    }
    else 
    {
        return (-1);
    }
}

/*
	tick机制：
	通过tick_high 记录溢出
*/
static uhos_u32 tick_high = 0;			//tick高位 添加高位用于扩展计数范围
static bool tick_half_flag = false;		//tick值过半标记 超过0x7FFFFFFF 便设置为1

uhos_u32 arch_os_tick_now( void )
{
	uhos_u32 tick = xTaskGetTickCount();

	if(tick > 0x7FFFFFFF)
		tick_half_flag = true;	//过半标记

	else if(true == tick_half_flag){	//若溢出 还没有标记
		tick_half_flag = false;
		tick_high++;
	}

	return tick;
}

uhos_u32 arch_os_tick_elapsed(uhos_u32 last_tick)
{
	uhos_u32 now = arch_os_tick_now();
	
	if(last_tick  < now)
		return now - last_tick;
	else//可能溢出
		return 0xFFFFFFFF - last_tick + now + 1;
}

/**
 * @brief 当前硬件启动后的计数毫秒数， 此数值在启动后持续增长，不受授时等操作影响
 * @return 自启动的毫秒数
 */
uhos_u32 uhos_current_time_get(void)
{
    return (ARCH_OS_TICK2MS(arch_os_tick_now()));
}

uhos_u32 uhos_ms_elapsed(uhos_u32 last_ms)
{
    return (ARCH_OS_TICK2MS(arch_os_tick_elapsed(ARCH_OS_MS2TICK(last_ms))));
}

uhos_s32 uhos_real_time_get(struct uhos_timespec *real_time)
{

}

/****************OS-THREAD*********************/

/**
 * @brief 线程创建,指定核心ID
 *
 * @param thread        供其他函数参考的线程ID
 * @param startroutine  线程函数
 * @param arg           作为启动传递给线程函数的指针
 * argument.
 * @param attr          线程属性; NULL: 默认值.
 * @return uhos_s32     0 成功
 *                      !0 失败
 */
uhos_s32 uhos_thread_create_coreID(uhos_thread_t *thread, void *(*startroutine)(void *), void *arg, const uhos_thread_attr_t *attr, int xCoreID)
{
    const static uhos_thread_attr_t default_attr = {
        .stack_size = 2048,
        .priority = 7,
        .name = "",
    };
    
    const uhos_thread_attr_t* inner_attr = UHOS_NULL;
    if( attr != UHOS_NULL ) {
        inner_attr = attr;
    }
    else {
        inner_attr = &default_attr;
    }
    uhos_char* pname = UHOS_NULL;
    if( inner_attr->name == UHOS_NULL )
    {
        pname = "";
    }
    else {
        pname = inner_attr->name;
    }

    BaseType_t result = xTaskCreatePinnedToCore((pdTASK_CODE)startroutine, (const char* const)pname, inner_attr->stack_size / sizeof(portSTACK_TYPE), arg, inner_attr->priority, (TaskHandle_t *)thread, xCoreID);
    if (result != pdPASS)
    {
        return UHOS_FAILURE;
    }

    return UHOS_SUCCESS;
}

/**
 * @brief 线程创建
 *
 * @param thread        供其他函数参考的线程ID
 * @param startroutine  线程函数
 * @param arg           作为启动传递给线程函数的指针
 * argument.
 * @param attr          线程属性; NULL: 默认值.
 * @return uhos_s32     0 成功
 *                      !0 失败
 */
uhos_s32 uhos_thread_create(uhos_thread_t *thread, void *(*startroutine)(void *), void *arg, const uhos_thread_attr_t *attr)
{
    const static uhos_thread_attr_t default_attr = {
        .stack_size = 2048,
        .priority = 7,
        .name = "",
    };
    
    const uhos_thread_attr_t* inner_attr = UHOS_NULL;
    if( attr != UHOS_NULL ) {
        inner_attr = attr;
    }
    else {
        inner_attr = &default_attr;
    }
    uhos_char* pname = UHOS_NULL;
    if( inner_attr->name == UHOS_NULL )
    {
        pname = "";
    }
    else {
        pname = inner_attr->name;
    }

    BaseType_t result = xTaskCreate((pdTASK_CODE)startroutine, (const char* const)pname, inner_attr->stack_size / sizeof(portSTACK_TYPE), arg, inner_attr->priority, (TaskHandle_t *)thread);
    if (result != pdPASS)
    {
        return UHOS_FAILURE;
    }

    return UHOS_SUCCESS;
}

/**
 * @brief 终止线程的执行并将其从活动线程中删除
 *
 * @param thread 通过uhos_thread_create或uhos_thread_getid获取线程ID
 * @return uhos_s32     0 成功
 *                      !0 失败
 */
uhos_s32 uhos_thread_delete(uhos_thread_t thread)
{
    vTaskDelete(thread);
    return UHOS_SUCCESS;
}

/**
 * @brief 返回当前运行线程的线程ID
 *
 * @return uhos_thread_t 线程ID
 */
uhos_thread_t uhos_thread_getid(void)
{
    return xTaskGetCurrentTaskHandle();
}

/**
 * @brief 返回当前线程的tid
 * @return uhos_u64
 */
uhos_u64 uhos_thread_gettid(void)
{

}

/**
 * @brief 获取内核及其接口中可见的线程名称
 *
 * @param thread
 * @param[out] namebuf
 * @param[in] buflen
 * @return thread name
 */
const uhos_char *uhos_thread_get_name(uhos_thread_t thread)
{
    return pcTaskGetTaskName(thread);
}

/**
 * @brief 导致调用线程暂停执行，直到指定的实时秒数
 * @param milliseconds
 * @return uhos_s32     0 成功
 *                      !0 失败
 */
uhos_s32 uhos_thread_sleep(uhos_u32 milliseconds)
{
    vTaskDelay( ARCH_OS_WAIT_MS2TICK(ms) );

    return UHOS_SUCCESS;
}

/****************OS-SEMAPHORE*********************/

#define UHOS_SEM_VALUE_MAX  0x7fffffff
/**
 * @brief 创建并初始化用于管理资源的信号量对象(二值信号量)
 * @param sem               信号量ID供其他函数参考
 * @return uhos_s32         0 : 成功
 *                          !0: 失败
 */
uhos_s32 uhos_sem_create_binary(uhos_sem_t *sem)
{
	*sem = xSemaphoreCreateMutex();
	if (*sem) {
		return UHOS_SUCCESS;
	}
	else {
		return UHOS_FAILURE;
	}
}

/**
 * @brief 创建并初始化用于管理资源的信号量对象
 * @param sem               信号量ID供其他函数参考
 * @param initial_count     可用标记的初始数量
 * @return uhos_s32         0 : 成功
 *                          !0: 失败
 */
uhos_s32 uhos_sem_create(uhos_sem_t *sem, uhos_u32 initial_count)
{
    *sem = xSemaphoreCreateCounting(UHOS_SEM_VALUE_MAX, initial_count);
	if (*sem) {
		return UHOS_SUCCESS;
	}
	else {
		return UHOS_FAILURE;
	}
}

/**
 * @brief 删除由 uhos_sem_creat 创建的信号量
 *
 * @param sem
 * @return uhos_s32         0 : 成功
 *                          !0: 失败
 */
uhos_s32 uhos_sem_delete(uhos_sem_t sem)
{
    vSemaphoreDelete(sem);
	return UHOS_SUCCESS;
}

/**
 * @brief 信号量对象的可用标记数
 * @param sem uhos_sem_creat 引用的信号量对象
 * @return uhos_u32 可用标记数量
 */
uhos_u32 uhos_sem_count(uhos_sem_t sem)
{
    return uxQueueMessagesWaiting(sem);  
}

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
uhos_s32 uhos_sem_wait(uhos_sem_t sem, uhos_u32 millisec)
{
	int ret;
	if (portIsInIsr()) {
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		ret = xSemaphoreTakeFromISR(sem, &xHigherPriorityTaskWoken);
		portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
	}
	else {
		ret = xSemaphoreTake(sem, ARCH_OS_WAIT_MS2TICK(millisec));
	}

	return ret == pdTRUE ? UHOS_SUCCESS : UHOS_FAILURE;
}

/**
 * @brief 释放信号量令牌 uhos_sem_wait
 *
 * @param sem uhos_sem_creat 引用的信号量对象
 * @return uhos_s32         0 : 成功
 *                          -1: 失败
 */
uhos_s32 uhos_sem_release(uhos_sem_t sem)
{
	int ret;
	if (portIsInIsr()) {
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		ret = xSemaphoreGiveFromISR(sem, &xHigherPriorityTaskWoken);
		portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
	}
	else {
		ret = xSemaphoreGive(sem);
	}

	return ret == pdTRUE ? UHOS_SUCCESS : UHOS_FAILURE;   
}


/****************OS-MUTEX*********************/

/**
 * @brief 创建并初始化互斥对象
 *
 * @param [out] mutex 供其他函数参考的互斥 ID
 * @return uhos_s32     0 成功
 *                      !0 失败
 */
uhos_s32 uhos_mutex_create(uhos_mutex_t *mutex)
{
	*mutex = xSemaphoreCreateRecursiveMutex();
	if(*mutex){
		return UHOS_SUCCESS;
	}
	else{
		return UHOS_FAILURE;
	}
}

/**
 * @brief 删除由 uhos_mutex_creat 创建的互斥体
 *
 * @param [in] mutex uhos_mutex_creat获得的互斥体ID
 * @return uhos_s32     0 成功
 *                      !0 失败
 */
uhos_s32 uhos_mutex_delete(uhos_mutex_t mutex)
{
	vSemaphoreDelete(mutex);

	return UHOS_SUCCESS;
}

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
uhos_s32 uhos_mutex_wait(uhos_mutex_t mutex, uhos_u32 millisec)
{
	int ret;
	if (portIsInIsr()) {
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		ret = xSemaphoreTakeFromISR(mutex, &xHigherPriorityTaskWoken);
		portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
	}
	else {
		ret = xSemaphoreTakeRecursive(mutex, ARCH_OS_WAIT_MS2TICK(millisec));
	}

	return ret == pdTRUE ? UHOS_SUCCESS : UHOS_FAILURE;
}

/**
 * @brief 释放由 uhos_mutex_wait 获得的互斥体
 *
 * @param mutex uhos_mutex_creat获得的互斥体ID.
 * @return uhos_s32     0 成功
 *                      !0 失败
 */
uhos_s32 uhos_mutex_release(uhos_mutex_t mutex)
{
	int ret;
	if (portIsInIsr()) {
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		ret = xSemaphoreGiveFromISR(mutex, &xHigherPriorityTaskWoken);
		portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
	}
	else {
		ret = xSemaphoreGiveRecursive(mutex);
	}

	return ret == pdTRUE ? UHOS_SUCCESS : UHOS_FAILURE;
}