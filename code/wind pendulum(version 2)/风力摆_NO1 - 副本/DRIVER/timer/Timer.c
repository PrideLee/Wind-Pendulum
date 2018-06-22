#include "Timer.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "semphr.h"

//二值信号量句柄
SemaphoreHandle_t TIMER_BinarySemaphore;	//二值信号量句柄


void Timer10msInit(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBase;
	NVIC_InitTypeDef NVIC_I;
	
	//创建二值信号量
	TIMER_BinarySemaphore=xSemaphoreCreateBinary();	
	
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM6,ENABLE);
	
	TIM_TimeBase.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBase.TIM_CounterMode=TIM_CounterMode_Up ;
	TIM_TimeBase.TIM_Period=10000-1;
	TIM_TimeBase.TIM_Prescaler=84-1;
//	TIM_TimeBase.TIM_RepetitionCounter=
	TIM_TimeBaseInit (TIM6,&TIM_TimeBase);
	
	NVIC_I.NVIC_IRQChannel= TIM6_DAC_IRQn;
	NVIC_I.NVIC_IRQChannelCmd=ENABLE;
	NVIC_I.NVIC_IRQChannelPreemptionPriority=6;
	NVIC_I.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_I);
	
	TIM_ITConfig (TIM6,TIM_IT_Update,ENABLE);
	
	TIM_Cmd (TIM6,DISABLE);
	
	TIM_ClearITPendingBit (TIM6,TIM_IT_Update);
	TIM_ClearFlag (TIM6,TIM_FLAG_Update);
	
}

extern uint8_t test_flag;
void TIM6_DAC_IRQHandler()
{
	BaseType_t xHigherPriorityTaskWoken;
	
	if(TIM_GetITStatus (TIM6,TIM_IT_Update)!=RESET)
	{
//		test_flag=1;
		xSemaphoreGiveFromISR(TIMER_BinarySemaphore,&xHigherPriorityTaskWoken);	//释放二值信号量
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);//如果需要的话进行一次任务切换
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
