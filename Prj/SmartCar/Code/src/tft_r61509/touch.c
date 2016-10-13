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
		TOUCH_DIN = dat >> 7;  	//放置最高位
		dat <<= 1;
		TOUCH_CLK = 0;			//上升沿放置数据

		TOUCH_CLK = 1;

	}
}

uint TOUCH_SPI_Read(void)
{
	uint i, dat=0;
	TOUCH_CLK = 0;
	for(i=0; i<12; i++)		//接收12位数据
	{
		dat <<= 1;

		TOUCH_CLK = 1;
		TOUCH_CLK = 0;

		dat |= TOUCH_DOUT;

	}
	return dat;	
}

#define XY_READ_TIMS  10		   //读取的次数

uint TOUCH_XPT_ReadData(uchar cmd)
{
	uchar i, j;
	uint readValue[XY_READ_TIMS];
	long endValue;

	TOUCH_CLK = 0;    //先拉低时间
	TOUCH_CS  = 0;    //选中芯片
	
	for(i=0; i<XY_READ_TIMS; i++)	 	//读取XY_READ_TIMS次结果
	{	
		TOUCH_SPI_Write(cmd);  //发送转换命令
		//Delay_6us();
		for(j=6; j>0; j--); 	//延时等待转换结果
		TOUCH_CLK = 1;	  //发送一个时钟周期，清除BUSY
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
	TOUCH_CS = 1; //释放片选

	//---软件滤波---//
	//---先大到小排序，除去最高值，除去最低值，求其平均值---//
   	for(i=0; i<XY_READ_TIMS - 1; i++)	//从大到小排序
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
	endValue = endValue/ (XY_READ_TIMS - 4);//求平均值

	return endValue;
}


XPT_XY xpt_xy;	 //定义一个全局变量保存X、Y的值
uchar TOUCH_XPT_ReadXY(void)
{	
	uint x1, x2, x, y1, y2, y;

	TOUCH_SPI_Start();
	//---分别读两次X值和Y值, 交叉着读可以提高一些读取精度---//
	x1 = TOUCH_XPT_ReadData(XPT_CMD_X);
	y1 = TOUCH_XPT_ReadData(XPT_CMD_Y);
	x2 = TOUCH_XPT_ReadData(XPT_CMD_X);
	y2 = TOUCH_XPT_ReadData(XPT_CMD_Y);

	//---求取X值的差值---//
	if (x1 > x2)
	{
		x = x1 - x2;
	}
	else
	{
		x = x2 - x1;
	}

	//---求取Y值的差值---//
	if (y1 > y2)
	{
		y = y1 - y2;
	}
	else
	{
		y = y2 - y1;
	}

	//---判断差值是否大于50，大于就返回0，表示读取失败---//
	if((x > 50) || (y > 50))  
	{
		return 0;
	}

	//---求取两次读取值的平均数作为读取到的XY值---//
	xpt_xy.x = (x1 + x2) / 2;
	xpt_xy.y = (y1 + y2) / 2;

	xpt_xy.x &= 0xFFF0;	  //去掉低四位
	xpt_xy.y &= 0xFFF0;

	//---确定XY值的范围，用在触摸屏大于TFT时---//
	if((xpt_xy.x < 100) || (xpt_xy.y > 3900))
	{
		return 0;
	}

	return 1;  // 返回1，表示读取成功
}



