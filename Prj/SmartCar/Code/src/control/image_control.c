#include "image_control.h"
#include "main.h"

#define	IMG_BLACK		0
#define	IMG_WHITE		255
#define FIND_CENTER		0
#define FIND_LEFT		1
#define FIND_RIGHT		2
#define CENTER_POINT	IMAGE_W/2

int16 centerLine[IMAGE_H+1] = {0};			// 最后一个元素用来记录转向点对应的行数
int16 leftLine[IMAGE_H] = {0};
int16 rightLine[IMAGE_H] = {0};

static uint8 leftFindFlag;					// 用来标记左黑线是否找到
static uint8 rightFindFlag;					// 用来标记右黑线是否找到

static int16 leftCount;
static int16 rightCount;
static int16 findLine;

int16 createPoint(int type, int line);

int16 *findCenterLine(uint8 (* image)[IMAGE_W])
{
	int8 temp, tmp;
	// 前十行从中间往两边查找
	for(findLine = IMAGE_H-1; findLine > IMAGE_H-11; findLine--)
	{
		leftFindFlag = 0;
		rightFindFlag = 0;
		for(temp = 0; temp < CENTER_POINT; temp++)
		{
			// 寻找左黑线
			if(leftFindFlag == 0
			&& image[findLine][CENTER_POINT-temp-1] == IMG_BLACK
			&& image[findLine][CENTER_POINT-temp] == IMG_WHITE
			&& image[findLine][CENTER_POINT-temp+1] == IMG_WHITE
			&& image[findLine][CENTER_POINT-temp+2] == IMG_WHITE)
			{
				leftLine[findLine] = CENTER_POINT-temp-1;
				leftFindFlag = 1;
			}
			// 寻找右黑线
			if(rightFindFlag == 0
			&& image[findLine][CENTER_POINT+temp] == IMG_BLACK
			&& image[findLine][CENTER_POINT+temp-1] == IMG_WHITE
			&& image[findLine][CENTER_POINT+temp-2] == IMG_WHITE
			&& image[findLine][CENTER_POINT+temp-3] == IMG_WHITE)
			{
				rightLine[findLine] = CENTER_POINT+temp;
				rightFindFlag = 1;
			}
			if(leftFindFlag == 1 && rightFindFlag == 1)
				break;
		}
		// 对未找到的黑线进行补全
		if(leftFindFlag == 0)	leftLine[findLine] = 0;
		if(rightFindFlag == 0)	rightLine[findLine] = IMAGE_W-1;
		// 对中线进行赋值
		centerLine[findLine] = (leftLine[findLine]+rightLine[findLine])/2;
	}
	
	// 十行后根据前面行位置查找黑线
	for(findLine = IMAGE_H-11; findLine >= 0; findLine--)
	{
		leftFindFlag = 0;
		rightFindFlag = 0;
		// 预测下一行黑线位置
		leftCount = createPoint(FIND_LEFT, findLine);
		rightCount = createPoint(FIND_RIGHT, findLine);
		//leftCount = (2 * leftLine[findLine+1] - leftLine[findLine+2]);
		//rightCount = (2 * rightLine[findLine+1] - rightLine[findLine+2]);
		
		/* 在预测点的左右 FIND_COUNT 个点查找黑线位置 */
		// 寻找左黑线
		for(temp = 0; temp < FIND_COUNT*2+1; temp++)
		{
			if(leftFindFlag != 0)
				break;
			else if((leftCount-temp+FIND_COUNT)+3 > IMAGE_W-1)
				continue;
			else if((leftCount-temp+FIND_COUNT) < 0)
				break;
			else if(image[findLine][leftCount-temp+FIND_COUNT] == IMG_BLACK
			&& image[findLine][leftCount-temp+FIND_COUNT+1] == IMG_WHITE
			&& image[findLine][leftCount-temp+FIND_COUNT+2] == IMG_WHITE
			&& image[findLine][leftCount-temp+FIND_COUNT+3] == IMG_WHITE)
			{
				leftLine[findLine] = leftCount-temp+FIND_COUNT;
				leftFindFlag = 1;
			}
		}
		// 寻找右黑线
		for(temp = 0; temp < FIND_COUNT*2+1; temp++)
		{
			if(rightFindFlag != 0)
				break;
			else if((rightCount+temp-FIND_COUNT)-3 < 0)
				continue;
			else if(rightCount+temp-FIND_COUNT > IMAGE_W-1)
				break;
			else if(image[findLine][rightCount+temp-FIND_COUNT] == IMG_BLACK
			&& image[findLine][rightCount+temp-FIND_COUNT-1] == IMG_WHITE
			&& image[findLine][rightCount+temp-FIND_COUNT-2] == IMG_WHITE
			&& image[findLine][rightCount+temp-FIND_COUNT-3] == IMG_WHITE)
			{
				rightLine[findLine] = rightCount+temp-FIND_COUNT;
				rightFindFlag = 1;
			}
		}
		
		// 补全未找到的左右黑线
		if(leftFindFlag == 0)
			leftLine[findLine] = leftCount;
		if(rightFindFlag == 0)
			rightLine[findLine] = rightCount;
		
		/* 查找中线 */
#if (FIND_TYPE == TYPE_1)				
		
		// 补全未找到的左右黑线
		// if(leftFindFlag == 0)
		// 	leftLine[findLine] = leftCount;
		// if(rightFindFlag == 0)
		// 	rightLine[findLine] = rightCount;
		
		/* 对中线进行赋值 */
		centerLine[findLine] = (leftLine[findLine]+rightLine[findLine])/2;
		if(centerLine[findLine] <= 0)
		{
			centerLine[findLine] = 0;
			break;
		}
		else if(centerLine[findLine] >= IMAGE_W-1)
		{
			centerLine[findLine] = IMAGE_W-1;
			break;
		}		
#else		
		// 补全未找到的左右黑线
		// if(leftFindFlag == 0)
		// 	leftLine[findLine] = (leftCount < 0) ? 0 : ((leftCount > IMAGE_W-1) ? IMAGE_W-1 : leftCount);
		// if(rightFindFlag == 0)
		// 	rightLine[findLine] = (rightCount < 0) ? 0 : ((rightCount > IMAGE_W-1) ? IMAGE_W-1 : rightCount);
		
		/* 对中线进行赋值 */
		// 左右黑线都存在则取左右黑线中值作为黑线值
		if(leftLine[findLine] > 0 && rightLine[findLine] < IMAGE_W-1)
			centerLine[findLine] = (leftLine[findLine]+rightLine[findLine])/2;
		// 左黑线超出范围
		else if(leftLine[findLine] <= 0 && rightLine[findLine] < IMAGE_W-1)
		{
			// 根据右黑线的偏移量来确定中线
			temp = centerLine[findLine+1] + (rightLine[findLine] - rightLine[findLine+1]);
			// 根据最小二乘法补全中线
			//temp = createPoint(FIND_CENTER, findLine);
			if(temp <= 0)
			{
				// 中线超出范围则跳出循环，记录该行为转向行
				centerLine[findLine] = 0;
				break;
			}
			else
				centerLine[findLine] = temp;
		}
		// 右黑线超出范围
		else if(leftLine[findLine] > 0 && rightLine[findLine] >= IMAGE_W-1)
		{
			// 根据左黑线的偏移量来确定中线
			temp = centerLine[findLine+1] + (leftLine[findLine] - leftLine[findLine+1]);
			// 根据最小二乘法补全中线
			//temp = createPoint(FIND_CENTER, findLine);
			if(temp >= IMAGE_W-1)
			{
				// 中线超出范围则跳出循环，记录该行为转向行
				centerLine[findLine] = IMAGE_W-1;
				break;
			}
			else
				centerLine[findLine] = temp;
		}
		// 左右黑线都超出范围
		else
		{
			// 根据最小二乘法补全中线
			temp = createPoint(FIND_CENTER, findLine);
			// 根据中线偏移量补全中线
			//temp = centerLine[findLine+1] + (rightLine[findLine] - rightLine[findLine+1]);
			if(temp <= 0)
			{
				// 中线超出范围则跳出循环，记录该行为转向行
				centerLine[findLine] = 0;
				break;
			}
			else if(temp >= IMAGE_W-1)
			{
				// 中线超出范围则跳出循环，记录该行为转向行
				centerLine[findLine] = IMAGE_W-1;
				break;
			}
			else
				centerLine[findLine] = temp;
		}		
#endif
		
	}
	if(findLine<0 && centerLine[0]<0)
		centerLine[0] = 0;
	else if(findLine<0 && centerLine[0]>IMAGE_W-1)
		centerLine[0] = IMAGE_W-1;
	
	// 最后一个元素用来记录转向行
	centerLine[IMAGE_H] = (findLine < 0) ? 0 : findLine;
	
/* 检查停车线 */
#ifdef RELEASE	
	if(centerLine[IMAGE_H] == 0 
	&& leftLine[0] >= 23 
	&& rightLine[0] < IMAGE_W-23
	&& (centerLine[0]-centerLine[29] <= 3 || centerLine[0]-centerLine[29] >= -3))
	{
		for(temp = IMAGE_H-1; temp >= centerLine[IMAGE_H]; temp--)
		{
			if(carParams.stopCarFlag != STOP_LINE
			&& image[temp][(leftLine[temp]+centerLine[temp])/2] == IMG_BLACK
			&& image[temp][(rightLine[temp]+centerLine[temp])/2] == IMG_BLACK
			&& image[temp][(leftLine[temp]+centerLine[temp])/2-1] == IMG_BLACK
			&& image[temp][(leftLine[temp]+centerLine[temp])/2+1] == IMG_BLACK
			&& image[temp][(rightLine[temp]+centerLine[temp])/2-1] == IMG_BLACK
			&& image[temp][(rightLine[temp]+centerLine[temp])/2+1] == IMG_BLACK
			//&& image[temp][leftLine[temp]+1] == IMG_WHITE
			//&& image[temp][rightLine[temp]-1] == IMG_WHITE
			&& carParams.carRunTime > 3000/ENCODE_TIME)		// 小车起跑超过三秒则为停车线
			{
				for(tmp = (leftLine[temp]+centerLine[temp])/2+1; (rightLine[temp]+centerLine[temp])/2-1; tmp++)
				{
					if(image[temp][tmp] == IMG_WHITE && image[temp][tmp+2] == IMG_WHITE)
					{
						carParams.stopCarFlag = STOP_LINE;
						goto STOP_LINE_PASS;
					}
				}
			}
		}
	STOP_LINE_PASS:
		if(carParams.stopCarFlag == STOP_LINE && carParams.passStopLine != PASS_STOP_LINE && temp < 0)
			carParams.passStopLine = PASS_STOP_LINE;		// 小车已越过停车线
	}
#endif
	
	/*
	// 在 TFT 上绘制出 3 线
	for(temp = IMAGE_H-1; temp >= centerLine[IMAGE_H]; temp--)
	{
		guiDot(3*leftLine[temp], 3*temp, GREEN);
		guiDot(3*rightLine[temp], 3*temp, RED);
		guiDot(3*centerLine[temp], 3*temp, BLUE);
	}
	*/
	
	return (int16 *)centerLine;
}

/* 利用最小二乘法生成需要补全的点 */
int16 createPoint(int type, int line)
{
	int16 *linePointer;
	int8 tmp = 0;
	double sumX = 0;
	double sumY = 0;
	double averageX = 0;
	double averageY = 0;
	double sumUp = 0;
	double sumDown = 0;
	double parameterA;
	double parameterB;
	
	if(type == FIND_LEFT)
		linePointer = &leftLine[line];
	else if(type == FIND_RIGHT)
		linePointer = &rightLine[line];
	else
		linePointer = &centerLine[line];
	
	// 取邻近的 POINT_COUNT 个点进行拟合
	while(++tmp <= POINT_COUNT)
	{
		sumX += (line+tmp);
		sumY += linePointer[tmp];
	}
	--tmp;
	averageX = sumX/tmp;
	averageY = sumY/tmp;
	do
	{
		sumUp += (linePointer[tmp]-averageY) * (line+tmp-averageX);
		sumDown += (line+tmp-averageX) * (line+tmp-averageX);
	} while(--tmp > 0);
	
	if(sumDown == 0)
		parameterB = 0;
	else
		parameterB = sumUp/sumDown;
	parameterA = averageY-parameterB*averageX;
	return (int16)(parameterA+parameterB*line+0.5);
}