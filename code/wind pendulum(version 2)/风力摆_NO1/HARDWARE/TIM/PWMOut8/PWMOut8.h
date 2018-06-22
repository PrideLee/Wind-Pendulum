#ifndef __PWMOUT_H
#define __PWMOUT_H

#include "stm32f4xx.h"

void PWMInit(void);

/* 8·PWM */
void TIM1_PWMOutInit(uint32_t Hz);
void TIM3_PWMOutInit(uint32_t Hz);

void SetPWMDuty_TIM1(uint8_t Duty, uint8_t Channel);
void SetPWMDuty_TIM3(uint8_t Duty, uint8_t Channel);

void SetTIM1Compare (uint16_t Data, uint8_t Channel);
void SetTIM3Compare(uint16_t Data, uint8_t Channel);

void TIM1Change(uint16_t Period, uint16_t Prescaler, uint8_t ChangePSC);
void TIM3Change(uint16_t Period, uint16_t Prescaler, uint8_t ChangePSC);

/*  */


#endif
