#ifndef __PID_CONTROL_H__
#define __PID_CONTROL_H__
#include "include.h"

#define SERVO_PID_TYPE	0
#define MOTOR_PID_TYPE	1

typedef struct
{
	int16 preOneDeviation;		// 代表上一场与目标 pwm 的偏差
	int16 preTwoDeviation;		// 代表上两场与目标 pwm 的偏差
	int16 pidPwm;				// 代表 pid 算出的 pwm
} pwmPID;

typedef struct
{
	float proportion;			// 比例常数
	float integral;				// 积分常数
	float derviative;			// 微分常数
} pwmPidParams;

void pwmPidInit(uint8 pidType, float _proportion, float _integral, float _derviative);
void updatePidParams(uint8 pidType, float _proportion, float _integral, float _derviative);
int16 calculatePwm(uint8 pidType, uint16 currentPwm, uint16 purpostPwm);

#endif