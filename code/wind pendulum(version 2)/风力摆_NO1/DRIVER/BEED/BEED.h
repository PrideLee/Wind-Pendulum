#ifndef __BEED_H
#define __BEED_H

#include "stm32f4xx.h"
#include "sys.h"

#define BEED PGout(7)

void BEED_Init(uint8_t Sta);
void BEED_1(uint16_t Time_RingOn);
void BEED_2(uint16_t Time_RingOn , uint16_t Time_RingOff);
void BEED_3(uint16_t Time_RingOn , uint16_t Time_RingOff);

#endif
