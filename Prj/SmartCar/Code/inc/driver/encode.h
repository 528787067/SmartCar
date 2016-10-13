#ifndef __ENCODE_H__
#define __ENCODE_H__

#include "include.h"
#include "define.h"
#include "tft_r61509.h"
#include "main.h"

#define	ENCODE_FTM	FTM1
#define	ENCODE_PIT	PIT0
#define	TIME		ENCODE_TIME

void encodeInit();							// 初始化编码器
void encodeWork();							// 使能编码器
void encodeStop();							// 关闭编码器
int16 getEncodeValue();						// 获取编码器的值

#endif