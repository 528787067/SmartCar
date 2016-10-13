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
	int16 preOneDeviation;		// 代表上一场与目标 pwm 的偏差
	int16 preTwoDeviation;		// 代表上两场与目标 pwm 的偏差
	int16 pidSpeed;				// 代表 pid 算出的 pwm
} speedPID;

typedef struct
{
	float proportion;			// 比例常数
	float integral;				// 积分常数
	float derviative;			// 微分常数
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
	// 当前速度与目标速度的偏差
	int16 currentDeviation = purpostSpeed - currentSpeed;
	// 计算 pid 增量，比例分量 + 积分分量 + 微分分量
	double speedIncremental = speedParams.proportion * currentDeviation
		+ speedParams.integral * (currentDeviation - speedPid.preOneDeviation)
		+ speedParams.derviative * ((currentDeviation - speedPid.preOneDeviation) - (speedPid.preOneDeviation - speedPid.preTwoDeviation));
	
	speedPid.preOneDeviation = currentDeviation;
	speedPid.preTwoDeviation = speedPid.preOneDeviation;
	
	// 计算 pid 速度，上场 pid 速度 + pid 速度增量
	speedPid.pidSpeed = currentSpeed+(int16)(speedIncremental+0.5);
	
	if(speedPid.pidSpeed > MOTOR_SPEED_MAX)
		speedPid.pidSpeed = MOTOR_SPEED_MAX;
	else if(speedPid.pidSpeed < 0-MOTOR_SPEED_MAX)
		speedPid.pidSpeed = 0-MOTOR_SPEED_MAX;
	
	return speedPid.pidSpeed;
}
