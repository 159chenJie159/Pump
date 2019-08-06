#ifndef _PID_H//�궨�壬�����ļ�����
#define _PID_H
#include "stm8s.h"//����STM8��ͷ�ļ�


typedef struct
{
  u16 P;                                //��������
  u16 I;                                //����	                      
//  u16 D;                                //΢��

//  u16 Out_Min;
//  u16 Out_Max;                        
          
//  float Up;
//  double Ui;
//  float Ud;

  int Err;                                //���
//  double LastErr;                         //��һ�����        
  double SumErr;                          //����ܺ�
  u16 Out;	                           
}PID_TypeDef;



void PID_Init(void);
void PID_DeInit(void);
u16 PID_Compute(u16 Target,u16 Real,PID_TypeDef* PID);
//u16 PID_Compute(unsigned int Target,unsigned int Real);
 


#endif //�����ļ����ƽ���


