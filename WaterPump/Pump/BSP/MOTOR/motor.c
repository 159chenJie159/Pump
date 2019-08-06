#include "motor.h"   

#include "timer.h" 




//���űۿ��ؿ��ƶ˿ڶ���
#define AH_PORT 	GPIOC
#define AH_PIN	        GPIO_PIN_3
#define BH_PORT 	GPIOC
#define BH_PIN	        GPIO_PIN_7
#define CH_PORT 	GPIOC
#define CH_PIN	        GPIO_PIN_6

//���űۿ��ؿ��ƶ˿ڶ���
#define AL_PORT         GPIOC
#define AL_PIN	        GPIO_PIN_2 
#define BL_PORT         GPIOC
#define BL_PIN	        GPIO_PIN_1 
#define CL_PORT         GPIOE
#define CL_PIN	        GPIO_PIN_5 

//���ű۵͵�ƽ���عܵ�ͨ

#define PWM_AL_OFF GPIO_WriteHigh(AL_PORT, AL_PIN) 
#define PWM_BL_OFF GPIO_WriteHigh(BL_PORT, BL_PIN) 
#define PWM_CL_OFF GPIO_WriteHigh(CL_PORT, CL_PIN)

#define PWM_AL_ON GPIO_WriteLow(AL_PORT, AL_PIN) 
#define PWM_BL_ON GPIO_WriteLow(BL_PORT, BL_PIN)
#define PWM_CL_ON GPIO_WriteLow(CL_PORT, CL_PIN)

#define STM8_FREQ_MHZ 16000000
#define PWM_FREQUENCY 16000    //  16K
#define ARRVAL STM8_FREQ_MHZ / PWM_FREQUENCY

MotorParam_TypeDef Motor;



//�����·���ع�IO��ʼ��
static void Motor_IO_Configuration(void)
{	

  
  //PB012 ���ű�0��Ч ,����Ϊ�ߵ�ƽ
  GPIO_Init(AL_PORT, AL_PIN,GPIO_MODE_OUT_PP_HIGH_FAST);
  GPIO_Init(BL_PORT, BL_PIN,GPIO_MODE_OUT_PP_HIGH_FAST);
  GPIO_Init(CL_PORT, CL_PIN,GPIO_MODE_OUT_PP_HIGH_FAST);
  
//    GPIOB->ODR = 0;
//    GPIOB->IDR = 0x1C;
//    GPIOB->DDR = 0;
//    GPIOB->CR1 = 0xE0;
//    GPIOB->CR2 = 0xE0;
  
  //PC123 ���ű�1��Ч,����Ϊ�͵�ƽ
  GPIO_Init(AH_PORT, AH_PIN,GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(BH_PORT, BH_PIN,GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(CH_PORT, CH_PIN,GPIO_MODE_OUT_PP_LOW_FAST);
  
//    GPIOC->ODR = 0x06;
//    GPIOC->IDR = 0x16;
//    GPIOC->DDR = 0xCE;
//    GPIOC->CR1 = 0xCE;
//    GPIOC->CR2 = 0xCE;
}

static void Motor_Configuration(void)
{
//  TIM1_DeInit();
//  
//  TIM1_TimeBaseInit(0, TIM1_COUNTERMODE_UP, ARRVAL, 0);
//
//  TIM1_OC1Init(TIM1_OCMODE_PWM1, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_DISABLE, ARRVAL*0, TIM1_OCPOLARITY_HIGH, TIM1_OCNPOLARITY_LOW, TIM1_OCIDLESTATE_RESET, TIM1_OCNIDLESTATE_SET); 
//
//  TIM1_OC2Init(TIM1_OCMODE_PWM1, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_DISABLE, ARRVAL*0, TIM1_OCPOLARITY_HIGH, TIM1_OCNPOLARITY_LOW, TIM1_OCIDLESTATE_RESET, TIM1_OCNIDLESTATE_SET); 
//
//  TIM1_OC3Init(TIM1_OCMODE_PWM1, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_DISABLE, ARRVAL*0, TIM1_OCPOLARITY_HIGH, TIM1_OCNPOLARITY_LOW, TIM1_OCIDLESTATE_RESET, TIM1_OCNIDLESTATE_SET); 
//
//  TIM1_CCPreloadControl(DISABLE);      //Ԥװ��
//  TIM1_Cmd(ENABLE);
    TIM1->CR1 = 0x01;
    TIM1->SR1 = 0x1f;
    TIM1->CCMR1 = 0x60;
    TIM1->CCMR2 = 0x60;
    TIM1->CCMR3 = 0x60;
    TIM1->CCER1 = 0X99;
    TIM1->CCER2 = 0X09;
    TIM1->ARRH = 0X03;
    TIM1->ARRL = 0XE8;
    TIM1->OISR = 0X2A;
    
}





//�������PWMֵ��
//Step:��ǰ���ಽ��0-5��OutPwmValue ���PWMֵ
/*���ຯ��*/
u8 Commutation(u8 Step,u16 OutPwmValue)
{

    TIM1->BKR &= (u8)(~TIM1_BKR_MOE);//��ֹPWM���
       
    PWM_AL_OFF;PWM_BL_OFF;PWM_CL_OFF; /*ȫ��*/

    //���ݻ��ಽ�򣬴򿪲�ͬ�Ŀ��عܣ���ʩ����ȷ��PWM�ź�
    if(Step==0)//AB
    {

        TIM1->CCR3H = (uint8_t)(OutPwmValue >> 8);
        TIM1->CCR3L = (uint8_t)(OutPwmValue);   
        
        TIM1->CCER1 =0x00;
        TIM1->CCER2 =0x01;

        PWM_BL_ON;
    }
    else if(Step==1)	//AC
    {
        TIM1->CCR3H = (uint8_t)(OutPwmValue >> 8);
        TIM1->CCR3L = (uint8_t)(OutPwmValue);  
        
        TIM1->CCER1 =0x00;
        TIM1->CCER2 =0x01;

        PWM_CL_ON;
    }
    else if(Step==2)	//BC
    {
        TIM1->CCR2H = (uint8_t)(OutPwmValue >> 8);
        TIM1->CCR2L = (uint8_t)(OutPwmValue); 
        
        TIM1->CCER1 =0x10;
        TIM1->CCER2 =0x00;
        
        PWM_CL_ON;
    }
    else if(Step==3)	//BA
    {
        TIM1->CCR2H = (uint8_t)(OutPwmValue >> 8);
        TIM1->CCR2L = (uint8_t)(OutPwmValue);  
        
        TIM1->CCER1 =0x10;
        TIM1->CCER2 =0x00; 

        PWM_AL_ON;
    }
    else if(Step==4)    //CA
    {
        TIM1->CCR1H = (uint8_t)(OutPwmValue >> 8);
        TIM1->CCR1L = (uint8_t)(OutPwmValue);        
        TIM1->CCER1 =0x01;
        TIM1->CCER2 =0x00;

        PWM_AL_ON;
    }
    else if(Step==5)	//CB
    {
        
        TIM1->CCR1H = (uint8_t)(OutPwmValue >> 8);
        TIM1->CCR1L = (uint8_t)(OutPwmValue);
        TIM1->CCER1 =0x01;
        TIM1->CCER2 =0x00;

        PWM_BL_ON;
    }

    TIM1->BKR|=TIM1_BKR_MOE;//ʹ��PWM���
    return Step;
}

/*���ͣ*/
void Motor_Stop(void)
{
//  Start_F=0;//������־��Ϊֹͣ
  Motor.StOK = 0;
  TIM1->BKR &= (uint8_t)(~TIM1_BKR_MOE);//��ֹPWM���
  PWM_AL_OFF;//�¹�ȫ��
  PWM_BL_OFF;
  PWM_CL_OFF;
}
/*���������ʼ��*/
void Motor_Init(void)
{
  Motor.StartFlag= 0;
  Motor.Step = 5;
  Motor.ErrorFlag = 0;
  Motor.BemfCount = 0;          //���綯���й������
  Motor.BemfSwitchStepCount = 0;//���綯������ȷ�������
  Motor.StartSwitchStepCount = 0;
  Motor.StOK = 0;
  Motor.PreDuty = 25;           //Ԥ��λ25%ռ�ձ�
  Motor.StartDuty = 40;           //����ռ�ձ�
  Motor.PwmValue = 0;        //ʵʱռ�ձ�
  Motor.Start_Delay = 5;            //�𶯹��̣���ʱʱ��msΪ��λ
  Motor.NumPair = 4;            //������
  
  Motor_IO_Configuration();
  Motor_Configuration();
  Motor_Stop();
}


  

/*�޸�����*/
u8 Sensorless_START(void)
{
    u16 StartCount = 0;      //���������еĻ������
    u16 PwmValue = 0;

    
    PwmValue =(ARRVAL * Motor.PreDuty)/100; 
    Motor.Step = Commutation(Motor.Step,PwmValue);//��Ԥ��λPWM�趨Ԥ��λ
    
    delay_us(10);//��Сʱ��			

    PwmValue =(ARRVAL * Motor.StartDuty)/100; 
    do
    {	
        Motor.Step++;
        if(Motor.Step>=6) Motor.Step=0;
        Motor.Step = Commutation(Motor.Step,PwmValue);//���PWM�źţ��������				
        delay_ms(Motor.Start_Delay);
        StartCount++;
    }while((Motor.StOK == 0) && (StartCount < 200));
            
     if(StartCount>=200)//200�λ��໹û�����ɹ�����Ϊ����ʧ��
    {
        Motor_Stop();
        return 0;
    }
            
     return 1;
}

/*��⻻�ಽ��*/
u8 Det_Step(MotorParam_TypeDef MotorParam)
{       

   if( MotorParam.Step >= 7)
   {
      MotorParam.ErrorFlag = 4; 
      return MotorParam.ErrorFlag;
   }
   return 0;
 
}