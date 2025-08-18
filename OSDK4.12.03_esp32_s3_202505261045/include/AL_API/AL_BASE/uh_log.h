/**
 * @addtogroup grp_uhostypes
 * @{
 * @copyright Copyright (c) 2021, Haier.Co, Ltd.
 * @file uh_log.h
 * @author tangjinqi@haier.com
 * @brief  不需要适配，做为基础头文件方便第三方适配或者第三方业务组件直接使用
 * @date 2021-09-28
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>version <th>Author  <th>Description
 * <tr><td>2021-09-28   <td>1.0     <td>tangjinqi        <td>init version
 * </table>
 */

#ifndef __UHOS_LOG_H__
#define __UHOS_LOG_H__

#include "uh_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LOG_FILE ""

#define LOG_FUNCTION __FUNCTION__

#if 0
#if defined(__GNUC__) && ((__GNUC__ == 4 && __GNUC_MINOR__ >= 4) || __GNUC__ > 4)
#define uhos_printflike(fmt_pos, vars_pos) __attribute__((format(gnu_printf, fmt_pos, vars_pos)))
#elif defined(__GNUC__)
#define uhos_printflike(fmt_pos, vars_pos) __attribute__((format(printf, fmt_pos, vars_pos)))
#else
#define uhos_printflike(fmt_pos, vars_pos)
#endif
#else
#define uhos_printflike(fmt_pos, vars_pos)
#endif

/* output log's level */
#define LOG_LVL_ASSERT  0
#define LOG_LVL_ERROR   1
#define LOG_LVL_WARN    2
#define LOG_LVL_INFO    3
#define LOG_LVL_DEBUG   4
#define LOG_LVL_VERBOSE 5

/**
 * uhos_log init and deinit
 */

uhos_s32 uhos_log_init(void);
void uhos_log_deinit(void);

/**
 * output the log
 *
 * @param level level
 * @param tag tag
 * @param file file name
 * @param func function name
 * @param line line number
 * @param format output format
 * @param ... args
 *
 */

UHSD_API void uhos_log(uhos_u8 level, const uhos_char *tag, const uhos_char *file, const uhos_char *func, const uhos_u32 line, const uhos_char *format, ...)
    uhos_printflike(6, 7);

/**
 * dump the hex format data to log
 *
 * @param name name for hex object, it will show on log header
 * @param width hex number for every line, such as: 16, 32
 * @param buf hex buffer
 * @param size buffer size
 */

void uhos_log_hexdump(const uhos_char *name, uhos_u8 width, const uhos_void *buf, uhos_u16 size);

/**
 * output RAW format log
 *
 * @param format output format
 * @param ... args
 */
void uhos_log_raw(const uhos_char *format, ...) uhos_printflike(1, 2);

#if !defined(LOG_TAG)
#define LOG_TAG "NO_TAG"
#endif

#if !defined(CONFIG_LOG_LVL)
#define CONFIG_LOG_LVL LOG_LVL_VERBOSE
#endif

#if CONFIG_LOG_LVL >= LOG_LVL_ASSERT
#define UHOS_LOGA(...) uhos_log(LOG_LVL_ASSERT, LOG_TAG, LOG_FILE, LOG_FUNCTION, __LINE__, __VA_ARGS__)
#else
static inline void UHOS_LOGA(const char *fmt, ...)
{
}
#endif

#if CONFIG_LOG_LVL >= LOG_LVL_ERROR
#define UHOS_LOGE(...) uhos_log(LOG_LVL_ERROR, LOG_TAG, LOG_FILE, LOG_FUNCTION, __LINE__, __VA_ARGS__)
#else
static inline void UHOS_LOGE(const char *fmt, ...)
{
}
#endif

#if CONFIG_LOG_LVL >= LOG_LVL_WARN
#define UHOS_LOGW(...) uhos_log(LOG_LVL_WARN, LOG_TAG, LOG_FILE, LOG_FUNCTION, __LINE__, __VA_ARGS__)
#else
static inline void UHOS_LOGW(const char *fmt, ...)
{
}
#endif

#if CONFIG_LOG_LVL >= LOG_LVL_INFO
#define UHOS_LOGI(...) uhos_log(LOG_LVL_INFO, LOG_TAG, LOG_FILE, LOG_FUNCTION, __LINE__, __VA_ARGS__)
#else
static inline void UHOS_LOGI(const char *fmt, ...)
{
}
#endif

#if CONFIG_LOG_LVL >= LOG_LVL_DEBUG
#define UHOS_LOGD(...) uhos_log(LOG_LVL_DEBUG, LOG_TAG, LOG_FILE, LOG_FUNCTION, __LINE__, __VA_ARGS__)
#else
static inline void UHOS_LOGD(const char *fmt, ...)
{
}
#endif

#if CONFIG_LOG_LVL >= LOG_LVL_VERBOSE
#define UHOS_LOGV(...) uhos_log(LOG_LVL_VERBOSE, LOG_TAG, LOG_FILE, LOG_FUNCTION, __LINE__, __VA_ARGS__)
#else
static inline void UHOS_LOGV(const char *fmt, ...)
{
}
#endif

#if CONFIG_LOG_LVL >= LOG_LVL_DEBUG
#define UHOS_LOGHEX(name, buf, len) uhos_log_hexdump(name, 16, buf, len)
#else
static inline void UHOS_LOGHEX(const char *fmt, ...)
{
}
#endif

#if CONFIG_LOG_LVL >= LOG_LVL_DEBUG
#define UHOS_LOGRAW uhos_log_raw
#else
#define UHOS_LOGRAW(...) \
    do                   \
    {                    \
    } while (0)
#endif

/**
 *  standout output of fail
 *
 */

#define MALLOC_FAILED   "mal err"
#define COM_INIT_FAILED "ini err"

#define UHOS_LOG_MEM_ALLOC_FAIL() uhos_log(LOG_LVL_ERROR, LOG_TAG, LOG_FILE, LOG_FUNCTION, __LINE__, MALLOC_FAILED)
#define UHOS_LOG_COM_INIT_FAIL()  uhos_log(LOG_LVL_ERROR, LOG_TAG, LOG_FILE, LOG_FUNCTION, __LINE__, COM_INIT_FAILED)

/**
 * Set the log level of global .
 * @param log_level  the level of log
 * @return
 **/

void uhos_set_log_global_level(uhos_u32 log_level);

/**
 * Get the log level of global .
 * @param void
 * @return  log level
 **/

uhos_u32 uhos_get_log_global_level(void);

/**
 * Set the log level of tag.
 * @param tag        the tag of log
 * @param log_level  the level of log
 * @return  >=0 on success, negative error on failure.
 **/

void uhos_set_log_tag_level(const char *tag, uhos_u32 log_level);

/**
 * display the log of tag
 * @param tag        the tag of log
 * @return
 **/
void uhos_set_display_tag(const char *tag);

/**
 * @brief turn on the log
 */
void uhos_turn_on_log(void);

/**
 * @brief turn off the log
 */
void uhos_turn_off_log(void);

#ifdef CONFIG_LOG_TO_FILE
void uhos_enable_write_to_file(void);  // 使能写日志文件
void uhos_disable_write_to_file(void); // 关闭写日志文件, 同时会删除当前的所有日志文件
/**
 * Set the file size of log.
 * @param len  the lenth of file
 * @return
 **/
void uhos_set_log_file_size(uhos_size_t len);
void uhos_rotate_log_file(void); // 告知日志组件将当前正在写的文件close, 提供给uhos_pop_oldest_file获取

const char *uhos_pop_oldest_file(void); // 返回当前日志中最旧的日志文件, NULL表示没有; 注意pop之后日志模块就不再对此文件管理了
#endif

#ifdef __cplusplus
}
#endif

#endif /* __UHOS_LOG_H__ */
