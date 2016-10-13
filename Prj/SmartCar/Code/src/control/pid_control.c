#include "pid_control.h"

static pwmPID _speedPid;
static pwmPidParams _speedParams;
static pwmPID _anglePid;
static pwmPidParams _angleParams;

void pwmPidInit(uint8 pidType, float _proportion, float _integral, float _derviative)
{
	if(pidType == SERVO_PID_TYPE)
	{
		_anglePid.preOneDeviation = 0;
		_anglePid.preTwoDeviation = 0;
		_anglePid.pidPwm = 0;
		
		_angleParams.proportion = _proportion;
		_angleParams.integral = _integral;
		_angleParams.derviative = _derviative;
	}
	else
	{
		_speedPid.preOneDeviation = 0;
		_speedPid.preTwoDeviation = 0;
		_speedPid.pidPwm = 0;
		
		_speedParams.proportion = _proportion;
		_speedParams.integral = _integral;
		_speedParams.derviative = _derviative;
	}
}

void updatePidParams(uint8 pidType, float _proportion, float _integral, float _derviative)
{
	if(pidType == SERVO_PID_TYPE)
	{
		_angleParams.proportion = _proportion;
		_angleParams.integral = _integral;
		_angleParams.derviative = _derviative;
	}
	else
	{
		_speedParams.proportion = _proportion;
		_speedParams.integral = _integral;
		_speedParams.derviative = _derviative;
	}
}

int16 calculatePwm(uint8 pidType, uint16 currentPwm, uint16 purpostPwm)
{
	int16 pwmIncremental;
	int16 currentDeviation = purpostPwm - currentPwm;		// ��ǰ PWM ��Ŀ�� PWM ��ƫ��
	if(pidType == SERVO_PID_TYPE)
	{
		// ���� pid �������������� + ���ַ��� + ΢�ַ���
		pwmIncremental = _angleParams.proportion * currentDeviation
			+ _angleParams.integral * (currentDeviation - _anglePid.preOneDeviation)
			+ _angleParams.derviative * ((currentDeviation - _anglePid.preOneDeviation) - (_anglePid.preOneDeviation - _anglePid.preTwoDeviation));
		
		_anglePid.preOneDeviation = currentDeviation;
		_anglePid.preTwoDeviation = _anglePid.preOneDeviation;
		
		_anglePid.pidPwm += pwmIncremental;					// ���� pid PWM���ϳ� pid PWM + pid PWM����
		return _anglePid.pidPwm;
	}
	else
	{
		// ���� pid �������������� + ���ַ��� + ΢�ַ���
		pwmIncremental = _speedParams.proportion * currentDeviation
			+ _speedParams.integral * (currentDeviation - _speedPid.preOneDeviation)
			+ _speedParams.derviative * ((currentDeviation - _speedPid.preOneDeviation) - (_speedPid.preOneDeviation - _speedPid.preTwoDeviation));
		
		_speedPid.preOneDeviation = currentDeviation;
		_speedPid.preTwoDeviation = _speedPid.preOneDeviation;
		
		_speedPid.pidPwm += pwmIncremental;					// ���� pid PWM���ϳ� pid PWM + pid PWM����
		return _speedPid.pidPwm;
	}
}