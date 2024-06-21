#ifndef __clock_h
#define __clock_h

#include "stm32f4xx_hal.h"

#ifdef STM32F429xx
#define F_CPU       180000000  /* STM32F429IG */
#else 
#define F_CPU       180000000  /* Default use STM32F429IG */
#endif

void system_clock_init(void);
void SystemClock_Config(void);
void Error_Handler(void);

#endif
