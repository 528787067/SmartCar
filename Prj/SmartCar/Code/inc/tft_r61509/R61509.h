#ifndef __R61509_H_
#define __R61509_H_

//---添加单片机的头文件---//
// #include <reg51.h>
// #include "common.h"
#include "include.h"
#include "tftPortSet.h"

//---重定义关键词---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint
#define uint  unsigned int
#endif

//---重定义使用的IO口---//
/*
#define TFT_DATAPORTH P1	  
#define TFT_DATAPORTL P0
sbit TFT_RST  = P3^3;       //RST 复位端口
sbit TFT_RD   = P2^6;	      //RD  读数据
sbit TFT_CS   = P2^7;	      //CS  片选端口
sbit TFT_WR   = P2^5;	      //WR  写数据
sbit TFT_RS   = P3^2;	      //RS  数据/命令选择端

sbit TFT_DB16 = P3^3;
sbit TFT_DB17 = P3^4;
*/
#define	TFT_DATAPORTH	_TFT_DATAPORTH
#define	TFT_DATAPORTL	_TFT_DATAPORTL
#define TFT_RST  _TFT_RST       //RST 复位端口
#define	TFT_RD   _TFT_RD	      //RD  读数据
#define	TFT_CS   _TFT_CS	      //CS  片选端口
#define	TFT_WR   _TFT_WR	      //WR  写数据
#define TFT_RS   _TFT_RS	      //RS  数据/命令选择端

#define TFT_DB16 _TFT_DB16
#define TFT_DB17 _TFT_DB17
  
//---定义屏的大小---//
#define TFT_XMAX 239		//设置TFT屏的大小
#define TFT_YMAX 399

//---定义颜色的宏---//
//#define WHITE          0xFFFF
//#define BLACK          0x0000
//#define BLUE           0x001F
//#define RED            0xF800
//#define MAGENTA        0xF81F
//#define GREEN          0x07E0
//#define CYAN           0x7FFF
//#define YELLOW         0xFFE0		 //定义颜色的宏
//LCD画笔和背景颜色
#define	POINT_COLOR		0x0000		//画笔颜色（黑色）
#define BACK_COLOR		0xFFFF		//背景颜色（白色）
//画笔颜色
#ifndef WHITE
#define	WHITE			0xFFFF		//白色
#endif
#ifndef BLACK
#define	BLACK			0x0000		//黑色
#endif
#ifndef BLUE
#define	BLUE			0x001F		//蓝色
#endif
#define	BRED			0XF81F
#define	GRED			0XFFE0
#define	GBLUE			0X07FF
#ifndef RED
#define	RED				0xF800		//红色
#endif
#define	MAGENTA			0xF81F		//洋红
#ifndef GREEN
#define	GREEN			0x07E0		//绿色
#endif
#define	CYAN			0x7FFF		//青色
#define	YELLOW			0xFFE0		//黄色
#define	BROWN			0XBC40		//棕色
#define	BRRED			0XFC07		//棕红色
#define	GRAY			0X8430		//灰色
//GUI颜色
#define	DARKBLUE		0X01CF		//深蓝色
#define	LIGHTBLUE		0X7D7C		//浅蓝色
#define	GRAYBLUE		0X5458		//灰蓝色
//以上三色为PANEL的颜色
#define	LIGHTGREEN		0X841F		//浅绿色
#define	LIGHTGRAY		0XEF5B		//浅灰色(PANNEL)
#define	LGRAY			0XC618		//浅灰色(PANNEL),窗体背景色
#define	LGRAYBLUE		0XA651		//浅灰蓝色(中间层颜色)
#define	LBBLUE			0X2B12		//浅棕蓝色(选择条目的反色)


//---声明全局变量---//
void TFT_Init();                     //初始化TFT
void TFT_WriteCmd(uint cmd);
void TFT_WriteData(uint dat);
void TFT_WriteCmdData(uint cmd, uint dat);	 //先写入命令，接着写入数据

//设置写入窗口的坐标
void TFT_SetWindow(uint xStart, uint yStart, uint xEnd, uint yEnd);  
void TFT_ClearScreen(uint backColor);        //清屏显示

#endif