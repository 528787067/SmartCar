#ifndef __FLASH_H__
#define __FLASH_H__

#include "include.h"
#include "params.h"

#define FLASH_SUCCESS 1
#define FLASH_FAIL	0

void flashInit();							// flash ��ʼ��
void getParamsFromFlash();					// ��ȡ������ flash �еĲ���
uint8 saveParamsToFlash();					// ����������� flash ��
void cleanAllParamsFromFlash();				// ��������� flash �е����в���

#endif