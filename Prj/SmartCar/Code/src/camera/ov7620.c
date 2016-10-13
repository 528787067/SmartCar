#include "ov7620.h"
#include "Landzo.h"

#define		SUCCESS			1
#define		FAIL 			0

#define		MAX_WIDTH		240							//ͼ�������
#define		MAX_HEIGHT		240							//ͼ�����߶�

#define		REAL_WIDTH		222							//ͼ����Ч���
#define		REAL_HEIGHT		240							//ͼ����Ч�߶�

#define		VSYNC_PORT		PTA29						// ���жϹܽ�����
#define		HERF_PORT		PTA28						// ���жϹܽ�����
#define		CLK_PORT		PTA27						// DMA �жϹܽ�����

#define		VSYNC_NUM		29							// ���жϹܽź�
#define		HERF_NUM		28							// ���жϹܽź�
#define		CLK_NUM			27							// DMA �жϹܽź�


typedef enum					//������������ͷ�Ĺ���״̬
{
	OV7620_NOTINIT = 0,
	OV7620_FINISH,           	//ͼ��ɼ����
	OV7620_FAIL,             	//ͼ��ɼ�ʧ��(�ɼ���������)
	OV7620_GATHER,          	//ͼ��ɼ���
	OV7620_START,          		//��ʼ�ɼ�ͼ��
	OV7620_STOP,             	//��ֹͼ��ɼ�
} OV7620_STATUS_e;

uint8 		*imgAddr;								//ͼ������ָ��
volatile	uint8 	ov7620_img_buff[MAX_WIDTH]={0};	//��ǰ�л�������
volatile	uint16	count;							//������ǵ�ǰת�Ƶĵ���
volatile	uint8	line;							//������ǵ�ǰ�ɼ�������
volatile 	OV7620_STATUS_e		ov7620_img_flag;	//��ʶ����ͷ����״̬

uint8 lineDis;										//�ɼ��м��
uint8 countDis;										//�ɼ�����
uint8 lineEnd;										//ͼ�������
uint8 errorCount;									//��Ч����

uint8 ov7620_portInit();							//��ʼ���ܽ�
uint8 ov7620_initCameraReg();						//��ʼ���Ĵ���
void OV7620_PORTA_IRQHandler();						//�ܽ��жϺ���
void OV7620_DMA0_IRQHandler();						//DMA �жϺ���
void ov7620_vsync();								//���жϺ���
void ov7620_href();									//���жϺ���
void ov7620_dma();									//DMA�жϺ���

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * ��ʼ������ͷ
 */
uint8 ov7620_initCamera(uint8 *imgaddr)
{		
	
	lineDis = MAX_HEIGHT/IMAGE_H;						//�ɼ��м��
	countDis = MAX_WIDTH/IMAGE_W;						//�ɼ�����
	lineEnd = MAX_HEIGHT-(MAX_HEIGHT/IMAGE_H);			//ͼ�������
	errorCount = (MAX_WIDTH - REAL_WIDTH)/countDis/2;	//��Ч����
	
	imgAddr = imgaddr;									//��ͼ���ַ���и�ֵ
	
	//while(!ov7620_initCameraReg())
		//DELAY_MS(100);								//��ʼ���Ĵ����ɹ�
	while(!LandzoIICEEROM_INIT())
	{
		printf("�Ĵ�����ʼ��ʧ��\n");
		DELAY_MS(100);
	}
	while(!ov7620_portInit());							//��ʼ������ͷ�ܽ�
	return SUCCESS;
}

/**
 * ��ʼ���ܽ�
 */
uint8 ov7620_portInit()
{
	//DMAͨ��0��ʼ����PTA27����Դ(Ĭ��������)��Դ��ַΪPTB_B0_IN��Ŀ�ĵ�ַΪ��ov7620_img_buff��ÿ�δ���1Byte
    dma_portx2buff_init(CAMERA_DMA_CH, (void *)&PTB_B0_IN, (void *)ov7620_img_buff, CLK_PORT, DMA_BYTE1, MAX_HEIGHT, DADDR_KEEPON);
	DMA_DIS(CAMERA_DMA_CH);										//��ֹͨ��0��DMA����
	disable_irq(PORTA_IRQn); 									//�ر�PTA���ж�
    DMA_IRQ_CLEAN(CAMERA_DMA_CH);                   			//���ͨ�������жϱ�־λ	
    DMA_IRQ_EN(CAMERA_DMA_CH);									//����ͨ��0������ɴ����ж�
	
    port_init(CLK_PORT, ALT1 | DMA_FALLING | PULLDOWN );   		//PCLK
    port_init(HERF_PORT, ALT1 | IRQ_RISING  | PULLDOWN | PF);   //���жϣ��������Ͻ��ش����жϣ����˲�
    port_init(VSYNC_PORT, ALT1 | IRQ_FALLING | PULLUP   | PF);  //���жϣ��������½��ش����жϣ����˲�
	
    //�����жϷ�����
    set_vector_handler(PORTA_VECTORn ,OV7620_PORTA_IRQHandler); //����LPTMR���жϷ�����Ϊ PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn ,OV7620_DMA0_IRQHandler);   //����DMA0���жϷ�����Ϊ DMA0_IRQHandler	
	return SUCCESS;
}

/**
 * ��ʼ��ȡһ��ͼ��
 */
uint8 *ov7620_startGetImg()
{
	ov7620_img_flag = OV7620_START;                 //��ʼ�ɼ�ͼ��
    PORTA_ISFR = ~0;                        		//д1���жϱ�־λ(����ģ���Ȼ�ص���һ���жϾ����ϴ����ж�)
    enable_irq(PORTA_IRQn);                 		//����PTA���ж�
    while(ov7620_img_flag != OV7620_FINISH)        	//�ȴ�ͼ��ɼ����
    {
        if(ov7620_img_flag == OV7620_FAIL)        	//����ͼ��ɼ����������¿�ʼ�ɼ�
        {
            ov7620_img_flag = OV7620_START;         //��ʼ�ɼ�ͼ��
            PORTA_ISFR = ~0;                		//д1���жϱ�־λ(����ģ���Ȼ�ص���һ���жϾ����ϴ����ж�)
            enable_irq(PORTA_IRQn);                 //����PTA���ж�
        }
    }
	return (uint8 *)imgAddr;
}

/**
 * ����ͷֹͣ��ȡͼ�񣬼��ر��жϣ���� DMA �жϱ�־
 */
uint8 ov7620_stopGetImg()
{
	disable_irq(PORTA_VECTORn);						//�ر��ж�
	PORTA_ISFR = ~0;								//����жϱ�ʶ
	DMA_DIS(CAMERA_DMA_CH);							//��ֹͨ��0��DMA����
	DMA_IRQ_CLEAN(CAMERA_DMA_CH);           		//���ͨ�������жϱ�־λ
	ov7620_img_flag = OV7620_STOP;					//����ͼ��ɼ���־λ
	return SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * PORTA�жϷ�����
 */
void OV7620_PORTA_IRQHandler()
{
	
    uint32 flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;           	//���жϱ�־λ
    
	//���ж�
    if(flag & (1 << VSYNC_NUM))     //PTA29�����ж�
    {
        ov7620_vsync();				//�������жϺ���
    }
	
	//���ж�
	if(flag & (1 << HERF_NUM))      //PTA29�����ж�
	{
		// �ж��Ƿ���ͼ��ɼ�״̬
		if(ov7620_img_flag == OV7620_GATHER)
			ov7620_href();			//�������жϺ���
	}
	
	
}

/**
 * DMA0�жϷ�����
 */
void OV7620_DMA0_IRQHandler()
{
    ov7620_dma();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * ���жϺ���
 */
void ov7620_vsync()
{
	if(ov7620_img_flag == OV7620_START)							//��ʼ�ɼ�
	{     
		ov7620_img_flag = OV7620_GATHER;						//���ͼ��ɼ���
		count = 0;
		line = 0;
	}
	else//ͼ��ɼ�����
    {
        disable_irq(PORTA_IRQn);                				//�ر�PTA���ж�
        ov7620_img_flag = OV7620_FAIL;             				//���ͼ��ɼ�ʧ��
    }
}

/**
 * ���жϺ���
 */
void ov7620_href()
{
	if(line % lineDis == 0)
	{
	#if 1
		//PORTA_ISFR = 1 <<  PT27;            					//���PCLK��־λ
		DMA_DADDR(CAMERA_DMA_CH) = (uint32)ov7620_img_buff;		//�ָ���ַ
		//PORTA_ISFR = 1 <<  PT27;            					//���PCLK��־λ
		DMA_EN(CAMERA_DMA_CH);                  				//ʹ��ͨ��CHn Ӳ������
	#else
		//PORTA_ISFR = 1 <<  PT27;            					//���PCLK��־λ
		dma_repeat(CAMERA_DMA_CH, (void *)&PTB_B0_IN, (void *)ov7620_img_buff,MAX_HEIGHT);
		DMA_EN(CAMERA_DMA_COUNT);                  				//ʹ��ͨ��CHn Ӳ������
	#endif
	}
	line++;
}

/**
 * DMA�жϷ�����
 */
int i;
void ov7620_dma()
{
	DMA_DIS(CAMERA_DMA_CH);									//��ֹͨ��0��DMA����
    DMA_IRQ_CLEAN(CAMERA_DMA_CH);           				//���ͨ�������жϱ�־λ
	
	//�ж�ͼ��ɼ��Ƿ�������ɼ�������ͼ��״̬��ʶ����Ϊ����״̬
	if(line >= lineEnd)
	{
		ov7620_img_flag = OV7620_FINISH ;
		disable_irq(PORTA_IRQn);
	}
	//else
	{
		for(i = 0; i < errorCount; i++)						//�����Ч���ص�
			imgAddr[count++] = 255;
		for(i = 0; i < REAL_WIDTH; i++)						//ÿ��4�����ص�ȡһ����
		{
			if(i%countDis == 0)
			#if (BIN_LEVEL == -1)							//��ʹ�ö�ֵ��
				imgAddr[count++] = ov7620_img_buff[i];
			#else											//��ֵ��ͼ��
				imgAddr[count++] = (ov7620_img_buff[i] >= BIN_LEVEL) ? 255 : 0;
			#endif
		}
		for(i = 0; i < errorCount; i++)						//�����Ч���ص�
			imgAddr[count++] = 255;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint8  CamreaReg[] = {
   0x03,0x00,   0x0c,0x00,  0X11,0X00,  0X12,0X01,  0X13,0X17,  0X15,0X02,  0X16,0X03,  0X03,0X00,
   0X17,0X00,   0X18,0Xa0,  0X19,0X00,  0X1a,0X78,  0X1b,0X2a,  0X1f,0X20,  0X20,0X02,  0X09,0X03,
   0X21,0X00,   0X22,0X20,  0X23,0X33,  0X24,0X8a,  0X25,0X7a,  0X2A,0X00,  0X2B,0X00,  0X2c,0X00,
   0X2d,0X4f,   0X2e,0X96,  0X2f,0X00,  0X3a,0X00,  0X3b,0X60,  0X3e,0X34,  0X3f,0Xb0,  0X40,0X4c,
   0X41,0X3b,   0X42,0X30,  0X43,0X25,  0X44,0X1d,  0X45,0X15,  0X46,0X10,  0X47,0X0f,  0X48,0X0d,
   0X49,0X0b,   0X4b,0X09,  0X4c,0X09,  0X4e,0X08,  0X4f,0X06,  0X50,0X05,  0X51,0X2f,  0X52,0X90,
   0X53,0X01,   0X54,0X87,  0X56,0X70,  0X57,0X32,  0X58,0X8b,  0X59,0X06,  0X5a,0X9b,  0X5b,0X35,
   0X64,0X02,   0X67,0X80,  0X68,0X80,  0X69,0XA0,  0X6a,0X89,  0X70,0Xae,  0X71,0Xa6,  0X72,0X4f,
   0X73,0X5f,   0X74,0X27,  0X80,0X41,  0X81,0X08,  0X82,0X16,  0X83,0X24,  0X84,0X24,  0X85,0X2a,
   0X86,0X4f,   0X89,0Xb2,  0X8a,0Xaa,  0X8b,0X02,  0X8e,0X02,  0X8f,0X70,  0X95,0X14,  0X9d,0X36,
   0X9e,0X04,   0Xa1,0X27,  0Xa2,0X12,  0Xa3,0X2d,  0Xa4,0X08,  0Xa5,0X2e,  0Xa6,0X04,  0Xa7,0X80,
   0Xa8,0X80,   0Xa9,0X21,  0Xaa,0X21,  0Xab,0X21,  0Xac,0X0a,  0Xad,0Xf0,  0Xae,0Xff,  0Xb1,0Xb0,
   0Xb2,0Xb0,   0Xb6,0X80,  0Xb7,0X80,  0Xb8,0X80,  0Xb9,0X00,  0Xe7,0X7c,  0Xf0,0X00,  0Xf1,0X01,
   0Xf1,0X01,   0Xf1,0X01,
};

/**
 * ��ʼ������ͷ�Ĵ���
 */
uint8 ov7620_initCameraReg()
{
    uint8 sccbOk;                                                       //sccb�����ɹ���־
    uint8 loopNum;                                                      //����ѭ������ʱ����
    uint8 readData;                                                     //���ڶ�ȡ�Ĵ����е�����
    uint8 eromCount = sizeof(CamreaReg)/sizeof(CamreaReg[0])/2;        //�����ļĴ�������
    
    SCCB_GPIO_init();                                                   //��ʼ�� SCCB
    
    //���Ĵ�����д����
    for(loopNum = 0; loopNum < eromCount; loopNum++)
    {
        sccbOk = SCCB_WriteByte(CamreaReg[loopNum * 2], CamreaReg[loopNum * 2 + 1]);        
        if(sccbOk == FAIL)                                         		//д��Ĵ���ʧ��
        {
            DEBUG_PRINTF("\n����:SCCBд���ݴ���");
            return FAIL;
        }
    }
	
	
	//�ӼĴ����ж�ȡ����
    for(loopNum = 0; loopNum < eromCount; loopNum++)
    {
        sccbOk = SCCB_ReadByte( &readData, 1, CamreaReg[loopNum * 2]);
        if(sccbOk == FAIL)                                         		//��ȡ�Ĵ���ʧ��
        {
            DEBUG_PRINTF("\n����:SCCBд���ݴ���");
            return FAIL;
        }
        
		if(readData != CamreaReg[loopNum * 2 + 1])
        {
            DEBUG_PRINTF("\n����:��ȡ�����ݴ���");
            return FAIL;
        }
		
    }
	
	DEBUG_PRINTF("\n��д�Ĵ����ɹ���");
	
    return SUCCESS;    
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////