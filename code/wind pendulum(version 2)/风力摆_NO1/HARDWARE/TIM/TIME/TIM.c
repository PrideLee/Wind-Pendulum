/*
	APB1��42MHz
	APB2��84MHz
	��ʱ��ʱ���� APBx �ṩ��APBʱ�ӷ�Ƶϵ����Ϊ1�����Զ�ʱ����ʱ���� APBx * 2
	
	TIMx->CR1 |= TIM_CR1_CEN;//                            ENABLE
	TIMx->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN));//   DISABLE

*/

#include "TIM.h"
#include "delay.h"
#include "LED.h"
#include "KEY.h"
#include "BEED.h" 
#include "usart.h"


/* 
	���ܣ�TIM6��ʱ������
	���룺��
	�������
	��ע��Ĭ�� 1ms �ж�   84��Ƶ �� 1us 
*/
void TIM6_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBase;
	NVIC_InitTypeDef NVIC_I;
	
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM6,ENABLE);
	
	TIM_TimeBase.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBase.TIM_CounterMode=TIM_CounterMode_Up ;
	TIM_TimeBase.TIM_Period=1000;
	TIM_TimeBase.TIM_Prescaler=84-1;
//	TIM_TimeBase.TIM_RepetitionCounter=
	TIM_TimeBaseInit (TIM6,&TIM_TimeBase);
	
	NVIC_I.NVIC_IRQChannel= TIM6_DAC_IRQn;
	NVIC_I.NVIC_IRQChannelCmd=ENABLE;
	NVIC_I.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_I.NVIC_IRQChannelSubPriority=3;
	NVIC_Init(&NVIC_I);
	
	TIM_ITConfig (TIM6,TIM_IT_Update,ENABLE);
	
	TIM_Cmd (TIM6,ENABLE);
	
	TIM_ClearITPendingBit (TIM6,TIM_IT_Update);
	TIM_ClearFlag (TIM6,TIM_FLAG_Update);
}

void TIM6_DAC_IRQHandler()
{
	if(TIM_GetITStatus (TIM6,TIM_IT_Update)!=RESET)
	{
//		LED0=~LED0;
		TIM_ClearITPendingBit (TIM6,TIM_IT_Update);
	}
}

void TIM6Open(void)
{
	TIM6->CR1 |= TIM_CR1_CEN;//                            ENABLE
}
void TIM6Close(void)
{
	TIM6->CR1 &= (uint16_t)(~((uint16_t)TIM_CR1_CEN));//   DISABLE
}
/* 
	���ܣ�TIM6��ʱʱ������
	���룺Preiod ����ʱʱ��=Preiod* 1us ��Ĭ�ϣ�
	�������
	��ע��Ĭ�� 84��Ƶ �� 1us 
*/
void SetTime(uint16_t Period)
{
	TIM_SetAutoreload (TIM6,Period);
}
/* 
	���ܣ���������TIM6��Ƶϵ��
	���룺PSC_ ����Ƶϵ��
	�������
	��ע�������������Ҫ�������� SetTime(uint16_t Preiod)
*/
void SetPSC(uint16_t PSC_)
{
	TIM_PrescalerConfig (TIM6,PSC_,TIM_PSCReloadMode_Update);
}


/********************************************************************************/// TIM7
/* TIM7������USMART */
/********************************************************************************/// TIM7

//void TIM7_Init(void)
//{
//	TIM_TimeBaseInitTypeDef  TIM_TimeBase;
//	NVIC_InitTypeDef NVIC_I;
//	
//	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM7,ENABLE);
//	
//	TIM_TimeBase.TIM_ClockDivision=TIM_CKD_DIV1;
//	TIM_TimeBase.TIM_CounterMode=TIM_CounterMode_Up ;
//	TIM_TimeBase.TIM_Period=100;
//	TIM_TimeBase.TIM_Prescaler=8400-1;
////	TIM_TimeBase.TIM_RepetitionCounter=
//	TIM_TimeBaseInit (TIM7,&TIM_TimeBase);
//	
//	NVIC_I.NVIC_IRQChannel= TIM7_IRQn;
//	NVIC_I.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_I.NVIC_IRQChannelPreemptionPriority=1;
//	NVIC_I.NVIC_IRQChannelSubPriority=1;
//	NVIC_Init(&NVIC_I);
//	
//	TIM_ITConfig (TIM7,TIM_IT_Update,ENABLE);
//	
//	TIM_Cmd (TIM7,ENABLE);
//	
//	TIM_ClearITPendingBit (TIM7,TIM_IT_Update);
//	TIM_ClearFlag (TIM7,TIM_FLAG_Update);
//}

//void TIM7_IRQHandler()
//{
//	if(TIM_GetITStatus (TIM7,TIM_IT_Update)!=RESET)
//	{
//		TIM_ClearITPendingBit (TIM7,TIM_IT_Update);
//	}
//}



