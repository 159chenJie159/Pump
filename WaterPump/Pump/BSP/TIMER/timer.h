#ifndef _TIMER_H//�궨�壬�����ļ�����
#define _TIMER_H
#include "stm8s.h"//����STM8��ͷ�ļ�



void Tim6_Init(void);
void Tim6_ReInit(void);

void delay_ms(u16 nCount);
void delay_us(u16 nCount);

#endif //�����ļ����ƽ���


