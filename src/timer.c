#include "timer.h"
#include "clock.h"      /* F_CPU */
#include "armcom.h"
#include "board_pins.h"

int base_timer_init(struct timer_device *p_timer) {

    HAL_TIM_Base_Init(p_timer->data->handle);

    // 使定时器中断的优先级保持最高
    HAL_NVIC_SetPriority(p_timer->data->IRQn,               \
                            p_timer->data->PreemptPriority, \
                            p_timer->data->SubPriorityv);      

    HAL_NVIC_DisableIRQ(p_timer->data->IRQn);

    HAL_TIM_Base_Start_IT(p_timer->data->handle);

    return 0;
}

int base_timer_irq_set(struct timer_device *p_timer, bool status) {

    if(status) {
        HAL_NVIC_EnableIRQ(p_timer->data->IRQn);
    }else{
        HAL_NVIC_DisableIRQ(p_timer->data->IRQn);
    }

    return 0;
}

int base_timer_generateEvent_update(struct timer_device *p_timer) {
    HAL_TIM_GenerateEvent(p_timer->data->handle, TIM_EVENTSOURCE_UPDATE);
    return 0;
}

int base_timer_clear_update_flag(struct timer_device *p_timer) {
    HAL_TIM_GenerateEvent(p_timer->data->handle, TIM_EVENTSOURCE_UPDATE);
    return 0;
}

int base_timer_set_reload(struct timer_device *p_timer, uint32_t reload) {
    __HAL_TIM_SET_AUTORELOAD(p_timer->data->handle, reload);
	return 0;
}

int base_timer_set_cnt(struct timer_device *p_timer, uint32_t cnt) {
    __HAL_TIM_SET_COUNTER(p_timer->data->handle, cnt);
    return 0;
}

TIM_HandleTypeDef htim_set = {
    .Instance = TIM3,
    .Init.ClockDivision = TIM_CLOCKDIVISION_DIV1,
    .Init.CounterMode = TIM_COUNTERMODE_UP,
    .Init.Period = 0,
    .Init.Prescaler = 3,
};

TIM_HandleTypeDef htim_reset = {
    .Instance = TIM4,
    .Init.ClockDivision = TIM_CLOCKDIVISION_DIV1,
    .Init.CounterMode = TIM_COUNTERMODE_UP,
    .Init.Period = 0,
    .Init.Prescaler = (180000000 / 1000000) - 1,
};

/* 用于stepper 拉高脉冲 */
struct timer_data set_timer_data = {
    &htim_set,
    TIM3_IRQn,
    0,
    0,
};

struct timer_data reset_timer_data = {
    &htim_reset,
    TIM4_IRQn,
    0,
    1,
};


struct timer_device g_set_timer = {
    "stm32_step_set",
    &set_timer_data, 
    base_timer_init,
    base_timer_irq_set,
    base_timer_generateEvent_update,
    base_timer_clear_update_flag,
    base_timer_set_reload,
    base_timer_set_cnt,

};

struct timer_device g_reset_timer = {
    "stm32_step_reset",
    &reset_timer_data, 
    base_timer_init,
    base_timer_irq_set,
    base_timer_generateEvent_update,
    base_timer_clear_update_flag,
    base_timer_set_reload,
    base_timer_set_cnt,
};

struct timer_device *g_base_timer_dev_num[] = {
    &g_set_timer,
    &g_reset_timer
};


struct timer_device *dev_base_timer_get(char *name) {

    for(int i=0; i< sizeof(g_base_timer_dev_num) / sizeof(g_base_timer_dev_num[0]); i++) {

        if( 0 == strcmp(name, g_base_timer_dev_num[i]->name)) {
            return g_base_timer_dev_num[i];
        }
    }

    return NULL;
}

void STEP_RESET_HANDLER(void) {

    HAL_TIM_IRQHandler(&htim_reset);

#if defined __CORTEX_M && (__CORTEX_M == 4U || __CORTEX_M == 7U)
    __DSB();
#endif
}

void STEP_SET_HANDLER(void) {

    HAL_TIM_IRQHandler(&htim_set);  

#if defined __CORTEX_M && (__CORTEX_M == 4U || __CORTEX_M == 7U)
    __DSB();
#endif
}



