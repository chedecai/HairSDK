
#include "esp_system.h"
#include "uh_random.h"

/**
 * @brief  生成随机数据,  如无硬件随机数，可直接使用参考代码
 * @param[out] output  输出的随机数据。
 * @param[in] output_len  输出缓存的大小。
 */
void uhos_random_generate(uhos_u8 *output, uhos_u32 output_len)
{
	unsigned int random = 0;

	for(size_t i=0,offset = 0; i < output_len; i++){
		offset = i & 0x3;
		if(offset == 0)
			random = esp_random();

		output[i] = (random >> (offset*8)) & 0xff;
	}

	return output_len;
}