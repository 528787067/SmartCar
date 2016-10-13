#include "flash.h"

#define PARAMS_ADDR			(FLASH_SECTOR_NUM-1)

#define FLASH_START_ADDR	(0 * 8)
#define FLASH_PID_LEVEL		(1 * 8)
#define FLASH_SERVO_P		(2 * 8)
#define FLASH_SERVO_I		(3 * 8)
#define FLASH_SERVO_D		(4 * 8)
#define FLASH_MOTOR_P		(5 * 8)
#define FLASH_MOTOR_I		(6 * 8)
#define FLASH_MOTOR_D		(7 * 8)
#define FLASH_END_ADDR		(8 * 8)

#define FLASH_START_CODE	0XAB
#define FLASH_END_CODE		~FLASH_START_CODE

void flashInit()
{
	flash_init();	
	if(flash_read(PARAMS_ADDR, FLASH_START_ADDR, uint32) == FLASH_START_CODE
	&& flash_read(PARAMS_ADDR, FLASH_END_ADDR, uint32) == FLASH_END_CODE)
		carParams.flashDataFlag = FLASH_TRUE;
	else
		carParams.flashDataFlag = FLASH_FALSE;
}

void getParamsFromFlash()
{
	if(flash_read(PARAMS_ADDR, FLASH_START_ADDR, uint32) == FLASH_START_CODE
	&& flash_read(PARAMS_ADDR, FLASH_END_ADDR, uint32) == FLASH_END_CODE)
		carParams.flashDataFlag = FLASH_TRUE;
	else
		carParams.flashDataFlag = FLASH_FALSE;
	carParams.pidLevel = flash_read(PARAMS_ADDR, FLASH_PID_LEVEL, uint32);	
	carParams.servo_P = *((float *)&flash_read(PARAMS_ADDR, FLASH_SERVO_P, int32));
	carParams.servo_I = *((float *)&flash_read(PARAMS_ADDR, FLASH_SERVO_I, int32));
	carParams.servo_D = *((float *)&flash_read(PARAMS_ADDR, FLASH_SERVO_D, int32));
	carParams.motor_P = *((float *)&flash_read(PARAMS_ADDR, FLASH_MOTOR_P, int32));
	carParams.motor_I = *((float *)&flash_read(PARAMS_ADDR, FLASH_MOTOR_I, int32));
	carParams.motor_D = *((float *)&flash_read(PARAMS_ADDR, FLASH_MOTOR_D, int32));
}

uint8 saveParamsToFlash()
{
	// 写入之前需要先擦除 flash
	flash_erase_sector(PARAMS_ADDR);
	
	// 写入数据到 flash，偏移地址为 0，必须一次写入 4 字节
	if(flash_write(PARAMS_ADDR, FLASH_START_ADDR, FLASH_START_CODE) == 1
	&& flash_write(PARAMS_ADDR, FLASH_PID_LEVEL, carParams.pidLevel) == 1
	&& flash_write(PARAMS_ADDR, FLASH_SERVO_P, *((int32 *)&carParams.servo_P)) == 1
	&& flash_write(PARAMS_ADDR, FLASH_SERVO_I, *((int32 *)&carParams.servo_I)) == 1
	&& flash_write(PARAMS_ADDR, FLASH_SERVO_D, *((int32 *)&carParams.servo_D)) == 1
	&& flash_write(PARAMS_ADDR, FLASH_MOTOR_P, *((int32 *)&carParams.motor_P)) == 1
	&& flash_write(PARAMS_ADDR, FLASH_MOTOR_I, *((int32 *)&carParams.motor_I)) == 1
	&& flash_write(PARAMS_ADDR, FLASH_MOTOR_D, *((int32 *)&carParams.motor_D)) == 1
	&& flash_write(PARAMS_ADDR, FLASH_END_ADDR, FLASH_END_CODE) == 1)	
	{
		carParams.flashDataFlag = FLASH_TRUE;
		return FLASH_SUCCESS;
	}
	else
	{
		carParams.flashDataFlag = FLASH_FALSE;
		return FLASH_FAIL;
	}
}

void cleanAllParamsFromFlash()
{
	flash_erase_sector(PARAMS_ADDR);
}





