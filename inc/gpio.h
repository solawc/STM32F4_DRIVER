#ifndef __gpio_h
#define __gpio_h

#include "stdint.h" // uint32_t

#define GPIO(PORT, NUM) (((PORT)-'A') * 16 + (NUM))       
#define GPIO2PORT(PIN) ((PIN) / 16)                     // 通过PIN来计算当前属于GPIOX X端口的端口号
#define GPIO2BIT(PIN) (1 << ((PIN) % 16))                 // 通过PIN推算当前GPIOX_PIN_Y Y的编号

// 编译相关
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))


void gpio_all_clock_init(void);

struct gpio_out {
    void *gpio_port;
    uint16_t bit;
};
struct gpio_out gpio_out_setup(uint32_t pin, uint32_t val);
void gpio_out_write(struct gpio_out g, uint32_t val);
void gpio_out_toggle_noirq(struct gpio_out g);
void gpio_out_toggle(struct gpio_out g);

struct gpio_in {
    void *gpio_port;
    uint16_t bit;
};
struct gpio_in gpio_in_setup(uint32_t pin, uint32_t val);
uint8_t gpio_in_read(struct gpio_in g);


/* for test function */
void goio_test_init(void);
void gpio_test_run(void);

#endif

