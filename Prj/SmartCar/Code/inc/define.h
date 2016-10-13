#ifndef __DEFINE_H__
#define __DEFINE_H__

#include "include.h"

/********************* 全局宏定义 ********************/

/********************* 摄像头定义 ********************/
#define 	OV7725				0		// 山外鹰眼 OV7725
#define		OV7620				1		// 蓝宙捕食者 OV7620

// 定义使用的摄像头类型
#define		CAMERA		OV7725			// 使用 CAMERA 宏定义切换不同摄像头

// 二值化级别，-1 代表不二值化（只对 OV7620 有效，鹰眼硬件二值化）
#define		BIN_LEVEL			50

#define		IMAGE_W				80		// 图像宽度（80的倍数）
#define		IMAGE_H				60		// 图像高度（60的倍数）
/*****************************************************/

/* 编码器定时器宏定义（单位：ms） */
#define		ENCODE_TIME			5

/* 舵机工作频率 */
#define		SERVO_FREQUENCY		300

/* 电机工作频率 */
#define		MOTOR_FREQUENCY		5000

/* 启动延时 */
#ifdef RELEASE
#define		START_DELAY			2000
#else
#define		START_DELAY			0
#endif

#endif