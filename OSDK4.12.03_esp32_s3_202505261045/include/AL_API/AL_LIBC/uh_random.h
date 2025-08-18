/**
 * @addtogroup grp_uhoslibc
 * @{
 * @copyright Copyright (c) 2021, Haier.Co, Ltd.
 * @file uh_random.h
 * @author Xiongwei.Xue (xuexiongwei@haier.com)
 * @brief
 * @date 2021-10-19
 *
 * @par History:
 * <table>
 * <tr><th>Date         <th>version <th>Author  <th>Description
 * <tr><td>2021-10-19   <td>1.0     <td>        <td>
 * </table>
 */
#ifndef __UH_RANDOM_H__
#define __UH_RANDOM_H__

#include "uh_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  生成随机数据,  如无硬件随机数，可直接使用参考代码
 * @param[out] output  输出的随机数据。
 * @param[in] output_len  输出缓存的大小。
 */
void uhos_random_generate(uhos_u8 *output, uhos_u32 output_len);

#ifdef __cplusplus
}
#endif

#endif /*__UH_RANDOM_H__*/
       /**@}*/