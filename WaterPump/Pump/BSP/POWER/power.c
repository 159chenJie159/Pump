#include "power.h"   
#include "confer.h"

#define MAX_CURRENT 3000  //3A




void Current_Init(void)//����
{
    GPIO_Init(GPIOD,GPIO_PIN_3,GPIO_MODE_IN_FL_NO_IT); //AIN4

    ADC1_ConversionConfig(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_4, ADC1_ALIGN_RIGHT);
//    ADC1->TDRL |= 0x02;
    ADC1_SchmittTriggerConfig(ADC1_SCHMITTTRIG_CHANNEL4, DISABLE);
    ADC1_ITConfig(ADC1_IT_EOCIE, DISABLE);
    ADC1_Cmd(ENABLE);
}




/*����ƫ����*/
u16 Current_Offset(void)
{
  u16 CurrentOffset = 0;
  
  ADC1_StartConversion();
  while(ADC1_GetFlagStatus(ADC1_FLAG_EOC) == 0);//�ȴ�ת�����
  ADC1_ClearFlag(ADC1_FLAG_EOC);

  CurrentOffset = ADC1_GetConversionValue();
  
  return CurrentOffset;
}

/*�������*/
u16 CurValue = 0;
u8 Detect_Current(u16 Offset,u16 *CurCurrent )
{
  u8 Error = 0;
  static u8 CurrentErrorCount = 0;
  
  ADC1_StartConversion();
  while(ADC1_GetFlagStatus(ADC1_FLAG_EOC) == 0);//�ȴ�ת�����
  ADC1_ClearFlag(ADC1_FLAG_EOC);
  
  CurValue = ADC1_GetConversionValue();
  if(CurValue > Offset)        //У׼  �����
  {
    CurValue -= Offset;
  }
  else
  {
    CurValue = Offset;
  }
  *CurCurrent = CurValue * 228;         //5mR

  if(*CurCurrent >= MAX_CURRENT)      //������ֵ
  {
    CurrentErrorCount++;
    if(CurrentErrorCount > 100)       //����100��  ����
    {
      Error =5;
    }

  }
  else
  {
    CurrentErrorCount = 0;
  }  
   
  return Error;
  
}


