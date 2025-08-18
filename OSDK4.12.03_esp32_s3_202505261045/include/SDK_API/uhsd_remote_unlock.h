#ifndef __UHSD_DEMO_ADD_DEV_H__
#define __UHSD_DEMO_ADD_DEV_H__

/**
 * @brief: 生成一次性临时、周期密码。
 * @param {uhsd_devHandle} devHandle 用来获取设备deviceId
 * @param {uhsd_char} *challengeCode 门锁生成的challenge码，ASCII码，字符集[0-9a-f]。
 * @param {uhsd_u32}  ts 时间戳：UTC时间，int，1970年1月1日至今的秒数（零时区时间）
 * @param {uhsd_char} *passwd1 当前localkey生成的验证码 长度至少17 最后一位为0
 * @param {uhsd_char} *passwd2 历史localkey生成的验证码 长度至少17 最后一位为0
 * @return {int} <0:校验无效
 * @note: 该接口为同步接口。
 */
uhsd_s32 uhsd_dev_create_temp_passwd(uhsd_devHandle devHandle, uhsd_char *challengeCode, uhsd_u32 ts, uhsd_char *passwd1, uhsd_char *passwd2);

/**
 * @brief: 校验设备临时验证码是否有效。
 * @param {uhsd_devHandle} devHandle 用来获取设备deviceId
 * @param {uhsd_char} *verificationCode 云端计算生成的远程开锁校验码, ASCII码，字符集[0-9a-f]
 * @param {uhsd_char} *challengeCode 门锁生成的challenge码，ASCII码，字符集[0-9a-f]。
 * @param {uhsd_u32} timestamp 时间戳：UTC时间，int，1970年1月1日至今的秒数（零时区时间）
 * @return {int} 0:校验合法。<0:校验无效。
 * @note: 该接口为同步接口。
 */
uhsd_s32 uhsd_dev_check_verification_code(uhsd_devHandle devHandle, const uhsd_char *verificationCode, const uhsd_char *challengeCode, uhsd_u32 timestamp);

#endif