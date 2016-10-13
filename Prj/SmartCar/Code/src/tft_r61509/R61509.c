#include "R61509.h"

/****************************************************************************
*��������TFT_WriteCmd
*��  �룺cmd��д���ʮ������
*      * cmh��д���18λ�����е������λ
*��  ����
*��  �ܣ���TFT��д��18λ���ݡ�
****************************************************************************/
uint8 j;
void TFT_WriteCmd(uint cmd)
{
    uchar cmdH, cmdL;
    
    cmdH = cmd >> 8;
    cmdL = cmd & 0x00FF;

    TFT_WR = 1; 
	TFT_CS = 0;	//��Ƭѡ
	TFT_RS = 0;	//ѡ��д���� 

    /* �������� */
	TFT_DATAPORTH = cmdH;
    TFT_DATAPORTL = cmdL;
	
	
	TFT_WR=0; 
	for(j=0; j<1; j++);
	TFT_WR=1;
	TFT_CS=1;	//�ر�Ƭѡ 
}

/****************************************************************************
*��������TFT_WriteData
*��  �룺dat��д���16λ����
*      * dah��д���18λ�����е����2λ
*��  ����
*��  �ܣ���TFT��д��18λ���
****************************************************************************/

void TFT_WriteData(uint dat)
{ 
    uchar datH, datL;

    datH = dat >> 8;
    datL = dat & 0x00FF;

    TFT_WR = 1; 
	TFT_CS = 0;  //��Ƭѡ 
	TFT_RS = 1;  //ѡ��д����  
	
	/* �������� */
	TFT_DATAPORTH = datH;
    TFT_DATAPORTL = datL;
	 
	
	TFT_WR=0; 
	for(j=0; j<1; j++);
	TFT_WR=1; 
	TFT_CS=1;//�ر�Ƭѡ  
}
/****************************************************************************
*������: TFT_WriteCmdData
*��  �룺cmd��16λ����
*      * dat��16λ����
*��  ����
*��  �ܣ���TFT����д��16λ�����д��16λ����
****************************************************************************/

void TFT_WriteCmdData(uint cmd, uint dat)
{
	TFT_WriteCmd(cmd);
	TFT_WriteData(dat);	
}

/**
 * ��ʼ���ܽ�
 */
void portInit()
{
	gpio_init(PTD0,GPO,1);
	gpio_init(PTD1,GPO,1);
	gpio_init(PTD2,GPO,1);
	gpio_init(PTD3,GPO,1);
	gpio_init(PTD4,GPO,1);
	gpio_init(PTD5,GPO,1);
	gpio_init(PTD6,GPO,1);
	gpio_init(PTD7,GPO,1);
	gpio_init(PTD8,GPO,1);
	gpio_init(PTD9,GPO,1);
	gpio_init(PTD10,GPO,1);
	gpio_init(PTD11,GPO,1);
	gpio_init(PTD12,GPO,1);
	gpio_init(PTD13,GPO,1);
	gpio_init(PTD14,GPO,1);
	gpio_init(PTD15,GPO,1);
	
	gpio_init(PTE0,GPO,1);
	gpio_init(PTE1,GPO,1);
	gpio_init(PTE2,GPO,1);
	gpio_init(PTE3,GPO,1);
	gpio_init(PTE4,GPO,1);
	gpio_init(PTE5,GPO,1);
	gpio_init(PTE6,GPI,0);		//���ڽ�������
	gpio_init(PTE7,GPO,1);
	gpio_init(PTE8,GPO,1);
	gpio_init(PTE9,GPO,1);
	gpio_init(PTE10,GPO,1);
	gpio_init(PTE11,GPO,1);
}
 
/****************************************************************************
*������: TFT_Init
*��  �룺��
*��  ������
*��  �ܣ���ʼ��TFT��
****************************************************************************/

void TFT_Init(void)
{
	uint i;
	
	portInit();	//��ʼ���ܽ�
	
	TFT_CS=1;   //�ر�Ƭѡ
	TFT_RD=1;	//IO��ʼ��				
	TFT_WR=1;	//IO��ʼ��
	
	TFT_RST = 1;
	for(i=500; i>0; i--);
	TFT_RST = 0;
	for(i=500; i>0; i--);
	TFT_RST = 1;
	for(i=5000; i>0; i--);	
	
	TFT_CS =0;  //��Ƭѡʹ��
	for(i=0; i<500; i++);
    
    TFT_WriteCmdData(0x0400,0x3102);
    TFT_WriteCmdData(0x0007,0x0021);
    for(i=0; i<5000; i++);
    TFT_WriteCmdData(0x0110,0x0001);
    for(i=0; i<5000; i++);  
    TFT_WriteCmdData(0x0100,0x17B0);
    TFT_WriteCmdData(0x0112,0x0060);
    TFT_WriteCmdData(0x0010,0x001F);
    TFT_WriteCmdData(0x0101,0x0014);
    TFT_WriteCmdData(0x0102,0x019E);
    TFT_WriteCmdData(0x0103,0x3200);
    TFT_WriteCmdData(0x0107,0x0000);
    for(i=0; i<5000; i++);
    TFT_WriteCmdData(0x0102,0x01BE);
    for(i=0; i<5000; i++);  

    TFT_WriteCmdData(0x0001,0x0100);
    TFT_WriteCmdData(0x0002,0x0000);
    TFT_WriteCmdData(0x0003,0x1230);
    TFT_WriteCmdData(0x0008,0x0808); 
    TFT_WriteCmdData(0x0009,0x0000);
    TFT_WriteCmdData(0x000B,0x0010);
    TFT_WriteCmdData(0x000C,0x0000);
    TFT_WriteCmdData(0x0090,0x0000);
    TFT_WriteCmdData(0x0300,0x0706);
    TFT_WriteCmdData(0x0301,0x0206);
    TFT_WriteCmdData(0x0302,0x0002);
    TFT_WriteCmdData(0x0303,0x0201);
    TFT_WriteCmdData(0x0304,0x0300);
    TFT_WriteCmdData(0x0305,0x0207);
    TFT_WriteCmdData(0x0306,0x1416);
    TFT_WriteCmdData(0x0307,0x0706);

    TFT_WriteCmdData(0x0308,0x0206);
    TFT_WriteCmdData(0x0309,0x0002);
    TFT_WriteCmdData(0x030A,0x0201);
    TFT_WriteCmdData(0x030B,0x0300);   
    TFT_WriteCmdData(0x030C,0x0207);
    TFT_WriteCmdData(0x030D,0x090B);
    //gamma
    TFT_WriteCmdData(0x0210,0x0000);
    TFT_WriteCmdData(0x0211,0x00EF);
    TFT_WriteCmdData(0x0212,0x0000);
    TFT_WriteCmdData(0x0213,0x018F);
    TFT_WriteCmdData(0x0401,0x0000);
    TFT_WriteCmdData(0x0404,0x0000);
    TFT_WriteCmdData(0x0010,0x0010);
    TFT_WriteCmdData(0x0011,0x0002);
    TFT_WriteCmdData(0x0012,0x0100);
    TFT_WriteCmdData(0x0101,0x0017);
    TFT_WriteCmdData(0x0200,0x0000);
    TFT_WriteCmdData(0x0201,0x0000);
    TFT_WriteCmd(0x0202);   
    //display on
    TFT_WriteCmdData(0x0007,0x0021);
    for(i=0; i<5000; i++);
    TFT_WriteCmdData(0x0007,0x0061);
    for(i=0; i<5000; i++); 
    TFT_WriteCmdData(0x0007,0x0173);
    for(i=0; i<5000; i++);  
    TFT_WriteCmd(0x0202);   
}

/****************************************************************************
*��������void TFT_Setwindow
*���룺xStart����ʵX����
*    * yStart����ʵY����
*    * xEnd������X����
*    * yEnd������Y����
*�����
*���ܣ����ô��ڵķ�Χ����ʼ��ַ����ֹ��ַ��
****************************************************************************/
void TFT_SetWindow(uint xStart, uint yStart,uint xEnd, uint yEnd) 
{
	TFT_WriteCmdData(0x200, xStart); 
    TFT_WriteCmdData(0x201, yStart); 

    TFT_WriteCmdData(0x0210, xStart);
    TFT_WriteCmdData(0x0212,yStart);
    
    TFT_WriteCmdData(0x211,xEnd);
    TFT_WriteCmdData(0x213,yEnd);

	TFT_WriteCmd(0x0202);
}

/****************************************************************************
*��������TFT_ClearScreen
*��  �룺backColor��16λ�����ֵ
*��  ����
*��  �ܣ�������Ϳ����ɫ��
****************************************************************************/	  
void TFT_ClearScreen(uint color)
{
  	uint i, j;

	TFT_SetWindow(0,0,TFT_XMAX,TFT_YMAX);	 //��������
  	for(i=0; i<TFT_XMAX+1; i++)
	{
		for (j=0; j<TFT_YMAX+1; j++)
		{
            TFT_WriteData(color);
		}

	}
}
