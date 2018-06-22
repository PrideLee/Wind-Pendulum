/**************************************************************** 
��������void TIM3_Config(void)  
����  ������TIM3�����PWM�źŵ�ģʽ 
CH1:��� T=2.5ms(f=1/2.5ms=400Hz)  D=0.6��PWM��(�ߵ�ƽ��ǰ���͵�ƽ�ں�) 
 *      CH2:��� T=2.5ms(f=1/2.5ms=400Hz)  D=0.4��PWM��(�ߵ�ƽ�ں󣬵͵�ƽ��ǰ) 
 *      ����һ��ͨ��T��TIMxCLK��ʱ��Դȷ��TIM_Period��TIM_Prescaler  
 *          T=(TIM_Period+1)*(TIM_Prescaler+1)/TIMxCLK=2.5ms  
 *          ��Ϊ TIM_Period<65535������ TIM_Prescaler>1,�� TIM_Prescaler=2 
 *          ���� TIM_Period=59999=0xEA5F 
 *      �����������TIM_Period��ֵ���ߵ͵�ƽ���Ⱥ�D��ȷ��CCR��TIM_OCPolarity 
 *          CH1����ΪD=0.6���ȸߺ�ͣ� 
 *              ����CCR1=(TIM_Period+1)* D=36000;TIM_OCPolarity=TIM_OCPolarity_High 
 *          CH2����ΪD=0.4���ȸߺ�ͣ� 
 *              ����CCR1=(TIM_Period+1)* (1-D)=36000;TIM_OCPolarity=TIM_OCPolarity_Low 
 *      �������������Ĵ�����ʼ�� 
 *      �����ģ�ͨ���Ĵ�����ʼ�� 
 *      �����壺ʹ��TIM3���ؼĴ���ARR 
 *      ��������ʹ��TIM3  
 * ����  ����  
 * ���  ����  
 * ����  ��main()����  
 ***************************************************************/  
 
#include "Time3_pwm_mode.h"
void Time3pwmGPIO_Config()
{
	//����GPIO A6
	GPIO_InitTypeDef GPIO_InitStructure;
	/* GPIOA  clock enable */ 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);  
	/*GPIOA Configuration: TIM3 channel 1  as alternate function push-pull */ 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7 ; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           // ����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	//����GPIO B0,B1
	/* GPIOA  clock enable */ 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  
	/*GPIOA Configuration: TIM3 channel 1  as alternate function push-pull */ 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           // ����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
}

void TIM3_Config(u16 duty_ratio)  
{  
	u16 CCR1,CCR2,CCR3,CCR4;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;  
    TIM_OCInitTypeDef  TIM_OCInitStructure; 
      
    /*PCLK1����2��Ƶ����ΪTIM3��ʱ��Դ����72MHz*/  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  
    /* Time base configuration */                                            
    TIM_TimeBaseStructure.TIM_Period =59999;  
    TIM_TimeBaseStructure.TIM_Prescaler = 2;                                    //����Ԥ��Ƶ��Ԥ��Ƶ=2����Ϊ72/3=24MHz  
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;                                //����ʱ�ӷ�Ƶϵ��������Ƶ  
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                 //���ϼ������ģʽ  
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);  
    /* PWM1 Mode configuration: Channel1 */  
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                           //����ΪPWMģʽ1  
	
	/* ͨ��1  PWM�źŵ�ƽ����ֵ */ 
	CCR1=59999*duty_ratio/100;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
    TIM_OCInitStructure.TIM_Pulse = CCR1;                                       //��������ֵ�������������������ֵʱ����ƽ��������  
    TIM_OCInitStructure.TIM_OCPolarity =TIM_OCPolarity_High;                    //����ʱ������ֵС��CCR1ʱΪ�ߵ�ƽ  
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);                                    //ʹ��ͨ��1      
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  
 
	/* ͨ��2  PWM�źŵ�ƽ����ֵ */ 
	CCR2=59999*duty_ratio/100;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
    TIM_OCInitStructure.TIM_Pulse = CCR2;                                       //��������ֵ�������������������ֵʱ����ƽ��������  
    TIM_OCInitStructure.TIM_OCPolarity =TIM_OCPolarity_High;                    //����ʱ������ֵС��CCR1ʱΪ�ߵ�ƽ  
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);                                    //ʹ��ͨ��2      
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
 
	/* ͨ��3  PWM�źŵ�ƽ����ֵ */ 
	CCR3=59999*duty_ratio/100;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
    TIM_OCInitStructure.TIM_Pulse = CCR3;                                       //��������ֵ�������������������ֵʱ����ƽ��������  
    TIM_OCInitStructure.TIM_OCPolarity =TIM_OCPolarity_High;                    //����ʱ������ֵС��CCR1ʱΪ�ߵ�ƽ  
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);                                    //ʹ��ͨ��1      
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  
 
	/* ͨ��4  PWM�źŵ�ƽ����ֵ */ 
	CCR4=59999*duty_ratio/100;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
    TIM_OCInitStructure.TIM_Pulse = CCR4;                                       //��������ֵ�������������������ֵʱ����ƽ��������  
    TIM_OCInitStructure.TIM_OCPolarity =TIM_OCPolarity_High;                    //����ʱ������ֵС��CCR1ʱΪ�ߵ�ƽ  
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);                                    //ʹ��ͨ��2      
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	
	TIM_ARRPreloadConfig(TIM3, ENABLE);                                         //ʹ��TIM3���ؼĴ���ARR  
    /* TIM3 enable counter */  
    TIM_Cmd(TIM3, ENABLE);                                                      //ʹ��TIM3   
}

void pwm1_ratio(u16 ratio)
{
	TIM3->CCR1=ratio;
}
