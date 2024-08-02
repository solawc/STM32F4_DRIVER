#ifndef __gpio_a_h
#define __gpio_a_h

#include "stdint.h" // uint32_t
#include "stdbool.h"
#include "stm32f4xx_hal.h"

#define GPIO(PORT, NUM) (((PORT)-'A') * 16 + (NUM))       
#define GPIO2PORT(PIN) ((PIN) / 16)                         // 通过PIN来计算当前属于GPIOX X端口的端口号
#define GPIO2BIT(PIN) (1 << ((PIN) % 16))                   // 通过PIN推算当前GPIOX_PIN_Y Y的编号

// 编译相关
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

void gpio_peripheral(uint32_t gpio, uint32_t mode, int pullup, uint32_t Alternate);

// gpio out define
struct gpio_out {   
    void *regs;
    uint32_t bit;
};
struct gpio_out gpio_out_setup(uint32_t pin, uint32_t val);
void gpio_out_reset(struct gpio_out g, uint32_t val);


// stepper
int stepper_gpio_init(void);
int stepper_en(bool status);
int stepper_dir(uint8_t mask);
int stepper_step(uint8_t mask);
uint8_t stepper_get_axis_mask(uint8_t axis);

// limit 
int limit_gpio_init(void);
uint8_t limit_gpio_get_state(uint8_t bit_select);
int limit_gpio_irq_set(bool state);

#endif

