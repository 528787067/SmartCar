#ifndef __TFT_R61509_H__
#define __TFT_R61509_H__

#include "include.h"
#include "tftInclude.h"
#include "params.h"

/** ---定义 TFT 大小的宏---
 * TFT_XMAX 239
 * TFT_YMAX 399
 */
/** ---定义颜色的宏---
 *
 *	POINT_COLOR		0x0000		//画笔颜色（黑色）
 *	BACK_COLOR		0xFFFF		//背景颜色（白色）
 * //画笔颜色
 *	WHITE			0xFFFF		//白色
 *	BLACK			0x0000		//黑色
 *	BLUE			0x001F		//蓝色
 *	BRED			0XF81F
 *	GRED			0XFFE0
 *	GBLUE			0X07FF
 *	RED				0xF800		//红色
 *	MAGENTA			0xF81F		//洋红
 *	GREEN			0x07E0		//绿色
 *	CYAN			0x7FFF		//青色
 *	YELLOW			0xFFE0		//黄色
 *	BROWN			0XBC40		//棕色
 *	BRRED			0XFC07		//棕红色
 *	GRAY			0X8430		//灰色
 * //GUI颜色
 *	DARKBLUE		0X01CF		//深蓝色
 *	LIGHTBLUE		0X7D7C		//浅蓝色
 *	GRAYBLUE		0X5458		//灰蓝色
 * //以上三色为PANEL的颜色
 *	LIGHTGREEN		0X841F		//浅绿色
 *	LIGHTGRAY		0XEF5B		//浅灰色(PANNEL)
 *	LGRAY			0XC618		//浅灰色(PANNEL),窗体背景色
 *	LGRAYBLUE		0XA651		//浅灰蓝色(中间层颜色)
 *	LBBLUE			0X2B12		//浅棕蓝色(选择条目的反色)
 */

// R61509
#define	tftInit()											TFT_Init()									// 初始化 TFT
#define	tftWriteCmd(cmd)									TFT_WriteCmd(cmd)							// 写入命令
#define	tftWriteData(data)									TFT_WriteData(data)							// 写入数据
#define	tftWriteCmdData(cmd, data)							TFT_WriteCmdData(cmd, data)					// 先写入命令，再写入数据
#define	tftSetWindow(xStart, yStart, xEnd, yEnd)			TFT_SetWindow(xStart, yStart, xEnd, yEnd)	// 设置写入窗口坐标
#define	tftCleanScreen(color)								TFT_ClearScreen(color)						// 清屏显示

// GUI
#define	guiDot(x, y, color) 								GUI_Dot(x, y, color)  						//画点
#define	guiBox(sx, sy, ex, ey, color)						GUI_Box(sx, sy, ex, ey, color)   			//画方框
#define	guiLine(xstart, yStart, xEnd, yEnd, color)			GUI_Line(xStart, yStart, xEnd, yEnd, color)	//画线条
#define	guiWrite32CnChar(x, y, cn, wordColor, backColor)	GUI_Write32CnChar(x, y, cn, wordColor, backColor);	 
#define	guiWrite14CnChar(x, y, cn, wordColor, backColor)	GUI_Write14CnChar(x, y, cn, wordColor, backColor);	
#define	guiWriteASCII(x, y, p, wordColor, backColor)		GUI_WriteASCII(x, y, p, wordColor, backColor); 
#define	guiShowPicture(x, y, wide, high)					GUI_ShowPicture(x, y, wide, high);

// SD

// TOUCH

// 自定义函数
void tftInitWithColor(uint color);

void showInTFT_L(uint8 *img, uint8 x, uint8 y, uint8 pointSize);						// 将图像竖屏显示在 TFT 上（指针）
#define showInTFT_L_A(img, x, y, pointSize)	showInTFT_L((uint8 *)img, x, y, pointSize)	// 将图像竖屏显示在 TFT 上（指针或数组）

void showInTFT_P(uint8 *img, uint8 x, uint8 y, uint8 pointSize);						// 将图像横屏显示在 TFT 上（指针）
#define showInTFT_P_A(img, x, y, pointSize)	showInTFT_P((uint8 *)img, x, y, pointSize)	// 将图像竖横屏示在 TFT 上（指针或数组）

/* 竖屏显示查找到的线 */
void showFindLine_L(int16 *findLine, uint8 x, uint8 y, uint8 pointSize, uint color);
/* 横屏显示查找到的线 */
void showFindLine_P(int16 *findLine, uint8 x, uint8 y, uint8 pointSize, uint color);




#define TFT_CENTER_LINE						0
#define TFT_ENCODE							1
#define TFT_LEVEL							2
#define TFT_SERVO_P							3
#define TFT_SERVO_I							4
#define TFT_SERVO_D							5
#define TFT_SERVO_PWM						6
#define TFT_MOTOR_P							7
#define TFT_MOTOR_I							8
#define TFT_MOTOR_D							9
#define TFT_MOTOR_PWM						10
#define TFT_SERVO							11
#define TFT_MOTOR							12
#define TFT_PID_WORKING						13
#define TFT_PID_SETTING						14
#define TFT_PID_DEFAULT						15

// 智能车工作时液晶屏初始化
void tftInitWithCar();
void tftWorkWithCar();
void tftWorkInReleaseWithCar();
void _cleanImageWithCar();
#define cleanImageWithCar() 				GUI_Box(0, 0, 239, 179, BLACK)

void showValueWithCar(uint8 type, double value);
void showLineWithCar(uint8 pidState);
void settingTextWithCar(uint8 parameter);

// 智能车工作时显示图像
void showImgWithCar(uint8 (* image)[IMAGE_W]);
void showCenterLineWithCar(int16 *centerLine);

#endif