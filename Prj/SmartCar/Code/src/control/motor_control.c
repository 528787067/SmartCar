#include "motor_control.h"

int16 calculateChangeSpeed(int16 angle)
{
	uint8 tmp;	
	int16 purpostSpeed;
	
#ifndef RELEASE
	purpostSpeed = motorSpeedTable[carParams.pidLevel][(angle>=0) ? (angle) : (-angle)];
#else
	purpostSpeed = (carParams.passStopLine != PASS_STOP_LINE)
				 ? motorSpeedTable[carParams.pidLevel][(angle>=0) ? (angle) : (-angle)]
				 : 0;
#endif
	
	for(tmp = 0; tmp < 101; tmp++)
		if(encodeTable[tmp] > carParams.encode)
			break;
	carParams.motor_PWM = calculatePidSpeed(tmp-1, purpostSpeed);
	return carParams.motor_PWM;
}

typedef struct
{
	int16 preOneDeviation;		// ������һ����Ŀ�� pwm ��ƫ��
	int16 preTwoDeviation;		// ������������Ŀ�� pwm ��ƫ��
	int16 pidSpeed;				// ���� pid ����� pwm
} speedPID;

typedef struct
{
	float proportion;			// ��������
	float integral;				// ���ֳ���
	float derviative;			// ΢�ֳ���
} speedPidParams;

static speedPID speedPid;
static speedPidParams speedParams;

void motorPidInit(float _proportion, float _integral, float _derviative)
{
	speedPid.preOneDeviation = 0;
	speedPid.preTwoDeviation = 0;
	speedPid.pidSpeed = 0;
	
	speedParams.proportion = _proportion;
	speedParams.integral = _integral;
	speedParams.derviative = _derviative;
}

void updateMotorPid(float _proportion, float _integral, float _derviative)
{
	speedParams.proportion = _proportion;
	speedParams.integral = _integral;
	speedParams.derviative = _derviative;
}

int16 calculatePidSpeed(int16 currentSpeed, int16 purpostSpeed)
{
	// ��ǰ�ٶ���Ŀ���ٶȵ�ƫ��
	int16 currentDeviation = purpostSpeed - currentSpeed;
	// ���� pid �������������� + ���ַ��� + ΢�ַ���
	double speedIncremental = speedParams.proportion * currentDeviation
		+ speedParams.integral * (currentDeviation - speedPid.preOneDeviation)
		+ speedParams.derviative * ((currentDeviation - speedPid.preOneDeviation) - (speedPid.preOneDeviation - speedPid.preTwoDeviation));
	
	speedPid.preOneDeviation = currentDeviation;
	speedPid.preTwoDeviation = speedPid.preOneDeviation;
	
	// ���� pid �ٶȣ��ϳ� pid �ٶ� + pid �ٶ�����
	speedPid.pidSpeed = currentSpeed+(int16)(speedIncremental+0.5);
	
	if(speedPid.pidSpeed > MOTOR_SPEED_MAX)
		speedPid.pidSpeed = MOTOR_SPEED_MAX;
	else if(speedPid.pidSpeed < 0-MOTOR_SPEED_MAX)
		speedPid.pidSpeed = 0-MOTOR_SPEED_MAX;
	
	return speedPid.pidSpeed;
}
