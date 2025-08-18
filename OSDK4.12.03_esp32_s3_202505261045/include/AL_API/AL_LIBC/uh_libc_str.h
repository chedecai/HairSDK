/**
 * @addtogroup grp_uhoslibc
 * @{
 * @copyright Copyright (c) 2021, Haier.Co, Ltd.
 * @file uh_libc_str.h
 * @author wangshaolin (wangshaolin@haier.com)
 * @brief
 * @version 0.1
 * @date 2022-02-21
 *
 *
 */
#ifndef __UH_LIBC_STR_H__
#define __UH_LIBC_STR_H__

#include "uh_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 将字符串转为整数
 * @param nptr 字符串指针
 * @return uhos_s32  整数值
 */
UHSD_API uhos_s32 uhos_libc_atoi(const uhos_char *nptr);

/**
 * @brief 将字符串转为long long
 * @param nptr
 * @return uhos_s64
 */
UHSD_API uhos_s64 uhos_libc_atoll(const uhos_char *nptr);

/**
 * @brief 计算字符串长度
 *
 * @param s 字符串指针
 * @return uhos_size_t 字符串长度
 */
UHSD_API uhos_size_t uhos_libc_strlen(const uhos_char *s);

/**
 * @brief 在字符串中查找特定字符.
 * @param s 字符串指针
 * @param c 字符
 * @return uhos_char* 返回第一次匹配字符的指针，不匹配，返回空
 * 字符串结尾的空字符'\0'也视为 s 的一部分
 */
UHSD_API uhos_char *uhos_libc_strchr(const uhos_char *s, int c);

/**
 * @brief 在字符串中查找最后一次出现的特定字符
 * @param s 字符串指针
 * @param c 字符
 * @return uhos_char* 返回最后一次匹配字符的指针，不匹配，返回空
 * 字符串结尾的空字符'\0'也视为 s 的一部分
 */
UHSD_API uhos_char *uhos_libc_strrchr(const uhos_char *s, int c);

/**
 * @brief  在字符串中查找第一次出现特定字符串的位置，不包含终止符 '\0'
 *
 * @param haystack 要被检索的字符串
 * @param needle  要搜索的小字符串
 * @return uhos_char* 返回在 haystack 中第一次出现 needle 字符串的位置，如果未找到则返回空
 */
UHSD_API uhos_char *uhos_libc_strstr(const uhos_char *haystack, const uhos_char *needle);

/**
 * @brief 把 src 所指向的字符串复制到 dest
 * @param dest 指向用于存储复制内容的目标数组
 * @param src 要复制的字符串.
 * @return uhos_char* 返回一个指向最终的目标字符串 dest 的指针
 */
UHSD_API uhos_char *uhos_libc_strcpy(uhos_char *dest, const uhos_char *src);

/**
 * @brief 把 src 所指向的字符串复制到 dest，最多复制 n 个字符。当 src 的长度小于 n 时，dest 的剩余部分将用空字节填充
 * @param dest 指向用于存储复制内容的目标数组.
 * @param src 要复制的字符串.
 * @param n 要从源中复制的字符数
 * @return uhos_char* 返回最终复制的字符串
 */
UHSD_API uhos_char *uhos_libc_strncpy(uhos_char *dest, const uhos_char *src, uhos_size_t n);

/**
 * @brief 把 s1 所指向的字符串和 s2 所指向的字符串进行比较
 * @param s1 要进行比较的第一个字符串
 * @param s2 要进行比较的第二个字符串
 * @return uhos_s32*
 * 如果返回值小于 0，则表示 str1 小于 str2
 * 如果返回值大于 0，则表示 str1 大于 str2
 * 如果返回值等于 0，则表示 str1 等于 str2
 */
UHSD_API uhos_s32 uhos_libc_strcmp(const uhos_char *s1, const uhos_char *s2);
UHSD_API uhos_s32 uhos_libc_strncmp(const uhos_char *s1, const uhos_char *s2, uhos_size_t n);

/* 字符串比较函数, 和上面2个函数相比, 只是忽略大小写 */
UHSD_API uhos_s32 uhos_libc_strcasecmp(const uhos_char *s1, const uhos_char *s2);
UHSD_API uhos_s32 uhos_libc_strncasecmp(const uhos_char *s1, const uhos_char *s2, uhos_size_t n);

/**
 * @brief 把 src 所指向的字符串追加到 dest 所指向的字符串的结尾
 * @param dest 指向目标数组，该数组包含了一个 C 字符串，且足够容纳追加后的字符串
 * @param src 指向要追加的字符串，该字符串不会覆盖目标字符串
 * @return uhos_char* 返回一个指向最终的目标字符串 dest 的指针.
 */
UHSD_API uhos_char *uhos_libc_strcat(uhos_char *dest, const uhos_char *src);
UHSD_API uhos_char *uhos_libc_strncat(uhos_char *dest, const uhos_char *src, uhos_size_t n);

/**
 * @brief 分解字符串 str 为一组字符串，delim 为分隔符
 * @param str 要被分解成一组小字符串的字符串
 * @param delim 包含分隔符的 C 字符串
 * @return uhos_char* 返回被分解的第一个子字符串，如果没有可检索的字符串，则返回一个空指针.
 */
UHSD_API uhos_char *uhos_libc_strtok(uhos_char *str, const uhos_char *delim);
UHSD_API uhos_char *uhos_libc_strtok_r(uhos_char *str, const uhos_char *delim, uhos_char **saveptr);

/**
 * @brief 将可变参数按照format的格式转化为字符串
 * @param str 转换后字符串
 * @param size str最大长度
 * @param format 转换格式
 * @param ... 可变参数
 * @return uhos_s32
 */
UHSD_API uhos_s32 uhos_libc_snprintf(uhos_char *str, uhos_size_t size, const uhos_char *format, ...);

/**
 * @brief 发送格式化输出到 str 所指向的字符串
 * @param str 字符串
 * @param format 格式
 * @param ... 可变参数
 * @return uhos_s32
 */
UHSD_API uhos_s32 uhos_libc_sprintf(uhos_char *str, const uhos_char *format, ...);

/**
 * @brief 读取格式化的字符串中的数据
 * @param str 原始数据
 * @param format 格式
 * @param ... 可变参数
 * @return uhos_s32
 */
UHSD_API uhos_s32 uhos_libc_sscanf(const uhos_char *str, const uhos_char *format, ...);

#ifdef __cplusplus
}
#endif

#endif // __UH_LIBC_STR_H__

/**@}*/