/**
 * @addtogroup grp_uhosse
 * @{
 * @copyright Copyright (c) 2023, Haier.Co, Ltd.
 * @file uh_crypt.h
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
#ifndef __UH_CRYPT_H__
#define __UH_CRYPT_H__

#include "uh_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    UHOS_AES_DECRYPT = 0,
    UHOS_AES_ENCRYPT = 1,
} UHOS_AES_CRYPT_E;

/**
 * @brief aes初始化。
 * @details
 * @return 成功返回aes句柄，失败返回NULL。
 */
uhos_void *uhos_aes_init(uhos_void);

/**
 * @brief aes反初始化。
 * @details
 * @param ctx aes句柄。
 * @return 无。
 */
uhos_void uhos_aes_free(uhos_void *ctx);

/**
 * @brief 设置aes解密秘钥。
 * @details
 * @param ctx aes句柄。
 * @param key 秘钥buffer。
 * @param keybits 秘钥bit位数。
 * @return 成功返回0，失败返回-1。
 */
uhos_s32 uhos_aes_setkey_dec(uhos_void *ctx, const uhos_u8 *key, uhos_u32 keybits);

/**
 * @brief 设置aes加密秘钥。
 * @details
 * @param ctx aes句柄。
 * @param key 秘钥buffer。
 * @param keybits 秘钥bit位数。
 * @return 成功返回0，失败返回-1。
 */
uhos_s32 uhos_aes_setkey_enc(uhos_void *ctx, const uhos_u8 *key, uhos_u32 keybits);

/**
 * @brief aes加解密数据。
 * @details
 * @param ctx aes句柄。
 * @param mode 加解密模式。
 *             UHOS_AES_DECRYPT：解密模式。
 *             UHOS_AES_ENCRYPT：加密模式。
 * @param length 数据长度。
 * @param iv 初始化向量(使用后更新)。
 * @param input 加解密之前的数据。
 * @param output 加解密之后的数据。
 * @return 成功返回0，失败返回-1。
 */
uhos_s32 uhos_aes_crypt_cbc(uhos_void *ctx, UHOS_AES_CRYPT_E crypt_mode, uhos_size_t length, uhos_u8 iv[16], const uhos_u8 *input, uhos_u8 *output);

#ifdef __cplusplus
}
#endif

#endif /*__UH_CRYPT_H__*/
       /**@}*/