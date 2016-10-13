#include "tft_r61509.h"
#include <stdio.h>


/* 使用背景色初始化 TFT */
void tftInitWithColor(uint color)
{
	TFT_Init();
	TFT_ClearScreen(color);
}

/* 将图像横屏显示在 TFT 上 */
void showInTFT_P(uint8 *img,uint8 x, uint8 y, uint8 pointSize)
{	
	uint8 row, count, i, j;
	TFT_SetWindow(y, TFT_YMAX-x-(IMAGE_W*pointSize-1), y+IMAGE_H*pointSize-1, TFT_YMAX-x);
	
	
	for(row = IMAGE_W; row > 0; row--)
	{
		// 发布模式时直接返回
		if(carParams.workStatusFlag == RELEASE_MODE)
			return;
		for(i = 0; i < pointSize; i++)
			for(count = 0; count < IMAGE_H; count++)
				for(j = 0; j < pointSize; j++)
				{
				#if (BIN_LEVEL == -1)
					#if (CAMERA == OV7620)
						point = img[count*IMAGE_W+(row-1)];
						TFT_WriteData( ((point/8) << 11)|((point/4) << 5)|(point/8) );
					#else
						TFT_WriteData(img[count*IMAGE_W+(row-1)] == BLACK ? BLACK : WHITE);
					#endif
				#else
					TFT_WriteData(img[count*IMAGE_W+(row-1)] == BLACK ? BLACK : WHITE);
				#endif
				}
	}
}

/* 将图像竖屏显示在 TFT 上 */
void showInTFT_L(uint8 *img, uint8 x, uint8 y, uint8 pointSize)
{		
	uint8 row, count, i, j;
	TFT_SetWindow(x, y, x+IMAGE_W*pointSize-1, y+IMAGE_H*pointSize-1);
	
	for(row = 0; row < IMAGE_H; row++)
	{
		// 发布模式时直接返回
		if(carParams.workStatusFlag == RELEASE_MODE)
			return;
		for(i = 0; i < pointSize; i++)
			for(count = 0; count < IMAGE_W; count++)
				for(j = 0; j < pointSize; j++)
				{
					// 发布模式时直接返回
					if(carParams.workStatusFlag == RELEASE_MODE)
						return;
				#if (BIN_LEVEL == -1)
					#if (CAMERA == OV7620)
						point = img[row*IMAGE_W+count];
						TFT_WriteData( ((point/8) << 11)|((point/4) << 5)|(point/8) );
					#else
						TFT_WriteData(img[row*IMAGE_W+count] == BLACK ? BLACK : WHITE);
					#endif
				#else
					TFT_WriteData(img[row*IMAGE_W+count] == BLACK ? BLACK : WHITE);
				#endif
				}
	}
}

/* 竖屏显示查找到的线 */
void showFindLine_L(int16 *findLine, uint8 x, uint8 y, uint8 pointSize, uint color)
{
	int8 temp;
	for(temp = IMAGE_H-1; temp >= findLine[IMAGE_H]; temp--)
	{
		// 发布模式时直接返回
		if(carParams.workStatusFlag == RELEASE_MODE)
			return;
		guiDot(findLine[temp]*pointSize+x, temp*pointSize+y, color);
	}
}

/* 横屏显示查找到的线 */
void showFindLine_P(int16 *findLine, uint8 x, uint8 y, uint8 pointSize, uint color)
{
	int8 temp;
	for(temp = IMAGE_H-1; temp >= findLine[IMAGE_H]; temp--)
	{
		// 发布模式时直接返回
		if(carParams.workStatusFlag == RELEASE_MODE)
			return;
		guiDot(temp*pointSize+y, (TFT_YMAX-findLine[temp]*pointSize)+x, color);
	}
}


void showTextWithCar();

// 智能车工作液晶初始化
void tftInitWithCar()
{
	TFT_Init();
	if(carParams.workStatusFlag == RELEASE_MODE)
		tftWorkInReleaseWithCar();
	else
		tftWorkWithCar();
}

void tftWorkInReleaseWithCar()
{
	TFT_ClearScreen(BLUE);
}

void _cleanImageWithCar()
{
	GUI_Box(0, 0, 239, 179, BLACK);
}

void tftWorkWithCar()
{
	// 发布模式时直接返回
	if(carParams.workStatusFlag == RELEASE_MODE)
		return;
	else
	{
		TFT_ClearScreen(BLACK);
		showLineWithCar(TFT_PID_WORKING);
		showTextWithCar();
	}
}

void showLineWithCar(uint8 pidState)
{
	uint color = (pidState == TFT_PID_SETTING) ? RED : YELLOW;
	
	// 发布模式时直接返回
	if(carParams.workStatusFlag == RELEASE_MODE)
		return;
	else
	{
		GUI_Line(0, 181, 239, 181, color);
		GUI_Line(0, 250, 239, 250, color);
		GUI_Line(66, 181, 66, 250, color);
		GUI_Line(173, 181, 173, 250, color);
		GUI_Line(119, 250, 119, 399, color);	
	}
}

void showValueWithCar(uint8 type, double value)
{
	uint16 x, y;
	uint8 tmp;
	char buf[15];
	
	// 发布模式时直接返回
	if(carParams.workStatusFlag == RELEASE_MODE)
		return;
	else
	{
		switch(type)
		{
		case TFT_CENTER_LINE:
			x = 10;
			y = 215;
			GUI_WriteASCII(x, y, "   ", RED, BLACK);
			break;
		case TFT_ENCODE:
			x = 76;
			y = 215;
			GUI_WriteASCII(x, y, "      ", RED, BLACK);
			break;
		case TFT_LEVEL:
			x = 183;
			y = 215;
			GUI_WriteASCII(x, y, "   ", RED, BLACK);
			break;
		case TFT_SERVO_P:
			x = 45;
			y = 289;
			GUI_WriteASCII(x, y, "    ", RED, BLACK);
			break;
		case TFT_SERVO_I:
			x = 45;
			y = 313;
			GUI_WriteASCII(x, y, "    ", RED, BLACK);
			break;
		case TFT_SERVO_D:
			x = 45;
			y = 337;
			GUI_WriteASCII(x, y, "    ", RED, BLACK);
			break;
		case TFT_SERVO_PWM:
			x = 45;
			y = 375;
			GUI_WriteASCII(x, y, "    ", RED, BLACK);
			break;
		case TFT_MOTOR_P:
			x = 166;
			y = 289;
			GUI_WriteASCII(x, y, "    ", RED, BLACK);
			break;
		case TFT_MOTOR_I:
			x = 166;
			y = 313;
			GUI_WriteASCII(x, y, "    ", RED, BLACK);
			break;
		case TFT_MOTOR_D:
			x = 166;
			y = 337;
			GUI_WriteASCII(x, y, "    ", RED, BLACK);
			break;
		case TFT_MOTOR_PWM:
			x = 166;
			y = 375;
			GUI_WriteASCII(x, y, "    ", RED, BLACK);
			break;
		default:
			return;
		}
		sprintf(buf, "%d", (int)(value*10));
		for(tmp = 0; tmp < 13; tmp++)
			if(buf[tmp] == '\0')
			{
				if(type == TFT_SERVO_P 
				|| type == TFT_SERVO_I 
				|| type == TFT_SERVO_D 
				|| type == TFT_MOTOR_P 
				|| type == TFT_MOTOR_I 
				|| type == TFT_MOTOR_D)
				{
					if(tmp > 1 && buf[0] != '-')
					{
						buf[tmp+1] = '\0';
						buf[tmp] = buf[tmp-1];
						buf[tmp-1] = '.';
					}
					else
					{
						buf[tmp+2] = '\0';
						buf[tmp+1] = buf[tmp-1];
						buf[tmp] = '.';
						buf[tmp-1] = '0';
					}
					break;
				}
				else
				{
					if(buf[0] != '0')
						buf[tmp-1] = '\0';
					break;				
				}
			}
		GUI_WriteASCII(x, y, (unsigned char *)buf, WHITE, BLACK);
	}
}

void settingTextWithCar(uint8 parameter)
{
	uint16 x, y;
	
	// 发布模式时直接返回
	if(carParams.workStatusFlag == RELEASE_MODE)
		return;
	else
	{
		GUI_WriteASCII(101, 289, " ", RED, BLACK);
		GUI_WriteASCII(101, 313, " ", RED, BLACK);
		GUI_WriteASCII(101, 337, " ", RED, BLACK);
		GUI_WriteASCII(223, 289, " ", RED, BLACK);
		GUI_WriteASCII(223, 313, " ", RED, BLACK);
		GUI_WriteASCII(223, 337, " ", RED, BLACK);
		switch(parameter)
		{
		case TFT_LEVEL:
			GUI_Write14CnChar(183, 191, "级别", RED, BLACK);
			return;
		case TFT_SERVO:
			GUI_Write14CnChar(8, 260, "舵机", RED, BLACK);
			return;
		case TFT_MOTOR:
			GUI_Write14CnChar(129, 260, "电机", RED, BLACK);
			return;		
		case TFT_PID_DEFAULT:	
			GUI_Write14CnChar(183, 191, "级别", WHITE, BLACK);	
			GUI_Write14CnChar(8, 260, "舵机", WHITE, BLACK);
			GUI_Write14CnChar(129, 260, "电机", WHITE, BLACK);
			return;
		case TFT_SERVO_P:
			x = 102;
			y = 289;
			break;
		case TFT_SERVO_I:
			x = 102;
			y = 313;
			break;
		case TFT_SERVO_D:
			x = 102;
			y = 337;
			break;
		case TFT_MOTOR_P:
			x = 223;
			y = 289;
			break;
		case TFT_MOTOR_I:
			x = 223;
			y = 313;
			break;
		case TFT_MOTOR_D:
			x = 223;
			y = 337;
			break;
		default:
			return;
		}
		GUI_WriteASCII(x, y, "*", RED, BLACK);
	}
}

void showImgWithCar(uint8 (* image)[IMAGE_W])
{
	// 发布模式或设置时直接返回
	if(carParams.workStatusFlag == RELEASE_MODE || carParams.settingFlag == SETTING_MODE)
		return;
	else
		showInTFT_L_A(image, 0, 0, 3);
}

void showCenterLineWithCar(int16 *centerLine)
{
	// 发布模式或设置时直接返回
	if(carParams.workStatusFlag == RELEASE_MODE || carParams.settingFlag == SETTING_MODE)
		return;
	else
		showFindLine_L(centerLine, 0, 0, 3, BLUE);
}

void showTextWithCar()
{
	GUI_Write14CnChar(10, 191, "中线", WHITE, BLACK);	
	GUI_Write14CnChar(76, 191, "编码器", WHITE, BLACK);	
	GUI_Write14CnChar(183, 191, "级别", WHITE, BLACK);	
	GUI_Write14CnChar(8, 260, "舵机", WHITE, BLACK);
	GUI_Write14CnChar(129, 260, "电机", WHITE, BLACK);	
	GUI_WriteASCII(8, 289, "P:", WHITE, BLACK);
	GUI_WriteASCII(8, 313, "I:", WHITE, BLACK);
	GUI_WriteASCII(8, 337, "D:", WHITE, BLACK);
	GUI_WriteASCII(8, 375, "->", WHITE, BLACK);	
	GUI_WriteASCII(129, 289, "P:", WHITE, BLACK);
	GUI_WriteASCII(129, 313, "I:", WHITE, BLACK);
	GUI_WriteASCII(129, 337, "D:", WHITE, BLACK);
	GUI_WriteASCII(129, 375, "->", WHITE, BLACK);
	
	showValueWithCar(TFT_CENTER_LINE, carParams.centerLine);
	showValueWithCar(TFT_ENCODE, carParams.encode);
	showValueWithCar(TFT_LEVEL, carParams.pidLevel+1);
	showValueWithCar(TFT_SERVO_P, carParams.servo_P);
	showValueWithCar(TFT_SERVO_I, carParams.servo_I);
	showValueWithCar(TFT_SERVO_D, carParams.servo_D);
	showValueWithCar(TFT_SERVO_PWM, carParams.servo_PWM);
	showValueWithCar(TFT_MOTOR_P, carParams.motor_P);
	showValueWithCar(TFT_MOTOR_I, carParams.motor_I);
	showValueWithCar(TFT_MOTOR_D, carParams.motor_D);
	showValueWithCar(TFT_MOTOR_PWM, carParams.motor_PWM);
	
}
