#ifndef __OV7620_H__
#define __OV7620_H__

#include "include.h"
#include "define.h"

uint8 ov7620_initCamera(uint8 *imgaddr);	//��ʼ������ͷ
uint8 *ov7620_startGetImg();				//��������ͷ��ȡһ��ͼ��
uint8 ov7620_stopGetImg();					//ֹͣ����ͷ��ȡͼ��
	
#endif