#include "exchangeData.h"

/* ����ͼ����λ�� */
void send_Img(uint8 *imgAddr)
{
    //uint8 cmd[4] = {0, 255, 1, 0};    						//yy_����ͷ���ڵ��� ʹ�õ�����
    //uart_putbuff(VCAN_PORT, cmd, sizeof(cmd));    			//�ȷ�������
	uart_putchar(VCAN_PORT,0x01);
    uart_putchar(VCAN_PORT,0xfe);
    uart_putbuff(VCAN_PORT, imgAddr, IMAGE_W*IMAGE_H);			//�ٷ���ͼ��
	uart_putchar(VCAN_PORT,0xfe);
	uart_putchar(VCAN_PORT,0x01);
}

/* ����ʾ���� */
void send_Ware(int8_t *wareAddr, uint8 wareSize)
{
	#define CMD_WARE 3
	uint8 cmdf[2] = {CMD_WARE, ~CMD_WARE};						// ��ʼ����
	uint8 cmdr[2] = {~CMD_WARE, CMD_WARE};						// ��������
	
	uart_putbuff(VCAN_PORT, cmdf, sizeof(cmdf));				// ���Ϳ�ʼ����
	uart_putbuff(VCAN_PORT, (uint8 *)wareAddr, wareSize);		// ���Ͳ���
	uart_putbuff(VCAN_PORT, cmdr, sizeof(cmdr));				// ���ͽ�������
}

/* ����ͨ�ţ����ڿ��ƿ���ͣ�� */
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







