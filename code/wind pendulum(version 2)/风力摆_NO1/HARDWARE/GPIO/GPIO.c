#include "GPIO.h"

/* 
	功能：IO口普通推挽输出设置
	输入：PA~PG                    :GPIO口
				GPIO_Pin_0~GPIO_Pin_15   :引脚
	输出：无
*/
void GPIO_GeneralOutInit(uint8_t Px , uint32_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_I;
	GPIO_TypeDef  *GPIOx;
	uint8_t PX;
	
	PX=Px;
	switch(PX)
	{
		case 0:GPIOx=GPIOA; RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);break;
		case 1:GPIOx=GPIOB; RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);break;
		case 2:GPIOx=GPIOC; RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);break;
		case 3:GPIOx=GPIOD; RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);break;
		case 4:GPIOx=GPIOE; RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);break;
		case 5:GPIOx=GPIOF; RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);break;
		case 6:GPIOx=GPIOG; RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);break;
	}
	
	GPIO_I.GPIO_Mode=GPIO_Mode_OUT ;
	GPIO_I.GPIO_OType=GPIO_OType_PP ;
	GPIO_I.GPIO_Pin=GPIO_Pin ;
	GPIO_I.GPIO_PuPd=GPIO_PuPd_UP ;
	GPIO_I.GPIO_Speed=GPIO_High_Speed ;
	GPIO_SetBits(GPIOx,GPIO_Pin);
	GPIO_Init (GPIOx,&GPIO_I);
	
	
}

/*
	功能：IO口普通上下拉输入设置
	输入：PA~PG                    :GPIO口
				GPIO_Pin_0~GPIO_Pin_15   :引脚
				0 or 1									 :0 下拉    1 上拉
	输出：无
*/
void GPIO_GeneralInInit(uint8_t Px , uint32_t GPIO_Pin, uint8_t Sta)
{
	GPIO_InitTypeDef GPIO_I;
	GPIO_TypeDef  *GPIOx;
	uint8_t PX,sta;
	
	sta=Sta;
	PX=Px;
	switch(PX)
	{
		case 0:GPIOx=GPIOA; RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);break;
		case 1:GPIOx=GPIOB; RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);break;
		case 2:GPIOx=GPIOC; RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);break;
		case 3:GPIOx=GPIOD; RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);break;
		case 4:GPIOx=GPIOE; RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);break;
		case 5:GPIOx=GPIOF; RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);break;
		case 6:GPIOx=GPIOG; RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);break;
	}
	
	GPIO_I.GPIO_Mode=GPIO_Mode_IN  ;
//	GPIO_I.GPIO_OType=GPIO_OType_PP ;
	GPIO_I.GPIO_Pin=GPIO_Pin ;
	
	if(sta)GPIO_I.GPIO_PuPd=GPIO_PuPd_UP ;
	else GPIO_I.GPIO_PuPd=GPIO_PuPd_DOWN ;
	
	GPIO_I.GPIO_Speed=GPIO_High_Speed ;
	GPIO_Init (GPIOx,&GPIO_I);
	
}


/*
  功能：IO口普通中断输入设置
	输入：PA~PG                    :GPIO口
				GPIO_Pin_0~GPIO_Pin_15   :引脚
				0 or 1									 :0 下降沿中断    1 上升沿中断 
				Preemption               :抢占优先级
				Sub											 :相应优先级
	输出：无
	备注：中断函数得另外写
*/
void EXTI_GeneralInInit(uint8_t Px , uint32_t GPIO_Pin, uint8_t Sta , uint8_t Preemption , uint8_t Sub)
{
	EXTI_InitTypeDef EXTI_I;
	NVIC_InitTypeDef NVIC_I;
	uint32_t EXTI_line;
	uint8_t EXTIx_IRQn;
	
	if(Sta)GPIO_GeneralInInit(Px,GPIO_Pin,0);
	else GPIO_GeneralInInit(Px,GPIO_Pin,1);
	
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_SYSCFG,ENABLE);
	
	switch(GPIO_Pin)
	{
		case GPIO_Pin_0 :SYSCFG_EXTILineConfig ((uint8_t)Px,EXTI_PinSource0);EXTI_line=EXTI_Line0;EXTIx_IRQn=6;break;
		case GPIO_Pin_1 :SYSCFG_EXTILineConfig ((uint8_t)Px,EXTI_PinSource1);EXTI_line=EXTI_Line1;EXTIx_IRQn=7;break;
		case GPIO_Pin_2 :SYSCFG_EXTILineConfig ((uint8_t)Px,EXTI_PinSource2);EXTI_line=EXTI_Line2;EXTIx_IRQn=8;break;
		case GPIO_Pin_3 :SYSCFG_EXTILineConfig ((uint8_t)Px,EXTI_PinSource3);EXTI_line=EXTI_Line3;EXTIx_IRQn=9;break;
		case GPIO_Pin_4 :SYSCFG_EXTILineConfig ((uint8_t)Px,EXTI_PinSource4);EXTI_line=EXTI_Line4;EXTIx_IRQn=10;break;
		case GPIO_Pin_5 :SYSCFG_EXTILineConfig ((uint8_t)Px,EXTI_PinSource5);EXTI_line=EXTI_Line5;EXTIx_IRQn=23;break;
		case GPIO_Pin_6 :SYSCFG_EXTILineConfig ((uint8_t)Px,EXTI_PinSource6);EXTI_line=EXTI_Line6;EXTIx_IRQn=23;break;
		case GPIO_Pin_7 :SYSCFG_EXTILineConfig ((uint8_t)Px,EXTI_PinSource7);EXTI_line=EXTI_Line7;EXTIx_IRQn=23;break;
		case GPIO_Pin_8 :SYSCFG_EXTILineConfig ((uint8_t)Px,EXTI_PinSource8);EXTI_line=EXTI_Line8;EXTIx_IRQn=23;break;
		case GPIO_Pin_9 :SYSCFG_EXTILineConfig ((uint8_t)Px,EXTI_PinSource9);EXTI_line=EXTI_Line9;EXTIx_IRQn=23;break;
		case GPIO_Pin_10:SYSCFG_EXTILineConfig ((uint8_t)Px,EXTI_PinSource10);EXTI_line=EXTI_Line10;EXTIx_IRQn=40;break;
		case GPIO_Pin_11:SYSCFG_EXTILineConfig ((uint8_t)Px,EXTI_PinSource11);EXTI_line=EXTI_Line11;EXTIx_IRQn=40;break;
		case GPIO_Pin_12:SYSCFG_EXTILineConfig ((uint8_t)Px,EXTI_PinSource12);EXTI_line=EXTI_Line12;EXTIx_IRQn=40;break;
		case GPIO_Pin_13:SYSCFG_EXTILineConfig ((uint8_t)Px,EXTI_PinSource13);EXTI_line=EXTI_Line13;EXTIx_IRQn=40;break;
		case GPIO_Pin_14:SYSCFG_EXTILineConfig ((uint8_t)Px,EXTI_PinSource14);EXTI_line=EXTI_Line14;EXTIx_IRQn=40;break;
		case GPIO_Pin_15:SYSCFG_EXTILineConfig ((uint8_t)Px,EXTI_PinSource15);EXTI_line=EXTI_Line15;EXTIx_IRQn=40;break;
	}
	
	EXTI_I.EXTI_Line=EXTI_line;
	EXTI_I.EXTI_LineCmd=ENABLE;
	EXTI_I.EXTI_Mode=EXTI_Mode_Interrupt ;
	if(Sta)EXTI_I.EXTI_Trigger=EXTI_Trigger_Rising;
	else EXTI_I.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init (&EXTI_I);
	
	NVIC_I.NVIC_IRQChannel=EXTIx_IRQn;
	NVIC_I.NVIC_IRQChannelCmd=ENABLE;
	NVIC_I.NVIC_IRQChannelPreemptionPriority=Preemption;
	NVIC_I.NVIC_IRQChannelSubPriority=Sub;
	NVIC_Init (&NVIC_I);
	
}


//{
//  GPIO_InitTypeDef GPIO_I;
//	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
//
//	GPIO_I.GPIO_Mode=GPIO_Mode_OUT ;
//	GPIO_I.GPIO_OType=GPIO_OType_PP ;
//	GPIO_I.GPIO_Pin=GPIO_Pin_0 ;
//	GPIO_I.GPIO_PuPd=GPIO_PuPd_UP ;
//	GPIO_I.GPIO_Speed=GPIO_High_Speed ;
//	GPIO_SetBits(GPIOA,GPIO_Pin);
//	GPIO_Init (GPIOA,&GPIO_I);
//}

