#ifndef __TFT_PORT_SET_H__
#define __TFT_PORT_SET_H__

// #include "common.h"
#include "include.h"

/* �ܽ����� */

// Һ�����ܽ�����
#define	_TFT_DATAPORTH	PTD_B1_OUT
#define	_TFT_DATAPORTL	PTD_B0_OUT
#define _TFT_RST  PTE0_OUT        //RST ��λ�˿�
#define	_TFT_RD   PTE1_OUT	      //RD  ������
#define	_TFT_CS   PTE2_OUT	      //CS  Ƭѡ�˿�
#define	_TFT_WR   PTE3_OUT	      //WR  д����
#define _TFT_RS   PTE4_OUT	      //RS  ����/����ѡ���

#define _TFT_DB16 PTE0_OUT
#define _TFT_DB17 PTE5_OUT

// �������ܽ�����
#define _TOUCH_DOUT PTE6_IN		  //���
#define _TOUCH_CLK  PTE7_OUT	  //ʱ��
#define _TOUCH_DIN  PTE8_OUT	  //����
#define _TOUCH_CS   PTE9_OUT	  //Ƭѡ
#define _TOUCH_PEN  PTE10_OUT	  //��ⴥ������Ӧ�ź�

// SD �ܽ�����
#define _SD_SPI_CLK	PTE7_OUT	 //SDʱ��
#define _SD_SPI_MO	PTE8_OUT	 //SD��DI����
#define _SD_SPI_MI	PTE6_IN		 //SD��DO���

#ifndef	_TOUCH_CS
	#define _TOUCH_CS   PTE9_OUT
#endif
#define _SD_CS      PTE11_OUT 


#endif








