#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f4xx.h"

/*
********************************
			�ⲿ���ú����ͱ���
********************************
*/
void SysTick_Init(void);
void delay_ms(u32 ms);
extern u32 SysTick_1ms, SysTick_100us;

#endif
