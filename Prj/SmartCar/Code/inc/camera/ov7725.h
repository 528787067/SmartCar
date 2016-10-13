#ifndef __OV7725_H__
#define __OV7725_H__

#include "include.h"

uint8 ov7725_initCamera(uint8 *imgaddr);	//初始化摄像头
uint8 *ov7725_startGetImg();				//驱动摄像头获取一副图像
uint8 ov7725_stopGetImg();					//停止摄像头获取图像
	
#endif