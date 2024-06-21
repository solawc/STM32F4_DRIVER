#include "timer.h"

TIM_HandleTypeDef htim3 = {
    .Instance = TIM3,
    .Init.Period = 0,
    .Init.Prescaler = 0,
    .Init.ClockDivision = TIM_CLOCKDIVISION_DIV1,
    .Init.CounterMode = TIM_COUNTERMODE_UP,
};

TIM_HandleTypeDef htim4 = {
    .Instance = TIM4,
    .Init.Period = 0,
    .Init.Prescaler = 0,
    .Init.ClockDivision = TIM_CLOCKDIVISION_DIV1,
    .Init.CounterMode = TIM_COUNTERMODE_UP,
};

struct timer_data g_stm32_tim3 = {
    &htim3,
    0,
    3,
    TIM3_IRQn,
};

struct timer_data g_stm32_tim4 = {
    &htim4,
    0,
    180000000 / 2,
    TIM4_IRQn,
};

static int base_timer_init(struct timer_dev *t) {

    HAL_TIM_Base_Init(t->data->handle);
    
    // 启动定时器中断，启动前先禁用
    HAL_NVIC_SetPriority(t->data->irq_n, 0, 0);
    HAL_NVIC_DisableIRQ(t->data->irq_n);
    HAL_TIM_Base_Start_IT(t->data->handle); 
}

static int base_timer_irq_set(struct timer_dev *t, bool status) {

    if(status)
        NVIC_EnableIRQ(t->data->irq_n);
    else
        NVIC_DisableIRQ(t->data->irq_n);

    return 0;
}

static int base_timer_irq_set_cnt(struct timer_dev *t, uint32_t cnt) {

    __HAL_TIM_SET_COUNTER(t->data->handle, cnt);

    return 0;
}

static int base_timer_irq_set_reload(struct timer_dev *t, uint32_t reload) {

    __HAL_TIM_SET_AUTORELOAD(t->data->handle, reload);

    return 0;
}

static int base_timer_genera_event_update(struct timer_dev *t) {

    HAL_TIM_GenerateEvent(t->data->handle, TIM_EVENTSOURCE_UPDATE);
}

static int base_timer_clear_flag_update(struct timer_dev *t) {

    __HAL_TIM_CLEAR_FLAG(t->data->handle, TIM_FLAG_UPDATE);
}


struct timer_dev st_tim_set = {
    "stepper_timer_set",
    &g_stm32_tim3,
    base_timer_init,
    base_timer_irq_set,
    base_timer_irq_set_cnt,
    base_timer_irq_set_reload,
    base_timer_genera_event_update,
    base_timer_clear_flag_update,
};

struct timer_dev st_tim_reset = {
    "stepper_timer_reset",
    &g_stm32_tim4,
    base_timer_init,
    base_timer_irq_set,
    base_timer_irq_set_cnt,
    base_timer_irq_set_reload,
    base_timer_genera_event_update,
    base_timer_clear_flag_update,
};


struct timer_dev *g_base_timer_dev_num[] = {
    &st_tim_set,
    &st_tim_reset,
};

struct timer_dev *dev_base_timer_get(char *name) {

    for(int i=0; i< sizeof(g_base_timer_dev_num) / sizeof(g_base_timer_dev_num[0]); i++) {

        if( 0 == strcmp(name, g_base_timer_dev_num[i]->name)) {
            return g_base_timer_dev_num[i];
        }
    }

    return NULL;
}



