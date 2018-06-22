/*
	相应函数功能见头文件
*/

#include "Function.h"
#include "semphr.h"
#include "timers.h"
#include "queue.h"
#include "croutine.h"
#include "event_groups.h"
#include "list.h"
#include "PWMOut8.h"
#include "lcd.h"

//*************************************************************************
	struct Exp_angle exp_Angle;
	struct Current_Angle current_Angle;
//*************************************************************************



/*
	描述：检测电机是否被按下
	输入：NUM  检测那个按键
	输出：按下返回1  否则返回0
	说明：
*/

//计数型信号量句柄   在APP.c中定义
extern SemaphoreHandle_t KEY1_CountSemaphore;//计数型信号量
extern SemaphoreHandle_t KEY2_CountSemaphore;
extern SemaphoreHandle_t KEY3_CountSemaphore;
extern SemaphoreHandle_t KEY4_CountSemaphore;

extern SemaphoreHandle_t BEEP_CountSemaphore;
extern SemaphoreHandle_t LED_CountSemaphore;

uint8_t Get_KEY(uint8_t NUM)
{
	uint8_t i=0;
	
	switch(NUM)
	{
		case 1:	if(xSemaphoreTake(KEY1_CountSemaphore,2)) i=1 ;
						break;
		
	  case 2: if(xSemaphoreTake(KEY2_CountSemaphore,2)) i=1 ;
						break;
			
		case 3:	if(xSemaphoreTake(KEY3_CountSemaphore,2)) i=1 ;
						break;
		
		case 4:	if(xSemaphoreTake(KEY4_CountSemaphore,2)) i=1 ;
						break;
	}
	return i;
}

void beep(void)
{
 xSemaphoreGive(BEEP_CountSemaphore);
}

void led1(void)
{
 xSemaphoreGive(LED_CountSemaphore);
}

/*
	描述：调节电机速度
	输入：speed：速度值       num：电机标号
	输出：按下返回1  否则返回0
	说明：speed 最大 20000
*/
void Set_MotoSpeed(uint16_t speed, uint8_t num)
{
	if(speed>3500)speed=3500;
	if(speed<2000)speed=2000;
  SetTIM3Compare (speed, num);
}

/*
	描述：读取MPU6050数据
	输入：struct MPU6050 
	输出：成功返回1  否则返回0
	说明：
*/
uint8_t Get_MPU6050(struct MPU6050 *mpu)
{
	uint8_t flag=0;
	
	taskENTER_CRITICAL();           //进入临界区
	flag=ReadMpu(mpu) ;
	taskEXIT_CRITICAL();            //退出临界区
	
	if(flag) return 1;//成功返回1
	else return 0;
}

void OUTput(uint8_t a)
{
	uint8_t q=0x03,p=0xfc,H,L;
	int16_t now_X=0,now_Y=0;
	int16_t expX=0,expY=0;
	
	now_X=((current_Angle.X*10));
	now_Y=((current_Angle.Y*10));
	
	expX=((int16_t)(exp_Angle.X*10));
	expY=((int16_t)(exp_Angle.Y*10));
	
		if(a)
	{
   printf_1_(&q,1);printf_1_(&p,1);//帧头
	
		H=now_X>>8;
		L=now_X;
		printf_1_(&L,1);//
		printf_1_(&H,1);//
		
		H=now_Y>>8;
		L=now_Y;
		printf_1_(&L,1);//
		printf_1_(&H,1);//
		
		
		H=expX>>8;
		L=expX;
		printf_1_(&L,1);//
		printf_1_(&H,1);//
		
		H=expY>>8;
		L=expY;
		printf_1_(&L,1);//
		printf_1_(&H,1);//
	
   printf_1_(&p,1);printf_1_(&q,1);//帧尾
  }
  else
	{
	printf("%.2f %.2f\n",current_Angle.X,current_Angle.Y);
//	printf("%f %f \n",exp_Angle.X,exp_Angle.Y);
	}
}

extern SemaphoreHandle_t USART_OUT_BinarySemaphore;//在APP.C
void OUTPUT ()
{
	xSemaphoreGive(USART_OUT_BinarySemaphore);	//释放二值信号量
}

