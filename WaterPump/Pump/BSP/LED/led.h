#ifndef _LED_H//�궨�壬�����ļ�����
#define _LED_H

#include "stm8s.h"//����STM8��ͷ�ļ�

#define LED_ON     GPIO_WriteHigh(GPIOD, GPIO_PIN_7)
#define LED_OFF    GPIO_WriteLow(GPIOD, GPIO_PIN_7)


void Led_Int(void);





#endif //�����ļ����ƽ���


