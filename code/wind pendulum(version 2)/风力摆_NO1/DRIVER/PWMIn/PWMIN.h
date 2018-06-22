#ifndef __PWMIN_H
#define __PWMIN_H
#include "stm32f4xx.h"

struct R_PWM
{
	float count_first;
	float count_hightlevel;
	float count_period;
	float count_CNT;
	float count_CNT_hightlevel;
	float count_CNT_period;
	
	uint8_t update_PWM;
	float first_use;
	float hightlevel_use;
	float period_use;
	float cnt_hightlevel_use;
	float cnt_period_use;
	
	uint8_t rangeout;
	uint8_t level_rangeout;
};

struct PWM
{
	float duty;
	float period;
	float hightlevel;
	float frequency;
};

extern uint8_t NEED_FULLPWM;
extern struct PWM PWMCH1,PWMCH2,PWMCH3,PWMCH4;
extern struct PWM PWMCH5,PWMCH6,PWMCH7,PWMCH8;

void PWMIN_TIM4(void);
void PWMIN_TIM5(void);

void ReadPWM(void);
void ShowPWM(uint8_t num);
void ShowHightLevel(void);
void Showduty(void);
void ShowPeriod(void);

void Show(void);
void ChangeMode(uint8_t Open);

void ContinueShow(uint8_t Y);

#endif
