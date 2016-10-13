#include "servo.h"

/* 初始化舵机 */
void servo_Init()
{
	ftm_pwm_init(SERVO_FTM, SERVO_CH, S_FREQUENCY, SERVO_CENTER);
}

/* 控制舵机打转 */
void servo_TurnToAngle(int16 servoAngle)
{
	ftm_pwm_duty(SERVO_FTM, SERVO_CH, (carParams.servoWorkFlag == STOP_MODE) ? SERVO_CENTER : servoAngle);
}