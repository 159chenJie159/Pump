#ifndef _CONFER_H
#define _CONFER_H

#include "stm8s.h"

#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80

#define MAX_SIGNAL_PERIOD       40      //����ź�����
#define MIN_SIGNAL_PERIOD       1       //��С
#define START_SIGNAL_PERIOD     13      //��ʼ
#define THRESHOLD_SIGNAL_PERIOD  28       //�ٽ�
typedef struct
{
    volatile u8 StartFlag;            //ת1  ͣ0
    volatile u8 Step;              
    volatile u8 ErrorFlag;        //�й���1  ��0
    volatile u8 BemfCount;        //���綯���й������
    volatile u8 BemfSwitchStepCount;        //���綯���л������
    
    
    u8  StartSwitchStepCount;	//���������ﻻ��ļ���
    u8  StOK;                    //��������1  δ��������0
    u16 PreDuty;                //Ԥ��λռ�ձ�
    u16 StartDuty;              //����ռ�ձ�
    u16 PwmValue;              //ʵʱPWM
    u16 Start_Delay;
    u8  NumPair;

}MotorParam_TypeDef;


typedef enum
{
    START = 0,
    OPEN_LOOP,
    CLOSE_LOOP,
    STOP,
    COMMUNICATION,

}WORK_STATE;










#endif 


