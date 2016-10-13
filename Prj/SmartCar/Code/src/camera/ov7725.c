#include "ov7725.h"

uint8 *img;
uint8 imgbuff[CAMERA_SIZE] = {0};

void img_extract(uint8 *dst, uint8 *src, uint32 srclen);
uint8 ov7725_portInit();
void OV7725_PORTA_IRQHandler();
void OV7725_DMA0_IRQHandler();

uint8 ov7725_initCamera(uint8 *imgaddr)								//��ʼ������ͷ
{
	img = imgaddr;
	camera_init(imgbuff);
	return ov7725_portInit();
}

uint8 *ov7725_startGetImg()											//��������ͷ��ȡһ��ͼ��
{
	camera_get_img();												//����ͷ��ȡͼ��
	img_extract(img, imgbuff,CAMERA_SIZE);							//��ѹͼ��
	return (uint8 *)img;
}


uint8 ov7725_stopGetImg()											//ֹͣ����ͷ��ȡͼ��
{
	disable_irq(PORTA_VECTORn);										//�ر��ж�
	PORTA_ISFR = ~0;												//����жϱ�ʶ
	DMA_DIS(CAMERA_DMA_CH);											//��ֹͨ��0��DMA����
	DMA_IRQ_CLEAN(CAMERA_DMA_CH);									//���ͨ�������жϱ�־λ
	return 1;
}

uint8 ov7725_portInit()
{
	//�����жϷ�����
    set_vector_handler(PORTA_VECTORn ,OV7725_PORTA_IRQHandler);    //����LPTMR���жϷ�����Ϊ PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn ,OV7725_DMA0_IRQHandler);      //����LPTMR���жϷ�����Ϊ PORTA_IRQHandler
	return 1;
}

/* ��ѹͼ�� */
void img_extract(uint8 *dst, uint8 *src, uint32 srclen)
{
    uint8 colour[2] = {255, 0}; 									//0 �� 1 �ֱ��Ӧ����ɫ
    //ע��ɽ�������ͷ 0 ��ʾ ��ɫ��1��ʾ ��ɫ
    uint8 tmpsrc;
    while(srclen --)
    {
        tmpsrc = *src++;
        *dst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
    }
}

/* �жϺ��� */
void OV7725_PORTA_IRQHandler()
{
    uint8  n = 0;   												//���ź�
    uint32 flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                  				//���жϱ�־λ

    n = 29;                                             			//���ж�
    if(flag & (1 << n))                                 			//PTA29�����ж�
    {
        camera_vsync();
    }
#if 0             													//ӥ��ֱ��ȫ�ٲɼ�������Ҫ���ж�
    n = 28;
    if(flag & (1 << n))                                 			//PTA28�����ж�
    {
        camera_href();
    }
#endif
}

/* DMA �жϺ��� */
void OV7725_DMA0_IRQHandler()
{
    camera_dma();
}