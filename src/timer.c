#include "timer.h"

struct timer_dev base_timer_init(TIM_TypeDef *tim, uint32_t period, uint32_t Prescaler) {

    struct timer_dev t;

    t.timer.Instance = tim; 
    t.timer.Init.Period = period;
    t.timer.Init.Prescaler = Prescaler;
    t.timer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    t.timer.Init.CounterMode = TIM_COUNTERMODE_UP;
    HAL_TIM_Base_Init(&t.timer);

    HAL_TIM_Base_Start_IT(&t.timer);

    return t;
}

