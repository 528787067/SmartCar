#ifndef __PID_CONTROL_H__
#define __PID_CONTROL_H__
#include "include.h"

#define SERVO_PID_TYPE	0
#define MOTOR_PID_TYPE	1

typedef struct
{
	int16 preOneDeviation;		// ������һ����Ŀ�� pwm ��ƫ��
	int16 preTwoDeviation;		// ������������Ŀ�� pwm ��ƫ��
	int16 pidPwm;				// ���� pid ����� pwm
} pwmPID;

typedef struct
{
	float proportion;			// ��������
	float integral;				// ���ֳ���
	float derviative;			// ΢�ֳ���
} pwmPidParams;

void pwmPidInit(uint8 pidType, float _proportion, float _integral, float _derviative);
void updatePidParams(uint8 pidType, float _proportion, float _integral, float _derviative);
int16 calculatePwm(uint8 pidType, uint16 currentPwm, uint16 purpostPwm);

#endif