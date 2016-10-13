#ifndef __OV7620_H__
#define __OV7620_H__

#include "include.h"
#include "define.h"

uint8 ov7620_initCamera(uint8 *imgaddr);	//初始化摄像头
uint8 *ov7620_startGetImg();				//驱动摄像头获取一副图像
uint8 ov7620_stopGetImg();					//停止摄像头获取图像
	
#endif