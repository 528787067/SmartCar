#include "main.h"

CarParams carParams;														// ���ܳ�����
uint8 imgBuff[IMAGE_H][IMAGE_W];											// ͼ��ɼ�����
int16 *center;																// �������ߵ�����ָ�� center[IMAGE_H+1]

void main()
{
	initAll();																// ��ʼ�������豸
	DELAY_MS(START_DELAY);													// ��ʱ����
	encodeWork();															// ʹ�ܱ�����
	while(TRUE)
	{
		center = getCenterLine(cameraGetImg());								// �ɼ�ͼ�񲢼�������							
		servoTurnToAngle(calculateTurnAngle(center));						// ���㲢���ƶ�����
		if(carParams.workStatusFlag == DEBUG_MODE)
			updateTFT();													// ���� TFT ��ʾ����
	}
}

/* �������жϻص����� */
void encodeCallBack()
{
	if(carParams.passStopLine != PASS_STOP_LINE)
		carParams.carRunTime++;												// С������ʱ���ʱ
#ifndef RELEASE
	queryBlueTooth();														// ��������
#endif
	motorChangeToSpeed(GO, calculateChangeSpeed(carParams.centerLine));		// ���㲢���Ƶ���ٶ�	
}

/* ��ʼ�������豸 */
void initAll()
{
	flashInit();															// flash ��ʼ��
	paramsInit();															// ������ʼ��
	keyInit();																// ������ʼ��
	tftInitWithCar();														// TFT ������ʼ��
	servoPidInit(carParams.servo_P, carParams.servo_I, carParams.servo_D);	// ��� PID ��ʼ��
	motorPidInit(carParams.motor_P, carParams.motor_I, carParams.motor_D);	// ��� PID ��ʼ��
	servoInit();															// �����ʼ��
	encodeInit();															// ��������ʼ��
	motorInit();															// �����ʼ��
	cameraInit(imgBuff);													// ����ͷ��ʼ��		
}

/* ��ʼ������ */
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

/* ���� TFT ��ʾ */
void updateTFT()
{
	showImgWithCar(imgBuff);												// ��ͼ����ʾ�� TFT Һ������
	showCenterLineWithCar(center);											// ��������ʾ�� TFT Һ������	
	showValueWithCar(TFT_CENTER_LINE, carParams.centerLine);				// �� TFT ����ʾ����ƫ��
	showValueWithCar(TFT_SERVO_PWM, carParams.servo_PWM);					// �� TFT ����ʾ��� PWM
	showValueWithCar(TFT_ENCODE, carParams.encode);							// �� TFT ����ʾ����������
	showValueWithCar(TFT_MOTOR_PWM, carParams.motor_PWM);					// �� TFT ����ʾ��� PWM
}

