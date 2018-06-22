#include "LED.h"
#include "GPIO.h"

/* 
	功能：LED配置
	输入：无
	输出：无
*/
void LED_Init(void)
{
	GPIO_GeneralOutInit(PE,GPIO_Pin_3);
	GPIO_GeneralOutInit(PE,GPIO_Pin_4);
	GPIO_GeneralOutInit(PG,GPIO_Pin_9);
}

/* 
	功能：LED设置
	输入：Num ：LED编号
				Set ：0 亮  1 暗
	输出：无
*/
void LED_set(uint8_t Num,uint8_t Set)
{
	uint8_t num=0,set=0;
	num=Num;
	set=Set;
	switch(num)
	{
		case 0:LED0=set;break;
		case 1:LED1=set;break;
		case 2:LED2=set;break;
	}
		
}
