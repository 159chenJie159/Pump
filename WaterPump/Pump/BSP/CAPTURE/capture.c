#include "capture.h"   

#define RISING_TRIGGER  0
#define FALL_TRIGGER    1
#define SIGNAL_PERIOD   20000

void Capture_Init(void)
{
//    TIM5_DeInit();
     //分频系数(32)   向上计数    最大计数值65535 
        
//    TIM5_TimeBaseInit(TIM5_PRESCALER_32,65535);   
//    TIM5_ICInit(TIM5_CHANNEL_1,TIM5_ICPOLARITY_RISING,TIM5_ICSELECTION_DIRECTTI,TIM5_ICPSC_DIV1,0);
//    TIM5_ITConfig(TIM5_IT_CC1,ENABLE);
    
//    ITC_SetSoftwarePriority(ITC_IRQ_TIM5_CAPCOM ,ITC_PRIORITYLEVEL_2);  //设置优先级
      
//    TIM5_ClearFlag(TIM5_FLAG_CC1);                      //清标志
//    
//    TIM5_Cmd(ENABLE);
    

    TIM5->PSCR = (0x05);                //32分频

    TIM5->ARRH = 0xff;  //65535>>8;
    TIM5->ARRL = 0xff;  //(u8)65535;

    TIM5->PSCR = (0x05);                //32分频

    TIM5->ARRH = 65535>>8;
    TIM5->ARRL = (u8)65535;

    
    TIM5->CCMR1  = 0x01;                
    TIM5->CCER1 = 0x01;

    TIM5->IER = 0x02;
    ITC_SetSoftwarePriority(ITC_IRQ_TIM5_CAPCOM ,ITC_PRIORITYLEVEL_2);  //设置优先级
    TIM5->CR1 = 0x01;                   //使能
    /*NWP  PD*/
    GPIO_Init(GPIOD, GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_FAST); 
}


u8 HighPeriod = 40;              //高电平时长
u8 CapCount = 0;                    //捕获次数   
void Task_Capture1(void)
{
    u16 CurValue = 0;
    static u8 TriggerStates = 0;              //触发中断状态
                    
    u8 temp = 0;
    if(TIM5_GetITStatus(TIM5_IT_CC1) == SET)
    {
      CapCount ++;

        if(TriggerStates == RISING_TRIGGER)
        {
          TIM5->CCER1 |=0x02;  //下降沿触发
          TriggerStates = FALL_TRIGGER;
          
          TIM5->CNTRH = 0;
          TIM5->CNTRL = 0;
//          TIM5_SetCounter(0);

        }
        else
        {
          CapCount ++;
          TIM5->CCER1 &= (uint8_t)(~0x02);    //上升沿触发
          TriggerStates = RISING_TRIGGER;
          
//          CurValue =  TIM5_GetCapture1();      //获取当前值
            CurValue = ((uint16_t)TIM5->CCR1H << 8); 
            CurValue = CurValue| (uint16_t)(TIM5->CCR1L);

          
          if(CurValue > SIGNAL_PERIOD)        //大于信号周期40ms
          {
            CurValue -= SIGNAL_PERIOD;
          }
          
          if(CapCount >= 2)
          {
            HighPeriod = (u8) (CurValue / 500);    //f = 500K
            temp = HighPeriod;
          }
          
          if((CapCount >=255) || (HighPeriod != temp))
          {
            CapCount = 0;
    
          }
        } 
    }
    
//    TIM5->SR1 = (~0X02);
//    TIM5->SR2 = 0;
    TIM5_ClearITPendingBit(TIM5_IT_CC1);        //清标志
    TIM5_ClearFlag(TIM5_FLAG_CC1);
}



/*计算转速*/
u8 Cal_Speed(u8 SigPeriod,u16 *Speed)
{
    u16 temp = 0;

   if(SigPeriod < START_SIGNAL_PERIOD || SigPeriod > THRESHOLD_SIGNAL_PERIOD)
   {
      *Speed = 100;
      return 0;
   }
   else if((SigPeriod >= START_SIGNAL_PERIOD) && (SigPeriod <=THRESHOLD_SIGNAL_PERIOD))
   {
     /* y=3.372*x^2 - 74.75*x + 1457.85  */
     temp = (3 * SigPeriod* SigPeriod )+1480 - 65 *SigPeriod;

     *Speed  = temp;
     return 1;
     
   }
   return 2;

}


/*无信号变化，信号周期复位判断*/
void Det_Signal_Period_Reset(u8 *DetPeriod,u8 DetCapCount)
{
  static u8 CurCount = 0;
  static u8 LastCapCount = 0;
  
  if( LastCapCount == DetCapCount)         //未进入捕获中断10次，复位信号周期
  {
    CurCount++;
    if(CurCount >= 20)
    { 
      *DetPeriod = MAX_SIGNAL_PERIOD;
      CurCount = 0;
     
    }
  }
  LastCapCount = DetCapCount;
}



void Nwp_Signal(void)
{
  static u8 i = 0;
  i = ~i;
  GPIOD->ODR = (i << 3) ;
}

void Nwp_Signal_H(void)
{
  GPIOD->ODR = (1 << 3);
  HighPeriod = 40;
}