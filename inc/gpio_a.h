#ifndef __gpio_a_h
#define __gpio_a_h

#include "stdint.h" // uint32_t
#include "stdbool.h"
#include "stm32f4xx_hal.h"

#define GPIO(PORT, NUM) (((PORT)-'A') * 16 + (NUM))       
#define GPIO2PORT(PIN) ((PIN) / 16)                     // 通过PIN来计算当前属于GPIOX X端口的端口号
#define GPIO2BIT(PIN) (1 << ((PIN) % 16))                 // 通过PIN推算当前GPIOX_PIN_Y Y的编号

// 编译相关
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))


struct gpio_genera {
    GPIO_TypeDef *gpio;
    uint16_t pin;
};

struct gpio_out {
    struct gpio_genera *data;
    int (*gpio_out_init)(struct gpio_out *g);
    int (*gpio_out_write)(struct gpio_out *g, bool status);
    int (*gpio_out_toggle)(struct gpio_out *g);
    int (*gpio_in_irq_set)(void);
};

typedef enum {
    GPIO_IN_PU=0,           /* GPIO 上拉输入 */
    GPIO_IN_PD,             /* GPIO 下拉输入 */
    GPIO_IN_FLOAT,          /* GPIO 浮空输入 */
}GPIO_PP_t;

struct gpio_in {
    struct gpio_genera *data;
    int (*gpio_in_init)(struct gpio_in *g, GPIO_PP_t p);
    int (*gpio_in_read)(struct gpio_in *g);
};



#endif

