#include "sd.h"

uchar SPI_LOWMODE;
uchar SD_TYPE;

/*******************************************************************************
* 函 数 名 : SD_Delay
* 函数功能 : 延时
* 输    入 : 无
* 输    出 : 无
*******************************************************************************/

void SD_Delay(void)   //误差 0us
{
    uchar a;
	a++;
	a++;
}

/*******************************************************************************
* 函 数 名 : SD_SPI_Start
* 函数功能 : 初始化SPI的IO
* 输    入 : 无
* 输    出 : 无
*******************************************************************************/

void SD_SPI_Start(void)
{
	TOUCH_CS = 1;        //因为触摸和SD共用SPI接口，所以先将触摸的关闭掉
	SD_SPI_CLK = 1;
	SD_SPI_MI = 1;
	SD_SPI_MO = 1;	
}

/*******************************************************************************
* 函 数 名 : SD_SPI_WriteData
* 函数功能 : 使用SPI写入一个数据
* 输    入 : dat：要输入的数据
* 输    出 : 无
*******************************************************************************/

void SD_SPI_WriteData(uchar dat)
{
	uchar i;

	for(i=0; i<8; i++)		   //发送8位数
	{
		SD_SPI_MO = dat >> 7;  //先发送最高位
		dat <<= 1;			   //发送完最高位，将最高位移走

		SD_SPI_CLK = 0;		   //写入时序
		if(SPI_LOWMODE)
		{
			SD_Delay();
			SD_Delay();
		}
		SD_SPI_CLK = 1;
		if(SPI_LOWMODE)
		{
			SD_Delay();
			SD_Delay();
		}
	}
	SD_SPI_MO = 1;		
}

/*******************************************************************************
* 函 数 名 : SD_SPI_ReadData
* 函数功能 : 使用SPI读取一个数据
* 输    入 : 无
* 输    出 : readValue:读取到的数据
*******************************************************************************/

uchar SD_SPI_ReadData(void)
{
	uchar i, readValue = 0;
	
	for(i=0; i<8; i++)		   //接收8位数据
	{
		SD_SPI_CLK = 0;
		if(SPI_LOWMODE)
		{
			SD_Delay();
		}
		SD_SPI_CLK = 1;
		if(SPI_LOWMODE)
		{
			SD_Delay();
		}

		readValue <<=1;		   //将最低位移走
		readValue |= SD_SPI_MI;//将读取到的数值放置到最低位					
	}
	
	return readValue;     
}

/*******************************************************************************
* 函 数 名 : SD_WriteCmd
* 函数功能 : 向SD卡写入一个命令
* 输    入 : cmd：命令；
*          * dat: 命令参数
*          * crc: CRC效验
* 输    出 : r1:SD卡返回的数据
*          * 0xFF:表示发送失败
* 备    注 ：SD卡一个命令格式包括8位命令，32位命令参数和7位CRC效验位，最后一位为1
*******************************************************************************/

uchar SD_WriteCmd(uchar cmd, ulong dat, uchar crc)
{
	uint i, r1 = 0;

	//--复位SD卡--//
	SD_CS = 1;
	SD_SPI_Start();
	SD_SPI_WriteData(0xFF);			 //额外提供8个时钟
	
	SD_CS = 0;
	while(SD_SPI_ReadData() != 0xFF) //等待卡是否准备好
	{
		i++;
		if(i > 200)
		{
			return 0xFF;	                 //等待失败返回
		}
	}

	//--发送数据--//
	SD_SPI_WriteData(cmd | 0x40);

	SD_SPI_WriteData(dat >> 24);     //发送Dat的最高8位
	SD_SPI_WriteData(dat >> 16);
	SD_SPI_WriteData(dat >> 8);
	SD_SPI_WriteData(dat & 0x00FF);

	SD_SPI_WriteData(crc);

	if(cmd == SD_CMD12)		 //如果是停止数据传输命令，额外多发一个时钟
	{
		SD_SPI_WriteData(0xFF);
	}

	i = 0;
	do
	{
		r1 = SD_SPI_ReadData();	
		i++;
		if(i > 200)
		{
			return 0xFF;
		}
	}
	while((r1 & 0x80) != 0);   //发送成功的最高位是0 

	return r1;
}

/*******************************************************************************
* 函 数 名 : SD_Init
* 函数功能 : 初始化SD卡
* 输    入 : 无
* 输    出 : 0：初始化失败
*		   * 1：初始化成功
*******************************************************************************/

uchar SD_Init(void)
{
	uchar i, r1;
	uchar buf[4];

	//--IO口初始化--//
	SD_CS = 1;
	TOUCH_CS = 1;        //因为触摸和SD共用SPI接口，所以先将触摸的关闭掉
	
	SPI_LOWMODE = 1;		 //使用低速模式
	SD_SPI_Start();

	//--将SD卡通信模式转换为SPI模式，上电默认是用DAT0作数据线--//
	//--接到CMD0时，CS信号有效，SPI模式启动--//
	for(i=0; i<0x0F; i++)//初始时，先发送至少74个时钟，这个是必须的。
	{
		SD_SPI_WriteData(0xFF);	 //发送120个
	}
	
	//当读取到0x01的时候表示初始化成功
	i = 0;
	while(SD_WriteCmd(SD_CMD0, 0, 0x95) != 0x01)
	{
		i++;
		if(i > 100)
		{
			return 0;        //初始化失败返回0
		}
	}
		
	//--发送CMD8，检测是否SD V2.0--//
	i = 0;
	do
	{
		i++;
		if(i > 100)	//若是发送超过次数跳出循环管
		{
			break;
		}
		r1 = SD_WriteCmd(SD_CMD8, 0x01AA, 0x87);
	}
	while(r1 != 0x01); //发送CMD8
	
	if(r1 == 0x01)     //如果CMD8有回应说明是SD V2.0协议
	{
		//--读取CMD8的返回值，检测是否支持电压--//
		//--读取CMD8的返回值，检测是否支持电压--//
		for(i=0; i<4; i++)
		{
			buf[i] = SD_SPI_ReadData();
		}
		
		//--卡电压不支持电压，返回错误--//
		if((buf[2] != 0x01) || (buf[3] != 0xAA))
		{
			return 0;
		}
		
		//--初始化SD卡--//
		i = 0;
		do
		{
			i++;
			if(i > 100)
			{
				return 0;   //返回失败
			}

			SD_WriteCmd(SD_CMD55, 0, 0x01);
			r1 = SD_WriteCmd(SD_CMD41, 0x40000000, 0x01);
		}
		while(r1 != 0);

		//--检测是SDHC卡还是SD卡--//
		i = 0;
		while(SD_WriteCmd(SD_CMD58, 0, 0x01) != 0)
		{
			i++;
			if(i > 100)
			{
				SD_TYPE = SD_TYPE_ERR;
				break;
			}		
		}

		//--读取OCR--//
		for(i=0; i<4; i++)
		{
			buf[i] = SD_SPI_ReadData();
		}

		if(buf[0] & 0x40) 
		{
			SD_TYPE = SD_TYPE_V2HC;
		}
		else
		{
			SD_TYPE = SD_TYPE_V2;
		}	
	}
			
	else //否则就是SD V1.0或者MMC V3
	{
		SD_WriteCmd(SD_CMD55, 0x00, 0x01);
		r1 = SD_WriteCmd(SD_CMD41, 0x00, 0x01);

		if(r1 <= 1)	   //对CMD41有回应说明是SD V1.0
		{
			SD_TYPE = SD_TYPE_V1;	 //是V1.0卡
			i = 0;
			do
			{
				if(i > 100)
				{
					return 0;
				}

				SD_WriteCmd(SD_CMD55, 0x00, 0x01);
				r1 = SD_WriteCmd(SD_CMD41, 0x00, 0x01);	
			}
			while(r1 != 0);	
		}

		else		  //没有回应说明是MMC V3
		{
			SD_TYPE = SD_TYPE_MMC;	 //卡类型是MMC卡
			i = 0;
			while(SD_WriteCmd(SD_CMD1, 0, 0x01) != 0)
			{
				i++;
				if(i > 100)
				{
					return 0;
				}
			}	
		}
	}		

	SPI_LOWMODE = 0;	   //使用高速模式

	SD_CS = 1;		   //取消片选
	SD_SPI_WriteData(0xFF); 

	return 1;
}

/*******************************************************************************
* 函 数 名 : SD_ReadBlock
* 函数功能 : 发送一个命令，并接受它的返回值
* 输    入 : cmd：命令
*          * dat：命令参数
*          * crc：CRC效验
*          * *buf：数据缓存区
*          * bytes：读取返回值的长度
* 输    出 : 0：初始化失败
*		   * 1：初始化成功
*******************************************************************************/

uchar SD_ReadBlock(uchar cmd, ulong dat, uchar crc, uchar *buf, uint bytes)
{
	uchar i;
	
	//--发送命令--//
	i = 0;
	while(SD_WriteCmd(cmd, dat, crc) != 0);
	{
		i++;
		if(i > 100)
		{
			return 0;  //发送命令失败
		}
	}
	
	//--等待起始令牌0XFE--//
	i = 0;
	while(SD_SPI_ReadData() != 0xFE)
	{
		i++;
		if(i > 100)
		{
			return 0;
		}
	}
	
	//--读取数据--//
	for(i=0; i<bytes; i++)
	{
		*buf = SD_SPI_ReadData();
		buf++;
	}

	//--读取两位CRC效验--//
	SD_SPI_ReadData();	  //RCC
	SD_SPI_ReadData();
	
	//--取消片选--//
	SD_CS = 1;			  
	SD_SPI_WriteData(0xFF);

	return 1;	
}

/*******************************************************************************
* 函 数 名 : SD_ReadCapacity
* 函数功能 : 读取SD卡的内存
* 输    入 : *capacity：保存内存值的地址
* 输    出 : 0：初始化失败
*		   * 1：初始化成功
*******************************************************************************/

uchar SD_ReadCapacity(ulong *capacity)
{
	uchar csdValue[16];
	uint n;

	if(SD_ReadBlock(SD_CMD9, 0, 0x01, csdValue, 16) != 1)
	{
		return 0;
	}

	//--SD V2.0的卡CSD第一个数据是0x40--//
	if((csdValue[0] & 0xC0) == 0x40)
	{
		//--计算C_SIZE，在CSD[69:48]--//
		*capacity = csdValue[9] + ((uint)csdValue[8] << 8) + 1;

		//--实际上就是乘以1024--//
		*capacity = (*capacity) << 10;//得到扇区数			
	}
	else   
	{
		//--内存算法是 capacity = BLOCKNR * BLOCK_LEN--//
		//--BLOCKNR = (C_SIZE + 1) * MULT；--//
		//--BLOCK_LEN = (READ_BL_LEN < 12) 或2^(READ_BL_LEN)--//

		//--计算BLOCK_LEN,C_SIZE_MULT在CSD[49:47];READ_BL_LEN在CSD[83:80]--//
		n = (csdValue[5] & 0x0A) + ((csdValue[10] & 0x80) >> 7) 
			 + ((csdValue[9] & 0x03) << 1) + 2;

		//--计算C_SIZE，C_SIZE在CSD[73:62]--//
		*capacity = (csdValue[8] >> 6) + ((uint)csdValue[7] << 2) 
		            + ((uint)(csdValue[6] & 3) << 10) + 1;
		*capacity = (*capacity) << (n - 9);//得到扇区数	
	}
	return 1;		
}

/*******************************************************************************
* 函 数 名 : SD_ReadDisk
* 函数功能 : 读取一个SD卡扇区
* 输    入 : *buf：数据缓存区
*          * sector：扇区
* 输    出 : 0：初始化失败
*		   * 1：初始化成功
*******************************************************************************/

uchar SD_ReadDisk(uchar *buf, ulong sector)
{
	uchar i;
	uint byte;

	if(SD_TYPE != SD_TYPE_V2HC)
	{
		sector <<= 9; //转换位字节地址
	}
	
	//--发送读取命令--//
	i = 0;
	while(SD_WriteCmd(SD_CMD17, sector, 0x01) != 0); 
	{
		i++;
		if(i > 100)
		{
			return 0;  //命令无反应，表明发送命令失败
		}	
	}

	//--等待起始令牌0XFE--//
	i = 0;
	while(SD_SPI_ReadData() != 0xFE)
	{
		i++;
		if(i > 100)
		{
			return 0;
		}
	}

	//--接收数据--//
	for(byte = 0; byte<512; byte++)
	{
		*buf = SD_SPI_ReadData();
		buf++;	
	}

	//--读完数据再读两位CRC效验，但是我们可以不需要它们--//
	SD_SPI_ReadData();
	SD_SPI_ReadData();			
	
	//--取消片选--//
	SD_CS = 1;
	SD_SPI_WriteData(0xFF);	

	return 1;
}

/*******************************************************************************
* 函 数 名 : SD_WriteDisk
* 函数功能 : 写一个SD卡扇区
* 输    入 : *buf：数据缓存区
*          * sector：扇区
* 输    出 : 0：初始化失败
*		   * 1：初始化成功
*******************************************************************************/

uchar SD_WriteDisk(uchar *buf, ulong sector)
{
	uchar i;
	uint byte;

	if(SD_TYPE != SD_TYPE_V2HC)
	{
		sector <<= 9; //转换位字节地址
	}
	
	//--发送写命令--//
	i = 0;
	while(SD_WriteCmd(SD_CMD24, sector, 0x01) != 0); 
	{
		i++;
		if(i > 100)
		{
			return 0;  //命令无反应，表明发送命令失败
		}	
	}

	//--等待发送若干个时钟--//
	i = 0;
	while(SD_SPI_ReadData() != 0xFF)
	{
		if(i > 100)
		{
			return 0;	
		}	
	}

	//--发送起始位--//
	SD_SPI_WriteData(0xFE);

	//--开始写入数据--//
	for(byte = 0; byte<512; byte++)
	{
		SD_SPI_WriteData(*buf);
		buf++;
	}

	//--发送两位CRC效验码，随便发--//
	SD_SPI_WriteData(0xFF);
	SD_SPI_WriteData(0xFF);

	//--读取返回值--//
	i = SD_SPI_ReadData();

	if((i & 0x1F) != 0x05)
	{
	 	SD_CS = 1;
		return 0;
	}

	SD_CS = 1;
	SD_SPI_WriteData(0xFF);
	return 1;
}

