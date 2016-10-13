#ifndef __SD_H
#define __SD_H

//---定义要使用的头文件---//
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

#ifndef ulong
#define ulong unsigned long
#endif

//---定义要使用的IO口---//
/*
sbit SD_SPI_CLK	= P2^1;	 //SD时钟
sbit SD_SPI_MO	= P2^2;	 //SD卡DI输入
sbit SD_SPI_MI	= P2^0;	 //SD卡DO输出

sbit TOUCH_CS	= P2^3;
sbit SD_CS      = P3^7; 
*/
#define SD_SPI_CLK	_SD_SPI_CLK	 //SD时钟
#define SD_SPI_MO	_SD_SPI_MO	 //SD卡DI输入
#define SD_SPI_MI	_SD_SPI_MI	 //SD卡DO输出

#define TOUCH_CS   _TOUCH_CS
#define SD_CS      _SD_CS 

//---定义卡的类型---//
#define SD_TYPE_ERR     0X00
#define SD_TYPE_MMC     0X01
#define SD_TYPE_V1      0X02
#define SD_TYPE_V2      0X04
#define SD_TYPE_V2HC    0X06

//--- SD卡指令表 ---//  	   
#define SD_CMD0    0       //卡复位
#define SD_CMD1    1
#define SD_CMD8    8       //命令8 ，SEND_IF_COND
#define SD_CMD9    9       //命令9 ，读CSD数据
#define SD_CMD10   10      //命令10，读CID数据
#define SD_CMD12   12      //命令12，停止数据传输
#define SD_CMD16   16      //命令16，设置SectorSize 应返回0x00
#define SD_CMD17   17      //命令17，读sector
#define SD_CMD18   18      //命令18，读Multi sector
#define SD_CMD23   23      //命令23，设置多sector写入前预先擦除N个block
#define SD_CMD24   24      //命令24，写sector
#define SD_CMD25   25      //命令25，写Multi sector
#define SD_CMD41   41      //命令41，应返回0x00
#define SD_CMD55   55      //命令55，应返回0x01
#define SD_CMD58   58      //命令58，读OCR信息
#define SD_CMD59   59      //命令59，使能/禁止CRC，应返回0x00

//---添加全局变量--//
extern uchar SPI_LOWMODE;
extern uchar SD_TYPE;

//---声明全局变量---//
void SD_Delay(void);   //误差 0us
void SD_SPI_WriteData(uchar cmd);
uchar SD_SPI_ReadData(void);
uchar SD_WriteCmd(uchar cmd, ulong dat, uchar crc);

uchar SD_Init(void);
uchar SD_ReadBlock(uchar cmd, ulong dat, uchar crc, uchar *buf, uint bytes);
uchar SD_ReadCapacity(ulong *capacity);
uchar SD_ReadDisk(uchar *buf, ulong sector);
uchar SD_WriteDisk(uchar *buf, ulong sector);

#endif