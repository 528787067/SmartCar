#ifndef __R61509_H_
#define __R61509_H_

//---��ӵ�Ƭ����ͷ�ļ�---//
// #include <reg51.h>
// #include "common.h"
#include "include.h"
#include "tftPortSet.h"

//---�ض���ؼ���---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint
#define uint  unsigned int
#endif

//---�ض���ʹ�õ�IO��---//
/*
#define TFT_DATAPORTH P1	  
#define TFT_DATAPORTL P0
sbit TFT_RST  = P3^3;       //RST ��λ�˿�
sbit TFT_RD   = P2^6;	      //RD  ������
sbit TFT_CS   = P2^7;	      //CS  Ƭѡ�˿�
sbit TFT_WR   = P2^5;	      //WR  д����
sbit TFT_RS   = P3^2;	      //RS  ����/����ѡ���

sbit TFT_DB16 = P3^3;
sbit TFT_DB17 = P3^4;
*/
#define	TFT_DATAPORTH	_TFT_DATAPORTH
#define	TFT_DATAPORTL	_TFT_DATAPORTL
#define TFT_RST  _TFT_RST       //RST ��λ�˿�
#define	TFT_RD   _TFT_RD	      //RD  ������
#define	TFT_CS   _TFT_CS	      //CS  Ƭѡ�˿�
#define	TFT_WR   _TFT_WR	      //WR  д����
#define TFT_RS   _TFT_RS	      //RS  ����/����ѡ���

#define TFT_DB16 _TFT_DB16
#define TFT_DB17 _TFT_DB17
  
//---�������Ĵ�С---//
#define TFT_XMAX 239		//����TFT���Ĵ�С
#define TFT_YMAX 399

//---������ɫ�ĺ�---//
//#define WHITE          0xFFFF
//#define BLACK          0x0000
//#define BLUE           0x001F
//#define RED            0xF800
//#define MAGENTA        0xF81F
//#define GREEN          0x07E0
//#define CYAN           0x7FFF
//#define YELLOW         0xFFE0		 //������ɫ�ĺ�
//LCD���ʺͱ�����ɫ
#define	POINT_COLOR		0x0000		//������ɫ����ɫ��
#define BACK_COLOR		0xFFFF		//������ɫ����ɫ��
//������ɫ
#ifndef WHITE
#define	WHITE			0xFFFF		//��ɫ
#endif
#ifndef BLACK
#define	BLACK			0x0000		//��ɫ
#endif
#ifndef BLUE
#define	BLUE			0x001F		//��ɫ
#endif
#define	BRED			0XF81F
#define	GRED			0XFFE0
#define	GBLUE			0X07FF
#ifndef RED
#define	RED				0xF800		//��ɫ
#endif
#define	MAGENTA			0xF81F		//���
#ifndef GREEN
#define	GREEN			0x07E0		//��ɫ
#endif
#define	CYAN			0x7FFF		//��ɫ
#define	YELLOW			0xFFE0		//��ɫ
#define	BROWN			0XBC40		//��ɫ
#define	BRRED			0XFC07		//�غ�ɫ
#define	GRAY			0X8430		//��ɫ
//GUI��ɫ
#define	DARKBLUE		0X01CF		//����ɫ
#define	LIGHTBLUE		0X7D7C		//ǳ��ɫ
#define	GRAYBLUE		0X5458		//����ɫ
//������ɫΪPANEL����ɫ
#define	LIGHTGREEN		0X841F		//ǳ��ɫ
#define	LIGHTGRAY		0XEF5B		//ǳ��ɫ(PANNEL)
#define	LGRAY			0XC618		//ǳ��ɫ(PANNEL),���屳��ɫ
#define	LGRAYBLUE		0XA651		//ǳ����ɫ(�м����ɫ)
#define	LBBLUE			0X2B12		//ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)


//---����ȫ�ֱ���---//
void TFT_Init();                     //��ʼ��TFT
void TFT_WriteCmd(uint cmd);
void TFT_WriteData(uint dat);
void TFT_WriteCmdData(uint cmd, uint dat);	 //��д���������д������

//����д�봰�ڵ�����
void TFT_SetWindow(uint xStart, uint yStart, uint xEnd, uint yEnd);  
void TFT_ClearScreen(uint backColor);        //������ʾ

#endif