/*
TIM1:  PA 8  9 10 11    //
TIM3:  PA 6  7          // 8路输出
			 PB       0  1    //

TIM9:  PE 5  6          //舵机 1 2
TIM12: PB 14 15					//     3 4
TIM10: PF 6             //         1
TIM11: PF 7							//步进电机 2
TIM13: PF 8							//         3
TIM14: PF 9							//         4

8路输出 15KHz

*/

#include "PWMOut8.h"

uint16_t g_Period_TIM1= 5600 ;// 1/84,000,000  *  g_period    5600  为15KHz
uint16_t g_Period_TIM3= 5600 ;

/* 1400Hz ~ 168KHz */
void TIM1_PWMOutInit(uint32_t Hz)
{
	uint16_t period=0;// 
	GPIO_InitTypeDef GPIO_I;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseI;
	TIM_OCInitTypeDef TIM_OCI;
	
	if(Hz<=1400)Hz=1400;
	else if(Hz>=168000)Hz=168000;

	period=84000000/Hz;
	g_Period_TIM1=period;
	
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOE,	ENABLE);
	
	GPIO_PinAFConfig (GPIOE,GPIO_PinSource9,  GPIO_AF_TIM1);
	GPIO_PinAFConfig (GPIOE,GPIO_PinSource11, GPIO_AF_TIM1);
	GPIO_PinAFConfig (GPIOE,GPIO_PinSource13, GPIO_AF_TIM1);
	GPIO_PinAFConfig (GPIOE,GPIO_PinSource14, GPIO_AF_TIM1);
	
	GPIO_I.GPIO_Mode=GPIO_Mode_AF ;
	GPIO_I.GPIO_OType=GPIO_OType_PP ;
	GPIO_I.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_14;
	GPIO_I.GPIO_PuPd=GPIO_PuPd_UP ;
	GPIO_I.GPIO_Speed=GPIO_High_Speed ;
	GPIO_Init (GPIOE,&GPIO_I);
	
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_TIM1,ENABLE);
	TIM_TimeBaseI.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseI.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseI.TIM_Period=g_Period_TIM1-1;
	TIM_TimeBaseI.TIM_Prescaler=2-1;  //  1/84,000,000
	TIM_TimeBaseInit (TIM1,&TIM_TimeBaseI);
	
//	TIM_OCI.TIM_OCIdleState=
	TIM_OCI.TIM_OCMode=TIM_OCMode_PWM1;
//	TIM_OCI.TIM_OCNIdleState=
//	TIM_OCI.TIM_OCNPolarity=
	TIM_OCI.TIM_OCPolarity=TIM_OCPolarity_High;
//	TIM_OCI.TIM_OutputNState=
	TIM_OCI.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCI.TIM_Pulse=(uint16_t)(0);
	TIM_OC1Init (TIM1,&TIM_OCI);
	
	TIM_OCI.TIM_Pulse=(uint16_t)(0);
	TIM_OC2Init (TIM1,&TIM_OCI);
	
	TIM_OCI.TIM_Pulse=(uint16_t)(0);
	TIM_OC3Init (TIM1,&TIM_OCI);
	
	TIM_OCI.TIM_Pulse=(uint16_t)(0);
	TIM_OC4Init (TIM1,&TIM_OCI);
	
	TIM_OC1PreloadConfig (TIM1,ENABLE);
	TIM_ARRPreloadConfig (TIM1,ENABLE);
	TIM_CtrlPWMOutputs (TIM1,ENABLE);
	
	TIM_Cmd(TIM1,ENABLE);
	
}

void TIM3_PWMOutInit(uint32_t Hz)
{
	uint16_t period=0;// 
	GPIO_InitTypeDef GPIO_I;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseI;
	TIM_OCInitTypeDef TIM_OCI;
	
	if(Hz<=1400)Hz=1400;
	else if(Hz>=168000)Hz=168000;

	period=84000000/Hz;
	g_Period_TIM3=period;

	
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA,	ENABLE);
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOB,	ENABLE);
	
	GPIO_PinAFConfig (GPIOA,GPIO_PinSource6, GPIO_AF_TIM3);
	GPIO_PinAFConfig (GPIOA,GPIO_PinSource7, GPIO_AF_TIM3);
	GPIO_PinAFConfig (GPIOB,GPIO_PinSource0, GPIO_AF_TIM3);
	GPIO_PinAFConfig (GPIOB,GPIO_PinSource1, GPIO_AF_TIM3);
	
	GPIO_I.GPIO_Mode=GPIO_Mode_AF ;
	GPIO_I.GPIO_OType=GPIO_OType_PP ;
	GPIO_I.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
	GPIO_I.GPIO_PuPd=GPIO_PuPd_UP ;
	GPIO_I.GPIO_Speed=GPIO_High_Speed ;
	GPIO_Init (GPIOA,&GPIO_I);
		GPIO_I.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
		GPIO_Init (GPIOB,&GPIO_I);
	
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM3,ENABLE);
	TIM_TimeBaseI.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseI.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseI.TIM_Period=g_Period_TIM3-1;
	TIM_TimeBaseI.TIM_Prescaler=0;  //  1/84,000,000
	TIM_TimeBaseInit (TIM3,&TIM_TimeBaseI);
	
//	TIM_OCI.TIM_OCIdleState=
	TIM_OCI.TIM_OCMode=TIM_OCMode_PWM1;
//	TIM_OCI.TIM_OCNIdleState=
//	TIM_OCI.TIM_OCNPolarity=
	TIM_OCI.TIM_OCPolarity=TIM_OCPolarity_High;
//	TIM_OCI.TIM_OutputNState=
	TIM_OCI.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCI.TIM_Pulse=(uint16_t)(40000);
	TIM_OC1Init (TIM3,&TIM_OCI);
	
	TIM_OCI.TIM_Pulse=(uint16_t)(40000);
	TIM_OC2Init (TIM3,&TIM_OCI);
	
	TIM_OCI.TIM_Pulse=(uint16_t)(40000);
	TIM_OC3Init (TIM3,&TIM_OCI);
	
	TIM_OCI.TIM_Pulse=(uint16_t)(40000);
	TIM_OC4Init (TIM3,&TIM_OCI);
	
	TIM_OC1PreloadConfig (TIM3,ENABLE);
	TIM_ARRPreloadConfig (TIM3,ENABLE);
	
	TIM_Cmd(TIM3,ENABLE);
	
}

/* 
	功能：TIM1占空比设置
	输入：Duty				：占空比（百分比制）
				Channel     ：通道
	输出：无
*/
void SetPWMDuty_TIM1(uint8_t Duty, uint8_t Channel)
{
	uint16_t ccrx=0;
	
	ccrx=g_Period_TIM1*Duty/100;
	
	switch(Channel)
	{
		case 1:TIM1->CCR1 = ccrx ;break;
		case 2:TIM1->CCR2 = ccrx ;break;
		case 3:TIM1->CCR3 = ccrx ;break;
		case 4:TIM1->CCR4 = ccrx ;break;
	}
}
	
/* 
	功能：TIM3占空比设置
	输入：Duty				：占空比（百分比制）
				Channel     ：通道
	输出：无
*/
void SetPWMDuty_TIM3(uint8_t Duty, uint8_t Channel)
{
	uint16_t ccrx=0;
	
	ccrx=g_Period_TIM1*Duty/100;
	
	switch(Channel)
	{
		case 1:TIM3->CCR1 = ccrx ;break;
		case 2:TIM3->CCR2 = ccrx ;break;
		case 3:TIM3->CCR3 = ccrx ;break;
		case 4:TIM3->CCR4 = ccrx ;break;
	}
}


/* 
	功能：TIM1周期/频率设置
	输入：Period				：周期
				Prescaler     ：分频值
        ChangePSC			：是否改变分频值  1 change  0 no change
	输出：无
*/
void TIM1Change(uint16_t Period, uint16_t Prescaler, uint8_t ChangePSC)
{
	TIM_SetAutoreload (TIM1,Period-1);
	g_Period_TIM1=Period;
	if(ChangePSC)TIM_PrescalerConfig (TIM1,Prescaler-1,TIM_PSCReloadMode_Update);
}
/* 
	功能：TIM3周期/频率设置
	输入：Period				：周期
				Prescaler     ：分频值
        ChangePSC			：是否改变分频值  1 change  0 no change
	输出：无
*/
void TIM3Change(uint16_t Period, uint16_t Prescaler, uint8_t ChangePSC)
{
	TIM_SetAutoreload (TIM3,Period-1);
	g_Period_TIM3=Period;
	if(ChangePSC)TIM_PrescalerConfig (TIM3,Prescaler-1,TIM_PSCReloadMode_Update);
}

/* 
	功能：直接调TIM1比较寄存器
	输入：Data				：新值
				Channel     ：通道 
	输出：无
*/
void SetTIM1Compare (uint16_t Data, uint8_t Channel)
{
	switch(Channel)
	{
		case 1:TIM_SetCompare1 (TIM1,Data);break;
		case 2:TIM_SetCompare2 (TIM1,Data);break;
		case 3:TIM_SetCompare3 (TIM1,Data);break;
		case 4:TIM_SetCompare4 (TIM1,Data);break;
	}
}
/* 
	功能：直接调TIM3比较寄存器
	输入：Data				：新值
				Channel     ：通道 1 or 2
	输出：无
*/
void SetTIM3Compare (uint16_t Data, uint8_t Channel)
{
	switch(Channel)
	{
		case 1:TIM_SetCompare1 (TIM3,Data);break;
		case 2:TIM_SetCompare2 (TIM3,Data);break;
		case 3:TIM_SetCompare3 (TIM3,Data);break;
		case 4:TIM_SetCompare4 (TIM3,Data);break;
	}
}


void PWMInit(void)
{
	TIM3_PWMOutInit(10000);
	TIM3Change(40000,42,1);// 50Hz  20ms
	SetTIM3Compare(2000,1);
	SetTIM3Compare(2000,2);
	SetTIM3Compare(2000,3);
	SetTIM3Compare(2000,4);
}



