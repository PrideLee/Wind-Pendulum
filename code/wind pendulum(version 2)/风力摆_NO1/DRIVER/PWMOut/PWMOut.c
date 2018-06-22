/*
	TIM13 :PF8
	TIM14 :PF9
*/

#include "PWMOut.h"

uint16_t g_Period_TIM13=0;
uint16_t g_Period_TIM14=0;

/* 
	���ܣ�һ��PWM������ã�TIM13
	���룺Period			������
				Prescaler		����Ƶֵ
				Duty				��ռ�ձȣ��ǰٷֱ��ƣ�
	�������
	��ע��84MHz
*/
void PWMOut_TIM13(uint16_t Period, uint16_t Prescaler, uint16_t Duty)
{
	GPIO_InitTypeDef GPIO_I;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseI;
	TIM_OCInitTypeDef TIM_OCI;
	
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOF,	ENABLE);
	
	GPIO_PinAFConfig (GPIOF,GPIO_PinSource8,  GPIO_AF_TIM13);
	
	GPIO_I.GPIO_Mode=GPIO_Mode_AF ;
	GPIO_I.GPIO_OType=GPIO_OType_PP ;
	GPIO_I.GPIO_Pin=GPIO_Pin_8;
	GPIO_I.GPIO_PuPd=GPIO_PuPd_UP ;
	GPIO_I.GPIO_Speed=GPIO_High_Speed ;
	GPIO_Init (GPIOF,&GPIO_I);
	
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM13,ENABLE);
	
	g_Period_TIM13=Period;
	
	TIM_TimeBaseI.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseI.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseI.TIM_Period=Period-1;
	TIM_TimeBaseI.TIM_Prescaler=Prescaler-1;  
	TIM_TimeBaseInit (TIM13,&TIM_TimeBaseI);
	
//	TIM_OCI.TIM_OCIdleState=
	TIM_OCI.TIM_OCMode=TIM_OCMode_PWM1;
//	TIM_OCI.TIM_OCNIdleState=
//	TIM_OCI.TIM_OCNPolarity=
	TIM_OCI.TIM_OCPolarity=TIM_OCPolarity_High;
//	TIM_OCI.TIM_OutputNState=
	TIM_OCI.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCI.TIM_Pulse=Duty;
	TIM_OC1Init (TIM13,&TIM_OCI);
	TIM_OC1PreloadConfig (TIM13,ENABLE);
	TIM_ARRPreloadConfig (TIM13,ENABLE);
	
	TIM_Cmd(TIM13,ENABLE);
}

/* 
	���ܣ�һ��PWM������ã�TIM14
	���룺Period			������
				Prescaler		����Ƶֵ
				Duty				��ռ�ձȣ��ǰٷֱ��ƣ�
	�������
	��ע��84MHz
*/
void PWMOut_TIM14(uint16_t Period, uint16_t Prescaler, uint16_t Duty)
{
	GPIO_InitTypeDef GPIO_I;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseI;
	TIM_OCInitTypeDef TIM_OCI;
	
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOF,	ENABLE);
	
	GPIO_PinAFConfig (GPIOF,GPIO_PinSource9,  GPIO_AF_TIM14);
	
	GPIO_I.GPIO_Mode=GPIO_Mode_AF ;
	GPIO_I.GPIO_OType=GPIO_OType_PP ;
	GPIO_I.GPIO_Pin=GPIO_Pin_9;
	GPIO_I.GPIO_PuPd=GPIO_PuPd_UP ;
	GPIO_I.GPIO_Speed=GPIO_High_Speed ;
	GPIO_Init (GPIOF,&GPIO_I);
	
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM14,ENABLE);
	
	g_Period_TIM14=Period;
	
	TIM_TimeBaseI.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseI.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseI.TIM_Period=Period-1;
	TIM_TimeBaseI.TIM_Prescaler=Prescaler-1;  
	TIM_TimeBaseInit (TIM14,&TIM_TimeBaseI);
	
//	TIM_OCI.TIM_OCIdleState=
	TIM_OCI.TIM_OCMode=TIM_OCMode_PWM1;
//	TIM_OCI.TIM_OCNIdleState=
//	TIM_OCI.TIM_OCNPolarity=
	TIM_OCI.TIM_OCPolarity=TIM_OCPolarity_High;
//	TIM_OCI.TIM_OutputNState=
	TIM_OCI.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCI.TIM_Pulse=Duty;
	TIM_OC1Init (TIM14,&TIM_OCI);
	TIM_OC1PreloadConfig (TIM14,ENABLE);
	TIM_ARRPreloadConfig (TIM14,ENABLE);
	
	TIM_Cmd(TIM14,ENABLE);
}

/* 
	���ܣ�TIM13ռ�ձ�����
	���룺Duty				��ռ�ձȣ��ٷֱ��ƣ�
	�������
*/
void SetPWMDuty_TIM13(uint8_t Duty)
{
	uint16_t duty=0;
	duty=Duty*(g_Period_TIM13-1)/100;
	TIM_SetCompare1 (TIM13,duty);
}
/* 
	���ܣ�TIM14ռ�ձ�����
	���룺Duty				��ռ�ձȣ��ٷֱ��ƣ�
	�������
*/
void SetPWMDuty_TIM14(uint8_t Duty)
{
	uint16_t duty=0;
	duty=Duty*(g_Period_TIM14-1)/100;
	TIM_SetCompare1 (TIM14,duty);
}

/* 
	���ܣ�TIM13����/Ƶ������
	���룺Period				������
				Prescaler     ����Ƶֵ
        ChangePSC			���Ƿ�ı��Ƶֵ  1 change  0 no change
	�������
*/
void TIM13Change(uint16_t Period, uint16_t Prescaler, uint8_t ChangePSC)
{
	TIM_SetAutoreload (TIM13,Period-1);
	g_Period_TIM13=Period;
	if(ChangePSC)TIM_PrescalerConfig (TIM13,Prescaler-1,TIM_PSCReloadMode_Update);
}
/* 
	���ܣ�TIM14����/Ƶ������
	���룺Period				������
				Prescaler     ����Ƶֵ
        ChangePSC			���Ƿ�ı��Ƶֵ  1 change  0 no change
	�������
*/
void TIM14Change(uint16_t Period, uint16_t Prescaler, uint8_t ChangePSC)
{
	TIM_SetAutoreload (TIM14,Period-1);
	g_Period_TIM14=Period;
	if(ChangePSC)TIM_PrescalerConfig (TIM14,Prescaler-1,TIM_PSCReloadMode_Update);
}


/* 
	���ܣ�ֱ�ӵ�TIM13�ȽϼĴ���
	���룺Data				����ֵ
				Channel     ��ͨ�� 1 or 2
	�������
*/
void SetTIM13Compare (uint16_t Data)
{
	TIM_SetCompare1 (TIM13,Data);
}
/* 
	���ܣ�ֱ�ӵ�TIM14�ȽϼĴ���
	���룺Data				����ֵ
				Channel     ��ͨ�� 1 or 2
	�������
*/
void SetTIM14Compare (uint16_t Data)
{
	TIM_SetCompare1 (TIM14,Data);
}


