#include "timer.h"  
#include "confer.h"




void Tim6_Init(void)
{
//  TIM6_TimeBaseInit(TIM6_PRESCALER_128,125);//��ʼ����ʱ��4
//  TIM6_ARRPreloadConfig(ENABLE);//ʹ���Զ���װ
//  ITC_SetSoftwarePriority(ITC_IRQ_TIM6_OVFTRI , ITC_PRIORITYLEVEL_1); //�������ȼ�
//  TIM6_ITConfig(TIM6_IT_UPDATE , ENABLE);//���ݸ����ж�
//  TIM6_Cmd(ENABLE);//����ʱ��
    
    
    TIM6->CR1 = 0x81;
    TIM6->CR2  = 0;
    TIM6->IER = 0x01;

    TIM6->EGR = 0;
    TIM6->ARR = 0x7d ;
    TIM6->PSCR = 0x07;
    TIM6->CNTR = 0;
  
}

void Tim6_ReInit(void)
{
  TIM6->CR1 	= 0X00;
  TIM6->CR2 	= 0X00;
  TIM6->SMCR 	= 0X00;
  TIM6->IER 	= 0X00;
  TIM6->CNTR 	= 0X00;
  TIM6->PSCR	= 0X00;
  TIM6->ARR 	= 0XFF;
  TIM6->SR1 	= 0X00;
//  TIM6_DeInit();
  
  TIM6->CR1 &= 0xF0;
//  TIM6_Cmd(DISABLE);//�ض�ʱ��
}

void delay_us(u16 nCount)   //16M ����ʱ  ��ʱ 1��΢��
{
    nCount*=3;
    while(--nCount);
}


void delay_ms(u16 nCount)  //16M ����ʱ  ��ʱ 1������
{
    while(nCount--)\
    {
        delay_us(1000);\
    }
}  