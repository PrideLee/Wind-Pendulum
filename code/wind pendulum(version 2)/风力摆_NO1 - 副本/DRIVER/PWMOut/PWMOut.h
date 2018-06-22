#ifndef __PWMOUT_H
#define __PWMOUT_H
#include "stm32f4xx.h"

void PWMOut_TIM13(uint16_t Period, uint16_t Prescaler, uint16_t Duty);
void PWMOut_TIM14(uint16_t Period, uint16_t Prescaler, uint16_t Duty);

void SetPWMDuty_TIM13(uint8_t Duty);
void SetPWMDuty_TIM14(uint8_t Duty);

void SetTIM13Compare (uint16_t Data);
void SetTIM14Compare(uint16_t Data);

void TIM13Change(uint16_t Period, uint16_t Prescaler, uint8_t ChangePSC);
void TIM14Change(uint16_t Period, uint16_t Prescaler, uint8_t ChangePSC);

#endif
