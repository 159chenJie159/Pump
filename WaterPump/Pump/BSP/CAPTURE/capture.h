#ifndef _CAPTURE_H//�궨�壬�����ļ�����
#define _CAPTURE_H
#include "stm8s.h"//����STM8��ͷ�ļ�
#include "confer.h"


void Capture_Init(void);
void Task_Capture1(void);
void Nwp_Signal(void);
void Nwp_Signal_H(void);
void Det_Signal_Period_Reset(u8 *DetPeriod,u8 DetCapCount);
u8 Cal_Speed(u8 SigPeriod,u16 *Speed);
#endif //�����ļ����ƽ���


