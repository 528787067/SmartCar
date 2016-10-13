#include "motor.h"

/* �����ʼ�� */
void motorInit()
{
	ftm_pwm_init(MOTOR_FTM, MOTOR_CH_1, M_FREQUENCY, MOTOR_SPEED0);
	ftm_pwm_init(MOTOR_FTM, MOTOR_CH_2, M_FREQUENCY, MOTOR_SPEED0);
}

/* �ı����ٶ� */
void motorChangeToSpeed(uint8 direction, int16 speedLevel)
{
	if(carParams.motorWorkFlag == STOP_MODE)
	{
		ftm_pwm_duty(MOTOR_FTM, MOTOR_CH_1, MOTOR_SPEED0);
		ftm_pwm_duty(MOTOR_FTM, MOTOR_CH_2, MOTOR_SPEED0);		
	}
	else
	{
		if(direction == GO)				// ǰ��
		{	
			ftm_pwm_duty(MOTOR_FTM, MOTOR_CH_1, (speedLevel >= 0) ? speedLevel : MOTOR_SPEED0);
			ftm_pwm_duty(MOTOR_FTM, MOTOR_CH_2, (speedLevel >= 0) ? MOTOR_SPEED0 : -speedLevel);
		}
		else if(direction == BACK)		// ����
		{
			ftm_pwm_duty(MOTOR_FTM, MOTOR_CH_1, (speedLevel >= 0) ? MOTOR_SPEED0 : -speedLevel);
			ftm_pwm_duty(MOTOR_FTM, MOTOR_CH_2, (speedLevel >= 0) ? speedLevel : MOTOR_SPEED0);	
		}
		else							// ֹͣ
		{
			ftm_pwm_duty(MOTOR_FTM, MOTOR_CH_1, MOTOR_SPEED0);
			ftm_pwm_duty(MOTOR_FTM, MOTOR_CH_2, MOTOR_SPEED0);		
		}
	}	
}