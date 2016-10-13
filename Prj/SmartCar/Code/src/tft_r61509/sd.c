#include "sd.h"

uchar SPI_LOWMODE;
uchar SD_TYPE;

/*******************************************************************************
* �� �� �� : SD_Delay
* �������� : ��ʱ
* ��    �� : ��
* ��    �� : ��
*******************************************************************************/

void SD_Delay(void)   //��� 0us
{
    uchar a;
	a++;
	a++;
}

/*******************************************************************************
* �� �� �� : SD_SPI_Start
* �������� : ��ʼ��SPI��IO
* ��    �� : ��
* ��    �� : ��
*******************************************************************************/

void SD_SPI_Start(void)
{
	TOUCH_CS = 1;        //��Ϊ������SD����SPI�ӿڣ������Ƚ������Ĺرյ�
	SD_SPI_CLK = 1;
	SD_SPI_MI = 1;
	SD_SPI_MO = 1;	
}

/*******************************************************************************
* �� �� �� : SD_SPI_WriteData
* �������� : ʹ��SPIд��һ������
* ��    �� : dat��Ҫ���������
* ��    �� : ��
*******************************************************************************/

void SD_SPI_WriteData(uchar dat)
{
	uchar i;

	for(i=0; i<8; i++)		   //����8λ��
	{
		SD_SPI_MO = dat >> 7;  //�ȷ������λ
		dat <<= 1;			   //���������λ�������λ����

		SD_SPI_CLK = 0;		   //д��ʱ��
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
* �� �� �� : SD_SPI_ReadData
* �������� : ʹ��SPI��ȡһ������
* ��    �� : ��
* ��    �� : readValue:��ȡ��������
*******************************************************************************/

uchar SD_SPI_ReadData(void)
{
	uchar i, readValue = 0;
	
	for(i=0; i<8; i++)		   //����8λ����
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

		readValue <<=1;		   //�����λ����
		readValue |= SD_SPI_MI;//����ȡ������ֵ���õ����λ					
	}
	
	return readValue;     
}

/*******************************************************************************
* �� �� �� : SD_WriteCmd
* �������� : ��SD��д��һ������
* ��    �� : cmd�����
*          * dat: �������
*          * crc: CRCЧ��
* ��    �� : r1:SD�����ص�����
*          * 0xFF:��ʾ����ʧ��
* ��    ע ��SD��һ�������ʽ����8λ���32λ���������7λCRCЧ��λ�����һλΪ1
*******************************************************************************/

uchar SD_WriteCmd(uchar cmd, ulong dat, uchar crc)
{
	uint i, r1 = 0;

	//--��λSD��--//
	SD_CS = 1;
	SD_SPI_Start();
	SD_SPI_WriteData(0xFF);			 //�����ṩ8��ʱ��
	
	SD_CS = 0;
	while(SD_SPI_ReadData() != 0xFF) //�ȴ����Ƿ�׼����
	{
		i++;
		if(i > 200)
		{
			return 0xFF;	                 //�ȴ�ʧ�ܷ���
		}
	}

	//--��������--//
	SD_SPI_WriteData(cmd | 0x40);

	SD_SPI_WriteData(dat >> 24);     //����Dat�����8λ
	SD_SPI_WriteData(dat >> 16);
	SD_SPI_WriteData(dat >> 8);
	SD_SPI_WriteData(dat & 0x00FF);

	SD_SPI_WriteData(crc);

	if(cmd == SD_CMD12)		 //�����ֹͣ���ݴ����������෢һ��ʱ��
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
	while((r1 & 0x80) != 0);   //���ͳɹ������λ��0 

	return r1;
}

/*******************************************************************************
* �� �� �� : SD_Init
* �������� : ��ʼ��SD��
* ��    �� : ��
* ��    �� : 0����ʼ��ʧ��
*		   * 1����ʼ���ɹ�
*******************************************************************************/

uchar SD_Init(void)
{
	uchar i, r1;
	uchar buf[4];

	//--IO�ڳ�ʼ��--//
	SD_CS = 1;
	TOUCH_CS = 1;        //��Ϊ������SD����SPI�ӿڣ������Ƚ������Ĺرյ�
	
	SPI_LOWMODE = 1;		 //ʹ�õ���ģʽ
	SD_SPI_Start();

	//--��SD��ͨ��ģʽת��ΪSPIģʽ���ϵ�Ĭ������DAT0��������--//
	//--�ӵ�CMD0ʱ��CS�ź���Ч��SPIģʽ����--//
	for(i=0; i<0x0F; i++)//��ʼʱ���ȷ�������74��ʱ�ӣ�����Ǳ���ġ�
	{
		SD_SPI_WriteData(0xFF);	 //����120��
	}
	
	//����ȡ��0x01��ʱ���ʾ��ʼ���ɹ�
	i = 0;
	while(SD_WriteCmd(SD_CMD0, 0, 0x95) != 0x01)
	{
		i++;
		if(i > 100)
		{
			return 0;        //��ʼ��ʧ�ܷ���0
		}
	}
		
	//--����CMD8������Ƿ�SD V2.0--//
	i = 0;
	do
	{
		i++;
		if(i > 100)	//���Ƿ��ͳ�����������ѭ����
		{
			break;
		}
		r1 = SD_WriteCmd(SD_CMD8, 0x01AA, 0x87);
	}
	while(r1 != 0x01); //����CMD8
	
	if(r1 == 0x01)     //���CMD8�л�Ӧ˵����SD V2.0Э��
	{
		//--��ȡCMD8�ķ���ֵ������Ƿ�֧�ֵ�ѹ--//
		//--��ȡCMD8�ķ���ֵ������Ƿ�֧�ֵ�ѹ--//
		for(i=0; i<4; i++)
		{
			buf[i] = SD_SPI_ReadData();
		}
		
		//--����ѹ��֧�ֵ�ѹ�����ش���--//
		if((buf[2] != 0x01) || (buf[3] != 0xAA))
		{
			return 0;
		}
		
		//--��ʼ��SD��--//
		i = 0;
		do
		{
			i++;
			if(i > 100)
			{
				return 0;   //����ʧ��
			}

			SD_WriteCmd(SD_CMD55, 0, 0x01);
			r1 = SD_WriteCmd(SD_CMD41, 0x40000000, 0x01);
		}
		while(r1 != 0);

		//--�����SDHC������SD��--//
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

		//--��ȡOCR--//
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
			
	else //�������SD V1.0����MMC V3
	{
		SD_WriteCmd(SD_CMD55, 0x00, 0x01);
		r1 = SD_WriteCmd(SD_CMD41, 0x00, 0x01);

		if(r1 <= 1)	   //��CMD41�л�Ӧ˵����SD V1.0
		{
			SD_TYPE = SD_TYPE_V1;	 //��V1.0��
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

		else		  //û�л�Ӧ˵����MMC V3
		{
			SD_TYPE = SD_TYPE_MMC;	 //��������MMC��
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

	SPI_LOWMODE = 0;	   //ʹ�ø���ģʽ

	SD_CS = 1;		   //ȡ��Ƭѡ
	SD_SPI_WriteData(0xFF); 

	return 1;
}

/*******************************************************************************
* �� �� �� : SD_ReadBlock
* �������� : ����һ��������������ķ���ֵ
* ��    �� : cmd������
*          * dat���������
*          * crc��CRCЧ��
*          * *buf�����ݻ�����
*          * bytes����ȡ����ֵ�ĳ���
* ��    �� : 0����ʼ��ʧ��
*		   * 1����ʼ���ɹ�
*******************************************************************************/

uchar SD_ReadBlock(uchar cmd, ulong dat, uchar crc, uchar *buf, uint bytes)
{
	uchar i;
	
	//--��������--//
	i = 0;
	while(SD_WriteCmd(cmd, dat, crc) != 0);
	{
		i++;
		if(i > 100)
		{
			return 0;  //��������ʧ��
		}
	}
	
	//--�ȴ���ʼ����0XFE--//
	i = 0;
	while(SD_SPI_ReadData() != 0xFE)
	{
		i++;
		if(i > 100)
		{
			return 0;
		}
	}
	
	//--��ȡ����--//
	for(i=0; i<bytes; i++)
	{
		*buf = SD_SPI_ReadData();
		buf++;
	}

	//--��ȡ��λCRCЧ��--//
	SD_SPI_ReadData();	  //RCC
	SD_SPI_ReadData();
	
	//--ȡ��Ƭѡ--//
	SD_CS = 1;			  
	SD_SPI_WriteData(0xFF);

	return 1;	
}

/*******************************************************************************
* �� �� �� : SD_ReadCapacity
* �������� : ��ȡSD�����ڴ�
* ��    �� : *capacity�������ڴ�ֵ�ĵ�ַ
* ��    �� : 0����ʼ��ʧ��
*		   * 1����ʼ���ɹ�
*******************************************************************************/

uchar SD_ReadCapacity(ulong *capacity)
{
	uchar csdValue[16];
	uint n;

	if(SD_ReadBlock(SD_CMD9, 0, 0x01, csdValue, 16) != 1)
	{
		return 0;
	}

	//--SD V2.0�Ŀ�CSD��һ��������0x40--//
	if((csdValue[0] & 0xC0) == 0x40)
	{
		//--����C_SIZE����CSD[69:48]--//
		*capacity = csdValue[9] + ((uint)csdValue[8] << 8) + 1;

		//--ʵ���Ͼ��ǳ���1024--//
		*capacity = (*capacity) << 10;//�õ�������			
	}
	else   
	{
		//--�ڴ��㷨�� capacity = BLOCKNR * BLOCK_LEN--//
		//--BLOCKNR = (C_SIZE + 1) * MULT��--//
		//--BLOCK_LEN = (READ_BL_LEN < 12) ��2^(READ_BL_LEN)--//

		//--����BLOCK_LEN,C_SIZE_MULT��CSD[49:47];READ_BL_LEN��CSD[83:80]--//
		n = (csdValue[5] & 0x0A) + ((csdValue[10] & 0x80) >> 7) 
			 + ((csdValue[9] & 0x03) << 1) + 2;

		//--����C_SIZE��C_SIZE��CSD[73:62]--//
		*capacity = (csdValue[8] >> 6) + ((uint)csdValue[7] << 2) 
		            + ((uint)(csdValue[6] & 3) << 10) + 1;
		*capacity = (*capacity) << (n - 9);//�õ�������	
	}
	return 1;		
}

/*******************************************************************************
* �� �� �� : SD_ReadDisk
* �������� : ��ȡһ��SD������
* ��    �� : *buf�����ݻ�����
*          * sector������
* ��    �� : 0����ʼ��ʧ��
*		   * 1����ʼ���ɹ�
*******************************************************************************/

uchar SD_ReadDisk(uchar *buf, ulong sector)
{
	uchar i;
	uint byte;

	if(SD_TYPE != SD_TYPE_V2HC)
	{
		sector <<= 9; //ת��λ�ֽڵ�ַ
	}
	
	//--���Ͷ�ȡ����--//
	i = 0;
	while(SD_WriteCmd(SD_CMD17, sector, 0x01) != 0); 
	{
		i++;
		if(i > 100)
		{
			return 0;  //�����޷�Ӧ��������������ʧ��
		}	
	}

	//--�ȴ���ʼ����0XFE--//
	i = 0;
	while(SD_SPI_ReadData() != 0xFE)
	{
		i++;
		if(i > 100)
		{
			return 0;
		}
	}

	//--��������--//
	for(byte = 0; byte<512; byte++)
	{
		*buf = SD_SPI_ReadData();
		buf++;	
	}

	//--���������ٶ���λCRCЧ�飬�������ǿ��Բ���Ҫ����--//
	SD_SPI_ReadData();
	SD_SPI_ReadData();			
	
	//--ȡ��Ƭѡ--//
	SD_CS = 1;
	SD_SPI_WriteData(0xFF);	

	return 1;
}

/*******************************************************************************
* �� �� �� : SD_WriteDisk
* �������� : дһ��SD������
* ��    �� : *buf�����ݻ�����
*          * sector������
* ��    �� : 0����ʼ��ʧ��
*		   * 1����ʼ���ɹ�
*******************************************************************************/

uchar SD_WriteDisk(uchar *buf, ulong sector)
{
	uchar i;
	uint byte;

	if(SD_TYPE != SD_TYPE_V2HC)
	{
		sector <<= 9; //ת��λ�ֽڵ�ַ
	}
	
	//--����д����--//
	i = 0;
	while(SD_WriteCmd(SD_CMD24, sector, 0x01) != 0); 
	{
		i++;
		if(i > 100)
		{
			return 0;  //�����޷�Ӧ��������������ʧ��
		}	
	}

	//--�ȴ��������ɸ�ʱ��--//
	i = 0;
	while(SD_SPI_ReadData() != 0xFF)
	{
		if(i > 100)
		{
			return 0;	
		}	
	}

	//--������ʼλ--//
	SD_SPI_WriteData(0xFE);

	//--��ʼд������--//
	for(byte = 0; byte<512; byte++)
	{
		SD_SPI_WriteData(*buf);
		buf++;
	}

	//--������λCRCЧ���룬��㷢--//
	SD_SPI_WriteData(0xFF);
	SD_SPI_WriteData(0xFF);

	//--��ȡ����ֵ--//
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

