#ifndef __TFT_R61509_H__
#define __TFT_R61509_H__

#include "include.h"
#include "tftInclude.h"
#include "params.h"

/** ---���� TFT ��С�ĺ�---
 * TFT_XMAX 239
 * TFT_YMAX 399
 */
/** ---������ɫ�ĺ�---
 *
 *	POINT_COLOR		0x0000		//������ɫ����ɫ��
 *	BACK_COLOR		0xFFFF		//������ɫ����ɫ��
 * //������ɫ
 *	WHITE			0xFFFF		//��ɫ
 *	BLACK			0x0000		//��ɫ
 *	BLUE			0x001F		//��ɫ
 *	BRED			0XF81F
 *	GRED			0XFFE0
 *	GBLUE			0X07FF
 *	RED				0xF800		//��ɫ
 *	MAGENTA			0xF81F		//���
 *	GREEN			0x07E0		//��ɫ
 *	CYAN			0x7FFF		//��ɫ
 *	YELLOW			0xFFE0		//��ɫ
 *	BROWN			0XBC40		//��ɫ
 *	BRRED			0XFC07		//�غ�ɫ
 *	GRAY			0X8430		//��ɫ
 * //GUI��ɫ
 *	DARKBLUE		0X01CF		//����ɫ
 *	LIGHTBLUE		0X7D7C		//ǳ��ɫ
 *	GRAYBLUE		0X5458		//����ɫ
 * //������ɫΪPANEL����ɫ
 *	LIGHTGREEN		0X841F		//ǳ��ɫ
 *	LIGHTGRAY		0XEF5B		//ǳ��ɫ(PANNEL)
 *	LGRAY			0XC618		//ǳ��ɫ(PANNEL),���屳��ɫ
 *	LGRAYBLUE		0XA651		//ǳ����ɫ(�м����ɫ)
 *	LBBLUE			0X2B12		//ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)
 */

// R61509
#define	tftInit()											TFT_Init()									// ��ʼ�� TFT
#define	tftWriteCmd(cmd)									TFT_WriteCmd(cmd)							// д������
#define	tftWriteData(data)									TFT_WriteData(data)							// д������
#define	tftWriteCmdData(cmd, data)							TFT_WriteCmdData(cmd, data)					// ��д�������д������
#define	tftSetWindow(xStart, yStart, xEnd, yEnd)			TFT_SetWindow(xStart, yStart, xEnd, yEnd)	// ����д�봰������
#define	tftCleanScreen(color)								TFT_ClearScreen(color)						// ������ʾ

// GUI
#define	guiDot(x, y, color) 								GUI_Dot(x, y, color)  						//����
#define	guiBox(sx, sy, ex, ey, color)						GUI_Box(sx, sy, ex, ey, color)   			//������
#define	guiLine(xstart, yStart, xEnd, yEnd, color)			GUI_Line(xStart, yStart, xEnd, yEnd, color)	//������
#define	guiWrite32CnChar(x, y, cn, wordColor, backColor)	GUI_Write32CnChar(x, y, cn, wordColor, backColor);	 
#define	guiWrite14CnChar(x, y, cn, wordColor, backColor)	GUI_Write14CnChar(x, y, cn, wordColor, backColor);	
#define	guiWriteASCII(x, y, p, wordColor, backColor)		GUI_WriteASCII(x, y, p, wordColor, backColor); 
#define	guiShowPicture(x, y, wide, high)					GUI_ShowPicture(x, y, wide, high);

// SD

// TOUCH

// �Զ��庯��
void tftInitWithColor(uint color);

void showInTFT_L(uint8 *img, uint8 x, uint8 y, uint8 pointSize);						// ��ͼ��������ʾ�� TFT �ϣ�ָ�룩
#define showInTFT_L_A(img, x, y, pointSize)	showInTFT_L((uint8 *)img, x, y, pointSize)	// ��ͼ��������ʾ�� TFT �ϣ�ָ������飩

void showInTFT_P(uint8 *img, uint8 x, uint8 y, uint8 pointSize);						// ��ͼ�������ʾ�� TFT �ϣ�ָ�룩
#define showInTFT_P_A(img, x, y, pointSize)	showInTFT_P((uint8 *)img, x, y, pointSize)	// ��ͼ��������ʾ�� TFT �ϣ�ָ������飩

/* ������ʾ���ҵ����� */
void showFindLine_L(int16 *findLine, uint8 x, uint8 y, uint8 pointSize, uint color);
/* ������ʾ���ҵ����� */
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

// ���ܳ�����ʱҺ������ʼ��
void tftInitWithCar();
void tftWorkWithCar();
void tftWorkInReleaseWithCar();
void _cleanImageWithCar();
#define cleanImageWithCar() 				GUI_Box(0, 0, 239, 179, BLACK)

void showValueWithCar(uint8 type, double value);
void showLineWithCar(uint8 pidState);
void settingTextWithCar(uint8 parameter);

// ���ܳ�����ʱ��ʾͼ��
void showImgWithCar(uint8 (* image)[IMAGE_W]);
void showCenterLineWithCar(int16 *centerLine);

#endif