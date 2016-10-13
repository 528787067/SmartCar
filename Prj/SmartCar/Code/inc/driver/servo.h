#ifndef	__SERVO_H__
#define __SERVO_H__

#include "include.h"
#include "define.h"
#include "params.h"


/* 舵机打转角度级别 */
#define	SERVO_CENTER			430
#define SERVO_LEFT_MAX			SERVO_CENTER+100
#define SERVO_RIGHT_MAX			SERVO_CENTER-100
//#define SERVO_CENTER			43
//#define SERVO_LEFT_LEVEL1		(SERVO_CENTER+1)
//#define SERVO_LEFT_LEVEL2		(SERVO_CENTER+2)
//#define SERVO_LEFT_LEVEL3		(SERVO_CENTER+3)
//#define SERVO_LEFT_LEVEL4		(SERVO_CENTER+4)
//#define SERVO_LEFT_LEVEL5		(SERVO_CENTER+5)
//#define SERVO_LEFT_LEVEL6		(SERVO_CENTER+6)
//#define SERVO_LEFT_LEVEL7		(SERVO_CENTER+7)
//#define SERVO_LEFT_LEVEL8		(SERVO_CENTER+8)
//#define SERVO_LEFT_LEVEL9		(SERVO_CENTER+9)
//#define SERVO_RIGHT_LEVEL1	(SERVO_CENTER-1)
//#define SERVO_RIGHT_LEVEL2	(SERVO_CENTER-2)
//#define SERVO_RIGHT_LEVEL3	(SERVO_CENTER-3)
//#define SERVO_RIGHT_LEVEL4	(SERVO_CENTER-4)
//#define SERVO_RIGHT_LEVEL5	(SERVO_CENTER-5)
//#define SERVO_RIGHT_LEVEL6	(SERVO_CENTER-6)
//#define SERVO_RIGHT_LEVEL7	(SERVO_CENTER-7)
//#define SERVO_RIGHT_LEVEL8	(SERVO_CENTER-8)
//#define SERVO_RIGHT_LEVEL9	(SERVO_CENTER-9)
//#define SERVO_LEFT_MAX		SERVO_LEFT_LEVEL9
//#define SERVO_RIGHT_MAX		SERVO_RIGHT_LEVEL9

#define	SERVO_FTM				FTM2
#define	SERVO_CH				FTM_CH1

#define	S_FREQUENCY				SERVO_FREQUENCY

// void servo_Init();							// 初始化舵机
// void servo_TurnToAngle(int16 servoAngle);	// 控制舵机打转

// 初始化舵机
#define servoInit()						ftm_pwm_init(SERVO_FTM, SERVO_CH, S_FREQUENCY, SERVO_CENTER)
// 控制舵机打转
#define	servoTurnToAngle(angleLevel)	ftm_pwm_duty(SERVO_FTM, SERVO_CH, (carParams.servoWorkFlag == STOP_MODE) ? SERVO_CENTER : angleLevel)

#endif