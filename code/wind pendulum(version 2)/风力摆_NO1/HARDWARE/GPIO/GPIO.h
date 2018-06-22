#ifndef __GPIO_H
#define __GPIO_H

#include "stm32f4xx.h"

#define PA 0
#define PB 1
#define PC 2
#define PD 3
#define PE 4
#define PF 5
#define PG 6
void GPIO_GeneralOutInit(uint8_t Px , uint32_t GPIO_Pin);//IO口普通推挽输出设置
void GPIO_GeneralInInit(uint8_t Px , uint32_t GPIO_Pin, uint8_t Sta);//IO口普通上下拉输入设置
void EXTI_GeneralInInit(uint8_t Px , uint32_t GPIO_Pin, uint8_t Sta , uint8_t Preemption , uint8_t Sub);//IO口普通中断输入设置

#endif
