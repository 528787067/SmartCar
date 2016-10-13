#ifndef __EXCHANGE_DATA_H__
#define __EXCHANGE_DATA_H__

#include "include.h"
#include "define.h"
#include "params.h"

void send_Img(uint8 *imgAddr);						// 发送图像到上位机
void send_Ware(int8_t *wareAddr, uint8 wareSize);	// 发送波形到虚拟示波器，参数 1 存放波形数据，参数 2 代表波形数目，即数组大小

#define	sendImg(imageAddr)				send_Img((uint8 *)imageAddr)
#define	sendWare(wareAddr, wareSize)	send_Ware((int8_t *)wareAddr, (uint8)wareSize)

#define BT_NO_CHAR				0x00
#define BT_SERVO_WORK			0x0a
#define BT_SERVO_STOP			0x0b
#define BT_MOTOR_WORK			0x0c
#define BT_MOTOR_STOP			0x0d
void queryBlueTooth();								// 查询蓝牙是否接收到命令

#endif