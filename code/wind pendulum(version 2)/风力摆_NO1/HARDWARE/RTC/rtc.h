#ifndef __RTC_H
#define __RTC_H	 
#include "sys.h" 

//////////////////////////////////////////////////////////////////////////////////	 

extern RTC_TimeTypeDef RTC_TimeStruct;
extern RTC_DateTypeDef RTC_DateStruct;
	
u8 RTC_InitConfig(void);						//RTC初始化

ErrorStatus RTC_SetTimes(u8 hour,u8 min,u8 sec);//设置时间
ErrorStatus RTC_Set_Date(u8 year,u8 month,u8 date);//设置日期
void RTC_GetTimes(void);//RTC_GetTimes(uint32_t RTC_Format);//获取时间日期

void RTC_SetAlarmA(u8 week,u8 hour,u8 min,u8 sec);	//设置闹钟时间(按星期闹铃,24小时制)
void RTC_SetWakeUp(u32 wktime,u16 autodata);				//设置唤醒定时器的时间

void RTC_CloseAlarmA(void);
void RTC_OpenAlarmA(void);

u8 RTC_GetWeek(u8 wyear,u8 wmonth,u8 wday);//内部调用

void ShowTime(void);
void ShowDay(void);
void ShowDayTime(void);

void RTC_AlarmSta(uint8_t Num,uint8_t Sta);
void RTC_WakeUpSta(uint8_t Sta);

#endif

















