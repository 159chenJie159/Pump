#include "pid.h"

#include "confer.h"

PID_TypeDef PID;


/*PID ��ʼ��*/
void PID_Init(void)
{
  PID.P = 60;                                //��������
  PID.I = 35;                                 //����
//  PID.D = 0;                                 //΢��
  
//  PID.Out_Max =1000  ;        // duty = 100%         //�޷�
//  PID.Out_Min = 1 ;          //duty = 0.1%
  
//  PID.Up = 0;
//  PID.Ui = 0;
//  PID.Ud = 0;
  PID.Err = 0;  
//  PID.LastErr = 0;
  PID.SumErr = 0;
  PID.Out = 0 ;
}

/*PID ��λ*/
void PID_DeInit(void)
{
  PID.P = 0;                                //��������
  PID.I = 0;                                 //����

  PID.Err = 0;  
  PID.SumErr = 0;
  PID.Out = 0 ;
}



/*PID����*/
u16 PID_Compute(u16 Target,u16 Real,PID_TypeDef* PID)
{  
    double i = 0;
  
//    PID->LastErr = PID->Err;   //�ϴ����
    
    PID->Err=Target - Real;        //�������
    
    PID->SumErr +=PID->Err;
    
    i = PID->P * PID->Err;                         //����Ϊ�� ������
    i = i + PID->I * PID->SumErr;
//    i = i + PID->D * (PID->Err - PID->LastErr);
    
//    PID->Up = PID->P * PID->Err;                         //����Ϊ�� ������
//    
//    PID->Ui= PID->I * PID->SumErr;  
//    PID->Ud = PID->D * (PID->Err - PID->LastErr);  
//    if(PID->Ui >  PID->I_max) PID->Ui =  PID->I_max;	             //�����޷�
//    if(PID->Ui <  PID->I_max) PID->Ui = -PID->I_max;	             //�����޷�
    
        
    PID->Out = i / 1000;
    
      if(PID->Out  >  1000)             // duty = 100%         //�޷�
      { 
        PID->Out = 1000;		
      } 
      else if(PID->Out  <  1)
      { 
        PID->Out = 1;				
      } 
    
    
    return PID->Out;
}

    




