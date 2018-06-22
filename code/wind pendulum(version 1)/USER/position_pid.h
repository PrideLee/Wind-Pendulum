#ifndef POSITION_PID_H
#define POSITION_PID_H
#include "sys.h"

#include <string.h>
#include <stdio.h>


/*************************************************************  λ��ʽPID�㷨   ***************************************************************/
typedef struct pid {
        double  SetPoint;           //  �趨Ŀ�� Desired Value
        double  Proportion;         //  �������� Proportional Const        
        double  Integral;           //  ���ֳ��� Integral Const        
        double  Derivative;         //  ΢�ֳ��� Derivative Const
        double  LastError;          //  Error[-1]        
        double  PrevError;          //  Error[-2]        
        double  SumError;           //  sums of Errors
}pid;

/*================================  pid���㲿��===============================*/
double pidCalc( struct pid *pp, double NextPoint )
{    
    double  dError,Error;
        
    Error = pp->SetPoint -  NextPoint;          // ƫ��        
    pp->SumError += Error;                      // ����        
    dError = pp->LastError - pp->PrevError;     // ��ǰ΢��   
	
    pp->PrevError = pp->LastError;        
    pp->LastError = Error;    
    
    return ( pp->Proportion * Error              // ������           
		 +   pp->Integral * pp->SumError         // ������            
		 +   pp->Derivative * dError             // ΢����        
     );
}

/*=======================  ��ʼ����pid�ṹ  Initialize pid Structure===========================*/


void pidInit(struct pid *pp)
{    
        memset(pp,0,sizeof(struct pid));
}
/*************************************************************  λ��ʽPID�㷨  ���� ***************************************************************/



#endif

