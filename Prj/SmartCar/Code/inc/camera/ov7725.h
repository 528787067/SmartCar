#ifndef __OV7725_H__
#define __OV7725_H__

#include "include.h"

uint8 ov7725_initCamera(uint8 *imgaddr);	//��ʼ������ͷ
uint8 *ov7725_startGetImg();				//��������ͷ��ȡһ��ͼ��
uint8 ov7725_stopGetImg();					//ֹͣ����ͷ��ȡͼ��
	
#endif