#ifndef _GUI_H_
#define _GUI_H_

//---����ͷ�ļ�---//
// #include <reg51.H>
// #include "common.h"
#include "include.h"
#include "R61509.h"

//---�ض���ؼ���---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint
#define uint  unsigned int
#endif

//---��ʾͼƬ���������---//
//#define PICTURE_SHOW

//---��ʾ32*29�ĺ���---//
//#define CHAR32_SHOW

//---��ʾ19x24�ĺ���---//
#define CHAR14_SHOW

//---��ʾASCII���Ӧ���ַ�---//
#define USE_ASCII

//---����ȫ�ֱ���---//
void GUI_Dot(uint x, uint y, uint color);  //����
void GUI_Box(uint sx, uint sy, uchar ex, uint ey, uint color);   //������
void GUI_Line(uint xStart, uint yStart, uchar xEnd, uint yEnd, uint color);//������
void GUI_Write32CnChar(uint x, uint y, uchar *cn, uint wordColor, uint backColor);	 
void GUI_Write14CnChar(uint x,uint y,uchar *cn,uint wordColor,uint backColor);	 
void GUI_ShowPicture(uint x, uint y, uchar wide, uint high);
void GUI_WriteASCII(uint x, uint y, uchar *p, uint wordColor, uint backColor);

#endif
