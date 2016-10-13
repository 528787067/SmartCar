#ifndef __SD_H
#define __SD_H

//---����Ҫʹ�õ�ͷ�ļ�---//
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

#ifndef ulong
#define ulong unsigned long
#endif

//---����Ҫʹ�õ�IO��---//
/*
sbit SD_SPI_CLK	= P2^1;	 //SDʱ��
sbit SD_SPI_MO	= P2^2;	 //SD��DI����
sbit SD_SPI_MI	= P2^0;	 //SD��DO���

sbit TOUCH_CS	= P2^3;
sbit SD_CS      = P3^7; 
*/
#define SD_SPI_CLK	_SD_SPI_CLK	 //SDʱ��
#define SD_SPI_MO	_SD_SPI_MO	 //SD��DI����
#define SD_SPI_MI	_SD_SPI_MI	 //SD��DO���

#define TOUCH_CS   _TOUCH_CS
#define SD_CS      _SD_CS 

//---���忨������---//
#define SD_TYPE_ERR     0X00
#define SD_TYPE_MMC     0X01
#define SD_TYPE_V1      0X02
#define SD_TYPE_V2      0X04
#define SD_TYPE_V2HC    0X06

//--- SD��ָ��� ---//  	   
#define SD_CMD0    0       //����λ
#define SD_CMD1    1
#define SD_CMD8    8       //����8 ��SEND_IF_COND
#define SD_CMD9    9       //����9 ����CSD����
#define SD_CMD10   10      //����10����CID����
#define SD_CMD12   12      //����12��ֹͣ���ݴ���
#define SD_CMD16   16      //����16������SectorSize Ӧ����0x00
#define SD_CMD17   17      //����17����sector
#define SD_CMD18   18      //����18����Multi sector
#define SD_CMD23   23      //����23�����ö�sectorд��ǰԤ�Ȳ���N��block
#define SD_CMD24   24      //����24��дsector
#define SD_CMD25   25      //����25��дMulti sector
#define SD_CMD41   41      //����41��Ӧ����0x00
#define SD_CMD55   55      //����55��Ӧ����0x01
#define SD_CMD58   58      //����58����OCR��Ϣ
#define SD_CMD59   59      //����59��ʹ��/��ֹCRC��Ӧ����0x00

//---���ȫ�ֱ���--//
extern uchar SPI_LOWMODE;
extern uchar SD_TYPE;

//---����ȫ�ֱ���---//
void SD_Delay(void);   //��� 0us
void SD_SPI_WriteData(uchar cmd);
uchar SD_SPI_ReadData(void);
uchar SD_WriteCmd(uchar cmd, ulong dat, uchar crc);

uchar SD_Init(void);
uchar SD_ReadBlock(uchar cmd, ulong dat, uchar crc, uchar *buf, uint bytes);
uchar SD_ReadCapacity(ulong *capacity);
uchar SD_ReadDisk(uchar *buf, ulong sector);
uchar SD_WriteDisk(uchar *buf, ulong sector);

#endif