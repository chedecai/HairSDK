/**
 * @addtogroup grp_uhosse
 * @{
 * @copyright Copyright (c) 2023, Haier.Co, Ltd.
 * @file uh_verify.h
 * @author liaoruxian@haier.com
 * @brief
 * @date 2023-03-08
 *
 * @par History:
 * <table>
 * <tr><th>Date       <th>version <th>Author     <th>Description
 * <tr><td>2023-03-08 <td>1.0     <td>liaoruxian <td>init version
 * </table>
 */
#ifndef __UH_VERIFY_H__
#define __UH_VERIFY_H__

#include "uh_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    UHOS_MD_NONE = 0,      /**< None. */
    UHOS_MD_MD2 = 1,       /**< The MD2 message digest. */
    UHOS_MD_MD4 = 2,       /**< The MD4 message digest. */
    UHOS_MD_MD5 = 3,       /**< The MD5 message digest. */
    UHOS_MD_SHA1 = 4,      /**< The SHA-1 message digest. */
    UHOS_MD_SHA224 = 5,    /**< The SHA-224 message digest. */
    UHOS_MD_SHA256 = 6,    /**< The SHA-256 message digest. */
    UHOS_MD_SHA384 = 7,    /**< The SHA-384 message digest. */
    UHOS_MD_SHA512 = 8,    /**< The SHA-512 message digest. */
    UHOS_MD_RIPEMD160 = 9, /**< The RIPEMD-160 message digest. */
} UHOS_MD_TYPE_E;

/**
 * @brief md初始化。
 * @details
 * @param md_type 算法类型。
 *                UHOS_MD_NONE,
 *                UHOS_MD_MD2,
 *                UHOS_MD_MD4,
 *                UHOS_MD_MD5,
 *                UHOS_MD_SHA1,
 *                UHOS_MD_SHA224,
 *                UHOS_MD_SHA256,
 *                UHOS_MD_SHA384,
 *                UHOS_MD_SHA512,
 *                UHOS_MD_RIPEMD160
 * @return 成功返回md句柄，失败返回NULL。
 */
UHSD_API uhos_void *uhos_md_init(UHOS_MD_TYPE_E md_type);
/**
 * @brief 根据算法类型加密数据段，数据可以分段式加密。
 * @details
 * @param md_ctx md句柄。
 * @param buf 加密数据段。
 * @param len 数据段长度。
 * @return 成功返回0，失败返回-1。
 */
UHSD_API uhos_void uhos_md_update(uhos_void *md_ctx, uhos_u8 *buf, uhos_size_t len);

/**
 * @brief 根据加密算法获取加密后的数据长度。
 * @details
 * @param md_type md加密类型。
 * @return 成功返回数据长度，失败返回-1。
 */
UHSD_API uhos_s32 uhos_md_get_size(UHOS_MD_TYPE_E md_type);

/**
 * @brief 释放md句柄并输出加密后的数据。
 * @details
 * @param md_ctx md句柄。
 * @param output 加密后的数据。
 * @return 无。
 */
UHSD_API uhos_void uhos_md_finish(uhos_void *md_ctx, uhos_u8 *output);

/**
 * @brief          计算通用HMAC
 * @details
 * @param md_type  Algorithm type (UHOS_MD_TYPE_E)
 *                 UHOS_MD_NONE,
 *                 UHOS_MD_MD2,
 *                 UHOS_MD_MD4,
 *                 UHOS_MD_MD5,
 *                 UHOS_MD_SHA1,
 *                 UHOS_MD_SHA224,
 *                 UHOS_MD_SHA256,
 *                 UHOS_MD_SHA384,
 *                 UHOS_MD_SHA512,
 *                 UHOS_MD_RIPEMD160。
 * \param key      HMAC secret key
 * \param keylen   HMAC key的字节长度
 * \param input    输入缓存指针
 * \param ilen     输入数据长度
 * \param output   Generic HMAC结果
 * \returns        0 成功, -0x5100 参数校验失败
 */
UHSD_API uhos_s32 uhos_md_hmac(UHOS_MD_TYPE_E md_type, const uhos_u8 *key, uhos_size_t keylen, const uhos_u8 *input, uhos_size_t ilen, uhos_u8 *output);

#ifdef __cplusplus
}
#endif

#endif /*__UH_VERIFY_H__*/

/**@}*/