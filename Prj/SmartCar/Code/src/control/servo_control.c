#include "servo_control.h"
#include "main.h"

#define IMG_CENTER	IMAGE_W/2


int16 calculateTurnAngle(int16 *centerLine)
{
	int16 angle = SERVO_CENTER;
	int16 startCount = centerLine[IMAGE_H-1];
	int16 middleCount = centerLine[(IMAGE_H-1+centerLine[IMAGE_H])/2];
	int16 endCount = centerLine[centerLine[IMAGE_H]];
	
	int16 end_middle = endCount - middleCount;
	int16 middle_start = middleCount - startCount;
	//int16 end_start = endCount - startCount;
	
	//int16 end_middle_abs = (end_middle<0) ? (0-end_middle) : end_middle;
	int16 middle_start_abs = (middle_start<0) ? (0-middle_start) : middle_start;
	//int16 end_start_abs = (end_start<0) ? (0-end_start) : end_start;
	
	int16 parameterX0;
	int16 parameterY0;
	int16 parameterX1;
	int16 parameterY1;
	double parameterK;
	double parameterB;
	int16 pointX;
	//int16 pointY;
	
	/* 单左拐或单右拐或直线或小 S */
	if((end_middle >= 0 && middle_start >= 0) || (end_middle <= 0 && middle_start <= 0) || middle_start_abs <= 3)
	{
		if(centerLine[IMAGE_H] > 0)
			angle = (centerLine[centerLine[IMAGE_H]]>=IMG_CENTER)
				  ? ((SERVO_CENTER-IMG_CENTER)-centerLine[IMAGE_H]*3)
				  : ((SERVO_CENTER+IMG_CENTER)+centerLine[IMAGE_H]*3);
		else
			angle = SERVO_CENTER-(centerLine[0]-IMG_CENTER);
	}

	/* 先右拐后左拐或先左拐先右拐 */
	else if((middle_start > 0 && end_middle < 0) || (middle_start < 0 && end_middle > 0))
	{
		parameterX0 = startCount;
		parameterY0 = IMAGE_H - 1;
		parameterX1 = middleCount;
		parameterY1 = (IMAGE_H-1+centerLine[IMAGE_H])/2;
		parameterK = (parameterY0 - parameterY1)/(parameterX0 - parameterX1);
		parameterB = parameterY0 - parameterK * parameterX0;			
		
		pointX = (int16)((0-parameterB/parameterK) + 0.5);
		
		if(pointX < 0)
		{
			//pointY = (int16)parameterB;
			//angle = (SERVO_CENTER+IMG_CENTER)+pointY*3;
			angle = SERVO_CENTER + 20;
		}
		else if(pointX > IMAGE_W-1)
		{
			//pointY = (int16)((IMAGE_W-1)*parameterK+parameterB+0.5);
			//angle = (SERVO_CENTER-IMG_CENTER)-pointY*3;
			angle = SERVO_CENTER - 20;
		}
		else
			// angle = SERVO_CENTER-(pointX-IMG_CENTER);
			angle = (int16)((SERVO_CENTER - (pointX-IMG_CENTER)/2) + 0.5);
	}

	// 中线偏移补给
	angle -= ((centerLine[IMAGE_H-1] - IMG_CENTER) * 3); 	
	carParams.servo_PWM = calculatePidAngle(angle);
	
	/* 赋值中线 */
	if(angle > SERVO_LEFT_MAX)
		angle = SERVO_LEFT_MAX;
	else if(angle < SERVO_RIGHT_MAX)
		angle = SERVO_RIGHT_MAX;	
	if((end_middle >= 0 && middle_start >= 0) || (end_middle <= 0 && middle_start <= 0) || middle_start_abs <= 3)
		carParams.centerLine = (int8)((SERVO_CENTER-angle)/10+0.5);
	else
		carParams.centerLine = 10;	
	if(carParams.centerLine > 10)
		carParams.centerLine = 10;
	else if(carParams.centerLine < -10)
		carParams.centerLine = -10;
	
	return carParams.servo_PWM;
}



typedef struct
{
	int16 preOneDeviation;		// 代表上一场与目标角度的偏差
	int16 preTwoDeviation;		// 代表上两场与目标角度的偏差
	int16 pidAngle;				// 代表 pid 算出的角度
} anglePID;

typedef struct
{
	float proportion;			// 比例常数
	float integral;				// 积分常数
	float derviative;			// 微分常数
} anglePidParams;

static anglePID anglePid;
static anglePidParams angleParams;

void servoPidInit(float _proportion, float _integral, float _derviative)
{
	anglePid.preOneDeviation = 0;
	anglePid.preTwoDeviation = 0;
	anglePid.pidAngle = 0;
	
	angleParams.proportion = _proportion;
	angleParams.integral = _integral;
	angleParams.derviative = _derviative;
}

void updateServoPid(float _proportion, float _integral, float _derviative)
{
	angleParams.proportion = _proportion;
	angleParams.integral = _integral;
	angleParams.derviative = _derviative;
}

int16 calculatePidAngle(int16 purpostAngle)
{
	int16 currentDeviation = purpostAngle - anglePid.pidAngle;
	
	//double increase = (carParams.encode>encodeTable[15]) ? (carParams.encode/encodeTable[15])
	//	: ((carParams.encode<-encodeTable[15]) ? -(carParams.encode/encodeTable[15]) : 1);
	
	double angleIncremental = angleParams.proportion * currentDeviation// * increase
		+ angleParams.integral * (currentDeviation - anglePid.preOneDeviation)
		+ angleParams.derviative * ((currentDeviation - anglePid.preOneDeviation) - (anglePid.preOneDeviation - anglePid.preTwoDeviation));
	
	anglePid.preOneDeviation = currentDeviation;
	anglePid.preTwoDeviation = anglePid.preOneDeviation;
	
	anglePid.pidAngle += (int16)(angleIncremental+0.5);
	
	//anglePid.pidAngle *= increase;
	
	if(anglePid.pidAngle > SERVO_LEFT_MAX)
		anglePid.pidAngle = SERVO_LEFT_MAX;
	else if(anglePid.pidAngle < SERVO_RIGHT_MAX)
		anglePid.pidAngle = SERVO_RIGHT_MAX;
	
	return anglePid.pidAngle;
}











































//int16 calculateTurnAngle(int16 *center)
//{
//	int16 angle = SERVO_CENTER;
//		
//	if(center[IMAGE_H] > 0)
//		angle = (center[center[IMAGE_H]]>=IMG_CENTER)
//			  ? ((SERVO_CENTER-40)-center[IMAGE_H]*2)
//			  : ((SERVO_CENTER+40)+center[IMAGE_H]*2);
//	else
//		angle = SERVO_CENTER-(center[0]-40);
// 
//	angle -= (center[IMAGE_H-1]-IMG_CENTER);
//	
// 
//	if(angle > SERVO_LEFT_MAX)
//		angle = SERVO_LEFT_MAX;
//	else if(angle < SERVO_RIGHT_MAX)
//		angle = SERVO_RIGHT_MAX;
//	
//	carParams.centerLine = SERVO_CENTER-angle;
//	carParams.servo_PWM = calculatePidAngle(angle);	
//	return carParams.servo_PWM;
//}

//int16 calculateTurnAngle(int16 *center)
//{
//	int16 angle = SERVO_CENTER;
//	if(center[IMAGE_H] > 30)
//		angle = center[center[IMAGE_H]] < IMG_CENTER ? SERVO_LEFT_LEVEL10 : SERVO_RIGHT_LEVEL10;
//	else if(center[IMAGE_H] <= 30 && center[IMAGE_H] > 25)
//		angle = center[center[IMAGE_H]] < IMG_CENTER ? SERVO_LEFT_LEVEL9 : SERVO_RIGHT_LEVEL9;
//	else if(center[IMAGE_H] <= 25 && center[IMAGE_H] > 20)
//		angle = center[center[IMAGE_H]] < IMG_CENTER ? SERVO_LEFT_LEVEL8 : SERVO_RIGHT_LEVEL8;
//	else if(center[IMAGE_H] <= 20 && center[IMAGE_H] > 15)
//		angle = center[center[IMAGE_H]] < IMG_CENTER ? SERVO_LEFT_LEVEL7 : SERVO_RIGHT_LEVEL7;
//	else if(center[IMAGE_H] <= 15 && center[IMAGE_H] > 10)
//		angle = center[center[IMAGE_H]] < IMG_CENTER ? SERVO_LEFT_LEVEL6 : SERVO_RIGHT_LEVEL6;
//	else if(center[IMAGE_H] <= 10 && center[IMAGE_H] > 5)
//		angle = center[center[IMAGE_H]] < IMG_CENTER ? SERVO_LEFT_LEVEL5 : SERVO_RIGHT_LEVEL5;
//	else if(center[IMAGE_H] <= 5 && center[IMAGE_H] > 0)
//		angle = center[center[IMAGE_H]] < IMG_CENTER ? SERVO_LEFT_LEVEL4 : SERVO_RIGHT_LEVEL4;
//	else
//	{		
//		if(center[0] > 30 && center[0] <= 49)
//			angle = SERVO_CENTER;
//		
//		else if(center[0] > 20 && center[0] <= 30)
//			angle = SERVO_LEFT_LEVEL1;
//		else if(center[0] > 49 && center[0] <= 59)
//			angle = SERVO_RIGHT_LEVEL1;
//		
//		else if(center[0] > 10 && center[0] <= 20)
//			angle = SERVO_LEFT_LEVEL2;
//		else if(center[0] > 59 && center[0] <= 69)
//			angle = SERVO_RIGHT_LEVEL2;
//		
//		else if(/*center[0] >= 0 && */center[0] <= 10)
//			angle = SERVO_LEFT_LEVEL3;
//		else if(center[0] > 69/* && center[0] <= 79*/)
//			angle = SERVO_RIGHT_LEVEL3;
//	}
//	if(center[IMAGE_H-1] < 34 && center[IMAGE_H-1] >= 30)
//		angle = (angle+1 > SERVO_LEFT_MAX) ? SERVO_LEFT_MAX : angle+1;
//	else if(center[IMAGE_H-1] > 44 && center[IMAGE_H-1] <= 49)
//		angle = (angle-1 < SERVO_RIGHT_MAX) ? SERVO_RIGHT_MAX : angle-1;
//		
//	else if(center[IMAGE_H-1] < 30)
//		angle = (angle+2 > SERVO_LEFT_MAX) ? SERVO_LEFT_MAX : angle+2;
//	else if(center[IMAGE_H-1] > 49)
//		angle = (angle-2 < SERVO_RIGHT_MAX) ? SERVO_RIGHT_MAX : angle-2;
//		
//	carParams.centerLine = SERVO_CENTER-angle;
//	carParams.servo_PWM = calculatePidAngle(angle);
//	
//	return carParams.servo_PWM;
//}