#include "usmart.h"
#include "usmart_str.h"
////////////////////////////用户配置区///////////////////////////////////////////////
//这下面要包含所用到的函数所申明的头文件(用户自己添加) 
#include "delay.h"		
#include "sys.h"
#include "LED.h"
#include "rtc.h"
//#include "TIM.h"
//#include "ServoPWMOut.h"
//#include "PWMIN.h"
//#include "PWMOut.h"
												 
//函数名列表初始化(用户自己添加)
//用户直接在这里输入要执行的函数名及其查找串
struct _m_usmart_nametab usmart_nametab[]=
{
#if USMART_USE_WRFUNS==1 	//如果使能了读写操作
	(void*)read_addr,"u32 read_addr(u32 addr)",
	(void*)write_addr,"void write_addr(u32 addr,u32 val)",	 
#endif		   
	(void*)delay_ms,"void delay_ms(u16 nms)",
 	(void*)delay_us,"void delay_us(u32 nus)",	
  (void*)LED_set,"void LED_set(uint8_t Num,uint8_t Set)",	
	(void*)RTC_SetTimes,"ErrorStatus RTC_SetTimes(u8 hour,u8 min,u8 sec)",//设置时间
	(void*)RTC_Set_Date,"ErrorStatus RTC_Set_Date(u8 year,u8 month,u8 date)",//设置日期
	(void*)RTC_GetTimes,"void RTC_GetTimes(void)",//RTC_GetTimes(uint32_t RTC_Format),//获取时间日期
	(void*)ShowTime,"void ShowTime(void)",
	(void*)ShowDay,"void ShowDay(void)",
	(void*)ShowDayTime,"void ShowDayTime(void)",
	(void*)RTC_SetAlarmA,"void RTC_SetAlarmA(u8 week,u8 hour,u8 min,u8 sec)",	//设置闹钟时间(按星期闹铃,24小时制)
	(void*)RTC_SetWakeUp,"void RTC_SetWakeUp(u32 wktime,u16 autodata)",				//设置唤醒定时器的时间
	(void*)RTC_AlarmSta,"void RTC_AlarmSta(uint8_t Num,uint8_t Sta)",
	(void*)RTC_WakeUpSta,"void RTC_WakeUpSta(uint8_t Sta)",
//	(void*)TIM6Open,"void TIM6Open(void);",
//  (void*)TIM6Close,"void TIM6Close(void);",
//  (void*)SetTime,"void SetTime(uint16_t Period);",
//  (void*)SetPSC,"void SetPSC(uint16_t PSC_);",
//	  (void*)SetPWMDuty_TIM9,"void SetPWMDuty_TIM9(uint8_t Duty, uint8_t Channel)",
//	  (void*)TIM9Change,"void TIM9Change(uint16_t Period, uint16_t Prescaler, uint8_t ChangePSC)",
//		(void*)SetTIM9Compare,"void SetTIM9Compare (uint16_t Data, uint8_t Channel)",
//		(void*)Show,"void Show()",
//		(void*)ChangeMode,"void ChangeMode(uint8_t Open)",
//		(void*)ContinueShow,"void ContinueShow(uint8_t Y)",
//		(void*)TIM13Change,"void TIM13Change(uint16_t Period, uint16_t Prescaler, uint8_t ChangePSC)",
//		(void*)SetPWMDuty_TIM13,"void SetPWMDuty_TIM13(uint8_t Duty)",
			

};						  
///////////////////////////////////END///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//函数控制管理器初始化
//得到各个受控函数的名字
//得到函数总数量
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),//函数数量
	0,	  	//参数数量
	0,	 	//函数ID
	1,		//参数显示类型,0,10进制;1,16进制
	0,		//参数类型.bitx:,0,数字;1,字符串	    
	0,	  	//每个参数的长度暂存表,需要MAX_PARM个0初始化
	0,		//函数的参数,需要PARM_LEN个0初始化
};   



















