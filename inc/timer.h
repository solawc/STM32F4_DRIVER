#ifndef __timer_h
#define __timer_h

#include "stdint.h"
#include "stdbool.h"
#include "stm32f4xx_hal.h" 

struct timer_data {
    TIM_HandleTypeDef *handle;
    uint32_t Period;
    uint32_t Prescaler;
    IRQn_Type irq_n;
};

struct timer_dev {
    char *name;  
    struct timer_data *data;
    int (*base_timer_init)(struct timer_dev *t);
    int (*base_timer_irq_set)(struct timer_dev *t, bool status);
    int (*base_timer_irq_set_cnt)(struct timer_dev *t, uint32_t cnt);
    int (*base_timer_irq_set_reload)(struct timer_dev *t, uint32_t reload);
    int (*base_timer_genera_event_update)(struct timer_dev *t);
    int (*base_timer_clear_flag_update)(struct timer_dev *t);
};

struct timer_dev *dev_base_timer_get(char *name);

#endif
