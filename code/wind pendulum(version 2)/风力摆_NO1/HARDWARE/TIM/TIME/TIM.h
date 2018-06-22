#ifndef __TIM_H
#define __TIM_H

#include "stm32f4xx.h"

void TIM6_Init(void);
//void TIM7_Init(void);

void TIM6Open(void);
void TIM6Close(void);
void SetTime(uint16_t Period);
void SetPSC(uint16_t PSC_);//重新配置TIM6分频系数

#endif
