#include "main.h"

CarParams carParams;														// 智能车参数
uint8 imgBuff[IMAGE_H][IMAGE_W];											// 图像采集数组
int16 *center;																// 保存中线的数组指针 center[IMAGE_H+1]

void main()
{
	initAll();																// 初始化所有设备
	DELAY_MS(START_DELAY);													// 延时启动
	encodeWork();															// 使能编码器
	while(TRUE)
	{
		center = getCenterLine(cameraGetImg());								// 采集图像并计算中线							
		servoTurnToAngle(calculateTurnAngle(center));						// 计算并控制舵机打角
		if(carParams.workStatusFlag == DEBUG_MODE)
			updateTFT();													// 更新 TFT 显示内容
	}
}

/* 编码器中断回调函数 */
void encodeCallBack()
{
	if(carParams.passStopLine != PASS_STOP_LINE)
		carParams.carRunTime++;												// 小车起跑时间计时
#ifndef RELEASE
	queryBlueTooth();														// 蓝牙控制
#endif
	motorChangeToSpeed(GO, calculateChangeSpeed(carParams.centerLine));		// 计算并控制电机速度	
}

/* 初始化所有设备 */
void initAll()
{
	flashInit();															// flash 初始化
	paramsInit();															// 参数初始化
	keyInit();																// 按键初始化
	tftInitWithCar();														// TFT 彩屏初始化
	servoPidInit(carParams.servo_P, carParams.servo_I, carParams.servo_D);	// 舵机 PID 初始化
	motorPidInit(carParams.motor_P, carParams.motor_I, carParams.motor_D);	// 电机 PID 初始化
	servoInit();															// 舵机初始化
	encodeInit();															// 编码器初始化
	motorInit();															// 电机初始化
	cameraInit(imgBuff);													// 摄像头初始化		
}

/* 初始化参数 */
void paramsInit()
{
#ifdef RELEASE	
	carParams.workStatusFlag = RELEASE_MODE;
	carParams.servoWorkFlag = WORK_MODE;
	carParams.motorWorkFlag = WORK_MODE;
#else
	carParams.workStatusFlag = DEBUG_MODE;
	carParams.servoWorkFlag = STOP_MODE;
	carParams.motorWorkFlag = STOP_MODE;
#endif
	carParams.carRunTime = 0;
	carParams.stopCarFlag = !STOP_LINE;
	carParams.passStopLine = !PASS_STOP_LINE;
	carParams.centerLine = 0;
	carParams.encode = 0;
	carParams.servo_PWM = 0;
	carParams.motor_PWM = 0;
	carParams.settingFlag = SHOWING_MODE;
	
	if(carParams.flashDataFlag == FLASH_TRUE)
		getParamsFromFlash();
	else
	{
		carParams.pidLevel = 0;
		carParams.servo_P = 1;
		carParams.servo_I = 0;
		carParams.servo_D = 0;
		carParams.motor_P = 1;
		carParams.motor_I = 0;
		carParams.motor_D = 0;
	}
}

/* 更新 TFT 显示 */
void updateTFT()
{
	showImgWithCar(imgBuff);												// 将图像显示到 TFT 液晶屏上
	showCenterLineWithCar(center);											// 将中线显示到 TFT 液晶屏上	
	showValueWithCar(TFT_CENTER_LINE, carParams.centerLine);				// 在 TFT 上显示中线偏差
	showValueWithCar(TFT_SERVO_PWM, carParams.servo_PWM);					// 在 TFT 上显示舵机 PWM
	showValueWithCar(TFT_ENCODE, carParams.encode);							// 在 TFT 上显示编码器数据
	showValueWithCar(TFT_MOTOR_PWM, carParams.motor_PWM);					// 在 TFT 上显示电机 PWM
}

