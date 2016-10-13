#include "ov7725.h"

uint8 *img;
uint8 imgbuff[CAMERA_SIZE] = {0};

void img_extract(uint8 *dst, uint8 *src, uint32 srclen);
uint8 ov7725_portInit();
void OV7725_PORTA_IRQHandler();
void OV7725_DMA0_IRQHandler();

uint8 ov7725_initCamera(uint8 *imgaddr)								//初始化摄像头
{
	img = imgaddr;
	camera_init(imgbuff);
	return ov7725_portInit();
}

uint8 *ov7725_startGetImg()											//驱动摄像头获取一副图像
{
	camera_get_img();												//摄像头获取图像
	img_extract(img, imgbuff,CAMERA_SIZE);							//解压图像
	return (uint8 *)img;
}


uint8 ov7725_stopGetImg()											//停止摄像头获取图像
{
	disable_irq(PORTA_VECTORn);										//关闭中断
	PORTA_ISFR = ~0;												//清除中断标识
	DMA_DIS(CAMERA_DMA_CH);											//禁止通道0的DMA请求
	DMA_IRQ_CLEAN(CAMERA_DMA_CH);									//清除通道传输中断标志位
	return 1;
}

uint8 ov7725_portInit()
{
	//配置中断服务函数
    set_vector_handler(PORTA_VECTORn ,OV7725_PORTA_IRQHandler);    //设置LPTMR的中断服务函数为 PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn ,OV7725_DMA0_IRQHandler);      //设置LPTMR的中断服务函数为 PORTA_IRQHandler
	return 1;
}

/* 解压图像 */
void img_extract(uint8 *dst, uint8 *src, uint32 srclen)
{
    uint8 colour[2] = {255, 0}; 									//0 和 1 分别对应的颜色
    //注：山外的摄像头 0 表示 白色，1表示 黑色
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

/* 中断函数 */
void OV7725_PORTA_IRQHandler()
{
    uint8  n = 0;   												//引脚号
    uint32 flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                  				//清中断标志位

    n = 29;                                             			//场中断
    if(flag & (1 << n))                                 			//PTA29触发中断
    {
        camera_vsync();
    }
#if 0             													//鹰眼直接全速采集，不需要行中断
    n = 28;
    if(flag & (1 << n))                                 			//PTA28触发中断
    {
        camera_href();
    }
#endif
}

/* DMA 中断函数 */
void OV7725_DMA0_IRQHandler()
{
    camera_dma();
}