/**
 * @addtogroup grp_uhosse
 * @{
 * @copyright Copyright (c) 2023, Haier.Co, Ltd.
 * @file uh_tls.h
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
#ifndef __UH_TLS_H__
#define __UH_TLS_H__

#include "uh_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    UHOS_TLS_RET_OK = 0,
    UHOS_TLS_RET_ERROR = -1,
    UHOS_TLS_RET_WANT_READ = -2,
    UHOS_TLS_RET_WANT_WRITE = -3,
} UHOS_TLS_RET_E;

typedef enum
{
    UHOS_TLS_RSA_WITH_AES_128_CBC_SHA256,
    UHOS_TLS_RSA_WITH_AES_256_CBC_SHA256,
    UHOS_TLS_DHE_RSA_WITH_AES_256_CBC_SHA256,
    UHOS_TLS_RSA_WITH_AES_256_GCM_SHA384,
    UHOS_TLS_DHE_RSA_WITH_AES_256_GCM_SHA384,
    UHOS_TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384,
    UHOS_TLS_ECDH_ECDSA_WITH_AES_256_CBC_SHA384,
    UHOS_TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA384,
    UHOS_TLS_ECDH_RSA_WITH_AES_256_CBC_SHA384,
    UHOS_TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384,
    UHOS_TLS_ECDH_ECDSA_WITH_AES_256_GCM_SHA384,
    UHOS_TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384,
    UHOS_TLS_ECDH_RSA_WITH_AES_256_GCM_SHA384,
} UHOS_TLS_CIPHERSUITES_E;

/**
 * @brief tls初始化。
 * @details 初始化成功后应用方拿到一个句柄，设置、握手、收发数据都使用该句柄。
 * @return 成功返回tls句柄，失败返回NULL。
 */
uhos_void *uhos_tls_init(uhos_void);

/**
 * @brief tls反初始化，释放tls所有资源。
 * @details
 * @param handle tls句柄。
 * @return 成功返回0，失败返回-1。
 */
uhos_s32 uhos_tls_uninit(uhos_void *handle);

/**
 * @brief tls句柄开始适配资源。
 * @details 调用该函数之后，tls的配置才开始生效，set接口只是传入设置值。
 * @param handle tls句柄。
 * @return 成功返回0，失败返回-1。
 */
uhos_s32 uhos_tls_start(uhos_void *handle);

/**
 * @brief tls握手。
 * @details 由于网络等原因，可能需要多次握手。
 * 握手期间返回UHOS_TLS_RET_WANT_READ或UHOS_TLS_RET_WANT_WRITE，并非握手失败，只是网络io没有数据可读或可写，需要再次握手。
 * @param handle tls句柄。
 * @return 成功返回0，失败返回-1，继续握手返回UHOS_TLS_RET_WANT_READ或UHOS_TLS_RET_WANT_WRITE。
 */
uhos_s32 uhos_tls_handshake(uhos_void *handle);

/**
 * @brief 发送tls数据。
 * @details 期间返回UHOS_TLS_RET_WANT_READ或UHOS_TLS_RET_WANT_WRITE，只是网络io没有数据可读或可写，需要再次发送。
 * @param handle tls句柄。
 * @param data 数据buffer。
 * @param data_len 数据长度。
 * @return 成功返回>0，失败返回-1，继续发送返回UHOS_TLS_RET_WANT_READ或UHOS_TLS_RET_WANT_WRITE。
 */
uhos_s32 uhos_tls_send(uhos_void *handle, const uhos_u8 *data, uhos_size_t data_len);

/**
 * @brief 接收tls数据。
 * @details 期间返回UHOS_TLS_RET_WANT_READ或UHOS_TLS_RET_WANT_WRITE，只是网络io没有数据可读或可写，需要再次接收。
 * @param handle tls句柄。
 * @param data 数据buffer。
 * @param data_len 数据长度。
 * @return 成功返回>0，失败返回-1，连接断开返回0，继续发送返回UHOS_TLS_RET_WANT_READ或UHOS_TLS_RET_WANT_WRITE。
 */
uhos_s32 uhos_tls_recv(uhos_void *handle, uhos_u8 *data, uhos_size_t data_len);

/**
 * @brief 获取应用方未读取的数据长度。
 * @details 每次读数据tls库会读取特定长度的数据，但是如果应用方读数据小于该长度，可通过该接口获取未读数据长度。
 * @param handle tls句柄。
 * @return 未读取的数据长度。
 */
uhos_s32 uhos_tls_get_avail_bytes(uhos_void *tls_handle);

/**
 * @brief 设置已连接的套接字。
 * @details tls库不会直接连接服务器，需要应用方连接服务器成功后设置fd给tls库用于数据收发。
 * @param handle tls句柄。
 * @param socket_fd 已连接的套接字。
 * @return 成功返回0，失败返回-1。
 */
uhos_s32 uhos_tls_set_connected_socket(uhos_void *handle, uhos_s32 socket_fd);

/**
 * @brief 设置加密套件。
 * @details
 * @param handle tls句柄。
 * @param ciphersuite 加密套件数组，设置值请见UHOS_TLS_CIPHERSUITES_E。
 * @param ciphersuite_count 加密套件个数，数组长度。
 * @return 成功返回0，失败返回-1。
 */
uhos_s32 uhos_tls_set_ciphersuites(uhos_void *handle, uhos_s32 *ciphersuites, uhos_u32 ciphersuites_count);

/**
 * @brief 设置服务器ca证书。
 * @details 如果是多个证书，将多个证书放到一个buffer里面。
 * @param handle tls句柄。
 * @param ca_cert ca证书buffer。
 * @param ca_cert_len ca证书buffer长度。
 * @return 成功返回0，失败返回-1。
 */
uhos_s32 uhos_tls_set_ca_cert(uhos_void *handle, const uhos_u8 *ca_cert, uhos_size_t ca_cert_len);

/**
 * @brief 设置设备证书和私钥。
 * @details 用于双向认证，服务器校验客户端身份。
 * @param handle tls句柄。
 * @param own_cert 客户端证书buffer。
 * @param own_cert_len 客户端证书长度。
 * @param priv_key 客户端私钥buffer。
 * @param priv_key_len 客户端私钥长度。
 * @return 成功返回0，失败返回-1。
 */
uhos_s32 uhos_tls_set_own_cert(uhos_void *handle, const uhos_u8 *own_cert, uhos_size_t own_cert_len, const uhos_u8 *priv_key, uhos_size_t priv_key_len);

/**
 * @brief 获取证书cn。
 * @details
 * @param cert_buf 证书buffer。
 * @param cert_buf_len 证书长度。
 * @param cert_cn_buf 证书cn buffer。
 * @param cert_cn_buf_len 证书cn buffer长度。
 * @return 成功返回0，失败返回-1。
 */
uhos_s32 uhos_tls_get_cert_cn(uhos_u8 *cert_buf, uhos_size_t cert_buf_len, uhos_u8 *cert_cn_buf, uhos_size_t cert_cn_buf_len);
#ifdef __cplusplus
}
#endif

#endif /*__UH_TLS_H__*/
       /**@}*/