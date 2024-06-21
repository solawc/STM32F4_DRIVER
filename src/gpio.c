#include "gpio_a.h"
#include "stm32f4xx_hal.h" 



int gpio_out_init(struct gpio_out *g) {

    GPIO_InitTypeDef GPIO_Init;

    GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init.Pin = g->data->pin;

    HAL_GPIO_Init(g->data->gpio, &GPIO_Init);
}








