#include "clock.h"   



//ϵͳʱ�����ã��ڲ�16M
void Clock_Init(void)
{
  /* Select fCPU = 16MHz */
  CLK->ICKR = 0X03;
  CLK->CMSR = 0XE1;
//  CLK->SWR = 0XE1;
  CLK->SWCR =0;
  CLK->CKDIVR = 0;
//  CLK->PCKENR1 = 0XFF;
  CLK->CSSR = 0;
  CLK->HSITRIMR = 0;
  CLK->SWIMCCR = 0;
//  CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
}

/*1s ���Ź���ʼ��*/
void Watch_Dog_Init(void)
{
  //  IWDG_Enable();
//  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
//
//  IWDG_SetPrescaler(IWDG_Prescaler_256);        /*128k / 2 / 256 */

//  IWDG_SetReload(250);
//  IWDG_ReloadCounter();  
  
  
  
  IWDG->KR = 0xcc;      //ʹ��
  IWDG->KR = 0x55;
  IWDG->PR = 0x06;
  IWDG->RLR =250 ;
  IWDG->KR =0xAA;       //ι��
  

}

void Feed_Dog(void)
{
  IWDG->KR =0xAA;
}

