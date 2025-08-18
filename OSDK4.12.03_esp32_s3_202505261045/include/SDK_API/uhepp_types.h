/**
 *
 * @defgroup grp_uhepp_type E++软件数据类型
 * @{
 * @author  zhoudong@haier.com
 * @brief
 * @date 2021-09-27
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>version <th>Author  <th>Description
 * <tr><td>2022-08-24   <td>1.0     <td>zhoudong        <td>init version
 * </table>
 * @file uhepp_types.h
 * @copyright Copyright (c) 2021, Haier.Co, Ltd.
 */

#ifndef __UHEPP_TYPES_H__
#define __UHEPP_TYPES_H__

#include <stdint.h>

#ifdef __cplusplus
#define UHOS_NULL 0L
#else
#define UHOS_NULL ((void *)0)
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char uhepp_u8;
typedef signed char uhepp_s8;
typedef unsigned short uhepp_u16;
typedef signed short uhepp_s16;
typedef signed int uhepp_s32;
typedef unsigned int uhepp_u32;
typedef unsigned long long uhepp_u64;
typedef signed long long uhepp_s64;
typedef char uhepp_char;
typedef void uhepp_void;
typedef unsigned char uhepp_bool;
typedef float uhepp_float;
typedef double uhepp_double;
// typedef long int          uhepp_l32;
// typedef unsigned long      uhepp_ulong;
// typedef long               uhepp_long;
typedef intptr_t uhepp_intptr;
typedef uintptr_t uhepp_uintptr;

/**
 * @brief 应统一定义在uhos的type中去，这里只做宏定义的改名
 *
 */
typedef struct
{
    const uhepp_u8 *name;
    const uhepp_u8 *value;
} uhepp_pair_t;

#define UHEPP_API

#ifdef __cplusplus
}
#endif

#endif /*__UHEPP_TYPES_H__*/

/** @}*/
