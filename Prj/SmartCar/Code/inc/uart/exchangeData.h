#ifndef __EXCHANGE_DATA_H__
#define __EXCHANGE_DATA_H__

#include "include.h"
#include "define.h"
#include "params.h"

void send_Img(uint8 *imgAddr);						// ����ͼ����λ��
void send_Ware(int8_t *wareAddr, uint8 wareSize);	// ���Ͳ��ε�����ʾ���������� 1 ��Ų������ݣ����� 2 ��������Ŀ���������С

#define	sendImg(imageAddr)				send_Img((uint8 *)imageAddr)
#define	sendWare(wareAddr, wareSize)	send_Ware((int8_t *)wareAddr, (uint8)wareSize)

#define BT_NO_CHAR				0x00
#define BT_SERVO_WORK			0x0a
#define BT_SERVO_STOP			0x0b
#define BT_MOTOR_WORK			0x0c
#define BT_MOTOR_STOP			0x0d
void queryBlueTooth();								// ��ѯ�����Ƿ���յ�����

#endif