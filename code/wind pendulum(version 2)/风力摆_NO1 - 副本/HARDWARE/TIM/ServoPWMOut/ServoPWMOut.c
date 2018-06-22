/*
	TIM9  :PE 5  6
	TIM12 :PB 14 15
*/

#include "ServoPWMOut.h"

uint16_t g_Period_TIM9=0;
uint16_t g_Period_TIM12=0;

/* 
	���ܣ�һ��PWM������ã�TIM9
	���룺Period			������
				Prescaler		����Ƶֵ
				Duty				��ռ�ձȣ��ǰٷֱ��ƣ�
	�������
	��ע��168MHz
*/
void PWMOut_TIM9(uint16_t Period, uint16_t Prescaler, uint16_t Duty)
{
	GPIO_InitTypeDef GPIO_I;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseI;
	TIM_OCInitTypeDef TIM_OCI;
	
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOE,	ENABLE);
	
	GPIO_PinAFConfig (GPIOE,GPIO_PinSource5,  GPIO_AF_TIM9);
	GPIO_PinAFConfig (GPIOE,GPIO_PinSource6,  GPIO_AF_TIM9);
	
	GPIO_I.GPIO_Mode=GPIO_Mode_AF ;
	GPIO_I.GPIO_OType=GPIO_OType_PP ;
	GPIO_I.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6;
	GPIO_I.GPIO_PuPd=GPIO_PuPd_UP ;
	GPIO_I.GPIO_Speed=GPIO_High_Speed ;
	GPIO_Init (GPIOE,&GPIO_I);
	
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_TIM9,ENABLE);
	
	g_Period_TIM9=Period;
	
	TIM_TimeBaseI.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseI.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseI.TIM_Period=Period-1;
	TIM_TimeBaseI.TIM_Prescaler=Prescaler-1;  
	TIM_TimeBaseInit (TIM9,&TIM_TimeBaseI);
	
//	TIM_OCI.TIM_OCIdleState=
	TIM_OCI.TIM_OCMode=TIM_OCMode_PWM1;
//	TIM_OCI.TIM_OCNIdleState=
//	TIM_OCI.TIM_OCNPolarity=
	TIM_OCI.TIM_OCPolarity=TIM_OCPolarity_High;
//	TIM_OCI.TIM_OutputNState=
	TIM_OCI.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCI.TIM_Pulse=Duty;
	TIM_OC1Init (TIM9,&TIM_OCI);
	TIM_OC2Init (TIM9,&TIM_OCI);
	
	TIM_OC1PreloadConfig (TIM9,ENABLE);
	TIM_ARRPreloadConfig (TIM9,ENABLE);
	
	TIM_Cmd(TIM9,ENABLE);
}

/* 
	���ܣ�һ��PWM������ã�TIM12
	���룺Period			������
				Prescaler		����Ƶֵ
				Duty				��ռ�ձȣ��ǰٷֱ��ƣ�
	�������
	��ע��84MHz
*/
void PWMOut_TIM12(uint16_t Period, uint16_t Prescaler, uint16_t Duty)
{
	GPIO_InitTypeDef GPIO_I;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseI;
	TIM_OCInitTypeDef TIM_OCI;
	
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOB,	ENABLE);
	
	GPIO_PinAFConfig (GPIOB,GPIO_PinSource14,  GPIO_AF_TIM12);
	GPIO_PinAFConfig (GPIOB,GPIO_PinSource15,  GPIO_AF_TIM12);
	
	GPIO_I.GPIO_Mode=GPIO_Mode_AF ;
	GPIO_I.GPIO_OType=GPIO_OType_PP ;
	GPIO_I.GPIO_Pin=GPIO_Pin_14|GPIO_Pin_15;
	GPIO_I.GPIO_PuPd=GPIO_PuPd_UP ;
	GPIO_I.GPIO_Speed=GPIO_High_Speed ;
	GPIO_Init (GPIOB,&GPIO_I);
	
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM12,ENABLE);
	
	g_Period_TIM12=Period;
	
	TIM_TimeBaseI.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseI.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseI.TIM_Period=Period-1;
	TIM_TimeBaseI.TIM_Prescaler=Prescaler-1;  
	TIM_TimeBaseInit (TIM12,&TIM_TimeBaseI);
	
//	TIM_OCI.TIM_OCIdleState=
	TIM_OCI.TIM_OCMode=TIM_OCMode_PWM1;
//	TIM_OCI.TIM_OCNIdleState=
//	TIM_OCI.TIM_OCNPolarity=
	TIM_OCI.TIM_OCPolarity=TIM_OCPolarity_High;
//	TIM_OCI.TIM_OutputNState=
	TIM_OCI.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCI.TIM_Pulse=Duty;
	TIM_OC1Init (TIM12,&TIM_OCI);
	TIM_OC1Init (TIM12,&TIM_OCI);
	
	TIM_OC1PreloadConfig (TIM12,ENABLE);
	TIM_ARRPreloadConfig (TIM12,ENABLE);
	
	TIM_Cmd(TIM12,ENABLE);
}

/* 
	���ܣ�TIM9ռ�ձ�����
	���룺Duty				��ռ�ձȣ��ٷֱ��ƣ�
				Channel     ��ͨ��
	�������
*/
void SetPWMDuty_TIM9(uint8_t Duty, uint8_t Channel)
{
	uint16_t duty=0;
	duty=Duty*(g_Period_TIM9-1)/100;
	if(Channel==1)TIM_SetCompare1 (TIM9,duty);
	else if(Channel==2)TIM_SetCompare2 (TIM9,duty);
	
}
/* 
	���ܣ�TIM12ռ�ձ�����
	���룺Duty				��ռ�ձȣ��ٷֱ��ƣ�
				Channel     ��ͨ��
	�������
*/
void SetPWMDuty_TIM12(uint8_t Duty, uint8_t Channel)
{
	uint16_t duty=0;
	duty=Duty*(g_Period_TIM12-1)/100;
	if(Channel==1)TIM_SetCompare1 (TIM12,duty);
	else if(Channel==2)TIM_SetCompare2 (TIM12,duty);
}

/* 
	���ܣ�TIM9����/Ƶ������
	���룺Period				������
				Prescaler     ����Ƶֵ
        ChangePSC			���Ƿ�ı��Ƶֵ  1 change  0 no change
	�������
*/
void TIM9Change(uint16_t Period, uint16_t Prescaler, uint8_t ChangePSC)
{
	TIM_SetAutoreload (TIM9,Period-1);
	g_Period_TIM9=Period;
	if(ChangePSC)TIM_PrescalerConfig (TIM9,Prescaler-1,TIM_PSCReloadMode_Update);
}
/* 
	���ܣ�TIM12����/Ƶ������
	���룺Period				������
				Prescaler     ����Ƶֵ
        ChangePSC			���Ƿ�ı��Ƶֵ  1 change  0 no change
	�������
*/
void TIM12Change(uint16_t Period, uint16_t Prescaler, uint8_t ChangePSC)
{
	TIM_SetAutoreload (TIM12,Period-1);
	g_Period_TIM12=Period;
	if(ChangePSC)TIM_PrescalerConfig (TIM12,Prescaler-1,TIM_PSCReloadMode_Update);
}

/* 
	���ܣ�ֱ�ӵ�TIM9�ȽϼĴ���
	���룺Data				����ֵ
				Channel     ��ͨ�� 1 or 2
	�������
*/
void SetTIM9Compare (uint16_t Data, uint8_t Channel)
{
	if(Channel==1)TIM_SetCompare1 (TIM9,Data);
	else  if(Channel==2)TIM_SetCompare2 (TIM9,Data);
}
/* 
	���ܣ�ֱ�ӵ�TIM12�ȽϼĴ���
	���룺Data				����ֵ
				Channel     ��ͨ�� 1 or 2
	�������
*/
void SetTIM12Compare (uint16_t Data, uint8_t Channel)
{
	if(Channel==1)TIM_SetCompare1 (TIM12,Data);
	else  if(Channel==2)TIM_SetCompare2 (TIM12,Data);
}

