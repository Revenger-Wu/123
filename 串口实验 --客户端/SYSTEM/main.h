#ifndef __MAIN_H
#define __MAIN_H

//采用go to语句让代码执行效率更高？

#include "stdint.h"
#include "stdio.h"	
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "led.h"
#include "delay.h"
#include "timer.h"
#include "stm32f10x_tim.h"
#include <stdarg.h>
#include <string.h>
#include "AGV_Client.h"

void Zigbee_SendData(u8 *Data,u8 len);
void Master_TO_P(u8 *AGVx,u8 *Data,u8 len);

#endif











