#include "control.h"   
#include "confer.h"


extern MotorParam_TypeDef Motor;
extern PID_TypeDef PID;
u16 Target = 2000;              //Ŀ��ת��
u16 Real = 0;                   //��ʵת��        



u16 CurrentInit ;               //����ƫ����
u16 RealCurrent;                //ʵ�ʵ���

extern u8 HighPeriod ;          //�ߵ�ƽʱ��
extern u8 CapCount;             //�����źŴ���

u8 Tx_Data[5]={0xBB,0x13,0,0,0xEE};
             


WORK_STATE Work_State = STOP;

/*Ӳ����ʼ��*/
void BSP_Init(void)
{    
    Watch_Dog_Init();
    Clock_Init();       //ʱ��
   
    Motor_Init();       
    UART_Init();
    Bemf_Init();
    Tim6_Init();
    
    Capture_Init();
    
    Current_Init();
    CurrentInit = Current_Offset();


    enableInterrupts();//�ж�ʹ��
} 



  
void Run(void)
{

  while (1)
  {
      switch(Work_State)
      {
          case (START):         //��������
                 if(Motor.StartFlag == 0)         //δ����
                {
                    Motor.StartFlag = 1;
              
                    if(Sensorless_START() != 0)//��������
                    {

                      Work_State = CLOSE_LOOP;
                    }
                    else                      //�޷�����
                    {
                      
                      Motor.ErrorFlag = 1;
                      Work_State = STOP;
                    }                
                }           
                 
                          break;	
                          
          case (CLOSE_LOOP):	//�ջ�ת
                if(Motor.ErrorFlag == 0 && Motor.StartFlag == 1)
                {
                  
                  PID_Init();
                  Motor.PwmValue = PID_Compute(Target , Real ,&PID);
                  Work_State = COMMUNICATION;
                  
                }
                else                      //�޷�����
                {                 
                  Motor.ErrorFlag = 2;
                  Work_State = STOP;
                }                
   
                          break;
                
          case (STOP):          //ֹͣ
                if(Motor.ErrorFlag != 0)//�й���
                {
                  Motor_Stop();
                  
                  Motor.StartFlag = 0;
//                  Motor.ErrorFlag = 0;          //�����                 
                  Motor.StOK = 0;
                  Motor.Step = 5;
                  Real = 0;
                

                  PID_DeInit();
                  Nwp_Signal_H();       //NWP �ø�
                  
                }
                else
                {

                  Work_State = START;
                }
                
                          
                          break;
          case (COMMUNICATION):	//ͨ��
            
            
                  /*���ź�ͨ��*/              
                   Motor.ErrorFlag = Det_Step(Motor);            //��⻻��Ĳ���
                   if(Motor.ErrorFlag != 0)
                   {
                      Work_State = STOP;                     
                   }

//                  if( Cal_Speed(HighPeriod ,&Target) == 1)//ת
//
//                  {
//                    if((Motor.StartFlag == 0) && (Motor.ErrorFlag == 0) &&(Motor.StOK == 0))    //δ����
//                    {
//                      Work_State = START;
//                    }
//                    else                        //����
//                    {
//                        //����ת50ms����
//                    }
//                  }
//                  else                          //ֹͣ
//                  {
//                    Motor.ErrorFlag = 3;
//                    Work_State = STOP;
//                  }

                       
                          break;                          
          default:break;
      }
  }
} 
 
u8  Count6ms=0,Count50ms=0;
u16 Count100ms=0;  //���ʱ��������

void Time_1ms(void)//1ms��ʱ�� 
{   
    Count6ms++;

    Count50ms++;
    
    Count100ms++;
    
    Task_50ms();
    Task();
//    Task_100ms();
    TIM6_ClearITPendingBit(TIM6_IT_UPDATE);//������±�־λ
   
}


/*��ʱ����*/
void Task(void)
{
  if((Count6ms % 6 )== 0 ) //6ms����
  {

    if(Motor.StartFlag == 1)
    {
      Nwp_Signal();
    }

  } 
  
  if(Count6ms  >= 30)           //30ms����
  {
    Feed_Dog();                                                         //ι��
    
    Motor.ErrorFlag = Detect_Current(CurrentInit,&RealCurrent);         //�������
    if(Motor.ErrorFlag != 0)
    {
      Work_State = STOP;
      if((Motor.ErrorFlag != 0) &&(Motor.ErrorFlag == 5))  //������ǿ��ͣ��
      {
        
        Tx_Data[1]=0x14;Tx_Data[2]=0;Tx_Data[3] = Motor.ErrorFlag;;	
        Uart_Send();
        
        while(1)
        {
          Motor_Stop();
        }
      }
    }

    Count6ms = 0;
  }
}



/*50ms����*/   
void Task_50ms(void)
{
  if(Count50ms >= 50)
  {
//    Det_Signal_Period_Reset(&HighPeriod ,CapCount);  //SWP�źŸ�λ
    
    Count50ms = 0;
  //RealS=NumCoun*20*60/6/MoPair;	//NumCoun  �������          MoPair  ����    
    Real= Motor.StartSwitchStepCount * 200/Motor.NumPair;//StartSwitchStepCount  �������  MoPair  ����
    
    Motor.StartSwitchStepCount=0;
  
    if(Motor.StartFlag==1)                      //����״̬
    Motor.PwmValue = PID_Compute(Target , Real ,&PID);
  }
}

void Task_100ms(void)
{
  if(Count100ms == 200)         //����
  {
      Tx_Data[1]=0x12;Tx_Data[2]=RealCurrent/256;Tx_Data[3]=RealCurrent%256;	
      Uart_Send(); 

  }
  else if(Count100ms == 400)    //ת��
  {
      Tx_Data[1]=0x13;Tx_Data[2]=Real/256;Tx_Data[3]=Real%256;	
      Uart_Send();
    
  }
  else if(Count100ms == 600)    //������
  {
      Tx_Data[1]=0x14;Tx_Data[2]=0;Tx_Data[3] = Motor.ErrorFlag;;	
      Uart_Send();
    
      Count100ms = 0;
  }
  

 
}


  

/*���ʹ�������*/
void Uart_Send(void)
{
  u8 i;

  for(i=0;i<5;i++)
  {
    while(UART1_GetFlagStatus(UART1_FLAG_TC)==RESET);
    UART1->DR = Tx_Data[i];

  }

}




