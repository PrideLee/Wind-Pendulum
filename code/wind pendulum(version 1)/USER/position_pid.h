#ifndef POSITION_PID_H
#define POSITION_PID_H
#include "sys.h"

#include <string.h>
#include <stdio.h>


/*************************************************************  位置式PID算法   ***************************************************************/
typedef struct pid {
        double  SetPoint;           //  设定目标 Desired Value
        double  Proportion;         //  比例常数 Proportional Const        
        double  Integral;           //  积分常数 Integral Const        
        double  Derivative;         //  微分常数 Derivative Const
        double  LastError;          //  Error[-1]        
        double  PrevError;          //  Error[-2]        
        double  SumError;           //  sums of Errors
}pid;

/*================================  pid计算部分===============================*/
double pidCalc( struct pid *pp, double NextPoint )
{    
    double  dError,Error;
        
    Error = pp->SetPoint -  NextPoint;          // 偏差        
    pp->SumError += Error;                      // 积分        
    dError = pp->LastError - pp->PrevError;     // 当前微分   
	
    pp->PrevError = pp->LastError;        
    pp->LastError = Error;    
    
    return ( pp->Proportion * Error              // 比例项           
		 +   pp->Integral * pp->SumError         // 积分项            
		 +   pp->Derivative * dError             // 微分项        
     );
}

/*=======================  初始化的pid结构  Initialize pid Structure===========================*/


void pidInit(struct pid *pp)
{    
        memset(pp,0,sizeof(struct pid));
}
/*************************************************************  位置式PID算法  结束 ***************************************************************/



#endif

