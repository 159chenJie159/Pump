#ifndef _MOTOR_H//�궨�壬�����ļ�����
#define _MOTOR_H
#include "stm8s.h"//����STM8��ͷ�ļ�
#include "confer.h"


static void Motor_IO_Configuration(void);
static void Motor_Configuration(void);

void Motor_Stop(void);
u8 Commutation(u8 Step,u16 OutPwmValue);

void Motor_Init(void);
u8 Sensorless_START(void);
u8 Det_Step(MotorParam_TypeDef MotorParam);


#endif //�����ļ����ƽ���

