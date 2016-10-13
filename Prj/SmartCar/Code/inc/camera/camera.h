#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "include.h"
#include "ov7620.h"
#include "ov7725.h"

#if (CAMERA == OV7620)
// OV7620 宏定义
	#define		_cameraInit(imageAddr)	ov7620_initCamera(imageAddr)
	#define		_cameraGetImg()			ov7620_startGetImg()
	// #define	_cameraStop()			ov7620_stopGetImg()	
#else
// OV7725 宏定义
	#define		_cameraInit(imageAddr)	ov7725_initCamera(imageAddr)
	#define		_cameraGetImg()			ov7725_startGetImg()
	// #define	_cameraStop()			ov7725_stopGetImg()
#endif

// #define	IMAGE_SIZE				(IMAGE_H * IMAGE_W)
#define		cameraInit(imageAddr)	_cameraInit((uint8 *)imageAddr)
#define		cameraGetImg()			_cameraGetImg()
// #define	cameraStop()			_cameraStop()

#endif