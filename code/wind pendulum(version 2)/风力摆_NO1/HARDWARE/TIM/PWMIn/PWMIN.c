/*
TIM4: PD 12 13 14 15
TIM5: PA 0  1  2  3

	�������Ϊ2Hz~6MHz��ʵ��Ӧ���ǲ�׼ȷ��
	����ȡ  4Hz~1MHz ��250ms~0.001ms T��   Ƶ��Խ��Խ����ȷ    ��Ƶ����״����������
	
*	��ʵ�飬ռ�ձȲ���̫�߻�̫�ͣ����ߵ�ƽ��͵�ƽʱ�䲻��̫С�������ж��ǳ�������Ҫʱ�䣬��Ҳ�������ˣ�
*	��PWM����Ƶ�ʴﵽ15KHz��duty<2% �� duty>97%  ʱ��PWM�����ѳ���
* NEED_FULLPWM=0 ʱ��PWMCH1.hightlevel�� 150,000 Hz��5% duty ʱ��׼

* �������ˣ�����Ƶ�ʾ�����Ҫ̫�߾��У�����������

* �� 15KHz  3%<=duty<=%97 ����NEED_FULLPWM1or0��׼ȷ ��
* �� 6KHz 1%<=duty<=%99 ����NEED_FULLPWM1or0��׼ȷ ��  8000Hzʱ 1%<=duty<=%99 ��׼
	
	��ǿ�ðɣ���������Щ��
	
**********************************************************************************************************
	���Լƻ����ö��ͨ�� TIM9Channel 2 ͨ��
						������  PWMOut_TIM9(uint16_t Period, uint16_t Prescaler, uint16_t Duty);  		��ʼ��
										SetPWMDuty_TIM9(uint8_t Duty, uint8_t Channel);												��ռ�ձ�
										SetTIM9Compare (uint16_t Data, uint8_t Channel)												ֱ�ӵ��ȽϼĴ���
										TIM9Change(uint16_t Period, uint16_t Prescaler, uint8_t ChangePSC);		��Ƶ
						Show(void);
						ChangeMode(uint8_t Open);
						ContinueShow();
						
						xHz x��Сֵ
						�ҳ�������� ��/�͵�ƽ��С����ʱ��  ��Ƶ����֤
						
	���Խ�� ��
						ÿ  1/12,000,000  �δ�һ��    ��С�����Ϊ 16 �� �δ𣬼� 1/750,000 s   !!!��Ҫ����!!!
						��/�͵�ƽ��Сʱ��Ϊ 1/750,000s ��Ҫռ�ձȷֱ��ʴ� 1% ���� ����Ƶ��<=7500Hz
						
						2Hz�ѳ��� 3Hz����
				
	7.5KHzʱ����С����� 1% 
	15KHzʱ�� ��С����� 2%  ����1/750000��/��1/15000�� 
	30KHzʱ�� ��С����� 4%
	�ֱ��ʣ�1/12,000,000s
**********************************************************************************************************
	
***ע�������ܻ��ж�������һ�����***
	
	duty��period Ϊ ms
	
	                                  ����������������ȡ����֮ǰ�� ReadPWM() ������������������
*/

#include "PWMIN.h"
#include "GPIO.h"
#include "usart.h"

uint8_t NEED_FULLPWM=1;
#define COUNT_CNT_OUT (uint8_t)100   // 0.5s //ÿ������ж�Ϊ 0.005s �� COUNT_CNT_OUT*0.005s  �����������Ԥ���ź��������ź�
																		 //�������С�����ٽ�Ƶ��Ϊ2Hz��ȡ��С����4Hz
struct R_PWM CH1,CH2,CH3,CH4;
struct R_PWM CH5,CH6,CH7,CH8;
struct PWM PWMCH1,PWMCH2,PWMCH3,PWMCH4;
struct PWM PWMCH5,PWMCH6,PWMCH7,PWMCH8;
	
void ReadPWM(void)
{
	if(CH1.rangeout)
	{
		if(NEED_FULLPWM)
		{
			PWMCH1.duty=CH1.level_rangeout;
			PWMCH1.period=CH1.level_rangeout;
		}
		PWMCH1.hightlevel=CH1.level_rangeout;
		PWMCH1.frequency=0;
	}
	else
		if(CH1.update_PWM)
		{
			PWMCH1.hightlevel=CH1.hightlevel_use-CH1.first_use+CH1.cnt_hightlevel_use*60000;
			if(NEED_FULLPWM)
			{
				PWMCH1.hightlevel=(CH1.hightlevel_use-CH1.first_use+CH1.cnt_hightlevel_use*60000)*(1.0/12000.0);
				PWMCH1.period=(CH1.period_use-CH1.first_use+CH1.cnt_period_use*60000)*(1.0/12000.0);
				PWMCH1.duty=PWMCH1.hightlevel*100/(PWMCH1.period+0.000001f);
				PWMCH1.frequency=1000/PWMCH1.period;
			}
			CH1.update_PWM=0;
		}

	if(CH2.rangeout)
	{
		if(NEED_FULLPWM)
		{
			PWMCH2.duty=CH2.level_rangeout;
			PWMCH2.period=CH2.level_rangeout;
		}
		PWMCH2.hightlevel=CH2.level_rangeout;
		PWMCH2.frequency=0;
	}
	else
		if(CH2.update_PWM)
		{
			PWMCH2.hightlevel=CH2.hightlevel_use-CH2.first_use+CH2.cnt_hightlevel_use*60000;
			if(NEED_FULLPWM)
			{
				PWMCH2.hightlevel=(CH2.hightlevel_use-CH2.first_use+CH2.cnt_hightlevel_use*60000)*(1.0/12000.0);
				PWMCH2.period=(CH2.period_use-CH2.first_use+CH2.cnt_period_use*60000)*(1.0/12000.0);
				PWMCH2.duty=PWMCH2.hightlevel*100/(PWMCH2.period+0.000001f);
				PWMCH2.frequency=1000/PWMCH2.period;
			}
			CH2.update_PWM=0;
		}
	if(CH3.rangeout)
	{
		if(NEED_FULLPWM)
		{
			PWMCH3.duty=CH3.level_rangeout;
			PWMCH3.period=CH3.level_rangeout;
		}
		PWMCH3.hightlevel=CH3.level_rangeout;
		PWMCH3.frequency=0;
	}
	else
		if(CH3.update_PWM)
		{
			PWMCH3.hightlevel=CH3.hightlevel_use-CH3.first_use+CH3.cnt_hightlevel_use*60000;
			if(NEED_FULLPWM)
			{
				PWMCH3.hightlevel=(CH3.hightlevel_use-CH3.first_use+CH3.cnt_hightlevel_use*60000)*(1.0/12000.0);
				PWMCH3.period=(CH3.period_use-CH3.first_use+CH3.cnt_period_use*60000)*(1.0/12000.0);
				PWMCH3.duty=PWMCH3.hightlevel*100/(PWMCH3.period+0.000001f);
				PWMCH3.frequency=1000/PWMCH3.period;
			}
			CH3.update_PWM=0;
		}
	if(CH4.rangeout)
	{
		if(NEED_FULLPWM)
		{
			PWMCH4.duty=CH4.level_rangeout;
			PWMCH4.period=CH4.level_rangeout;
		}
		PWMCH4.hightlevel=CH4.level_rangeout;
		PWMCH4.frequency=0;
	}
	else 
		if(CH4.update_PWM)
		{
			PWMCH4.hightlevel=CH4.hightlevel_use-CH4.first_use+CH4.cnt_hightlevel_use*60000;
			if(NEED_FULLPWM)
			{
				PWMCH4.hightlevel=(CH4.hightlevel_use-CH4.first_use+CH4.cnt_hightlevel_use*60000)*(1.0/12000.0);
				PWMCH4.period=(CH4.period_use-CH4.first_use+CH4.cnt_period_use*60000)*(1.0/12000.0);
				PWMCH4.duty=PWMCH4.hightlevel*100/(PWMCH4.period+0.000001f);
				PWMCH4.frequency=1000/PWMCH4.period;
			}
			CH4.update_PWM=0;
		}
		
	if(CH5.rangeout)
	{
		if(NEED_FULLPWM)
		{
			PWMCH5.duty=CH5.level_rangeout;
			PWMCH5.period=CH5.level_rangeout;
		}
		PWMCH5.hightlevel=CH5.level_rangeout;
		PWMCH5.frequency=0;
	}
	else
		if(CH5.update_PWM)
		{
			PWMCH5.hightlevel=CH5.hightlevel_use-CH5.first_use+CH5.cnt_hightlevel_use*60000;
			if(NEED_FULLPWM)
			{
				PWMCH5.hightlevel=(CH5.hightlevel_use-CH5.first_use+CH5.cnt_hightlevel_use*60000)*(1.0/12000.0);
				PWMCH5.period=(CH5.period_use-CH5.first_use+CH5.cnt_period_use*60000)*(1.0/12000.0);
				PWMCH5.duty=PWMCH5.hightlevel*100/(PWMCH5.period+0.000001f);
				PWMCH5.frequency=1000/PWMCH5.period;
			}
			CH5.update_PWM=0;
		}

	if(CH6.rangeout)
	{	
		if(NEED_FULLPWM)
		{
			PWMCH6.duty=CH6.level_rangeout;
			PWMCH6.period=CH6.level_rangeout;
		}
		PWMCH6.hightlevel=CH6.level_rangeout;
		PWMCH6.frequency=0;
	}
	else
		if(CH6.update_PWM)
		{
			PWMCH6.hightlevel=CH6.hightlevel_use-CH6.first_use+CH6.cnt_hightlevel_use*60000;
			if(NEED_FULLPWM)
			{
				PWMCH6.hightlevel=(CH6.hightlevel_use-CH6.first_use+CH6.cnt_hightlevel_use*60000)*(1.0/12000.0);
				PWMCH6.period=(CH6.period_use-CH6.first_use+CH6.cnt_period_use*60000)*(1.0/12000.0);
				PWMCH6.duty=PWMCH6.hightlevel*100/(PWMCH6.period+0.000001f);
				PWMCH6.frequency=1000/PWMCH6.period;
			}
			CH6.update_PWM=0;
		}
	if(CH7.rangeout)
	{
		if(NEED_FULLPWM)
		{
			PWMCH7.duty=CH7.level_rangeout;
			PWMCH7.period=CH7.level_rangeout;
		}
		PWMCH7.hightlevel=CH7.level_rangeout;
		PWMCH7.frequency=0;
	}
	else
		if(CH7.update_PWM)
		{
			PWMCH7.hightlevel=CH7.hightlevel_use-CH7.first_use+CH7.cnt_hightlevel_use*60000;
			if(NEED_FULLPWM)
			{
				PWMCH7.hightlevel=(CH7.hightlevel_use-CH7.first_use+CH7.cnt_hightlevel_use*60000)*(1.0/12000.0);
				PWMCH7.period=(CH7.period_use-CH7.first_use+CH7.cnt_period_use*60000)*(1.0/12000.0);
				PWMCH7.duty=PWMCH7.hightlevel*100/(PWMCH7.period+0.000001f);
				PWMCH7.frequency=1000/PWMCH7.period;
			}
			CH7.update_PWM=0;
		}
	if(CH8.rangeout)
	{
		if(NEED_FULLPWM)
		{
			PWMCH8.duty=CH8.level_rangeout;
			PWMCH8.period=CH8.level_rangeout;
		}
		PWMCH8.hightlevel=CH8.level_rangeout;
		PWMCH8.frequency=0;
	}
	else 
		if(CH8.update_PWM)
		{
			PWMCH8.hightlevel=CH8.hightlevel_use-CH8.first_use+CH8.cnt_hightlevel_use*60000;
			if(NEED_FULLPWM)
			{
				PWMCH8.hightlevel=(CH8.hightlevel_use-CH8.first_use+CH8.cnt_hightlevel_use*60000)*(1.0/12000.0);
				PWMCH8.period=(CH8.period_use-CH8.first_use+CH8.cnt_period_use*60000)*(1.0/12000.0);
				PWMCH8.duty=PWMCH8.hightlevel*100/(PWMCH8.period+0.000001f);
				PWMCH8.frequency=1000/PWMCH8.period;
			}
			CH8.update_PWM=0;
		}
}

void ShowPWM(uint8_t num)
{
	if(num==1||num==2||num==3)
	switch(num)
	{
		case 1:ShowHightLevel();
		case 2:ShowPeriod();
		case 3:Showduty();
	}
	else printf("show error!\n");
}

void ShowHightLevel(void)
{
	printf("%f  %f  %f  %f  %f  %f  %f  %f\n",PWMCH1.hightlevel,PWMCH2.hightlevel,PWMCH3.hightlevel,PWMCH4.hightlevel,PWMCH5.hightlevel,PWMCH6.hightlevel,PWMCH7.hightlevel,PWMCH8.hightlevel);
}
void Showduty(void)
{
	printf("%f  %f  %f  %f  %f  %f  %f  %f\n",PWMCH1.duty,PWMCH2.duty,PWMCH3.duty,PWMCH4.duty,PWMCH5.duty,PWMCH6.duty,PWMCH7.duty,PWMCH8.duty);
}
void ShowPeriod(void)
{
	printf("%f  %f  %f  %f  %f  %f  %f  %f\n",PWMCH1.period,PWMCH2.period,PWMCH3.period,PWMCH4.period,PWMCH5.period,PWMCH6.period,PWMCH7.period,PWMCH8.period);
}

void PWMIN_TIM4(void)
{
	GPIO_InitTypeDef GPIO_I;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseI;
	TIM_ICInitTypeDef TIM_ICI;
	NVIC_InitTypeDef NVIC_I;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_SYSCFG,ENABLE);
	
	GPIO_PinAFConfig (GPIOD,GPIO_PinSource12,GPIO_AF_TIM4);
	GPIO_PinAFConfig (GPIOD,GPIO_PinSource13,GPIO_AF_TIM4);
	GPIO_PinAFConfig (GPIOD,GPIO_PinSource14,GPIO_AF_TIM4);
	GPIO_PinAFConfig (GPIOD,GPIO_PinSource15,GPIO_AF_TIM4);
	
	GPIO_I.GPIO_Mode=GPIO_Mode_AF ;
	GPIO_I.GPIO_OType=GPIO_OType_PP ;
	GPIO_I.GPIO_Pin=GPIO_Pin_12;
	GPIO_I.GPIO_PuPd=GPIO_PuPd_DOWN ;
	GPIO_I.GPIO_Speed=GPIO_High_Speed ;
	GPIO_ResetBits(GPIOD,GPIO_Pin_12);
	GPIO_Init (GPIOD,&GPIO_I);
	
	GPIO_I.GPIO_Pin=GPIO_Pin_13;
	GPIO_ResetBits(GPIOD,GPIO_Pin_13);
	GPIO_Init (GPIOD,&GPIO_I);
	GPIO_I.GPIO_Pin=GPIO_Pin_14;
	GPIO_ResetBits(GPIOD,GPIO_Pin_14);
	GPIO_Init (GPIOD,&GPIO_I);
	GPIO_I.GPIO_Pin=GPIO_Pin_15;
	GPIO_ResetBits(GPIOD,GPIO_Pin_15);
	GPIO_Init (GPIOD,&GPIO_I);
	
	NVIC_I.NVIC_IRQChannel=TIM4_IRQn;
	NVIC_I.NVIC_IRQChannelCmd=ENABLE;
	NVIC_I.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_I.NVIC_IRQChannelSubPriority=0;
	NVIC_Init (&NVIC_I);
	
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM4,ENABLE);
	
	TIM_TimeBaseI.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseI.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseI.TIM_Period=60000-1;  //
	TIM_TimeBaseI.TIM_Prescaler=7-1;//  84/7=12   1/12 000 000  ÿ 1/12us ����++  ALLCNT= 0.005 s
	TIM_TimeBaseInit (TIM4,&TIM_TimeBaseI);
	
	TIM_ICI.TIM_Channel=TIM_Channel_1 ;
	TIM_ICI.TIM_ICFilter=0;
	TIM_ICI.TIM_ICPolarity=TIM_ICPolarity_Rising;
	TIM_ICI.TIM_ICPrescaler=TIM_ICPSC_DIV1;
	TIM_ICI.TIM_ICSelection=TIM_ICSelection_DirectTI ;
	TIM_ICInit(TIM4,&TIM_ICI);
	
	TIM_ICI.TIM_Channel=TIM_Channel_2 ;
	TIM_ICInit(TIM4,&TIM_ICI);
	TIM_ICI.TIM_Channel=TIM_Channel_3 ;
	TIM_ICInit(TIM4,&TIM_ICI);
	TIM_ICI.TIM_Channel=TIM_Channel_4 ;
	TIM_ICInit(TIM4,&TIM_ICI);
	
	TIM_ITConfig (TIM4,TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4|TIM_IT_Update,ENABLE);
	
	TIM_Cmd (TIM4,ENABLE);
	
	TIM_ClearFlag (TIM4,TIM_FLAG_Update|TIM_FLAG_CC1|TIM_FLAG_CC2|TIM_FLAG_CC3|TIM_FLAG_CC4);
	
}


void PWMIN_TIM5(void)
{
	GPIO_InitTypeDef GPIO_I;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseI;
	TIM_ICInitTypeDef TIM_ICI;
	NVIC_InitTypeDef NVIC_I;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_SYSCFG,ENABLE);
	
	GPIO_PinAFConfig (GPIOA,GPIO_PinSource0,GPIO_AF_TIM5);
	GPIO_PinAFConfig (GPIOA,GPIO_PinSource1,GPIO_AF_TIM5);
	GPIO_PinAFConfig (GPIOA,GPIO_PinSource2,GPIO_AF_TIM5);
	GPIO_PinAFConfig (GPIOA,GPIO_PinSource3,GPIO_AF_TIM5);
	
	GPIO_I.GPIO_Mode=GPIO_Mode_AF ;
	GPIO_I.GPIO_OType=GPIO_OType_PP ;
	GPIO_I.GPIO_Pin=GPIO_Pin_0;
	GPIO_I.GPIO_PuPd=GPIO_PuPd_DOWN ;
	GPIO_I.GPIO_Speed=GPIO_High_Speed ;
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	GPIO_Init (GPIOA,&GPIO_I);
	
	GPIO_I.GPIO_Pin=GPIO_Pin_1;
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	GPIO_Init (GPIOA,&GPIO_I);
	GPIO_I.GPIO_Pin=GPIO_Pin_2;
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);
	GPIO_Init (GPIOA,&GPIO_I);
	GPIO_I.GPIO_Pin=GPIO_Pin_3;
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);
	GPIO_Init (GPIOA,&GPIO_I);
	
	NVIC_I.NVIC_IRQChannel=TIM5_IRQn;
	NVIC_I.NVIC_IRQChannelCmd=ENABLE;
	NVIC_I.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_I.NVIC_IRQChannelSubPriority=1;
	NVIC_Init (&NVIC_I);
	
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM5,ENABLE);
	
	TIM_TimeBaseI.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseI.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseI.TIM_Period=60000-1;  //
	TIM_TimeBaseI.TIM_Prescaler=7-1;//  84/7=12   1/12 000 000  ÿ 1/12us ����++  ALLCNT= 0.005 s
	TIM_TimeBaseInit (TIM5,&TIM_TimeBaseI);
	
	TIM_ICI.TIM_Channel=TIM_Channel_1 ;
	TIM_ICI.TIM_ICFilter=0;
	TIM_ICI.TIM_ICPolarity=TIM_ICPolarity_Rising;
	TIM_ICI.TIM_ICPrescaler=TIM_ICPSC_DIV1;
	TIM_ICI.TIM_ICSelection=TIM_ICSelection_DirectTI ;
	TIM_ICInit(TIM5,&TIM_ICI);
	
	TIM_ICI.TIM_Channel=TIM_Channel_2 ;
	TIM_ICInit(TIM5,&TIM_ICI);
	TIM_ICI.TIM_Channel=TIM_Channel_3 ;
	TIM_ICInit(TIM5,&TIM_ICI);
	TIM_ICI.TIM_Channel=TIM_Channel_4 ;
	TIM_ICInit(TIM5,&TIM_ICI);
	
	TIM_ITConfig (TIM5,TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4|TIM_IT_Update,ENABLE);
	
	TIM_Cmd (TIM5,ENABLE);
	
	TIM_ClearFlag (TIM5,TIM_FLAG_Update|TIM_FLAG_CC1|TIM_FLAG_CC2|TIM_FLAG_CC3|TIM_FLAG_CC4);
	
	
}

void TIM4_IRQHandler(void)
{
	static uint8_t upordown[4]={1,1,1,1};
	
	if(TIM_GetITStatus(TIM4,TIM_IT_CC1)!=RESET)
	{
		if(upordown[0])
		{
			CH1.count_period=TIM4->CCR1;
			CH1.count_CNT_period=CH1.count_CNT;
			
			CH1.first_use=CH1.count_first;
			CH1.hightlevel_use=CH1.count_hightlevel;
			CH1.period_use=CH1.count_period;
			CH1.cnt_hightlevel_use=CH1.count_CNT_hightlevel;
			CH1.cnt_period_use=CH1.count_CNT_period;
			CH1.update_PWM=1;
			CH1.rangeout=0;
			
			CH1.count_first=TIM4->CCR1;
			CH1.count_CNT=0;
			
			upordown[0]=0;
			TIM_OC1PolarityConfig (TIM4,TIM_OCPolarity_Low);
		}
		else
		{
			CH1.count_hightlevel=TIM4->CCR1;
			CH1.count_CNT_hightlevel=CH1.count_CNT;
			
			TIM_OC1PolarityConfig (TIM4,TIM_OCPolarity_High);
			upordown[0]=1;
		}
		
		TIM4->SR = (uint16_t)~TIM_IT_CC1;
	}
	if(TIM_GetITStatus(TIM4,TIM_IT_CC2)!=RESET)
	{
		if(upordown[1])
		{
			CH2.count_period=TIM_GetCapture2(TIM4);
			CH2.count_CNT_period=CH2.count_CNT;
			
			CH2.first_use=CH2.count_first;
			CH2.hightlevel_use=CH2.count_hightlevel;
			CH2.period_use=CH2.count_period;
			CH2.cnt_hightlevel_use=CH2.count_CNT_hightlevel;
			CH2.cnt_period_use=CH2.count_CNT_period;
			CH2.update_PWM=1;
			CH2.rangeout=0;
			
			CH2.count_first=TIM_GetCapture2(TIM4);
			CH2.count_CNT=0;
			
			upordown[1]=0;
			TIM_OC2PolarityConfig (TIM4,TIM_OCPolarity_Low);
		}
		else
		{
			CH2.count_hightlevel=TIM_GetCapture2(TIM4);
			CH2.count_CNT_hightlevel=CH2.count_CNT;
			
			TIM_OC2PolarityConfig (TIM4,TIM_OCPolarity_High);
			upordown[1]=1;
		}
		
		TIM_ClearITPendingBit (TIM4,TIM_IT_CC2);
	}
	if(TIM_GetITStatus(TIM4,TIM_IT_CC3)!=RESET)
	{
		if(upordown[2])
		{
			CH3.count_period=TIM_GetCapture3(TIM4);
			CH3.count_CNT_period=CH3.count_CNT;
			
			CH3.first_use=CH3.count_first;
			CH3.hightlevel_use=CH3.count_hightlevel;
			CH3.period_use=CH3.count_period;
			CH3.cnt_hightlevel_use=CH3.count_CNT_hightlevel;
			CH3.cnt_period_use=CH3.count_CNT_period;
			CH3.update_PWM=1;
			CH3.rangeout=0;
			
			CH3.count_first=TIM_GetCapture3(TIM4);
			CH3.count_CNT=0;
			
			upordown[2]=0;
			TIM_OC3PolarityConfig (TIM4,TIM_OCPolarity_Low);
		}
		else
		{
			CH3.count_hightlevel=TIM_GetCapture3(TIM4);
			CH3.count_CNT_hightlevel=CH3.count_CNT;
			
			TIM_OC3PolarityConfig (TIM4,TIM_OCPolarity_High);
			upordown[2]=1;
		}
		
		TIM_ClearITPendingBit (TIM4,TIM_IT_CC3);
	}
	if(TIM_GetITStatus(TIM4,TIM_IT_CC4)!=RESET)
	{
		if(upordown[3])
		{
			CH4.count_period=TIM_GetCapture4(TIM4);
			CH4.count_CNT_period=CH4.count_CNT;
			
			CH4.first_use=CH4.count_first;
			CH4.hightlevel_use=CH4.count_hightlevel;
			CH4.period_use=CH4.count_period;
			CH4.cnt_hightlevel_use=CH4.count_CNT_hightlevel;
			CH4.cnt_period_use=CH4.count_CNT_period;
			CH4.update_PWM=1;
			CH4.rangeout=0;
			
			CH4.count_first=TIM_GetCapture4(TIM4);
			CH4.count_CNT=0;
			
			upordown[3]=0;
			TIM_OC4PolarityConfig (TIM4,TIM_OCPolarity_Low);
		}
		else
		{
			CH4.count_hightlevel=TIM_GetCapture4(TIM4);
			CH4.count_CNT_hightlevel=CH4.count_CNT;
			
			TIM_OC4PolarityConfig (TIM4,TIM_OCPolarity_High);
			upordown[3]=1;
		}
		
		TIM_ClearITPendingBit (TIM4,TIM_IT_CC4);
	}
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET)
	{
		CH1.count_CNT++;
		CH2.count_CNT++;
		CH3.count_CNT++;
		CH4.count_CNT++;
		if(CH1.count_CNT>=COUNT_CNT_OUT)
		{
			CH1.count_CNT=0;
			
			CH1.cnt_hightlevel_use=0;
			CH1.cnt_period_use=0;
			CH1.count_CNT=0;
			CH1.count_CNT_hightlevel=0;
			CH1.count_CNT_period=0;
			CH1.count_hightlevel=0;
			CH1.count_first=0;
			CH1.count_period=0;
			CH1.hightlevel_use=0;
			CH1.first_use=0;
			CH1.period_use=0;
			CH1.update_PWM=0;
			
			CH1.level_rangeout=GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12);
			CH1.rangeout=1;
		}
		if(CH2.count_CNT>=COUNT_CNT_OUT)
		{
			CH2.count_CNT=0;
			
			CH2.cnt_hightlevel_use=0;
			CH2.cnt_period_use=0;
			CH2.count_CNT=0;
			CH2.count_CNT_hightlevel=0;
			CH2.count_CNT_period=0;
			CH2.count_hightlevel=0;
			CH2.count_first=0;
			CH2.count_period=0;
			CH2.hightlevel_use=0;
			CH2.first_use=0;
			CH2.period_use=0;
			CH2.update_PWM=0;
			
			CH2.level_rangeout=GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_13);
			CH2.rangeout=1;
		}
		if(CH3.count_CNT>=COUNT_CNT_OUT)
		{
			CH3.count_CNT=0;
			
			CH3.cnt_hightlevel_use=0;
			CH3.cnt_period_use=0;
			CH3.count_CNT=0;
			CH3.count_CNT_hightlevel=0;
			CH3.count_CNT_period=0;
			CH3.count_hightlevel=0;
			CH3.count_first=0;
			CH3.count_period=0;
			CH3.hightlevel_use=0;
			CH3.first_use=0;
			CH3.period_use=0;
			CH3.update_PWM=0;
			
			CH3.level_rangeout=GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_14);
			CH3.rangeout=1;
		}
		if(CH4.count_CNT>=COUNT_CNT_OUT)
		{
			CH4.count_CNT=0;
			
			CH4.cnt_hightlevel_use=0;
			CH4.cnt_period_use=0;
			CH4.count_CNT=0;
			CH4.count_CNT_hightlevel=0;
			CH4.count_CNT_period=0;
			CH4.count_hightlevel=0;
			CH4.count_first=0;
			CH4.count_period=0;
			CH4.hightlevel_use=0;
			CH4.first_use=0;
			CH4.period_use=0;
			CH4.update_PWM=0;
			
			CH4.level_rangeout=GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_15);
			CH4.rangeout=1;
		}
		TIM_ClearITPendingBit (TIM4,TIM_IT_Update);
	}
}

/*
		
*/
void TIM5_IRQHandler(void)
{
	static uint8_t upordown[4]={1,1,1,1};
	
	if(TIM_GetITStatus(TIM5,TIM_IT_CC1)!=RESET)
	{
		if(upordown[0])
		{
			CH5.count_period=TIM_GetCapture1(TIM5);
			CH5.count_CNT_period=CH5.count_CNT;
			
			CH5.first_use=CH5.count_first;
			CH5.hightlevel_use=CH5.count_hightlevel;
			CH5.period_use=CH5.count_period;
			CH5.cnt_hightlevel_use=CH5.count_CNT_hightlevel;
			CH5.cnt_period_use=CH5.count_CNT_period;
			CH5.update_PWM=1;
			CH5.rangeout=0;
			
			CH5.count_first=TIM_GetCapture1(TIM5);
			CH5.count_CNT=0;
			
			upordown[0]=0;
			TIM_OC1PolarityConfig (TIM5,TIM_OCPolarity_Low);
		}
		else
		{
			CH5.count_hightlevel=TIM_GetCapture1(TIM5);
			CH5.count_CNT_hightlevel=CH5.count_CNT;
			
			TIM_OC1PolarityConfig (TIM5,TIM_OCPolarity_High);
			upordown[0]=1;
		}
		
		TIM_ClearITPendingBit (TIM5,TIM_IT_CC1);
	}
	if(TIM_GetITStatus(TIM5,TIM_IT_CC2)!=RESET)
	{
		if(upordown[1])
		{
			CH6.count_period=TIM_GetCapture2(TIM5);
			CH6.count_CNT_period=CH6.count_CNT;
			
			CH6.first_use=CH6.count_first;
			CH6.hightlevel_use=CH6.count_hightlevel;
			CH6.period_use=CH6.count_period;
			CH6.cnt_hightlevel_use=CH6.count_CNT_hightlevel;
			CH6.cnt_period_use=CH6.count_CNT_period;
			CH6.update_PWM=1;
			CH6.rangeout=0;
			
			CH6.count_first=TIM_GetCapture2(TIM5);
			CH6.count_CNT=0;
			
			upordown[1]=0;
			TIM_OC2PolarityConfig (TIM5,TIM_OCPolarity_Low);
		}
		else
		{
			CH6.count_hightlevel=TIM_GetCapture2(TIM5);
			CH6.count_CNT_hightlevel=CH6.count_CNT;
			
			TIM_OC2PolarityConfig (TIM5,TIM_OCPolarity_High);
			upordown[1]=1;
		}
		
		TIM_ClearITPendingBit (TIM5,TIM_IT_CC2);
	}
	if(TIM_GetITStatus(TIM5,TIM_IT_CC3)!=RESET)
	{
		if(upordown[2])
		{
			CH7.count_period=TIM_GetCapture3(TIM5);
			CH7.count_CNT_period=CH7.count_CNT;
			
			CH7.first_use=CH7.count_first;
			CH7.hightlevel_use=CH7.count_hightlevel;
			CH7.period_use=CH7.count_period;
			CH7.cnt_hightlevel_use=CH7.count_CNT_hightlevel;
			CH7.cnt_period_use=CH7.count_CNT_period;
			CH7.update_PWM=1;
			CH7.rangeout=0;
			
			CH7.count_first=TIM_GetCapture3(TIM5);
			CH7.count_CNT=0;
			
			upordown[2]=0;
			TIM_OC3PolarityConfig (TIM5,TIM_OCPolarity_Low);
		}
		else
		{
			CH7.count_hightlevel=TIM_GetCapture3(TIM5);
			CH7.count_CNT_hightlevel=CH7.count_CNT;
			
			TIM_OC3PolarityConfig (TIM5,TIM_OCPolarity_High);
			upordown[2]=1;
		}
		
		TIM_ClearITPendingBit (TIM5,TIM_IT_CC3);
	}
	if(TIM_GetITStatus(TIM5,TIM_IT_CC4)!=RESET)
	{
		if(upordown[3])
		{
			CH8.count_period=TIM_GetCapture4(TIM5);
			CH8.count_CNT_period=CH8.count_CNT;
			
			CH8.first_use=CH8.count_first;
			CH8.hightlevel_use=CH8.count_hightlevel;
			CH8.period_use=CH8.count_period;
			CH8.cnt_hightlevel_use=CH8.count_CNT_hightlevel;
			CH8.cnt_period_use=CH8.count_CNT_period;
			CH8.update_PWM=1;
			CH8.rangeout=0;
			
			CH8.count_first=TIM_GetCapture4(TIM5);
			CH8.count_CNT=0;
			
			upordown[3]=0;
			TIM_OC4PolarityConfig (TIM5,TIM_OCPolarity_Low);
		}
		else
		{
			CH8.count_hightlevel=TIM_GetCapture4(TIM5);
			CH8.count_CNT_hightlevel=CH8.count_CNT;
			
			TIM_OC4PolarityConfig (TIM5,TIM_OCPolarity_High);
			upordown[3]=1;
		}
		
		TIM_ClearITPendingBit (TIM5,TIM_IT_CC4);
	}
	if(TIM_GetITStatus(TIM5,TIM_IT_Update)!=RESET)
	{
		CH5.count_CNT++;
		CH6.count_CNT++;
		CH7.count_CNT++;
		CH8.count_CNT++;
		if(CH5.count_CNT>=COUNT_CNT_OUT)
		{
			CH5.count_CNT=0;
			
			CH5.cnt_hightlevel_use=0;
			CH5.cnt_period_use=0;
			CH5.count_CNT=0;
			CH5.count_CNT_hightlevel=0;
			CH5.count_CNT_period=0;
			CH5.count_hightlevel=0;
			CH5.count_first=0;
			CH5.count_period=0;
			CH5.hightlevel_use=0;
			CH5.first_use=0;
			CH5.period_use=0;
			CH5.update_PWM=0;
			
			CH5.level_rangeout=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);
			CH5.rangeout=1;
		}
		if(CH6.count_CNT>=COUNT_CNT_OUT)
		{
			CH6.count_CNT=0;
			
			CH6.cnt_hightlevel_use=0;
			CH6.cnt_period_use=0;
			CH6.count_CNT=0;
			CH6.count_CNT_hightlevel=0;
			CH6.count_CNT_period=0;
			CH6.count_hightlevel=0;
			CH6.count_first=0;
			CH6.count_period=0;
			CH6.hightlevel_use=0;
			CH6.first_use=0;
			CH6.period_use=0;
			CH6.update_PWM=0;
			
			CH6.level_rangeout=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1);
			CH6.rangeout=1;
		}
		if(CH7.count_CNT>=COUNT_CNT_OUT)
		{
			CH7.count_CNT=0;
			
			CH7.cnt_hightlevel_use=0;
			CH7.cnt_period_use=0;
			CH7.count_CNT=0;
			CH7.count_CNT_hightlevel=0;
			CH7.count_CNT_period=0;
			CH7.count_hightlevel=0;
			CH7.count_first=0;
			CH7.count_period=0;
			CH7.hightlevel_use=0;
			CH7.first_use=0;
			CH7.period_use=0;
			CH7.update_PWM=0;
			
			CH7.level_rangeout=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2);
			CH7.rangeout=1;
		}
		if(CH8.count_CNT>=COUNT_CNT_OUT)
		{
			CH8.count_CNT=0;
			
			CH8.cnt_hightlevel_use=0;
			CH8.cnt_period_use=0;
			CH8.count_CNT=0;
			CH8.count_CNT_hightlevel=0;
			CH8.count_CNT_period=0;
			CH8.count_hightlevel=0;
			CH8.count_first=0;
			CH8.count_period=0;
			CH8.hightlevel_use=0;
			CH8.first_use=0;
			CH8.period_use=0;
			CH8.update_PWM=0;
			
			CH8.level_rangeout=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3);
			CH8.rangeout=1;
		}
		TIM_ClearITPendingBit (TIM5,TIM_IT_Update);
	}
	
}

void Show(void)
{
	ReadPWM(); 
	printf("%f  ",PWMCH1.hightlevel);
	if(NEED_FULLPWM)
	{
		printf("%f  ",PWMCH1.period);   printf("%f  ",PWMCH1.duty);   printf("%f  ",PWMCH1.frequency);
	}
	printf("\n");
}

/* 
	���ܣ��ı����ģʽ  NEED_FULLPWM=0  or  NEED_FULLPWM=1
	���룺Open��  1  or  0
	�������
*/
void ChangeMode(uint8_t Open)
{
	if(Open)NEED_FULLPWM=1;
	else NEED_FULLPWM=0;
}

/*
	�Ƿ����ⲿ������ʾ
*/
uint8_t Yes=0;
void ContinueShow(uint8_t Y)
{
	if(Y)Yes=1;
	else Yes=0;
}

