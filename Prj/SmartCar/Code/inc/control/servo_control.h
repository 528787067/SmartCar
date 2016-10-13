#ifndef	__SERVO_CONTROL_H__
#define	__SERVO_CONTROL_H__

#include "include.h"
#include "define.h"
#include "params.h"

int16 calculateTurnAngle(int16 *center);										// �������߼��������
void servoPidInit(float _proportion, float _integral, float _derviative);		// ��ʼ����� PID
void updateServoPid(float _proportion, float _integral, float _derviative);	// ���¶�� PID ����
int16 calculatePidAngle(int16 purpostAngle);									// ���� PID ���

#endif