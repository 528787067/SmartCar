#include "touch.h"

void TOUCH_SPI_Start(void)
{
	TOUCH_CLK = 0;
	TOUCH_CS  = 1;
	TOUCH_DIN = 1;
	TOUCH_CLK = 1;
	TOUCH_CS  = 0;		
}

void TOUCH_SPI_Write(uchar dat)
{
	uchar i;
	TOUCH_CLK = 0;
	for(i=0; i<8; i++)
	{
		TOUCH_DIN = dat >> 7;  	//�������λ
		dat <<= 1;
		TOUCH_CLK = 0;			//�����ط�������

		TOUCH_CLK = 1;

	}
}

uint TOUCH_SPI_Read(void)
{
	uint i, dat=0;
	TOUCH_CLK = 0;
	for(i=0; i<12; i++)		//����12λ����
	{
		dat <<= 1;

		TOUCH_CLK = 1;
		TOUCH_CLK = 0;

		dat |= TOUCH_DOUT;

	}
	return dat;	
}

#define XY_READ_TIMS  10		   //��ȡ�Ĵ���

uint TOUCH_XPT_ReadData(uchar cmd)
{
	uchar i, j;
	uint readValue[XY_READ_TIMS];
	long endValue;

	TOUCH_CLK = 0;    //������ʱ��
	TOUCH_CS  = 0;    //ѡ��оƬ
	
	for(i=0; i<XY_READ_TIMS; i++)	 	//��ȡXY_READ_TIMS�ν��
	{	
		TOUCH_SPI_Write(cmd);  //����ת������
		//Delay_6us();
		for(j=6; j>0; j--); 	//��ʱ�ȴ�ת�����
		TOUCH_CLK = 1;	  //����һ��ʱ�����ڣ����BUSY
		/*
		_nop_();
		_nop_();
		*/
		DELAY_US(2);
		
		TOUCH_CLK = 0;
		/*
		_nop_();
		_nop_();
		*/
		DELAY_US(2);
	
		readValue[i] = TOUCH_SPI_Read();
	}
	TOUCH_CS = 1; //�ͷ�Ƭѡ

	//---����˲�---//
	//---�ȴ�С���򣬳�ȥ���ֵ����ȥ���ֵ������ƽ��ֵ---//
   	for(i=0; i<XY_READ_TIMS - 1; i++)	//�Ӵ�С����
	{
		for(j= i+1; j<XY_READ_TIMS; j++)
		{
			if(readValue[i] < readValue[j])
			{
				endValue = readValue[i];
				readValue[i] = readValue[j];
				readValue[j] = endValue;
			}  
		}
	}
//	if((readValue[2] - readValue[3]) > 5)
//	{
//		return 0;
//	}
	endValue = 0;
	for(i=2; i<XY_READ_TIMS-2; i++)		   
	{
		endValue += readValue[i]; 
	}
	endValue = endValue/ (XY_READ_TIMS - 4);//��ƽ��ֵ

	return endValue;
}


XPT_XY xpt_xy;	 //����һ��ȫ�ֱ�������X��Y��ֵ
uchar TOUCH_XPT_ReadXY(void)
{	
	uint x1, x2, x, y1, y2, y;

	TOUCH_SPI_Start();
	//---�ֱ������Xֵ��Yֵ, �����Ŷ��������һЩ��ȡ����---//
	x1 = TOUCH_XPT_ReadData(XPT_CMD_X);
	y1 = TOUCH_XPT_ReadData(XPT_CMD_Y);
	x2 = TOUCH_XPT_ReadData(XPT_CMD_X);
	y2 = TOUCH_XPT_ReadData(XPT_CMD_Y);

	//---��ȡXֵ�Ĳ�ֵ---//
	if (x1 > x2)
	{
		x = x1 - x2;
	}
	else
	{
		x = x2 - x1;
	}

	//---��ȡYֵ�Ĳ�ֵ---//
	if (y1 > y2)
	{
		y = y1 - y2;
	}
	else
	{
		y = y2 - y1;
	}

	//---�жϲ�ֵ�Ƿ����50�����ھͷ���0����ʾ��ȡʧ��---//
	if((x > 50) || (y > 50))  
	{
		return 0;
	}

	//---��ȡ���ζ�ȡֵ��ƽ������Ϊ��ȡ����XYֵ---//
	xpt_xy.x = (x1 + x2) / 2;
	xpt_xy.y = (y1 + y2) / 2;

	xpt_xy.x &= 0xFFF0;	  //ȥ������λ
	xpt_xy.y &= 0xFFF0;

	//---ȷ��XYֵ�ķ�Χ�����ڴ���������TFTʱ---//
	if((xpt_xy.x < 100) || (xpt_xy.y > 3900))
	{
		return 0;
	}

	return 1;  // ����1����ʾ��ȡ�ɹ�
}



