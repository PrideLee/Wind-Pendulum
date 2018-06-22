/*
		位置式PID
*/

#include "PID.h"
#include "Function.h"

#define T 0.01f   //积分时间
uint16_t r=30;  //积分分离阈值
float IE=1,DE=1;//是否积分或微分

#define Coefficient 0.5f  //电机系数


//********************************************************************************************************
//																					X方向
//********************************************************************************************************

float X_P=150.0f,X_I=0.9f,X_D=0.55f;      //PID参数  


float Xe=0.0f,Xe_1=0.0f;  //偏差（需更新）  Xe:当前偏差额e(k)  Xe_1：上一次偏差e(k-1)  Xe:再上一次的偏差e(k-2)
float XIerror=0.0f;

float X_PID(float X_Rlt, float X_E)
{
	float N_Reality=0,N_Exp=0;//实际值与期望值
	float XU_Out=0;//输出增量
	float K_P=0,K_I=0,K_D=0;//PID分量
	uint8_t f=0;//积分分离变量
	
	N_Reality=X_Rlt;
	N_Exp=X_E;
	//计算当前偏差
	Xe=N_Exp-N_Reality;//期望角-当前角
	//积分分离
	if(Xe>r||Xe<-r)f=0;
	else f=1;
	//分别计算P I D
	K_P=X_P*Xe;
	
	XIerror+=Xe;//更新积分偏差
	if(XIerror>1000)XIerror=1000;//积分限幅
	else if(XIerror<-1000)XIerror=-1000;
	K_I=(X_P*T/X_I)*XIerror;
	
	K_D=(X_P*X_D/T)*(Xe-Xe_1);
	
	//计算位置U
	XU_Out=K_P +K_I*f *IE +K_D;

	//更新偏差
	Xe_1=Xe;

	return XU_Out;	
}
//********************************************************************************************************
//																					Y方向
//********************************************************************************************************

float Y_P=150.0f,Y_I=0.85f,Y_D=0.55f;      //PID参数  


float Ye=0.0f,Ye_1=0.0f;  //偏差（需更新）  Ye:当前偏差额e(k)  Ye_1：上一次偏差e(k-1)  Ye:再上一次的偏差e(k-2)
float YIerror=0.0f;

float Y_PID(float Y_Rlt, float Y_E)
{
	float N_Reality=0,N_Exp=0;//实际值与期望值
	float YU_Out=0;//输出增量
	float K_P=0,K_I=0,K_D=0;//PID分量
	uint8_t f=0;//积分分离变量
	
	N_Reality=Y_Rlt;
	N_Exp=Y_E;
	//计算当前偏差
	Ye=N_Exp-N_Reality;//期望角-当前角
	//积分分离
	if(Ye>r||Xe<-r)f=0;
	else f=1;
	//分别计算P I D
	K_P=Y_P*Ye;
	
	YIerror+=Ye;//更新积分偏差
	if(YIerror>1000)YIerror=1000;//积分限幅
	if(YIerror<-1000)YIerror=-1000;
	K_I=(Y_P*T/Y_I)*YIerror;
	
	K_D=(Y_P*Y_D/T)*(Ye-Ye_1);
	
	//计算位置U
	YU_Out=K_P +K_I*f *IE +K_D;

	//更新偏差
	Ye_1=Ye;
	

	return YU_Out;	
}

void moto(float XU_Out, float YU_Out )
{
	uint16_t speed=0;
	
		
	//调电机
	if(XU_Out>=0)
	{
		speed=(uint16_t)(Coefficient*XU_Out);
		Set_MotoSpeed(speed+2000,3);
		Set_MotoSpeed(0,1);
	}
	else
	{
		speed=(uint16_t)(Coefficient*(-XU_Out));
		Set_MotoSpeed(speed+2000,1);
		Set_MotoSpeed(0,3);
	}
//	printf("%d  ",speed);
	
	
		//调电机
	if(YU_Out>=0)
	{
		speed=(uint16_t)(Coefficient*YU_Out);
		Set_MotoSpeed(speed+2000,2);
		Set_MotoSpeed(0,4);
	}
	else
	{
		speed=(uint16_t)(Coefficient*(-YU_Out));
		Set_MotoSpeed(speed+2000,4);
		Set_MotoSpeed(0,2);
	}
//	printf("%d\n",speed);
	
}

void SetPID(uint8_t tasknum)
{
	switch(tasknum)
	{
		case 1:{
							IE=1;
							X_P=150.0f; X_I=0.9f; X_D=0.55f;
							Y_P=150.0f; Y_I=0.85f; Y_D=0.55f;
							break;
						}
		case 2:{
							break;
						}
		case 3:{
							break;
						}
		case 4:{
							IE=0;
							X_P=80.0f; X_I=9.0f; X_D=0.5f;
							Y_P=50.0f; Y_I=9.0f; Y_D=0.55f;
							break;
						}
	}
}


