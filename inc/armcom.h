#ifndef __ARMCOM_H
#define __ARMCOM_H

#include "stdint.h" // uint32_t
#include "stm32f4xx_hal.h"

void armcom_timer_init(void);
void udelay(uint32_t usecs);
uint32_t timer_read_time(void);
uint8_t timer_is_before(uint32_t time1, uint32_t time2);
uint32_t timer_from_us(uint32_t us);

void armcom_nvic_set_priority(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority);
void armcom_nvic_enable(IRQn_Type IRQn);
void armcom_nvic_disable(IRQn_Type IRQn);

#endif

