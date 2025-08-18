#ifndef __UHSD_QRCODE_BIND_H__
#define __UHSD_QRCODE_BIND_H__

#include "uhsd_types.h"

/**
 * 用户侧绑定中的扫码登录绑定
 * 扫码登陆绑定方式
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    UHSD_QRCODE_TYPE_LOGIN = 1, //仅登录
    UHSD_QRCODE_TYPE_BIND,      //仅绑定
    UHSD_QRCODE_TYPE_LOGIN_BIND //登录绑定
} uhsd_qrcode_type_t;

typedef enum
{
    QRCODE_STATE_DEFAULT = 0,   //无效状态
    QRCODE_STATE_CREATED = 1,   //二维码创建
    QRCODE_STATE_COMPLETED = 2, //二维码已、登录或绑定 或登录并绑定
    QRCODE_STATE_INVALID = 3,   //已失效
    QRCODE_STATE_SCANNED = 4,   //已扫码
} uhsd_qrcode_status_e;

typedef struct
{
    uhsd_char appId[40 + 1];         /* 应用Id，海极网申请 */
    uhsd_char clientId[32 + 1];      /* 客户端唯一标识 uhome_client_id 客户端标识，用于唯一标识客户端，目前为网关设备mac地址*/
    uhsd_char uc_client_id[64 + 1];  /* user_center_client_id 用户中心客户端标识，用户中心颁发*/
    uhsd_char client_secret[64 + 1]; /* 客户端秘钥 */
    uhsd_char devid[33];
    uhsd_char typeid[64 + 1];
} uhsd_qrcode_param_t;

typedef struct
{
    uhsd_s32 bind_result; // 绑定成功：1，其他值绑定失败
    uhsd_char *access_token;
    uhsd_char *refresh_token;
    uhsd_u32 as_expires_time; // assess_token 超时时间（utc秒数）
    uhsd_char *user_info;     // json字符串
} uhsd_qrcode_bind_login_info_t;
/**
 * @brief 二维码获取通知回调，在用户调用 获取或刷新二维码接口 后触发
 * @param deviceid 设备唯一标识
 * @param err_num 错误码，成功为0，失败为<0
 * @param qrcode_img 二维码数据，err_num非0时为NULL
 * @param qrcode_token 回传参数
 * @return 成功返回0，失败返回<0
 */
typedef uhsd_s32 (*uhsd_qrcode_data_cb)(const uhsd_char *deviceId, uhsd_s32 err_num, const uhsd_char *qrcode_img, const uhsd_char *qrcode_token);

/**
 * @brief 使用二维码登录结果回调定义
 * @param deviceid 设备唯一标识
 * @param err_num  错误吗，成功为0，失败为非0 二维码超时 -100
 * @param QRstatus 二维码的状态，
 * @param info 二维码登录绑定信息，只有QRstatus = QRCODE_COMPLETED是，该参数才有效
 */
typedef uhsd_s32 (*uhsd_qrcode_login_result_cb)(uhsd_char *deviceId, uhsd_s32 err_num, uhsd_qrcode_status_e QRstatus, uhsd_qrcode_bind_login_info_t *info);

/**
 * @brief 请求二维码、登录、绑定结果回调集合
 *
 */
typedef struct
{
    uhsd_qrcode_data_cb qrcode_data_cb;                  //获取二维码通知回调
    uhsd_qrcode_login_result_cb qrcode_result_notify_cb; //二维码登录绑定通知回调
} uhsd_qrcode_result_cbs_t;

/**
 * @brief 扫码登录功能相关参数初始化，包括用户参数、结果回调
 *
 * @param params 用户参数
 * @param cbs   登录、绑定结果回调
 * @return uhsd_s32 0 成功，非0 失败
 */
UHSD_API uhsd_s32 uhsd_qrcode_init(uhsd_qrcode_param_t *param, uhsd_qrcode_result_cbs_t *cbs);

/**
 * @brief 扫码登录绑定功能反初始化
 *
 * @return uhsd_s32 0 成功，非0 失败
 */
UHSD_API uhsd_s32 uhsd_qrcode_uninit(uhsd_void);

/**
 * @brief 请求获取二维码
 * @param dev_handle 主设备handle
 * @param type    类型：1 登录，2 绑定，3 登录和绑定
 * @return uhsd_s32 0 成功，非0 失败
 */
UHSD_API uhsd_s32 uhsd_qrcode_fetch(uhsd_s32 dev_handle, uhsd_qrcode_type_t type);

/**
 * @brief 更新token结果通知回调定义
 * @param deviceid 设备id
 * @param err_num  0：更新成功，1：无需更新，负值：更新失败
 * @param refresh_token 刷新token
 * @param access_token  access token
 * @param access_token_expire access token 超时时间
 * @param user_id 用户id
 * @param cb_param 回传参数
 *
 */
typedef uhsd_s32 (*uhsd_qrcode_update_token_cb)(const uhsd_char *deviceId,
                                                uhsd_s32 err_num,
                                                const uhsd_char *refresh_token,
                                                const uhsd_char *access_token,
                                                uhsd_u32 access_token_expire,
                                                uhsd_char *user_id,
                                                uhsd_void *cb_param);

/**
 * @brief 请求更新token
 *
 * @param dev_handle 主设备handle
 * @param refresh_token 刷新token
 * @param cb 回调
 * @param cb_param 回调参数
 * @return uhsd_s32 0 成功；<0 失败
 */
UHSD_API uhsd_s32 uhsd_qrcode_update_token(uhsd_s32 dev_handle, const uhsd_char *refresh_token, uhsd_qrcode_update_token_cb token_cb, uhsd_void *cb_param);

#ifdef __cplusplus
}
#endif
#endif
