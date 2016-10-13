#ifndef __FLASH_H__
#define __FLASH_H__

#include "include.h"
#include "params.h"

#define FLASH_SUCCESS 1
#define FLASH_FAIL	0

void flashInit();							// flash 初始化
void getParamsFromFlash();					// 获取保存在 flash 中的参数
uint8 saveParamsToFlash();					// 将参数保存的 flash 中
void cleanAllParamsFromFlash();				// 清除保存在 flash 中的所有参数

#endif