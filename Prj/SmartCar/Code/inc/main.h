#ifndef __MAIN_H__
#define __MAIN_H__

#include "define.h"
#include "include.h"
#include "params.h"
#include "camera.h"
#include "tft_r61509.h"
#include "exchangeData.h"
#include "encode.h"
#include "motor.h"
#include "servo.h"
#include "key.h"
#include "flash.h"

#include "image_control.h"
#include "servo_control.h"
#include "motor_control.h"

#define FALSE	0
#define TRUE	1

void initAll();					// ��ʼ�������豸
void paramsInit();				// ��ʼ������
void encodeCallBack();			// �������жϻص�
void updateTFT();				// ���� TFT ��ʾ����

#endif