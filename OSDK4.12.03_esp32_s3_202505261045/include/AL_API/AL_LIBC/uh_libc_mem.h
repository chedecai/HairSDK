/**
 * @addtogroup grp_uhoslibc
 * @{
 * @copyright Copyright (c) 2021, Haier.Co, Ltd.
 * @file uh_libc_mem.h
 * @author wangshaolin (wangshaolin@haier.com)
 * @brief
 * @version 0.1
 * @date 2022-02-21
 *
 *
 */
#ifndef __UH_LIBC_MEM_H__
#define __UH_LIBC_MEM_H__

#include "uh_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _UHOS_DEBUG_MEM_ // DEBUG_MEM模式, SDK内部调试时使用, 南向接口在适配实现时不必关心

/**
 * @brief 分配一块size字节大小的内存
 *
 * @param size 分配内存块的大小（字节）
 * @return 成功，返回分配的内存块指针
 *         失败，返回空指针
 */
uhos_void *uhos_libc_malloc(uhos_size_t size);

/**
 * @brief 释放分配的内存块
 *
 * @param ptr 指向分配内存块的指针
 *
 */
UHSD_API uhos_void uhos_libc_free(uhos_void *ptr);
/**
 * @brief 分配一块size字节大小的内存，并清零
 *
 * @param size 分配内存块的大小（字节）
 * @return 成功，返回分配的内存块指针
 *         失败，返回空指针
 */
UHSD_API uhos_void *uhos_libc_malloc(uhos_size_t size);
UHSD_API uhos_void *uhos_libc_zalloc(uhos_size_t size);
UHSD_API uhos_void *uhos_libc_calloc(uhos_size_t nmemb, uhos_size_t size);
UHSD_API uhos_void *uhos_libc_realloc(uhos_void *ptr, uhos_size_t size);

/**
 * @brief  从存储区 src 复制 n 个字节到存储区 dest.
 *
 * @param dest 指向用于存储复制内容的目标数组，类型强制转换为 void* 指针
 * @param src 指向要复制的数据源，类型强制转换为 void* 指针
 * @param n 要被复制的字节数
 * @return uhos_void* 返回一个指向目标存储区 dest 的指针
 */
UHSD_API uhos_void *uhos_libc_memcpy(uhos_void *dest, const void *src, uhos_size_t n);

/**
 * @brief 复制字符 c（一个无符号字符）到参数 s 所指向的字符串的前 n 个字符
 *
 * @param s 指向要填充的内存块
 * @param c 要被设置的值
 * @param n 要被设置为该值的字符数
 * @return uhos_void* 返回一个指向存储区s的指针
 */
UHSD_API uhos_void *uhos_libc_memset(uhos_void *s, uhos_s32 c, uhos_size_t n);

/**
 * @brief 从存储区 s2 复制 n 个字节到存储区 s1
 *
 * @param s1 指向用于存储复制内容的目标数组，类型强制转换为 void* 指针
 * @param s2 指向要复制的数据源，类型强制转换为 void* 指针
 * @param n 要被复制的字节数
 * @return uhos_s32 返回一个指向目标存储区 s1 的指针
 *
 */
UHSD_API uhos_s32 uhos_libc_memcmp(const uhos_void *s1, const uhos_void *s2, uhos_size_t n);

/**
 * @brief 从 str2 复制 n 个字符到 str1，但是在重叠内存块这方面，memmove() 是比 memcpy() 更安全的方法。区域可以有重叠的
 *
 * @param dest 指向用于存储复制内容的目标数组，类型强制转换为 void* 指针
 * @param src 指向要复制的数据源，类型强制转换为 void* 指针
 * @param n 要被复制的字节数
 * @return uhos_void* 返回一个指向目标存储区 dest 的指针
 */
UHSD_API uhos_void *uhos_libc_memmove(uhos_void *dest, const void *src, uhos_size_t n);

#else
#include "uh_mem.h"
#endif

#ifdef __cplusplus
}
#endif

#endif // __UH_LIBC_MEM_H__
       /**@}*/