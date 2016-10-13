#include "key.h"

#define KEY_0 					0
#define KEY_1 					1
#define KEY_2 					2
#define KEY_3 					3
#define KEY_LEVEL_0				0
#define KEY_LEVEL_1				1
#define KEY_LEVEL_2				2
#define KEY_LEVEL_3				3
#define KEY_LEVEL_4				4
#define PID_SETTING_DEFAULT		0
#define PID_SETTING_SERVO		1
#define PID_SETTING_MOTOR		2
#define SETTING_DEFAULT			0
#define SETTING_P				1
#define SETTING_I				2
#define SETTING_D				3
#define PID_LEVEL_1				0
#define PID_LEVEL_2				1
#define PID_LEVEL_3				2
#define PID_ADD					0
#define PID_SUB					1

KEY_MSG_t keymsg;
volatile static uint8 keyLevel;
volatile static uint8 settingType;
volatile static uint8 pidType;

void lptmr_hander(void);
void keyHander();
void key_0_hander();
void key_1_hander();
void key_2_hander();
void key_3_hander();
void setPidParameter(uint8 changeType);
void saveParams();

PTXn_e keyMap[4] = {PTC0, PTC1, PTC2, PTC3};

void keyInit()
{				
	keyLevel = KEY_LEVEL_0;
	settingType = PID_SETTING_DEFAULT;
	pidType = SETTING_DEFAULT;
	
	// 初始化全部按键
	key_map_init(keyMap);
	// LPTMR 定时 10 ms
	lptmr_timing_ms(10);  
	// 设置中断服务函数到中断向量表里					
	set_vector_handler(LPTMR_VECTORn,lptmr_hander);   
	// 使能 LPTMR 中断
	enable_irq(LPTMR_IRQn);  
}

void keyStartScan()
{
	// 使能 LPTMR 中断
	enable_irq(LPTMR_IRQn);  
}

void keyStopScan()
{
	// 禁止 LPTMR 中断
	disable_irq(LPTMR_IRQn);  
}

void lptmr_hander(void)
{
	LPTMR0_CSR |= LPTMR_CSR_TCF_MASK;   // 清除 LPT 比较标志位
	key_IRQHandler();					// 把按键扫描程序加入到定时中断服务函数里，定时执行
	if(get_key_msg(&keymsg) == 1)
		keyHander();
}

void keyHander()
{
	// 发布模式时屏蔽其他按键
	if(carParams.workStatusFlag == RELEASE_MODE && keymsg.key != KEY_0)
		return;
	
	if(keymsg.status == KEY_DOWN)		// 按键按下
		switch(keymsg.key)
		{
		case KEY_0:
			key_0_hander();
			break;
		case KEY_1:
			key_1_hander();
			break;
		case KEY_2:
			key_2_hander();
			break;
		case KEY_3:
			key_3_hander();
			break;
		default:
			return;
		}
	else if(keymsg.status == KEY_UP);	// 按键弹起
	else								// 按键长按
	{
		switch(keymsg.key)
		{
		case KEY_1:
			if(keyLevel == KEY_LEVEL_3)
				setPidParameter(PID_ADD);
			break;
		case KEY_2:
			if(keyLevel == KEY_LEVEL_3)
				setPidParameter(PID_SUB);
			break;
		case KEY_3:
			keyLevel = KEY_LEVEL_0;
			settingType = PID_SETTING_DEFAULT;
			pidType = SETTING_DEFAULT;
			saveParams();
			showLineWithCar(TFT_PID_WORKING);
			settingTextWithCar(TFT_PID_DEFAULT);
			if(carParams.settingFlag == SETTING_MODE)
				carParams.settingFlag = SHOWING_MODE;
			break;
		default:
			return;
		}
	}
}

/* 按键0处理事件 */
void key_0_hander()
{
	switch(keyLevel)
	{
	case KEY_LEVEL_0:
		carParams.workStatusFlag = !(carParams.workStatusFlag);
		if(carParams.workStatusFlag == RELEASE_MODE)
			tftWorkInReleaseWithCar();
		else
			tftWorkWithCar();
		break;
	case KEY_LEVEL_1:
		keyLevel = KEY_LEVEL_4;
		settingTextWithCar(TFT_LEVEL);
		break;
	case KEY_LEVEL_2:
		keyLevel = KEY_LEVEL_3;
		pidType = SETTING_P;
		settingTextWithCar((settingType == PID_SETTING_SERVO) ? TFT_SERVO_P : TFT_MOTOR_P);
		break;
	case KEY_LEVEL_3:
		keyLevel = KEY_LEVEL_0;
		settingType = PID_SETTING_DEFAULT;
		pidType = SETTING_DEFAULT;
		saveParams();
		showLineWithCar(TFT_PID_WORKING);
		settingTextWithCar(TFT_PID_DEFAULT);		
		if(carParams.settingFlag == SETTING_MODE)
			carParams.settingFlag = SHOWING_MODE;
		break;
	case KEY_LEVEL_4:
		carParams.pidLevel = PID_LEVEL_1;
		showValueWithCar(TFT_LEVEL, carParams.pidLevel+1);
		break;
	default:
		break;
	}
}
/* 按键1处理事件 */
void key_1_hander()
{
	switch(keyLevel)
	{
	case KEY_LEVEL_0:
		carParams.servoWorkFlag = !(carParams.servoWorkFlag);
		servoTurnToAngle(SERVO_CENTER);
		break;
	case KEY_LEVEL_1:
		keyLevel = KEY_LEVEL_2;
		settingType = PID_SETTING_SERVO;
		settingTextWithCar(TFT_SERVO);
		break;
	case KEY_LEVEL_2:
		keyLevel = KEY_LEVEL_3;
		pidType = SETTING_I;
		settingTextWithCar((settingType == PID_SETTING_SERVO) ? TFT_SERVO_I : TFT_MOTOR_I);
		break;
	case KEY_LEVEL_3:
		setPidParameter(PID_ADD);
		break;
	case KEY_LEVEL_4:
		carParams.pidLevel = PID_LEVEL_2;
		showValueWithCar(TFT_LEVEL, carParams.pidLevel+1);
		break;
	default:
		break;
	}
}
/* 按键2处理事件 */
void key_2_hander()
{
	switch(keyLevel)
	{
	case KEY_LEVEL_0:
		carParams.motorWorkFlag = !(carParams.motorWorkFlag);
		motorChangeToSpeed(STOP, 0);
		break;
	case KEY_LEVEL_1:
		keyLevel = KEY_LEVEL_2;
		settingType = PID_SETTING_MOTOR;
		settingTextWithCar(TFT_MOTOR);
		break;
	case KEY_LEVEL_2:
		keyLevel = KEY_LEVEL_3;
		pidType = SETTING_D;
		settingTextWithCar((settingType == PID_SETTING_MOTOR) ? TFT_MOTOR_D : TFT_SERVO_D);
		break;
	case KEY_LEVEL_3:
		setPidParameter(PID_SUB);
		break;
	case KEY_LEVEL_4:
		carParams.pidLevel = PID_LEVEL_3;
		showValueWithCar(TFT_LEVEL, carParams.pidLevel+1);
		break;
	default:
		break;
	}
}
/* 按键3处理事件 */
void key_3_hander()
{
	switch(keyLevel)
	{
	case KEY_LEVEL_0:
		if(carParams.settingFlag == SHOWING_MODE)
		{
			carParams.settingFlag = SETTING_MODE;
			cleanImageWithCar();
		}
		keyLevel = KEY_LEVEL_1;
		showLineWithCar(TFT_PID_SETTING);
		break;
	case KEY_LEVEL_1:
		keyLevel = KEY_LEVEL_0;
		saveParams();
		showLineWithCar(TFT_PID_WORKING);
		if(carParams.settingFlag == SETTING_MODE)
			carParams.settingFlag = SHOWING_MODE;
		break;
	case KEY_LEVEL_2:
		keyLevel = KEY_LEVEL_1;
		settingTextWithCar(TFT_PID_DEFAULT);
		break;
	case KEY_LEVEL_3:
		keyLevel = KEY_LEVEL_2;		
		if(settingType == PID_SETTING_SERVO)			
		   settingTextWithCar(TFT_SERVO);
		else if(settingType == PID_SETTING_MOTOR)			
		   settingTextWithCar(TFT_MOTOR);
		else
		   settingTextWithCar(TFT_PID_SETTING);
		break;
	case KEY_LEVEL_4:
		keyLevel = KEY_LEVEL_1;
		settingTextWithCar(TFT_PID_DEFAULT);
		break;
	default:
		break;
	}
}

// 改变 PID 参数
void setPidParameter(uint8 changeType)
{
	if(settingType == PID_SETTING_SERVO && pidType == SETTING_P)
	{
		(changeType == PID_ADD) ? ((carParams.servo_P)+=0.1) : ((carParams.servo_P)-=0.1);
		showValueWithCar(TFT_SERVO_P, carParams.servo_P);
	}
	else if(settingType == PID_SETTING_SERVO && pidType == SETTING_I)
	{
		(changeType == PID_ADD) ? ((carParams.servo_I)+=0.1) : ((carParams.servo_I)-=0.1);
		showValueWithCar(TFT_SERVO_I, carParams.servo_I);
	}
	else if(settingType == PID_SETTING_SERVO && pidType == SETTING_D)
	{
		(changeType == PID_ADD) ? ((carParams.servo_D)+=0.1) : ((carParams.servo_D)-=0.1);
		showValueWithCar(TFT_SERVO_D, carParams.servo_D);
	}
	else if(settingType == PID_SETTING_MOTOR && pidType == SETTING_P)
	{
		(changeType == PID_ADD) ? ((carParams.motor_P)+=0.1) : ((carParams.motor_P)-=0.1);
		showValueWithCar(TFT_MOTOR_P, carParams.motor_P);
	}
	else if(settingType == PID_SETTING_MOTOR && pidType == SETTING_I)
	{
		(changeType == PID_ADD) ? ((carParams.motor_I)+=0.1) : ((carParams.motor_I)-=0.1);
		showValueWithCar(TFT_MOTOR_I, carParams.motor_I);
	}
	else if(settingType == PID_SETTING_MOTOR && pidType == SETTING_D)
	{
		(changeType == PID_ADD) ? ((carParams.motor_D)+=0.1) : ((carParams.motor_D)-=0.1);
		showValueWithCar(TFT_MOTOR_D, carParams.motor_D);
	}
	else
		return;
}


void saveParams()
{
	saveParamsToFlash();
	updateServoPid(carParams.servo_P, carParams.servo_I, carParams.servo_D);
	updateMotorPid(carParams.motor_P, carParams.motor_I, carParams.motor_D);
}