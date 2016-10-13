#ifndef __TOUCH_H
#define __TOUCH_H

//---����ͷ�ļ�---//
// #include <reg51.h>
// #include "common.h"
#include "include.h"
#include "tftPortSet.h"
// #include <intrins.h>

//---�ض���ؼ���---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint
#define uint  unsigned int
#endif

//---������������---//
typedef struct 
{
	uint x;
	uint y;
} XPT_XY;

extern XPT_XY xpt_xy;

//---����оƬ�����ֽ�---//
#define XPT_CMD_X	0xD0    //��ȡX�������
#define XPT_CMD_Y	0x90	//��ȡY�������

//---����ʹ�õ�IO��---//
/*
sbit TOUCH_DOUT = P2^0;	  //���
sbit TOUCH_CLK  = P2^1;	  //ʱ��
sbit TOUCH_DIN  = P2^2;	  //����
sbit TOUCH_CS   = P2^3;	  //Ƭѡ
sbit TOUCH_PEN  = P2^4;	  //��ⴥ������Ӧ�ź�
*/
#define TOUCH_DOUT _TOUCH_DOUT	  //���
#define TOUCH_CLK  _TOUCH_CLK	  //ʱ��
#define TOUCH_DIN  _TOUCH_DIN	  //����
#define TOUCH_CS   _TOUCH_CS	  //Ƭѡ
#define TOUCH_PEN  _TOUCH_PEN	  //��ⴥ������Ӧ�ź�

//---����ȫ�ֺ���---//
void TOUCH_SPI_Start(void);
void TOUCH_SPI_Write(uchar dat);
uint TOUCH_SPI_Read(void);
uint TOUCH_XPT_ReadData(uchar cmd);
uchar TOUCH_XPT_ReadXY(void);



#endif
