#include "bemf.h"   //�����Զ��幫��ͷ�ļ�
#include "motor.h"
#include "confer.h"


//���綯����������
#define FC_PORT GPIOB->IDR  
#define FC_PIN  BIT5

#define FB_PORT GPIOB->IDR
#define FB_PIN  BIT6

#define FA_PORT GPIOB->IDR
#define FA_PIN  BIT7

#define STCount 5    //���10�ι����ź�


extern MotorParam_TypeDef Motor;
u8 HallSteps[8]={7,5,3,4,1,0,2,7};
u8 CheckBEMF[6]={0x04,0x06,0x02,0x03,0x01,0x05};


void Bemf_Init(void)
{
  Bemf_Configuration();
  
  //ʹ���ⲿ�ж�
  GPIOB->CR2|=BIT5|BIT6|BIT7;

  EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOB, EXTI_SENSITIVITY_RISE_FALL);
  
}


/*��ʼ���޸бȽ϶˿�*/
void Bemf_Configuration(void)
{  
    //�Ƚ�������
    GPIO_Init(GPIOB, GPIO_PIN_5,GPIO_MODE_IN_PU_NO_IT);//C
    GPIO_Init(GPIOB, GPIO_PIN_6,GPIO_MODE_IN_PU_NO_IT);//B
    GPIO_Init(GPIOB, GPIO_PIN_7,GPIO_MODE_IN_PU_NO_IT);//A

    	
}

/*��������*/
void Task_Bemf(void)
{
    u8 State =0;
    u16 i = 0;
    if((Motor.StartFlag==0) || (Motor.ErrorFlag!=0))//���û������������ڹ���״̬��������
      return;                                   
    
    /* �˲� �ж�*/
    State = GPIOB->IDR;  //��״̬
    for(i=0;i<=550;i++);                           //��ʱ�˲�
    
    i = GPIOB->IDR;
    
    if(State!=i) return;
    
    State = 0;	
            
    //A��������
    if (FA_PORT & FA_PIN)
    {
        State |= BIT2;          //PB7        
    }
    
    //B��������
    if (FB_PORT & FB_PIN)
    {
        State |= BIT1;          //PB6
    }
                    
    //C��������
    if (FC_PORT & FC_PIN)
    {
        State |= BIT0;          //PB5
    }

     if(Motor.StOK == 0)//��������
     { 
        if(State==CheckBEMF[Motor.Step])	//��ȷ��⵽��Ӧ����Ĺ����
        {
          Motor.BemfCount++;
          GPIO_WriteReverse(GPIOD,GPIO_PIN_7);
        }//ָʾ�Ʒ�תָʾ  
        else 
        {
            Motor.BemfCount = 0;
        }
         
        if(Motor.BemfCount >= STCount)//���������̶������Ĺ����ʱ���л������㻻��
        {
            Motor.StOK=1;//�����ɹ���־
        }
     }

    if(Motor.StOK == 1)
    {
        Motor.Step = HallSteps[State];//�õ����ಽ��

        if (Motor.Step == 7)//���й����У�״̬����Ϊ����ֹͣPWM���
        { //����
                Motor.ErrorFlag=4;
                return;
        }
        
        Motor.StartSwitchStepCount++;//��ȷ���������1
        Commutation(Motor.Step,Motor.PwmValue);//���PWM�źţ��������
    }		
}




