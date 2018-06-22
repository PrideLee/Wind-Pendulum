/**************************************************************** 
函数名：void TIM3_Config(void)  
描述  ：配置TIM3输出的PWM信号的模式 
CH1:输出 T=2.5ms(f=1/2.5ms=400Hz)  D=0.6的PWM波(高电平在前，低电平在后) 
 *      CH2:输出 T=2.5ms(f=1/2.5ms=400Hz)  D=0.4的PWM波(高电平在后，低电平在前) 
 *      步骤一：通过T和TIMxCLK的时钟源确定TIM_Period和TIM_Prescaler  
 *          T=(TIM_Period+1)*(TIM_Prescaler+1)/TIMxCLK=2.5ms  
 *          因为 TIM_Period<65535，所以 TIM_Prescaler>1,即 TIM_Prescaler=2 
 *          所以 TIM_Period=59999=0xEA5F 
 *      步骤二：根据TIM_Period的值，高低电平的先后D，确定CCR和TIM_OCPolarity 
 *          CH1：因为D=0.6，先高后低； 
 *              所以CCR1=(TIM_Period+1)* D=36000;TIM_OCPolarity=TIM_OCPolarity_High 
 *          CH2：因为D=0.4，先高后低； 
 *              所以CCR1=(TIM_Period+1)* (1-D)=36000;TIM_OCPolarity=TIM_OCPolarity_Low 
 *      步骤三：基础寄存器初始化 
 *      步骤四：通道寄存器初始化 
 *      步骤五：使能TIM3重载寄存器ARR 
 *      步骤六：使能TIM3  
 * 输入  ：无  
 * 输出  ：无  
 * 调用  ：main()调用  
 ***************************************************************/  
 
#include "Time3_pwm_mode.h"
void Time3pwmGPIO_Config()
{
	//配置GPIO A6
	GPIO_InitTypeDef GPIO_InitStructure;
	/* GPIOA  clock enable */ 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);  
	/*GPIOA Configuration: TIM3 channel 1  as alternate function push-pull */ 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7 ; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           // 复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	//配置GPIO B0,B1
	/* GPIOA  clock enable */ 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  
	/*GPIOA Configuration: TIM3 channel 1  as alternate function push-pull */ 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           // 复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
}

void TIM3_Config(u16 duty_ratio)  
{  
	u16 CCR1,CCR2,CCR3,CCR4;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;  
    TIM_OCInitTypeDef  TIM_OCInitStructure; 
      
    /*PCLK1经过2倍频后作为TIM3的时钟源等于72MHz*/  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  
    /* Time base configuration */                                            
    TIM_TimeBaseStructure.TIM_Period =59999;  
    TIM_TimeBaseStructure.TIM_Prescaler = 2;                                    //设置预分频：预分频=2，即为72/3=24MHz  
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;                                //设置时钟分频系数：不分频  
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                 //向上计数溢出模式  
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);  
    /* PWM1 Mode configuration: Channel1 */  
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                           //配置为PWM模式1  
	
	/* 通道1  PWM信号电平跳变值 */ 
	CCR1=59999*duty_ratio/100;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
    TIM_OCInitStructure.TIM_Pulse = CCR1;                                       //设置跳变值，当计数器计数到这个值时，电平发生跳变  
    TIM_OCInitStructure.TIM_OCPolarity =TIM_OCPolarity_High;                    //当定时器计数值小于CCR1时为高电平  
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);                                    //使能通道1      
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  
 
	/* 通道2  PWM信号电平跳变值 */ 
	CCR2=59999*duty_ratio/100;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
    TIM_OCInitStructure.TIM_Pulse = CCR2;                                       //设置跳变值，当计数器计数到这个值时，电平发生跳变  
    TIM_OCInitStructure.TIM_OCPolarity =TIM_OCPolarity_High;                    //当定时器计数值小于CCR1时为高电平  
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);                                    //使能通道2      
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
 
	/* 通道3  PWM信号电平跳变值 */ 
	CCR3=59999*duty_ratio/100;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
    TIM_OCInitStructure.TIM_Pulse = CCR3;                                       //设置跳变值，当计数器计数到这个值时，电平发生跳变  
    TIM_OCInitStructure.TIM_OCPolarity =TIM_OCPolarity_High;                    //当定时器计数值小于CCR1时为高电平  
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);                                    //使能通道1      
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  
 
	/* 通道4  PWM信号电平跳变值 */ 
	CCR4=59999*duty_ratio/100;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
    TIM_OCInitStructure.TIM_Pulse = CCR4;                                       //设置跳变值，当计数器计数到这个值时，电平发生跳变  
    TIM_OCInitStructure.TIM_OCPolarity =TIM_OCPolarity_High;                    //当定时器计数值小于CCR1时为高电平  
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);                                    //使能通道2      
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	
	TIM_ARRPreloadConfig(TIM3, ENABLE);                                         //使能TIM3重载寄存器ARR  
    /* TIM3 enable counter */  
    TIM_Cmd(TIM3, ENABLE);                                                      //使能TIM3   
}

void pwm1_ratio(u16 ratio)
{
	TIM3->CCR1=ratio;
}
