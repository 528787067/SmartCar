#ifndef	__SERVO_CONTROL_H__
#define	__SERVO_CONTROL_H__

#include "include.h"
#include "define.h"
#include "params.h"

int16 calculateTurnAngle(int16 *center);										// 根据中线计算舵机打角
void servoPidInit(float _proportion, float _integral, float _derviative);		// 初始化舵机 PID
void updateServoPid(float _proportion, float _integral, float _derviative);	// 更新舵机 PID 参数
int16 calculatePidAngle(int16 purpostAngle);									// 计算 PID 打角

#endif