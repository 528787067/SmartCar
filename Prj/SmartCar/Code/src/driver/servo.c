#include "servo.h"

/* ��ʼ����� */
void servo_Init()
{
	ftm_pwm_init(SERVO_FTM, SERVO_CH, S_FREQUENCY, SERVO_CENTER);
}

/* ���ƶ����ת */
void servo_TurnToAngle(int16 servoAngle)
{
	ftm_pwm_duty(SERVO_FTM, SERVO_CH, (carParams.servoWorkFlag == STOP_MODE) ? SERVO_CENTER : servoAngle);
}