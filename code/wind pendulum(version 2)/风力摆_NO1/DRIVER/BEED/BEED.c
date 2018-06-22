/*
	配置AND调用
*/

#include "BEED.h"
#include "GPIO.h"
#include "delay.h"

uint8_t ring_on=1;
uint8_t ring_off=0;

/* 
	功能：BEED配置
	输入：Sta : 1：置1响
							0：置0响
	输出：无
*/
void BEED_Init(uint8_t Sta)
{
	uint8_t sta;
	sta=Sta;
	
	GPIO_GeneralOutInit(PG,GPIO_Pin_7);
	
	if(sta) 
	{
		ring_on=1;
		ring_off=0;
		BEED=0;
	}
	else 
	{
		ring_on=0;
		ring_off=1;
		BEED=1;
	}
}

/* 
	功能：BEED响1声
  输入：Time_RingOn：响的时间
	输出：无
*/
void BEED_1(uint16_t Time_RingOn)
{
	uint16_t time_ring_on;
	time_ring_on=Time_RingOn;
	
	BEED=ring_on;   delay_ms(time_ring_on);   BEED=ring_off;
	
}

/* 
	功能：BEED配置
	输入：Time_RingOn     ：响的时间   
				Time_RingOff    ：两声响间隔时间
	输出：无
*/
void BEED_2(uint16_t Time_RingOn , uint16_t Time_RingOff)
{
	uint16_t time_ring_on,time_ring_off;
	time_ring_on=Time_RingOn;
	time_ring_off=Time_RingOff;
	
	BEED=ring_on;   delay_ms(time_ring_on);   BEED=ring_off;   delay_ms(time_ring_off);
	BEED=ring_on;   delay_ms(time_ring_on);   BEED=ring_off;
	
}
/* 
	功能：BEED配置
	输入：Time_RingOn     ：响的时间   
				Time_RingOff    ：两声响间隔时间
	输出：无
*/
void BEED_3(uint16_t Time_RingOn , uint16_t Time_RingOff)
{
	uint16_t time_ring_on,time_ring_off;
	time_ring_on=Time_RingOn;
	time_ring_off=Time_RingOff;
	
	BEED=ring_on;   delay_ms(time_ring_on);   BEED=ring_off;   delay_ms(time_ring_off);
	BEED=ring_on;   delay_ms(time_ring_on);   BEED=ring_off;   delay_ms(time_ring_off);
	BEED=ring_on;   delay_ms(time_ring_on);   BEED=ring_off;
	
}
