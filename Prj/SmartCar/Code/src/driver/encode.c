#include "encode.h"

void ENCODE_IRQHandler();

/* 初始化编码器 */
void encodeInit()
{
    ftm_quad_init(ENCODE_FTM);                              //FTM1 正交解码初始化（所用的管脚可查 port_cfg.h ）
    pit_init_ms(ENCODE_PIT, TIME);                          //初始化PIT0，定时时间为： TIME
    set_vector_handler(PIT0_VECTORn ,ENCODE_IRQHandler);    //设置PIT0的中断服务函数为 PIT0_IRQHandler
    // enable_irq(PIT0_IRQn);                               //使能PIT0中断
}

/* 使能编码器 */
void encodeWork()
{
	enable_irq(PIT0_IRQn);									//使能PIT0中断
}

/* 关闭编码器 */
void encodeStop()
{
	disable_irq(PIT0_IRQn);
	carParams.encode = 0;
}

/* 获得编码器的值 */
int16 getEncodeValue()
{
	return carParams.encode;
}

void ENCODE_IRQHandler()
{
    carParams.encode = ftm_quad_get(ENCODE_FTM);      		// 获取FTM 正交解码 的脉冲数(负数表示反方向)
    ftm_quad_clean(ENCODE_FTM);

    // if(encodeVal>=0)
    // {
    //     printf("\n正转：%d",encodeVal);
    // }
    // else
    // {
    //     printf("\n反转：%d",-encodeVal);
    // }

	encodeCallBack();										// 回调 main 函数中相应的处理方法
    PIT_Flag_Clear(ENCODE_PIT);       						// 清中断标志位
}