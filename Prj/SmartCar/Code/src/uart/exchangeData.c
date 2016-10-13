#include "exchangeData.h"

/* 发送图像到上位机 */
void send_Img(uint8 *imgAddr)
{
    //uint8 cmd[4] = {0, 255, 1, 0};    						//yy_摄像头串口调试 使用的命令
    //uart_putbuff(VCAN_PORT, cmd, sizeof(cmd));    			//先发送命令
	uart_putchar(VCAN_PORT,0x01);
    uart_putchar(VCAN_PORT,0xfe);
    uart_putbuff(VCAN_PORT, imgAddr, IMAGE_W*IMAGE_H);			//再发送图像
	uart_putchar(VCAN_PORT,0xfe);
	uart_putchar(VCAN_PORT,0x01);
}

/* 虚拟示波器 */
void send_Ware(int8_t *wareAddr, uint8 wareSize)
{
	#define CMD_WARE 3
	uint8 cmdf[2] = {CMD_WARE, ~CMD_WARE};						// 开始命令
	uint8 cmdr[2] = {~CMD_WARE, CMD_WARE};						// 结束命令
	
	uart_putbuff(VCAN_PORT, cmdf, sizeof(cmdf));				// 发送开始命令
	uart_putbuff(VCAN_PORT, (uint8 *)wareAddr, wareSize);		// 发送波形
	uart_putbuff(VCAN_PORT, cmdr, sizeof(cmdr));				// 发送结束命令
}

/* 蓝牙通信，用于控制开车停车 */
void queryBlueTooth()
{
	char blueToothFlag = BT_NO_CHAR;
	if(uart_querychar(UART3, &blueToothFlag) != BT_NO_CHAR)
	{
		switch(blueToothFlag)
		{
		case BT_SERVO_WORK:
			carParams.servoWorkFlag = WORK_MODE;
			break;
		case BT_SERVO_STOP:
			carParams.servoWorkFlag = STOP_MODE;
			break;
		case BT_MOTOR_WORK:
			carParams.motorWorkFlag = WORK_MODE;
			break;
		case BT_MOTOR_STOP:
			carParams.motorWorkFlag = STOP_MODE;
			break;
		default:
			break;
		}
	}
}







