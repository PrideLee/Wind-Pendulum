#include "stm32f4xx.h" 

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "usart2.h"	

#include "FreeRTOS.h"
#include "task.h"

#include "PWMIN.h"
#include "APP.h"
#include "BEED.h" 
#include "led.h"
#include "KEY.h"
#include "PWMOut8.h"
#include "MPU6050.h"
#include "lcd.h"
#include "Timer.h"

#include "Function.h"
//************************************************
//				函数
//************************************************
void _Test_(void);//（此函数用于硬件测试，不经过FreeRTOS）
void Throttle_adjust(void);
void MPU_adjust(void);
//************************************************
//				数据
//************************************************
uint8_t Debug=0;
uint8_t tasknum=0;

uint8_t use_freertos=1;//在输出中用

int main()
{
	uint8_t i=1;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	delay_init(168);
	uart_init(115200);
	uart2_init(9600);
	
	PWMIN_TIM4();
	LED_Init();
	BEED_Init(1);
	KEY_Init();
	PWMInit();
	MPU6050_Init();
	LCD_Init();
	Timer10msInit();
	
	LCD_Display_Dir(0);
	LCD_AUTOScan_Dir(L2R_U2D);
	LCD_DisplayOn();
	
	if(KEY_Scan(1))// 油门行程校准
		{
			BEED_1(50);
			LCD_DisplayString(50,50,12,"Set PWM");
			LCD_DisplayString(50,70,12,"KEY2 EXIT");
			Throttle_adjust();
			LCD_Fill_onecolor(50,50,240,82,WHITE);
		}
		
	if(!MPU6050_Init())//检查MPU6050
	{
		printf("MPU ERR\n");
		while(1);
	}
	printf("MPU OK\n");
	MPU_adjust();//MPU6050调零
	
	LCD_DisplayString(10,10,12,"Ready");
	
	LCD_DisplayString(50,100,12,"Debug");
	LCD_DisplayString(50,120,12,"yes KEY1 no KEY2");
	
	
	while(i)//是否开启调试
	{
		if(KEY_Scan(1))
		{
			LCD_DisplayString(80,10,12,"Debug");
			i=0;
			BEED_1(50);
			Debug=1;
		}
		if(KEY_Scan(2))
		{
			i=0;
			BEED_1(50);
			Debug=0;
		}
	}
	LCD_Fill_onecolor(50,50,240,132,WHITE);
	LCD_DisplayString(50,50,12,"GO");
	
//	_Test_();
	AllAppInit();
	vTaskStartScheduler();          //开启任务调度
	
	while(1)
	{
		;
	}
}


/*
	描述：此函数仅仅用于测试硬件（LED\BEED\MPU6050\PWM）是否正常
*/
uint8_t test_flag=0;
void _Test_(void)
{
//	uint16_t Speed=1000;
//	uint8_t i=1;
	struct MPU6050 mpu;
	struct Attitude AT;
	
//	while(i)//先按KEY1
//	{
//		if(KEY_Scan(1))
//		{
//			i=0;
//			LED_set(0,0);
//			BEED=1;
//			delay_ms(200);
//			BEED=0;
//			LED_set(0,1);
//		}
//	}
//	i=1;
//	while(i)//再按KEY2
//	{
//		if(KEY_Scan(2))
//		{
//			i=0;
//			LED_set(1,0);
//			BEED=1;
//			delay_ms(200);
//			BEED=0;
//			LED_set(1,1);
//		}
//	}
//	i=1;
//	while(i)//再按KEY3
//	{
//		if(KEY_Scan(3))
//		{
//			i=0;
//			LED_set(2,0);
//			BEED=1;
//			delay_ms(200);
//			BEED=0;
//			LED_set(2,1);
//		}
//	}
	TIM6Open();
	while(1)
	{
		if(test_flag)
		{
			ReadMpu(&mpu) ;
			IMUupdate(mpu.Gx, mpu.Gy, mpu.Gz, mpu.Ax, mpu.Ay, mpu.Az, &AT);
			printf("%f \n",AT.X);
			test_flag=0;
		}
	}
	
//	while(1)
//	{
//		if(Speed==1000)i=1;
//		if(Speed==19000)i=0;
//		
//		if(i) Speed+=1000;
//		else Speed-=1000;
//		
//			Set_MotoSpeed(Speed,1);
//			Set_MotoSpeed(Speed,2);
//			Set_MotoSpeed(Speed,3);
//			Set_MotoSpeed(Speed,4);
//		
//		if(ReadMpu(&mpu))
//		{
//		//融合
//		
//		//输出
////		  OUTput(1);
//			USART_Senser_Data_output(&mpu,0);
//		}
//		else printf("mpu error\n");
//		
//		
//		delay_ms(100);
//	}
}

//
//  PWM.CH1 接收机输入至芯片      CH2电机通道输出至CH2以检查电机输出
//
void Throttle_adjust()
{
	uint16_t speed=0;
	uint8_t i=1;
	float duty=0;
	
	while(i)
	{
		ReadPWM();
		if(PWMCH1.duty<=5.3f)duty=5;  
			else 
				if(PWMCH1.duty>=9.5f)duty=10;  
					else duty=PWMCH1.duty;
		
		
		speed=duty*40000/100;
		SetTIM3Compare(speed,1);
			SetTIM3Compare(speed,2);
			SetTIM3Compare(speed,3);
			SetTIM3Compare(speed,4);
		printf("%f  %f  %f\n"  ,PWMCH1.hightlevel*12000,PWMCH1.duty,PWMCH1.frequency);
		delay_ms(200);
		
		if(KEY_Scan(2))
		{
			i=0;
			LED_set(0,0);
			BEED=1;
			delay_ms(50);
			BEED=0;
			LED_set(0,1);
		}
	}
}

void MPU_adjust()
{
	uint8_t i=0;
//	uint16_t num=0;
//	float x=0,y=0;
	struct MPU6050 mpuOffset;
	struct MPU6050 mpuOffset_;
	struct Attitude AT;
	
//	ReadMpu(&mpuOffset);
//	IMUupdate(mpuOffset.Gx, mpuOffset.Gy, mpuOffset.Gz, mpuOffset.Ax, mpuOffset.Ay, mpuOffset.Az, &AT);
//	printf("%.2f %.2f\n",AT.X,AT.Y);
	
		MPU_Offset.Ax=0;
		MPU_Offset.Ay=0;
		MPU_Offset.Az=0;
		
		MPU_Offset.Gx=0;
		MPU_Offset.Gy=0;
		MPU_Offset.Gz=0;
	
		mpuOffset.Ax=0;
		mpuOffset.Ay=0;
		mpuOffset.Az=0;
		
		mpuOffset.Gx=0;
		mpuOffset.Gy=0;
		mpuOffset.Gz=0;
	
	for(i=200;i>0;i--)
	{
		R_MPU_Original(&mpuOffset_);
		mpuOffset.Ax+=mpuOffset_.Ax;
		mpuOffset.Ay+=mpuOffset_.Ay;
		mpuOffset.Az+=mpuOffset_.Az;
		
		mpuOffset.Gx+=mpuOffset_.Gx;
		mpuOffset.Gy+=mpuOffset_.Gy;
		mpuOffset.Gz+=mpuOffset_.Gz;
		
		printf("%f  %f  %f\n",mpuOffset_.Ax,mpuOffset_.Ay,mpuOffset_.Az);
		printf("%f  %f  %f  ",mpuOffset_.Gx,mpuOffset_.Gy,mpuOffset_.Gz);
		printf("%d\n\n",i);
		
		delay_ms(10);
	}	
		MPU_Offset.Ax=mpuOffset.Ax/200;
		MPU_Offset.Ay=mpuOffset.Ay/200;
		MPU_Offset.Az=mpuOffset.Az/200;
		
		MPU_Offset.Gx=mpuOffset.Gx/200;
		MPU_Offset.Gy=mpuOffset.Gy/200;
		MPU_Offset.Gz=mpuOffset.Gz/200;
	printf("%f  %f  %f \n",MPU_Offset.Ax,MPU_Offset.Ay,MPU_Offset.Az);
	printf("%f  %f  %f     ",MPU_Offset.Gx,MPU_Offset.Gy,MPU_Offset.Gz);
	
	IMUupdate(MPU_Offset.Gx, MPU_Offset.Gy, MPU_Offset.Gz, MPU_Offset.Ax, MPU_Offset.Ay, MPU_Offset.Az, &AT);
	printf("%.1f %.1f\n\n",AT.X,AT.Y);
	
//	x=AT.X;
//	y=AT.Y;
//	
//	delay_ms(10);
//	
//	AT.X=0;
//	AT.Y=0;
//	
//	R_MPU_Original(&mpuOffset);
//	printf("%f  %f  %f \n",mpuOffset.Ax,mpuOffset.Ay,mpuOffset.Az);
//	printf("%f  %f  %f    ",mpuOffset.Gx,mpuOffset.Gy,mpuOffset.Gz);
//	IMUupdate(mpuOffset.Gx, mpuOffset.Gy, mpuOffset.Gz, mpuOffset.Ax, mpuOffset.Ay, mpuOffset.Az, &AT);
//	printf("\n%.1f %.1f\n",AT.X,AT.Y);
//	
//	printf("%.1f %.1f\n",AT.X-x,AT.Y-y);
//	
//	delay_ms(10);
//	
//	R_MPU_Original(&mpuOffset);
//	disposedata(&mpuOffset);
//	IMUupdate(mpuOffset.Gx, mpuOffset.Gy, mpuOffset.Gz, mpuOffset.Ax, mpuOffset.Ay, mpuOffset.Az, &AT);
//	printf("%f  %f  %f \n",mpuOffset.Ax,mpuOffset.Ay,mpuOffset.Az);
//	printf("%f  %f  %f     ",mpuOffset.Gx,mpuOffset.Gy,mpuOffset.Gz);
//	printf("%.1f %.1f\n",AT.X,AT.Y);
//	
////	num=(uint16_t)AT.X*100;
////	LCD_DisplayNum(50,280,num,4,12,1);
////	num=(uint16_t)AT.Y*100;
////	LCD_DisplayNum(50,300,num,4,12,1);
}

	

	
