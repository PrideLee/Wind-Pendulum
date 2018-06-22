#ifndef __LED_H
#define __LED_H

#include "stm32f4xx.h"
#include "sys.h"

#define LEDON  0
#define LEDOFF 1

#define LED0 PEout(3)
#define LED1 PEout(4)
#define LED2 PGout(9)

void LED_Init(void);
void LED_set(uint8_t Num,uint8_t Set);
#endif
