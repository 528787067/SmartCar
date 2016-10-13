#ifndef __DEFINE_H__
#define __DEFINE_H__

#include "include.h"

/********************* ȫ�ֺ궨�� ********************/

/********************* ����ͷ���� ********************/
#define 	OV7725				0		// ɽ��ӥ�� OV7725
#define		OV7620				1		// ���沶ʳ�� OV7620

// ����ʹ�õ�����ͷ����
#define		CAMERA		OV7725			// ʹ�� CAMERA �궨���л���ͬ����ͷ

// ��ֵ������-1 ������ֵ����ֻ�� OV7620 ��Ч��ӥ��Ӳ����ֵ����
#define		BIN_LEVEL			50

#define		IMAGE_W				80		// ͼ���ȣ�80�ı�����
#define		IMAGE_H				60		// ͼ��߶ȣ�60�ı�����
/*****************************************************/

/* ��������ʱ���궨�壨��λ��ms�� */
#define		ENCODE_TIME			5

/* �������Ƶ�� */
#define		SERVO_FREQUENCY		300

/* �������Ƶ�� */
#define		MOTOR_FREQUENCY		5000

/* ������ʱ */
#ifdef RELEASE
#define		START_DELAY			2000
#else
#define		START_DELAY			0
#endif

#endif