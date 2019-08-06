//#include "power.h" /************************ �Զ��幫��ͷ�ļ� **************************************/

#ifndef _CONTROL_H//�궨�壬�����ļ�����
#define _CONTROL_H

#include "stm8s.h"//����STM8��ͷ�ļ�
#include "clock.h" 

#include "timer.h"  
#include "bemf.h"
#include "motor.h"
#include "pid.h"
#include "capture.h"
#include "power.h" 
#include "usart.h" 


void BSP_Init(void);
void Run(void);


void Time_1ms(void);

void Task(void);
void Task_50ms(void);
void Task_100ms(void);



void Uart_Send(void);










#endif //�����ļ����ƽ���


