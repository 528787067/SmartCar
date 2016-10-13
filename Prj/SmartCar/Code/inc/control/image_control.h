#ifndef	__IMAGE_CONTROL_H__
#define	__IMAGE_CONTROL_H__

#include "include.h"
#include "define.h"

#define FIND_COUNT		3				// ������������ķ�Χ
#define TYPE_1			1
#define TYPE_2			2
#define FIND_TYPE		TYPE_2			// �������ߵĲ���ģʽ
#define POINT_COUNT		10				// ������С���˷���ϵĵ���

// �����άͼ�����飬������������
int16 *findCenterLine(uint8 (* image)[IMAGE_W]);
// ����ͼ��ָ�룬������������
#define getCenterLine(imagePointer)		findCenterLine((uint8 (*)[IMAGE_W])imagePointer)

#endif