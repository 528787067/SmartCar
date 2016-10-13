#ifndef __PARAMS_H__
#define __PARAMS_H__

#include "include.h"

#define DEBUG_MODE		0
#define RELEASE_MODE	1
#define STOP_MODE		0
#define WORK_MODE		1
#define FLASH_FALSE		0
#define FLASH_TRUE		1
#define SHOWING_MODE	0
#define SETTING_MODE	1
#define STOP_LINE		1
#define PASS_STOP_LINE	1

static uint8 motorSpeedTable[3][11] = {
	{16, 15, 14, 13, 13, 12, 12, 11, 11, 10, 10},		// ��һ���ٶ�
	{26, 24, 22, 26, 15, 14, 13, 13, 12, 12, 11},		// �ڶ����ٶ�
	{32, 30, 28, 26, 22, 15, 14, 14, 13, 13, 12},		// �������ٶ�
};

typedef struct
{
	long carRunTime;									// ���ڱ��泵������ʱ��
	uint8 stopCarFlag;									// ͣ���߱��
	uint8 passStopLine;									// Խ��ͣ���߱��
	uint8 flashDataFlag;								// ���ڱ�� flash ���Ƿ��б������
	
	uint8 workStatusFlag;								// ���ڱ�ǹ���״̬������/������
	uint8 servoWorkFlag;								// ���ڱ�Ƕ���Ƿ���
	uint8 motorWorkFlag;								// ���ڱ�ǵ���Ƿ���
	uint8 settingFlag;									// ���ڱ���Ƿ�������״̬
	
	int8 centerLine;									// ����ƫ��
	int16 encode;										// ������
	uint8 pidLevel;										// PID ����
	
	float servo_P;										// ��� P ֵ
	float servo_I;										// ��� I ֵ
	float servo_D;										// ��� D ֵ
	float motor_P;										// ��� P ֵ
	float motor_I;										// ��� I ֵ
	float motor_D;										// ��� D ֵ
	
	int16 servo_PWM;									// ��� PWM
	int16 motor_PWM;									// ��� PWM
} CarParams;

extern CarParams carParams;

#endif