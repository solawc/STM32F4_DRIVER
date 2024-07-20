#include "pwm.h"
#include "clock.h"
#include "board_pins.h"

struct pwm_data { 

    TIM_HandleTypeDef *handler;
    GPIO_InitTypeDef *gpio_handler;
    GPIO_TypeDef  *GPIOx;
    HAL_TIM_ActiveChannel ch;
};

struct pwm_device {
    char *name;
    struct pwm_data *data;
    int (*pwm_init)(struct pwm_device *p_pwm, uint32_t period);
    int (*pwm_set)(struct pwm_device *p_pwm, uint32_t duty);
    uint32_t (*pwm_ccr_get)(struct pwm_device *p_pwm);
};

int pwm_init(struct pwm_device *p_pwm, uint32_t period) {

    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};

    HAL_GPIO_Init(p_pwm->data->GPIOx, p_pwm->data->gpio_handler);

    p_pwm->data->handler->Instance = LASER_TIM_PORT;
    p_pwm->data->handler->Init.Prescaler = (F_CPU / 1000000) - 1;         
    p_pwm->data->handler->Init.CounterMode = TIM_COUNTERMODE_UP;
    p_pwm->data->handler->Init.Period = period -1;              
    p_pwm->data->handler->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    p_pwm->data->handler->Init.RepetitionCounter = 0;
    p_pwm->data->handler->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_OC_Init(p_pwm->data->handler) != HAL_OK) {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(p_pwm->data->handler, &sMasterConfig) != HAL_OK) {
        Error_Handler();
    }

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity = TIM_OCNPOLARITY_LOW;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    if (HAL_TIM_OC_ConfigChannel(p_pwm->data->handler, &sConfigOC, p_pwm->data->ch) != HAL_OK) {
        Error_Handler();
    }
    
    HAL_TIM_PWM_Start(p_pwm->data->handler, p_pwm->data->ch);
}

int pwm_set(struct pwm_device *p_pwm, uint32_t duty) {
    __HAL_TIM_SetCompare(p_pwm->data->handler, p_pwm->data->ch, duty);
}

uint32_t pwm_ccr_get(struct pwm_device *p_pwm) {
    return __HAL_TIM_GetCompare(p_pwm->data->handler, p_pwm->data->ch);
}