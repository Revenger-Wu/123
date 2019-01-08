#ifndef __TIMER_H
#define __TIMER_H
#include "main.h"

extern u8 SecureTime_Flag;

void TIM2_Secure_Init(u16 arr,u16 psc);
void TIM3_SendLocal_Init(u16 arr,u16 psc);
void TIM4_Echo_Init(u16 arr,u16 psc);

 
#endif
