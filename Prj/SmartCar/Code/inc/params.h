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
	{16, 15, 14, 13, 13, 12, 12, 11, 11, 10, 10},		// 第一档速度
	{26, 24, 22, 26, 15, 14, 13, 13, 12, 12, 11},		// 第二档速度
	{32, 30, 28, 26, 22, 15, 14, 14, 13, 13, 12},		// 第三档速度
};

typedef struct
{
	long carRunTime;									// 用于保存车子运行时间
	uint8 stopCarFlag;									// 停车线标记
	uint8 passStopLine;									// 越过停车线标记
	uint8 flashDataFlag;								// 用于标记 flash 中是否有保存参数
	
	uint8 workStatusFlag;								// 用于标记工作状态（调试/发布）
	uint8 servoWorkFlag;								// 用于标记舵机是否工作
	uint8 motorWorkFlag;								// 用于标记电机是否工作
	uint8 settingFlag;									// 用于标记是否处于设置状态
	
	int8 centerLine;									// 中线偏差
	int16 encode;										// 编码器
	uint8 pidLevel;										// PID 级别
	
	float servo_P;										// 舵机 P 值
	float servo_I;										// 舵机 I 值
	float servo_D;										// 舵机 D 值
	float motor_P;										// 电机 P 值
	float motor_I;										// 电机 I 值
	float motor_D;										// 电机 D 值
	
	int16 servo_PWM;									// 舵机 PWM
	int16 motor_PWM;									// 电机 PWM
} CarParams;

extern CarParams carParams;

#endif