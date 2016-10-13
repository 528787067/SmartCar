#ifndef __TFT_PORT_SET_H__
#define __TFT_PORT_SET_H__

// #include "common.h"
#include "include.h"

/* 管脚配置 */

// 液晶屏管脚配置
#define	_TFT_DATAPORTH	PTD_B1_OUT
#define	_TFT_DATAPORTL	PTD_B0_OUT
#define _TFT_RST  PTE0_OUT        //RST 复位端口
#define	_TFT_RD   PTE1_OUT	      //RD  读数据
#define	_TFT_CS   PTE2_OUT	      //CS  片选端口
#define	_TFT_WR   PTE3_OUT	      //WR  写数据
#define _TFT_RS   PTE4_OUT	      //RS  数据/命令选择端

#define _TFT_DB16 PTE0_OUT
#define _TFT_DB17 PTE5_OUT

// 触摸屏管脚配置
#define _TOUCH_DOUT PTE6_IN		  //输出
#define _TOUCH_CLK  PTE7_OUT	  //时钟
#define _TOUCH_DIN  PTE8_OUT	  //输入
#define _TOUCH_CS   PTE9_OUT	  //片选
#define _TOUCH_PEN  PTE10_OUT	  //检测触摸屏响应信号

// SD 管脚配置
#define _SD_SPI_CLK	PTE7_OUT	 //SD时钟
#define _SD_SPI_MO	PTE8_OUT	 //SD卡DI输入
#define _SD_SPI_MI	PTE6_IN		 //SD卡DO输出

#ifndef	_TOUCH_CS
	#define _TOUCH_CS   PTE9_OUT
#endif
#define _SD_CS      PTE11_OUT 


#endif








