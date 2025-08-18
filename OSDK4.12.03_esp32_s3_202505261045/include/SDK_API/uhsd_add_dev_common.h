/**
 * @brief 1. 添加设备公共接口头文件，在调用设备添加接口前调用本文件的接口；
 *        2. 用于设置设备信息
 */

#ifndef __UHSD_ADD_DEV_COMMON_H__
#define __UHSD_ADD_DEV_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "uhsd_types.h"

/**
 * @brief 创建设备句柄，该设备句柄全局唯一
 *
 * @param[in] role 设备角色 @ref uhsd_dev_role_t
 * @return 成功返回非负整数的设备句柄,失败返回<0
 *
 * @note 创建的第一个设备句柄为主设备句柄，主设备句柄不可释放
 */
UHSD_API uhsd_devHandle uhsd_dev_new_devHandle(uhsd_dev_role_t role);

/**
 * @brief 创建设备句柄扩展版
 *
 * @param[in] role 设备角色 @ref uhsd_dev_role_t
 * @param[in] typeId 设备类型标识符，海极网创建设备模型时生成，
 * @param[in] devKey 设备KEY，海极网创建设备模型时生成
 * @param[in] deviceId 设备唯一标识，
 *                     当role是组设备时，值为组设备ID;
 *                     当role不是组设备 且 设备是存量设备，值为存量设备的devid；
 *                     当role不是组设备 且 设备不是存量设备，此值必须传入NULL;
 * 
 * @return 成功返回非负整数的设备句柄,失败返回<0
 *
 * @note  1. 此接口用于多设备接入的场景，如果只通过OSDK接入一个设备，建议使用接口uhsd_dev_new_devHandle();
 *        2. 此接口与uhsd_dev_new_devHandle()互斥，只能使用其中一个；
 *        3. 后续在调用添加module接口时，必须调用reg_factor结构体中成员变量modue_type对应的相应module类型接口及传入相同的module_id值;
 *        4. 此接口调用后，不能在单独调用set接口设置typeId、devKey、deviceId、sub_id信息,否则调用后也是无效调用;
 * 
 *        5. 创建hdl失败常见case：
 *           1) 参数非法
 *           2) 设备句柄数量超过限制
 * 
 */

UHSD_API uhsd_devHandle uhsd_dev_new_devHandle_ext(uhsd_dev_role_t role, uhsd_char *deviceId, uhsd_dev_reg_factor_t *reg_factor);

/**
 * @brief 释放设备句柄【已废弃】
 *
 * @param[in] devHandle 设备句柄
 * @return 成功返回0，失败返回<0
 */
UHSD_API uhsd_s32 uhsd_dev_free(uhsd_devHandle devHandle);

/**
 * @brief 设置主设备mac
 *
 * @param[in] devHandle 设备句柄
 * @param[in] devMac 设备mac，字符串
 * @return 成功返回0，失败返回<0
 */
UHSD_API uhsd_s32 uhsd_dev_set_devHandle_dMac(uhsd_devHandle devHandle, const uhsd_char *devMac);

/**
 * @brief 设置设备类型标识符
 *
 * @param[in] devHandle 设备句柄
 * @param[in] typeId 设备类型标识符,海极网创建设备模型时生成
 * @return 成功返回0，失败返回<0
 */
UHSD_API uhsd_s32 uhsd_dev_set_devHandle_typeId(uhsd_devHandle devHandle, const uhsd_char *typeId);

/**
 * @brief 设置设备KEY
 *
 * @param[in] devHandle 设备句柄
 * @param[in] devKey 设备KEY
 * @return 成功返回0，失败返回<0
 */
UHSD_API uhsd_s32 uhsd_dev_set_devHandle_devKey(uhsd_devHandle devHandle, const uhsd_char *devKey);

/**
 * @brief 设置设备subId信息
 *
 * @param[in] devHandle 设备句柄
 * @param[in] subId 设备标号
 * @return 成功返回0，失败返回<0
 *
 * @note 附件设备必须调用此接口，非附件设备不能调用此接口
 */
UHSD_API uhsd_s32 uhsd_dev_set_devHandle_subId(uhsd_devHandle devHandle, const uhsd_char *subId);

/**
 * @brief 设置设备完整成品编码，非产品短编码; 注：upCodeT(U+ product Code type)
 *
 * @param[in] devHandle 设备句柄
 * @param[in] upCodeT 产品编码,海极网创建设备模型时生成
 * @return 成功返回0，失败返回<0
 */
UHSD_API uhsd_s32 uhsd_dev_set_devHandle_upCodeT(uhsd_devHandle devHandle, const uhsd_char *upCodeT);

/**
 * @brief 设置设备名称【非必调接口】
 *
 * @param[in] devHandle 设备句柄
 * @param[in] dName 设备名称
 * @return 成功返回0，失败返回<0
 */
UHSD_API uhsd_s32 uhsd_dev_set_devHandle_dName(uhsd_devHandle devHandle, const uhsd_char *dName);

/**
 * @brief 设置设备证书数据【非必调接口】
 * @param[in] devHandle 设备句柄
 * @param[in] cert_data 海尔设备证书数据
 * @param[in] cert_data_len 设备证书数据长度
 * @return 成功返回0，失败返回<0
 *
 * @note 仅双向认证的设备调用该接口;
 */
UHSD_API uhsd_s32 uhsd_dev_set_devHandle_cert_data(uhsd_devHandle devHandle, const uhsd_u8 *cert_data, uhsd_u32 cert_data_len);

/**
 * @brief 设置主设备的shortId，用于softap创建热点名
 * @param[in] devHandle 主设备句柄
 * @param[in] shortId 设备型号短Id,海极网自动生成，长度固定为4, 取值范围[0-9]/[A-Z]/[a-z]
 * @return 成功返回0，失败返回<0
 */
UHSD_API uhsd_s32 uhsd_dev_set_devHandle_shortId(uhsd_devHandle devHandle, const uhsd_char *shortId);

/**
 * @brief 设置组控能力
 *
 * @param devHandle 设备句柄【仅主设备调用】
 * @param groupCtrl 设备组控能力[0/1/2], 0:不具备组控能力。1:主设备(网关设备)具备组控能力,此时其下的从设备也会自动具备组控能力
 * @return 成功返回0，失败返回<0
 */
UHSD_API uhsd_s32 uhsd_dev_set_devHandle_groupCtrl(uhsd_devHandle devHandle, const uhsd_u8 groupCtrl);

/**
 * @brief 整机升级信息设置
 *
 * @param[in] devHandle 设备句柄
 * @param[in] upFwType 整机固件类型
 * @param[in] upVer 整机版本
 * @return 成功返回0，失败返回<0
 */
UHSD_API uhsd_s32 uhsd_dev_set_devHandle_fotaInfo(uhsd_devHandle devHandle, const uhsd_char *upFwType, const uhsd_char *upVer);

/**
 * @brief 设置父设备句柄
 *
 * @param[in] devHandle 设备句柄
 * @param[in] parent_devHandle 父设备句柄
 * @return 成功返回0，失败返回<0
 *
 * @note 当设备角色为子设备/附件设备时，需要调用该接口
 */
UHSD_API uhsd_s32 uhsd_dev_set_devHandle_parent(uhsd_devHandle devHandle, uhsd_devHandle parent_devHandle);

/**
 * @brief 设置子设备的通信协议(附件设备和主设备不需要调用)
 *
 * @param[in] devHandle 子设备句柄
 * @param[in] protocol 子设备通信协议，具体参见uhsd_dev_protocol_t
 * @return 成功返回0，失败返回<0
 */
UHSD_API uhsd_s32 uhsd_dev_set_devHandle_protocol(uhsd_devHandle devHandle, uhsd_dev_protocol_t protocol);

/**
 * @brief 设置存量设备deviceId
 *
 * @param[out] deviceId 设备ID,
 * @return 成功返回0，失败返回<0
 *
 * @note 当注册设备时不想改变已有的设备devid时，可以调用此接口，调用后，注册devid时会原样返回此devid
 */
UHSD_API uhsd_s32 uhsd_dev_set_devHandle_deviceId(uhsd_devHandle devHandle, uhsd_char *deviceId);

/**
 * @brief 设置设备是否支持休眠和远程唤醒能力【非必调接口】
 * @param[in] devHandle 设备句柄
 * @param[in] sleepable 是否为可休眠设备，0:不是；1:是
 * @param[in] remoteWakeupable 是否支持远程唤醒，0:不支持；1:支持
 * @return 成功返回0，失败返回<0
 *
 * @note: 支持低功耗休眠的设备才需要调用该接口；不支持低功耗休眠的设备无需调用
 */
UHSD_API uhsd_s32 uhsd_dev_set_devHandle_sleep_remoteWakeup_ability(uhsd_devHandle devHandle, uhsd_s32 sleepable, uhsd_s32 remoteWakeupable);

/**
 * @brief 设置设备上线/启动原因【非必调接口】
 * @param[in] devHandle 设备句柄
 * @param[in] online_reason 上线原因，0:上电启动；1:设备远程唤醒；2:设备本机唤醒
 * @return 成功返回0，失败返回<0
 *
 * @note: 支持低功耗休眠的设备需要调用该接口；不支持低功耗休眠的设备无需调用
 */
UHSD_API uhsd_s32 uhsd_dev_set_devHandle_online_reason(uhsd_devHandle devHandle, uhsd_s32 online_reason);

/**
 * @brief 注册设备deviceId模式
 * @param mode 0：默认模式，优先从本地缓存文件获取; 1:强制直从云端获取，该模式相对耗时;
 * @return 成功返回0，失败返回<0
 *
 * @note  1. 此接口设置的注册模式全局有效，即只需要调用一次就会对后续的所有设备注册均有效，所以只需要在注册第一个设备前调用一次即可；
 *        2. 此接口可以重复调用，即mode设置为1后，也可以在后续某个时刻在设置为mode为0，反之也是如此；
 */
UHSD_API uhsd_s32 uhsd_dev_set_devHandle_dev_reg_mode(uhsd_s32 mode);

/**
 * @brief 设置子设备配对时间戳
 * @param devHandle 设备信息句柄
 * @param pair_time_ms 配对时间戳
 * @return 成功返回0，失败返回<0
 */
UHSD_API uhsd_s32 uhsd_dev_set_devHandle_pairTime(uhsd_devHandle devHandle, uhsd_u64 pair_time_ms);

/*
 * @brief 设定设备楼层、房间类型、房间名等信息
 * @param devHandle 	设备信息句柄
 * @param floor		楼层信息 负三层,负二层,负一层,一层,二层,三层,四层,五层,六层,七层,八层,九层,十层
 * @param roomType 	房间类型 客厅,卧室,厨房,卫生间,阳台,儿童房,老人房,休息室,书房,娱乐室,活动室,衣帽间,玄关,走廊,车库,花园,餐厅
 * @param room 		房间名称 仅支持6字以内的中英数字组合
 * @return 成功返回0，失败返回<0 参数非法
 */
UHSD_API uhsd_s32 uhsd_dev_set_devHandle_roomInfo(uhsd_devHandle devHandle, const char *floor, const char *roomType, const char *room);

/*
 * @brief 设置设备房间信息上报云端失效，调用此接口uhsd_dev_info_set_roomInfo设置的设备信息在云端将失效
 * @param devHandle 	设备信息句柄
 * @return 成功返回0，失败返回<0 参数非法
 */
UHSD_API uhsd_s32 uhsd_dev_set_devHandle_rptDevInfoDisable(uhsd_devHandle devHandle);

/**
 * @brief 设置设备热点名称前缀
 *
 * @param[in] devHandle 设备句柄
 * @param[in] hotspotName 热点名称, 注意：
 *            1. 热点名前缀为海极网上定义的功能集中的热点名称，格式为"U-XXX",
 *            OSDK会在hotspotName后面拼接其他关键信息后，形成最终搜索发现的
 *            完整热点名称。
 *            2. 特别注意，该字段值需要携带"U-",例如"U-AC"。
 *            3. 长度范围为3~8， 格式固定为"U-XXX",
 *               其中"XXX"为边长字符串，长度范围为1~6，可取字符集[A-F]
 *
 * @return 成功返回0，失败返回<0
 *
 * @note
 * 1. 支持(softAP || BLE配网) && 主设备 必须调用该API，否则softAP配网功能不可用。
 * 2. 不支持(softAP || BLE配网) || 非主设备 不需要调用该API。
 *
 */
UHSD_API uhsd_s32 uhsd_dev_set_devHandle_hotspotName(uhsd_devHandle devHandle, const uhsd_char *hotspotName);

/**
 * @brief 获取设备deviceId
 *
 * @param[out] deviceId 设备ID, 内存区域需要devApp分配和释放
 * @return 成功返回0，失败返回<0
 */
UHSD_API uhsd_s32 uhsd_dev_get_deviceId(uhsd_devHandle devHandle, uhsd_char deviceId[33]);

/**
 * @brief 创建voicemodule模块句柄
 *
 * @param[in] mac 模块硬件标识
 * @param[in] hwType 硬件类型
 * @param[in] hwVers 硬件版本
 * @param[in] swType 软件类型
 * @param[in] swVers 软件版本
 * @param[in] upgrade 是否支持模块升级, 0:不支持，1:支持
 * @return 成功返回非负整数的模块句柄,失败返回<0
 */
UHSD_API uhsd_moduleHandle uhsd_dev_new_voice_module(const uhsd_char *mac,
                                                     const uhsd_char *hwType,
                                                     const uhsd_char *hwVers,
                                                     const uhsd_char *swType,
                                                     const uhsd_char *swVers,
                                                     uhsd_s32 upgrade);

/**
 * @brief 创建blemesh模块句柄【暂不支持】
 *
 * @param[in] mac 模块硬件标识
 * @param[in] hwType 硬件类型
 * @param[in] swVers 软件版本
 * @return 成功返回非负整数的模块句柄,失败返回<0
 */
UHSD_API uhsd_moduleHandle uhsd_dev_new_blemesh_module(const uhsd_char *mac, const uhsd_char *hwType, const uhsd_char *swVers);

/**
 * @brief 创建hostapp模块信息【不可以是首个module】
 *
 * @param[in] appName APP名称
 * @param[in] appVers APP版本
 * @return 成功返回非负整数的模块句柄,失败返回<0
 *
 * @note 此模块不能作为第一个模块信息添加到设备信息中
 */
UHSD_API uhsd_moduleHandle uhsd_dev_new_hostapp_module(const uhsd_char *appName, const uhsd_char *appVers);

/**
 * @brief 创建nbiot模块句柄
 *
 * @param[in] imei 移动设备标识
 * @param[in] imsi 国际移动用户识别码
 * @param[in] iccid 集成电路卡识别码
 * @param[in] hwType 软件版本
 * @param[in] swVers 硬件类型
 * @return 成功返回非负整数的模块句柄,失败返回<0
 */
UHSD_API uhsd_moduleHandle
uhsd_dev_new_nbiot_module(const uhsd_char *imei, const uhsd_char *imsi, const uhsd_char *iccid, const uhsd_char *hwType, const uhsd_char *swVers);

/**
 * @brief 创建4G模块句柄
 *
 * @param[in] imei 移动设备标识
 * @param[in] imsi 国际移动用户识别码
 * @param[in] iccid 集成电路卡识别码
 * @param[in] hwType 硬件类型
 * @param[in] swVers 软件版本
 * @return 成功返回非负整数的模块句柄,失败返回<0
 */
UHSD_API uhsd_moduleHandle
uhsd_dev_new_4g_module(const uhsd_char *imei, const uhsd_char *imsi, const uhsd_char *iccid, const uhsd_char *hwType, const uhsd_char *swVers);

/**
 * @brief 创建wifinetcard模块句柄
 *
 * @param[in] mac 模块硬件标识
 * @param[in] hwType 硬件类型
 * @param[in] swVers 软件版本
 * @return 成功返回非负整数的模块句柄,失败返回<0
 */
UHSD_API uhsd_moduleHandle uhsd_dev_new_wifinetcard_module(const uhsd_char *mac, const uhsd_char *hwType, const uhsd_char *swVers);

/**
 * @brief 创建ethnetcard模块句柄
 *
 * @param[in] mac
 * @param[in] hwType 硬件类型。
 * @param[in] swVers 软件版本。
 * @return 成功返回非负整数的模块句柄,失败返回<0
 */
UHSD_API uhsd_moduleHandle uhsd_dev_new_ethnetcard_module(const uhsd_char *mac, const uhsd_char *hwType, const uhsd_char *swVers);

/**
 * @brief 创建zigbee模块句柄
 *
 * @param[in] ieeeAddr IEEE 802 地址
 * @param[in] hwType 硬件类型
 * @param[in] swVers 软件版本
 * @return 成功返回非负整数的模块句柄,失败返回<0
 */
UHSD_API uhsd_moduleHandle uhsd_dev_new_zigbee_module(const uhsd_char *ieeeAddr, const uhsd_char *hwType, const uhsd_char *swVers);

/**
 * @brief 创建baseboard模块句柄【不可以是首个module】
 * @details 子板信息注册
 *
 * @param[in] boardID 子板ID，最大32
 * @param[in] boardVers 子板版本
 * @param[in] upgrade 子板是否支持OTA
 * @return 成功返回非负整数的模块句柄,失败返回<0
 */
UHSD_API uhsd_moduleHandle uhsd_dev_new_baseboard_module(const uhsd_char *boardID, const uhsd_char *boardVers, uhsd_s32 upgrade);

/**
 * @brief 清除所有添加到目标设备的baseBoard模块信息
 *
 * @param[in] devHandle 设备句柄
 * @return 成功返回0,失败返回<0
 *
 * @note 当升级子板成功后，设备APP需要先清除底板信息，然后重新添加升级后的底板模块信息
 */
UHSD_API uhsd_s32 uhsd_dev_clear_baseboard_module(uhsd_devHandle devHandle);

/**
 * @brief 添加module到设备
 *
 * @param[in] devHandle  设备句柄
 * @param[in] moduleHandle 模块句柄
 * @return 成功返回0,失败返回<0
 *
 * @note 1. 每一个设备中仅能添加一个非hostApp模块信息;
 *       2. 每一个设备添加的第一个模块信息不能是hostApp模块；
 */
UHSD_API uhsd_s32 uhsd_dev_add_module_to_device(uhsd_devHandle devHandle, uhsd_moduleHandle moduleHandle);

#ifdef __cplusplus
}
#endif

#endif /*__UHSD_ADD_DEV_COMMON_H__*/
