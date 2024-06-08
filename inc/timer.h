#ifndef __timer_h
#define __timer_h

#include "stdint.h"
#include "stm32f4xx_hal.h" 

struct timer_dev {
    TIM_HandleTypeDef timer;
    void (*irq_handler)(void);
};



struct timer_dev base_timer_init(TIM_TypeDef *tim, uint32_t period, uint32_t Prescaler);


#endif
