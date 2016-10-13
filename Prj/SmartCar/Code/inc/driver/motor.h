#ifndef	__MOTOR_H__
#define __MOTOR_H__

#include "include.h"
#include "define.h"
#include "params.h"

/* 电机速度级别 */
#define	MOTOR_SPEED0			0
#define	MOTOR_SPEED1			10
#define	MOTOR_SPEED2			20
#define	MOTOR_SPEED3			30
#define	MOTOR_SPEED4			40
#define	MOTOR_SPEED5			50
#define	MOTOR_SPEED6			60
#define	MOTOR_SPEED7			70
#define	MOTOR_SPEED8			80
#define	MOTOR_SPEED9			90
#define	MOTOR_SPEED10			100
#define MOTOR_SPEED_MAX			50

#define	MOTOR_FTM				FTM0
#define	MOTOR_CH_1				FTM_CH1
#define MOTOR_CH_2				FTM_CH2

#define	M_FREQUENCY				MOTOR_FREQUENCY					// PWM 频率

#define	STOP					0								// 停止
#define	GO						1								// 前进
#define	BACK					2								// 后退

void motorInit();												// 电机初始化
void motorChangeToSpeed(uint8 direction, int16 speedLevel);		// 改变电机速度

#endif