#ifndef __ENCODE_H__
#define __ENCODE_H__

#include "include.h"
#include "define.h"
#include "tft_r61509.h"
#include "main.h"

#define	ENCODE_FTM	FTM1
#define	ENCODE_PIT	PIT0
#define	TIME		ENCODE_TIME

void encodeInit();							// ��ʼ��������
void encodeWork();							// ʹ�ܱ�����
void encodeStop();							// �رձ�����
int16 getEncodeValue();						// ��ȡ��������ֵ

#endif