#ifndef _POWER_H//�궨�壬�����ļ�����
#define _POWER_H
#include "stm8s.h"//����STM8��ͷ�ļ�

void Current_Init(void);


u16 Current_Offset(void);
u8 Detect_Current(u16 Offset,u16 *CurCurrent );




#endif //�����ļ����ƽ���


