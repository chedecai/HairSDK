/**
 * @copyright Copyright (c) 2022, Haier.Co, Ltd.
 * @file uh_ble_common.c
 * @author maaiguo (maaiguo@haier.com)
 * @brief 基于ESP32-S3硬件模块的BLE通用功能函数实现
 * @date 2025-08-09
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>version <th>Author  <th>Description
 * <tr><td>2025-08-09   <td>1.0     <td>maaiguo <td>
 * </table>
 */

#define LOG_TAG "ble-m"

/**************************************************************************************************/
/*                           #include (依次为标准头文件、非标准头文件)                            */
/**************************************************************************************************/
#include "esp_mac.h"
#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_gattc_api.h"
#include "esp_gatt_defs.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_gatt_common_api.h"
#include "esp_bt_device.h"
#include "esp_system.h"

#include "uh_types.h"
#include "uh_osal.h"
#include "uh_log.h"


/**************************************************************************************************/
/*                                          外部引用声明                                          */
/**************************************************************************************************/


/**************************************************************************************************/
/*                                           内部宏定义                                           */
/**************************************************************************************************/


/**************************************************************************************************/
/*                                        内部数据类型定义                                        */
/**************************************************************************************************/


/**************************************************************************************************/
/*                                        全局(静态)变量                                          */
/**************************************************************************************************/


/**************************************************************************************************/
/*                                          内部函数原型                                          */
/**************************************************************************************************/


/**************************************************************************************************/
/*                                          内部函数实现                                          */
/**************************************************************************************************/


/**************************************************************************************************/
/*                                          全局函数实现                                          */
/**************************************************************************************************/

/**
 * @brief       获取BLE的MAC地址
 * @param[out]  mac     MAC地址
 * @return      uhos_ble_status_t 执行结果
 *              UHOS_BLE_SUCCESS  成功
 */
uhos_ble_status_t uhos_ble_address_get(uhos_ble_addr_t mac)
{
	esp_err_t ret = 0;
    uhos_u8 addr_ptr[6] = {0,};

    if(mac == NULL)
        return UHOS_BLE_ERROR;

    ret = esp_read_mac(addr_ptr, ESP_MAC_WIFI_STA);
    if(ret != 0)
    {
		UHOS_LOGE("ble address get error:%s", esp_err_to_name(ret));
        return UHOS_BLE_ERROR;
    }

    mac[0] = addr_ptr[5];
    mac[1] = addr_ptr[4];
    mac[2] = addr_ptr[3];
    mac[3] = addr_ptr[2];
    mac[4] = addr_ptr[1];
    mac[5] = addr_ptr[0];
    
    return UHOS_BLE_SUCCESS;
}

/**
 * @brief       使能BLE
 * @return      uhos_ble_status_t 执行结果
 * @retval      UHOS_BLE_SUCCESS  成功
 */
uhos_ble_status_t uhos_ble_enable(void)
{
    esp_err_t ret;
	esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();

	/* classic bt resource release */
	ret = esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT);
    if (ret) {
        UHOS_LOGE("release BT RAM failed: %s", esp_err_to_name(ret));
        return UHOS_BLE_ERROR;
    }

	ret = esp_bt_controller_init(&bt_cfg);
	if (ret) {
		UHOS_LOGE("esp_bt_controller_init failed!");
		return UHOS_BLE_ERROR;
	}

	/* enable ble mode*/
	ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
	if (ret) {
		UHOS_LOGE("esp_bt_controller_enable failed!");
		return UHOS_BLE_ERROR;
	}

	/* init ble */
	esp_bluedroid_config_t bluedroid_cfg = BT_BLUEDROID_INIT_CONFIG_DEFAULT();
    ret = esp_bluedroid_init_with_cfg(&bluedroid_cfg);
	if (ret) {
		UHOS_LOGE("esp_bluedroid_init failed!");
		return UHOS_BLE_ERROR;
	}

	ret = esp_bluedroid_enable();
	if (ret) {
		UHOS_LOGE("esp_bluedroid_enable failed!");
		return UHOS_BLE_ERROR;
	}

    ret = esp_ble_gatts_register_callback(NULL);
    if (ret) {
        UHOS_LOGE("gatts register error, error code = %x", ret);
        return UHOS_BLE_ERROR;
    }

    ret = esp_ble_gattc_register_callback(NULL);
    if (ret) {
        UHOS_LOGE("gattc register error, error code = %x", ret);
        return UHOS_BLE_ERROR;
    }

    ret = esp_ble_gap_register_callback(NULL);
    if (ret) {
        UHOS_LOGE("gap register error, error code = %x", ret);
        return UHOS_BLE_ERROR;
    }

    ret = esp_ble_gatts_app_register(0);
    if (ret) {
        UHOS_LOGE("gatts app register error, error code = %x", ret);
        return UHOS_BLE_ERROR;
    }

    ret = esp_ble_gattc_app_register(0);
    if (ret) {
        UHOS_LOGE("gattc app register error, error code = %x", ret);
        return UHOS_BLE_ERROR;
    }

    if (ESP_BLUEDROID_STATUS_ENABLED != esp_bluedroid_get_status()) {
        UHOS_LOGE("stack init error");
        return UHOS_BLE_ERROR;
    }

	return UHOS_BLE_SUCCESS;
}

/**
 * @brief       失能BLE
 *
 * @return      uhos_ble_status_t 执行状态
 * @retval      UHOS_BLE_SUCCESS  成功
 */
uhos_ble_status_t uhos_ble_disable(void)
{
  	esp_err_t ret = 0;

	UHOS_LOGI("disable BLE");
    ret = esp_bluedroid_disable();
    if (ret) {
		UHOS_LOGE("bluetooth disable failed: = %x", ret);
        return UHOS_BLE_ERROR;
    }

    ret = esp_bluedroid_deinit();
    if (ret) {
		UHOS_LOGE("deinit bluetooth failed = %x", ret);
        return UHOS_BLE_ERROR;
    }

    ret = esp_bt_controller_disable();
    if (ret) {
		UHOS_LOGE("disable controller failed = %x", ret);
        return UHOS_BLE_ERROR;
    }

    ret = esp_bt_controller_deinit();
    if (ret) {
		UHOS_LOGE("deinit controller failed = %x", ret);
        return UHOS_BLE_ERROR;
    }

    return UHOS_BLE_SUCCESS;
}

/**
 * @brief       启动获取信号强度
 * @param[in]   conn_handle 连接句柄
 * @return      uhos_ble_status_t 执行状态
 * @retval      UHOS_BLE_SUCCESS  成功
 */
uhos_ble_status_t uhos_ble_rssi_start(uhos_u16 conn_handle)
{
    // esp_err_t ret = 0;

    // //通过 conn_handle 获取到 连接参数

    // ret = esp_ble_gap_read_rssi();
    // if (ret) {
	// 	UHOS_LOGE("gap read rssi start failed: = %x", ret);
    //     return UHOS_BLE_ERROR;
    // }

    return UHOS_BLE_SUCCESS;
}

/**
 * @brief       直接从缓存数据中获取信号强度值（rssi）
 * @note        ASR5822S上和uhos_ble_rssi_get接口一致
 * @param[in]   conn_handle 连接句柄（未使用该参数）
 * @param[out]  rssi         信号强度值
 * @return      uhos_ble_status_t 执行状态
 * @retval      UHOS_BLE_SUCCESS  成功
 */
uhos_ble_status_t uhos_ble_rssi_get_detect(uhos_u16 conn_handle, uhos_s8 *rssi)
{
    return UHOS_BLE_SUCCESS;
}

/**
 * @brief       读取BLE信号强度
 * @param[in]   conn_handle 连接句柄
 * @param[out]  rssi        信号强度值
 * @return      uhos_ble_status_t 执行结果
 * @retval      UHOS_BLE_SUCCESS  成功
 */
uhos_ble_status_t uhos_ble_rssi_get(uhos_u16 conn_handle, uhos_s8 *rssi)
{
    return UHOS_BLE_SUCCESS;
}

/**
 * @brief       停止获取信号强度
 * @param[in]   conn_handle 连接句柄
 * @return      uhos_ble_status_t 执行结果
 * @retval      UHOS_BLE_SUCCESS  成功
 */
uhos_ble_status_t uhos_ble_rssi_stop(uhos_u16 conn_handle)
{
    //这个不需要实现
    return UHOS_BLE_SUCCESS;
}

/**
 * @brief       设置BLE的发射功率等级
 * @note        该接口暂未实现
 * @param[in]   conn_handle 连接句柄
 * @param[in]   tx_power    功率等级值
 * @return      uhos_ble_status_t 执行结果
 * @retval      UHOS_BLE_SUCCESS  成功
 */
uhos_ble_status_t uhos_ble_tx_power_set(uhos_u16 conn_handle, uhos_s8 tx_power)
{
    return UHOS_BLE_SUCCESS;
}

