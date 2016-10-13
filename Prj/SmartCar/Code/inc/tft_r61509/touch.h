#ifndef __TOUCH_H
#define __TOUCH_H

//---包含头文件---//
// #include <reg51.h>
// #include "common.h"
#include "include.h"
#include "tftPortSet.h"
// #include <intrins.h>

//---重定义关键词---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint
#define uint  unsigned int
#endif

//---定义数据类型---//
typedef struct 
{
	uint x;
	uint y;
} XPT_XY;

extern XPT_XY xpt_xy;

//---定义芯片命令字节---//
#define XPT_CMD_X	0xD0    //读取X轴的命令
#define XPT_CMD_Y	0x90	//读取Y轴的命令

//---定义使用的IO口---//
/*
sbit TOUCH_DOUT = P2^0;	  //输出
sbit TOUCH_CLK  = P2^1;	  //时钟
sbit TOUCH_DIN  = P2^2;	  //输入
sbit TOUCH_CS   = P2^3;	  //片选
sbit TOUCH_PEN  = P2^4;	  //检测触摸屏响应信号
*/
#define TOUCH_DOUT _TOUCH_DOUT	  //输出
#define TOUCH_CLK  _TOUCH_CLK	  //时钟
#define TOUCH_DIN  _TOUCH_DIN	  //输入
#define TOUCH_CS   _TOUCH_CS	  //片选
#define TOUCH_PEN  _TOUCH_PEN	  //检测触摸屏响应信号

//---声明全局函数---//
void TOUCH_SPI_Start(void);
void TOUCH_SPI_Write(uchar dat);
uint TOUCH_SPI_Read(void);
uint TOUCH_XPT_ReadData(uchar cmd);
uchar TOUCH_XPT_ReadXY(void);



#endif
