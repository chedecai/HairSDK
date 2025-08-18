/**
 * @defgroup grp_uhoswifi WIFI适配接口定义
 * @{
 * @copyright Copyright (c) 2021, Haier.Co, Ltd.
 * @file uh_wifi.h
 * @author liujian (liujian@haier.com)
 * @brief
 * @date 2021-11-10
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>version <th>Author  <th>Description
 * <tr><td>2021-11-10   <td>1.0     <td>liujian <td>初版
 * </table>
 */

#ifndef __UH_WIFI_H__
#define __UH_WIFI_H__

#include "uh_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NEED_TO_OPTIMALIZE

/** @def WIFI_MODE_XXX
 *
 * @brief WIFI模式。
 * AP AP模式。
 * STA Station模式。
 * AP_STA AP+Station模式。
 *
 */
#define WIFI_MODE_AP     1
#define WIFI_MODE_STA    2
#define WIFI_MODE_AP_STA 3

/** @def WIFI_ENCRYPT_AUTH_XXX
 *
 * @brief WIFI鉴权模式。
 * NONE 开放。
 * WPA_PSK WPA PSK。
 * WPA2_PSK WPA2 PSK。
 * MASK 鉴权模式掩码。
 *
 */
#define WIFI_ENCRYPT_AUTH_NONE     0x0
#define WIFI_ENCRYPT_AUTH_WPA_PSK  0x1
#define WIFI_ENCRYPT_AUTH_WPA2_PSK 0x2
#define WIFI_ENCRYPT_AUTH_WPA3_SAE 0x4
#define WIFI_ENCRYPT_AUTH_MASK     0xFFFF

/** @def WIFI_ENCRYPT_CIPHER_XXX
 *
 * @brief WIFI加密方式。
 * NONE 不加密。
 * TKIP TKIP。
 * CCMP CCMP。
 * WEP WEP
 * MASK 加密方式掩码。
 *
 */
#define WIFI_ENCRYPT_CIPHER_NONE 0x00000
#define WIFI_ENCRYPT_CIPHER_TKIP 0x10000
#define WIFI_ENCRYPT_CIPHER_CCMP 0x20000
#define WIFI_ENCRYPT_CIPHER_WEP  0x40000
#define WIFI_ENCRYPT_CIPHER_MASK 0xFFFF0000

/** @def WIFI_DOWN/WIFI_UP
 *
 * @brief WIFI状态。
 * DOWN WIFI不可用。
 * UP WIFI可用。
 *
 */
#define WIFI_DOWN 0
#define WIFI_UP   1

/**
 * @brief WIFI功能打开。WIFI必须支持AP模式和STA模式。
 * @param wifi_mode WIFI工作模式，WIFI_MODE_XXX。
 * @return 成功返回0，失败返回-1。
 */
extern uhos_s32 uhos_wifi_power_on(uhos_u8 wifi_mode);

/**
 * @brief WIFI功能关闭。
 * @param mode 0 断开wifi 1 卸载wifi功能（BLE存在时不能设置1）
 * @return 成功返回0，失败返回-1。
 */
extern uhos_s32 uhos_wifi_power_off(uhos_u8 mode);

/**
 * @brief WIFI功能是否打开。
 * @param
 * @return 成功返回0，失败返回-1。
 */
extern uhos_s32 uhos_wifi_is_on(void);

/**
 * @brief 断开WIFI连接。
 * @details 如果是STA模式，取消当前正在进行的连接或者断开已经存在的连接；如果是AP模式，断开当前所有连接。断开WIFI连接后，不能自动再进行新的连接。
 * @return 成功返回0，失败返回-1。
 */
extern uhos_s32 uhos_wifi_disconnect(void);

/**
 * @brief 设置WIFI区域。不同区域支持的信道不同，需要根据区域设置所支持的信道。
 * @param country_code 区域码。可以设置的区域码包括：中国“CN”、北美“US”、欧洲“EU”和日本“JP”。
 * @return 成功返回0，失败返回-1。
 */
extern uhos_s32 uhos_wifi_country_set(uhos_char *country_code);

/**
 * @typedef promisc_frame_info_t
 * @brief 混杂模式下收到的报文附加信息。
 * frame_is_ok 是否有完整信息的报文，1表示正常的报文，0表示报文不可用，仅长度可用。
 * wifi_strength 接收信号强度，0-100，0xFF表示此参数无效。
 * rx_channel 接收的信道，0表示信道不可用。
 *
 */
typedef struct
{
    uhos_u8 frame_is_ok;
    uhos_u8 wifi_strength;
    uhos_u16 rx_channel;
} promisc_frame_info_t;

#pragma pack(1)
struct uhos_ieee80211_frame
{
    uhos_u8 i_fc[2];
    uhos_u8 i_dur[2];
    uhos_u8 i_addr1[6];
    uhos_u8 i_addr2[6];
    uhos_u8 i_addr3[6];
    uhos_u8 i_seq[2];
};
#pragma pack()

/**
 * @typedef promisc_enhance_info_t
 * @brief 锁定信道后，增加混杂模式下该目标AP收取转发报文能力。
 * ap_mac 目标AP。
 *
 */
typedef struct
{
    uhos_u8 ap_mac[6];
} promisc_enhance_info_t;

/**
 * @brief 锁定信道后，增加混杂模式下该目标AP收取转发报文能力。
 * @details WIFI需支持Promiscuous模式（在所设定的监听信道内）。在Promiscuous模式下，会根据目标AP信息，通过RSSI增加调节底层收包增益以提高收包效率。
 * @param enable 使能参数；开启设定为1；关闭设定为0。
 * @return 成功返回0，失败返回-1。
 */
uhos_s32 uhos_wifi_sniffer_enhance(uhos_s32 enable, promisc_enhance_info_t *info);

/**
 * @typedef promisc_cb_func
 * @brief 混杂模式下报文接收接口。
 * @param frame 802.11帧，包含帧头和帧体。
 * @param length 802.11帧长度。
 * @param info 报文附加信息。
 * @return N/A。
 */
typedef void (*promisc_cb_func)(uhos_u8 *frame, uhos_s32 length, promisc_frame_info_t *info);

/**
 * @brief 进入WIFI Promiscuous模式。
 * @details WIFI需支持Promiscuous模式（在所设定的监听信道内）。在Promiscuous模式下，需要将所有嗅探到的正确的WIFI数据包进行回调处理。
 * @param cb 混杂模式下报文接收接口。
 * @return 成功返回0，失败返回-1。
 */
extern uhos_s32 uhos_wifi_sniffer_start(promisc_cb_func cb);

/**
 * @brief 退出WIFI promiscuous模式。
 * @return 成功返回0，失败返回-1。
 */
extern uhos_s32 uhos_wifi_sniffer_stop(void);

/**
 * @brief 设置WIFI信道。
 * @details 当WIFI工作在AP模式时，设置AP的工作信道。当WIFI工作在Promiscuous模式时，设置sniffer的监听信道。其他模式，此设置不生效。
 * @param channel WIFI工作信道（2.4GHZ），1-14，最大信道与WIFI区域相关，可能是11、13或者14。
 * @return 成功返回0，失败返回-1。
 */
extern uhos_s32 uhos_wifi_channel_set(uhos_u16 channel);

/**
 * @brief 获取WIFI信道。
 * @details 获取信道信息。
 * @param channel 信道指针，返回参数。
 * @return 成功返回0，失败返回-1。
 */
extern uhos_s32 uhos_wifi_channel_get(uhos_u16 *channel);

/** @def WIFI_EXT_CHANNEL_XXX
 *
 * @brief 扩展信道。
 * NONE 不使用/不支持扩展信道。
 * ABOVE 当前信道与后一个信道合并成40M信道。
 * BELOW 当前信道与前一个信道合并成40M信道。
 * KEEP 保持不变。
 *
 */
#define WIFI_EXT_CHANNEL_NONE  0
#define WIFI_EXT_CHANNEL_ABOVE 1
#define WIFI_EXT_CHANNEL_BELOW 2
#define WIFI_EXT_CHANNEL_KEEP  3

/**
 * @brief 设置WIFI扩展信道。
 * @details 用于40MHz信道的设置。当WIFI工作在Promiscuous模式时，设置sniffer的监听信道。其他模式，此设置不生效。
 * @param channel WIFI工作信道（2.4GHZ），1-14，最大信道与WIFI区域相关，可能是11、13或者14。
 * @param ext_channel WIFI_EXT_CHANNEL_XXX。
 * @return 成功返回0，失败返回-1。
 */
extern uhos_s32 uhos_wifi_sniffer_channel_set(uhos_u16 channel, uhos_s32 ext_channel);

/**
 * @typedef wifi_scan_cb_func
 * @brief 扫描完成后的回调通知。
 * @param param 用户参数。
 * @return N/A。
 */
typedef void (*wifi_scan_cb_func)(void *param);

/**
 * @brief 开始扫描WIFI AP。
 * @param cb 扫描完成后的回调通知。可以设置为NULL。
 * @param param 回调接口的用户参数。
 * @return 成功返回0，失败返回-1。
 */
extern uhos_s32 uhos_wifi_scan_start(wifi_scan_cb_func cb, void *param);

/**
 * @brief 开始扫描指定SSID的WIFI AP。
 * @param ssid 待扫描的SSID。
 * @param ssid_len 待扫描的SSID长度。
 * @param cb 扫描完成后的回调通知。可以设置为NULL。
 * @param param 回调接口的用户参数。
 * @return 成功返回0，失败返回-1。
 */
extern uhos_s32 uhos_wifi_scan_with_ssid_start(const uhos_char *ssid, uhos_u16 ssid_len, wifi_scan_cb_func cb, void *param);

/**
 * @brief 停止扫描WIFI AP。
 * @return 成功返回0，失败返回-1。
 */
extern uhos_s32 uhos_wifi_scan_stop(void);

typedef struct
{
    uhos_char ssid[32 + 1];
    uhos_u8 ap_mac[6];
    uhos_s8 ap_power;
    uhos_u32 encryption;
    uhos_u16 channel;
} ap_list_api_t;

typedef struct uh_wifi_scan_item
{
    uhos_u8 ssid[32 + 1]; /*!<SSID。*/
    uhos_u16 ssid_len;    /*!<SSID长度。*/
    uhos_u8 bssid[6];     /*!<BSSID。*/
    uhos_u8 bssid_len;    /*!<BSSID长度。*/
    uhos_s32 rssi;        /*!<信号强度。*/
    uhos_u16 channel;     /*!<信道。*/
    uhos_s32 security;    /*!<安全信息。*/
} uh_wifi_scan_item_t;

/**
 * @typedef uh_wifi_scan_result_cb
 * @brief 扫描结果回调通知。
 * @param results 扫描结果。
 * @param len 扫描结果长度。
 * @param param 回调接口的用户参数。
 * @return N/A。
 */
typedef void (*uh_wifi_scan_results_cb)(uh_wifi_scan_item_t *results, int len, void *param);

/**
 * @brief 开始扫描指定SSID的WIFI AP。
 * @param ssid 待扫描的SSID，NULL表示通配。
 * @param ssid_len 待扫描的SSID长度。
 * @param channels 扫描的信道列表，以 0 结尾的信道数组，NULL表示所有支持的信道。
 * @param bssid 带扫描的设备BSSID，NULL表示不对bssid做过滤。
 * @param cb 扫描完成后的回调通知。可以设置为NULL。
 * @param param 回调接口的用户参数。
 * @return 成功返回0，失败返回-1。
 */
extern uhos_s32 uhos_wifi_scan_general(unsigned char *ssid,
                                       int ssid_len,
                                       unsigned short *channels,
                                       unsigned char *bssid,
                                       uh_wifi_scan_results_cb cb,
                                       void *param);

/**
 * @brief 查询WIFI AP列表。
 * @note 此接口是同步接口，自扫描AP开始需在10秒以内获得。扫描的AP列表需按照信号强度由高到低排序。
 * @param ap_number 作为输入，表示list能存储的最大AP数量；作为输出，表示实际扫描到的AP数量。
 * @param list，扫描到的AP列表。
 * @return 成功返回0，失败返回-1。
 */
extern uhos_s32 uhos_wifi_ap_list_get(uhos_u8 *ap_number, ap_list_api_t *list);

/**
 * @brief 使能WIFI省电模式。
 * @return 成功返回0，失败返回-1。
 */
extern uhos_s32 uhos_wifi_ps_enable(void);

/**
 * @brief 关闭WIFI省电模式。
 * @return 成功返回0，失败返回-1。
 */
extern uhos_s32 uhos_wifi_ps_disable(void);

/**
 * @brief 查询WIFI状态。
 * @param status WIFI状态WIFI_DOWN/WIFI_UP。
 * @param wifi_strength，当前所连接AP的信号强度，-96-0。如果status是WIFI_DOWN，则wifi_strength等于0。
 * @return 成功返回0，失败返回-1。
 */
UHSD_API extern uhos_s32 uhos_wifi_status_get(uhos_u8 *status, uhos_s8 *wifi_strength);

/**
 * @brief 获取WIFI信噪比。
 * @param snr，当前所连接AP的信噪比。
 * @return 成功返回0，失败返回-1。
 */
extern uhos_s32 uhos_wifi_snr_get(uhos_u8 *snr);

#define WIFI_DOWN_UNDEF             0
#define WIFI_DOWN_TIMEOUT           -1
#define WIFI_DOWN_CONNECTIVITY_FAIL -2
#define WIFI_DOWN_AP_PAPA_CHANGE    -3
#define WIFI_DOWN_AP_RESTART        -4
#define WIFI_DOWN_AP_DISCONNECT     -10
#define WIFI_DOWN_AP_DISCONNECT_MAX -(10 + 66) /*def in 802.11, total error code 66*/

/**
 * @brief WIFI状态变化通知。
 * @note 当WIFI状态发生变化（UP变化为DOWN或者DOWN变化为UP）时，通过此回调接口通知调用者。
 * @param status WIFI状态WIFI_DOWN/WIFI_UP。
 * @param extra_error  WIFI_FOWN时的附加错误码 WIFI_DOWN_XXX
 * @return 成功返回0，失败返回-1。
 */
typedef uhos_s32 (*uhos_wifi_status_callback)(uhos_u8 status, uhos_s32 extra_error);

/**
 * @brief 设置Wi-Fi连接状态变化通知回调。
 * @param callback 回调函数，设置为 NULL 时将清除回调。
 * @return 成功返回0，失败返回-1。
 */
UHSD_API extern uhos_s32 uhos_wifi_set_status_callback(uhos_wifi_status_callback callback);

/**
 * @brief 获取WIFI MAC。
 * @param mac 6字节MAC地址。
 * @return 成功返回0，失败返回-1。
 */
UHSD_API extern uhos_s32 uhos_wifi_mac_get(uhos_u8 *mac);

/** @def WIFI_NET_CFG_XXX
 *
 * @brief WIFI连接错误码。
 * OK 正确。
 * ERR_GENERAL 其他错误。
 * ERR_NONE_NETWORK 找不到网络（SSID）。
 * ERR_CONNECT_FAIL 连接失败。
 * AUTH_FAIL 密码错。
 * MAYBE_AUTH_FAIL 疑似密码错。
 *
 */
#define WIFI_NET_CFG_OK                  0
#define WIFI_NET_CFG_ERR_GENERAL         (-1)
#define WIFI_NET_CFG_ERR_NONE_NETWORK    (-2)
#define WIFI_NET_CFG_ERR_CONNECT_FAIL    (-3)
#define WIFI_NET_CFG_ERR_AUTH_FAIL       (-4)
#define WIFI_NET_CFG_ERR_MAYBE_AUTH_FAIL (-5)

/**
 * @brief 启动WIFI网络连接。
 * @note 此接口是同步接口，超时时间为15秒。
 * @param wifi_mode WIFI工作模式，WIFI_MODE_AP/WIFI_MODE_STA。
 * @param wifi_ssid SSID，最长32字节。
 * @param wifi_key passphrase，最长64字节。
 * @param channel AP信道。WIFI_MODE_STA模式可以为0，表示未指定信道。
 * @param encryption，加密方式。WIFI_ENCRYPT_AUTH_XXX和WIFI_ENCRYPT_CIPHER_XXX。
 * @return 成功返回0，失败返回错误码。
 */
extern uhos_s32 uhos_wifi_network_set(uhos_u8 wifi_mode, uhos_char *wifi_ssid, uhos_char *wifi_key, uhos_u16 channel, uhos_u32 encryption);

/**
 * @brief 查询WIFI网络信息。
 * @param wifi_mode WIFI工作模式，WIFI_MODE_STA。
 * @param wifi_ssid SSID，最长32字节。
 * @param wifi_key passphrase，最长64字节。
 * @param channel AP信道。WIFI_MODE_STA模式可以为0，表示未指定信道。
 * @param encryption，加密方式。WIFI_ENCRYPT_AUTH_XXX和WIFI_ENCRYPT_CIPHER_XXX。
 * @return 成功返回0，失败返回-1。
 */
extern uhos_s32 uhos_wifi_network_get(uhos_u8 wifi_mode, uhos_char *wifi_ssid, uhos_u16 *channel, uhos_u32 *encryption, uhos_u8 *bssid);

/** @def WIFI_CUSTOM_IE_TRANS_XXX
 *
 * @brief WIFI管理帧类型，按比特表示。
 * BEACON Beacon帧。
 * PROBE_REQ Probe Request帧。
 * PROBE_RSP Probe Response帧。
 *
 */
#define WIFI_CUSTOM_IE_TRANS_BEACON    1
#define WIFI_CUSTOM_IE_TRANS_PROBE_REQ 2
#define WIFI_CUSTOM_IE_TRANS_PROBE_RSP 4

#define WIFI_CUSTOM_IE_INFO_MAX_LEN (255)
typedef struct wifi_custom_ie
{
    uhos_u8 id;
    uhos_u8 len;
    uhos_u8 info[WIFI_CUSTOM_IE_INFO_MAX_LEN];
} wifi_custom_ie_t;

/**
 * @brief WIFI管理帧中增加自定义IE。
 * @param ie_info IE信息，802.11定义的IE TLV格式 wifi_custom_ie_t
 * @param trans_type 管理帧类型，按bit表示，WIFI_CUSTOM_IE_TRANS_XXX。
 * @return 成功返回索引，用于删除；失败返回-1。
 */
extern uhos_s32 uhos_wifi_custom_ie_add(uhos_u8 *ie_info, uhos_u16 trans_type);

/**
 * @brief WIFI管理帧中删除自定义IE。
 * @param index 增加自定义IE返回的索引。
 * @return 成功返回0，失败返回-1。
 */
extern uhos_s32 uhos_wifi_custom_ie_del(uhos_s32 index);

/**
 * @brief 获取BSSID。
 * @param bssid 如果已经连接到AP，返回AP的BSSID；否则返回全0。
 * @param bssid_len BSSID长度，当前固定返回6。
 * @return 成功返回0，失败返回-1。
 */
extern uhos_s32 uhos_wifi_get_bssid(uhos_u8 *bssid, uhos_u16 *bssid_len);

/**
 * @typedef wifi_mgmt_cb_func
 * @brief 收到802.11帧的回调通知。
 * @param frame 802.11帧，包含帧头和帧体。
 * @param length 802.11帧长度。
 * @param param 用户参数。
 * @return N/A。
 */
typedef void (*wifi_frame_cb_func)(uhos_u8 *frame, uhos_s32 length, void *param);

/**
 * @brief 设置802.11帧接收回调。
 * @details 802.11帧主要包括beacon帧和probe response帧。目前仅支持在STA模式下，接收所连接AP发出的帧。
 * @param enable 使能或者禁止接收802.11帧回调。
 * @param cb 收到帧的回调通知。设置为NULL表示不需要管理帧回调。
 * @param param 回调接口的用户参数。
 * @return 成功返回0，失败返回-1。
 */
extern uhos_s32 uhos_wifi_set_rx_frame(uhos_s32 enable, wifi_frame_cb_func cb, void *param);

/**
 * @brief 发送802.11帧。
 * @details 802.11帧主要包括probe request帧。
 * @param frame 802.11帧。
 * @param length 802.11帧长度。
 * @return 成功返回0，失败返回-1。
 */
extern uhos_s32 uhos_wifi_tx_frame(uhos_u8 *frame, uhos_s32 length);

typedef enum
{
    UHOS_WIFI_OPTIM_OFF = 0,
    /* take performance into account when optimization,
      connection with extremely low throughput is not acceptable. */
    UHOS_WIFI_OPTIM_THROUGHPUT_0,
    UHOS_WIFI_OPTIM_THROUGHPUT_1,
    UHOS_WIFI_OPTIM_THROUGHPUT_2,
    UHOS_WIFI_OPTIM_THROUGHPUT_3,
    /* try very hard to see if we can connect to the ap, let alone the performance. */
    UHOS_WIFI_OPTIM_TRY_HARD_0,
    UHOS_WIFI_OPTIM_TRY_HARD_1,
    UHOS_WIFI_OPTIM_TRY_HARD_2,
    UHOS_WIFI_OPTIM_TRY_HARD_3,
} uhos_wifi_optim_level_e;

/**
 * @brief 设置弱网优化级别；
 * @details 设置弱网优化级别；
 * @param level 设置的若绾优化级别；
 * @return 成功返回0，失败返回-1。
 */
extern uhos_s32 uhos_wifi_set_poor_signal_optim_level(uhos_wifi_optim_level_e level);

/**
 * @brief 获取弱网优化级别；
 * @details 获取当前设置的弱网优化级别；
 * @return 返回当前的弱网优化级别。
 */
extern uhos_wifi_optim_level_e uhos_wifi_get_poor_signal_optim_level(void);

/**
 * @brief 获取安全发射功率；
 * @details 获取模组的发射功率；
 * @param max_txpower 模组的安全发射功率，单位0.01dB；
 * @return 成功返回0，失败返回-1。
 */
uhos_s32 uhos_wifi_get_max_tx_power(uhos_s32 *max_txpower);

/**
 * @brief 设置基于校准功率的发射功率调整值；
 * @details 设置基于校准功率的发射功率调整值；
 * @param offset 基于校准功率的发射功率调整值，单位100dB；
 * @return 成功返回0，失败返回-1。
 */
extern uhos_s32 uhos_wifi_set_tx_power_offset(uhos_s32 offset);

#ifdef NEED_TO_OPTIMALIZE

/**
 * @brief 根据ssid获取该路由器的通道、加密方式和bssid
 * @param wifi_ssid 入参，路由器的ssid
 * @param channel scan后获取的该路由器通道
 * @param encryption scan后获取的该路由器加密方式
 * @param bssid scan后获取的该路由器的bssid
 * @return uhos_s32 成功返回0，失败返回-1。
 */
extern uhos_s32 uhos_wifi_get_network_and_bssid_check(uhos_char *wifi_ssid, uhos_u16 *channel, uhos_u32 *encryption, uhos_u8 *bssid);

/**
 * @brief       启动AP扫描
 * @param       channel_list 指定信道列表，NULL表示所有信道。
 * @param       len 信道列表数量
 * @return      uhos_s32 成功返回0，失败返回-1
 * @note        AP扫描结果通过uhos_wifi_get_scan获取。
 */
extern uhos_s32 uhos_wifi_set_scan(uhos_u16 *channel_list, uhos_u8 len);

/**
 * @brief      获取AP扫描结果。
 * @param      result AP扫描结果wifi_scan_item_t列表
 * @param      result_len AP扫描数量，输入输出，输入表示列表的最大数量，输出表示列表的实际数量
 * @return     uhos_s32 成功返回0，失败返回-1
 */
extern uhos_s32 uhos_wifi_get_scan(uh_wifi_scan_item_t *result, uhos_u16 *result_len);

/**
 * @brief      启动指定SSID的AP扫描。
 * @param      ssid SSID。
 * @param      ssid_len SSID长度。
 * @return     uhos_s32 成功返回0，失败返回-1
 * @note       AP扫描结果通过uhos_wifi_get_scan获取。。
 */
extern uhos_s32 uhos_wifi_set_scan_with_ssid(const uhos_u8 *ssid, uhos_u16 ssid_len);

/**
 * @brief      加密算法转换为加密方式
 * @param      security 加密算法：
 *             WIFI_WEP_ENABLED
 *             WIFI_TKIP_ENABLED
 *             WIFI_CCMP_ENABLED
 *             WIFI_SHARED_ENABLED
 *             WIFI_WPA_SECURITY
 *             WIFI_WPA2_SECURITY
 *             WIFI_WPS_ENABLED
 * @return     uhos_u32 加密方式
 *             WIFI_ENCRYPT_CIPHER_WEP
 *             WIFI_ENCRYPT_CIPHER_TKIP
 *             WIFI_ENCRYPT_CIPHER_CCMP
 *             WIFI_ENCRYPT_AUTH_WPA_PSK
 *             WIFI_ENCRYPT_AUTH_WPA2_PSK
 */
extern uhos_u32 uhos_wifi_security_to_encryption(uhos_u32 security);

/*
    double_freq for 5g
*/

typedef struct wifi_fuzzy_match
{
    uhos_char wifi_ssid[32 + 1];
    uhos_u16 channel;
    uhos_u32 encryption;

    uhos_u8 is_fuzzy_valid;
} wifi_fuzzy_match_t;

/**
 * @brief 如果开启模糊匹配功能获得模糊匹配的路由器信息，如果ssid有5G关键字获取相对应的非5G ssid
 * @param wifi_ssid 输入输出参数，如果有5G关键字返回非5G路由器ssid
 * @param channel 新获取的路由器通道
 * @param encryption 新获取的路由器加密方式
 * @param bssid 新获取路由器的bssid
 * @param enable_fuzzy_matching 是否开启模糊匹配 1 开启 0 不开启
 * @param fuzzy_match_wifi 出参，模糊匹配后的数据
 * @return uhos_s32
 */
extern uhos_s32 uhos_wifi_get_network_for_dual_freq(uhos_char *wifi_ssid,
                                                    uhos_u16 *channel,
                                                    uhos_u32 *encryption,
                                                    uhos_u8 *bssid,
                                                    uhos_u8 enable_fuzzy_matching,
                                                    wifi_fuzzy_match_t *fuzzy_match_wifi);

#endif

#ifdef __cplusplus
}
#endif

#endif // __UH_WIFI_H__
       /**@} grp_uhoswifi end */