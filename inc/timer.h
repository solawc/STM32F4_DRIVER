#ifndef __timer_h
#define __timer_h

#include "stdint.h"
#include "stdbool.h"
#include "stm32f4xx_hal.h" 


struct timer_data { 

    TIM_HandleTypeDef *handle;
    IRQn_Type IRQn;
    uint32_t PreemptPriority;
    uint32_t SubPriorityv;
};

struct timer_device {
    char *name;
    struct timer_data *data;
    int (*base_timer_init)(struct timer_device *p_timer);
    int (*base_timer_irq_set)(struct timer_device *p_timer, bool status);
    int (*base_timer_generateEvent_update)(struct timer_device *p_timer);
    int (*base_timer_clear_update_flag)(struct timer_device *p_timer);
    int (*base_timer_set_reload)(struct timer_device *p_timer, uint32_t reload);
    int (*base_timer_set_cnt)(struct timer_device *p_timer, uint32_t cnt);
};

struct timer_device *dev_base_timer_get(char *name);

#endif
