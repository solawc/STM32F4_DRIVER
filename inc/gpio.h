#ifndef __gpio_h
#define __gpio_h

#include "stdint.h"
#include "stm32f4xx_hal.h"

#define GPIO(PORT, NUM) (((PORT)-'A') * 16 + (NUM))       
#define GPIO2PORT(PIN) ((PIN) / 16)                     // 通过PIN来计算当前属于GPIOX X端口的端口号
#define GPIO2BIT(PIN) (1 << ((PIN) % 16))                 // 通过PIN推算当前GPIOX_PIN_Y Y的编号

// 编译相关
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

struct gpio_out {
    void *gpio_port;
    uint16_t bit;
};

struct gpio_in {
    void *gpio_port;
    uint16_t bit;
};

int regs_to_pin(GPIO_TypeDef  *regs, uint32_t bit);
struct gpio_out gpio_out_setup(uint32_t pin, uint32_t val);


void goio_test_init(void);
void gpio_test_run(void);

#endif

