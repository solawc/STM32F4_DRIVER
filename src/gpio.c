#include "gpio.h"


GPIO_TypeDef  * const digital_regs[]={
    GPIOA,
    GPIOB,
    GPIOC,
#ifdef GPIOD
    GPIOD,
#endif

#ifdef GPIOE
    GPIOE,
#endif

#ifdef GPIOF
    GPIOF,
#endif

#ifdef GPIOG
    GPIOG,
#endif

#ifdef GPIOH
    GPIOH,
#endif

#ifdef GPIOI
	GPIOI,
#endif

#ifdef GPIOJ
	GPIOJ,
#endif

#ifdef GPIOK
	GPIOK
#endif
};

int ffs(int num) {
    // 如果num为0，则返回0
    if (num == 0) return 0;

    // 使用位运算找到最低位的设置位的位置
    int position = 1;
    while ((num & 1) == 0) {
        num >>= 1;
        position++;
    }
    return position;
}


int regs_to_pin(GPIO_TypeDef  *regs, uint32_t bit) {
	int i;
    for (i=0; i<ARRAY_SIZE(digital_regs); i++)
        if (digital_regs[i] == regs)
            return GPIO('A' + i, ffs(bit)-1);
    return 0;
}

// 校验GPIO端口和pin引脚是否和芯片复合
static int
gpio_valid(uint32_t pin)
{
    uint32_t port = GPIO2PORT(pin);

    int array_size = ARRAY_SIZE(digital_regs);

    GPIO_TypeDef *gpio_port = digital_regs[port];

    // return port < ARRAY_SIZE(digital_regs) && digital_regs[port];
    return port < array_size && digital_regs[port];
}

static void gpio_out_init(GPIO_TypeDef  *GPIOx ,uint32_t gpio_pin, uint32_t val) {

    GPIO_InitTypeDef GPIO_Init;

    HAL_GPIO_WritePin(GPIOx, gpio_pin, (GPIO_PinState)val);

    GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init.Pin = gpio_pin;
    GPIO_Init.Pull = GPIO_NOPULL;
    GPIO_Init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOx, &GPIO_Init);
}
 
struct gpio_out 
gpio_out_setup(uint32_t pin, uint32_t val) {

    // HAL CLK Init
    // 在stm32的底层接口上，基本使用HAL库，方便迁移。

    int gpio_valid_get = gpio_valid(pin);

    if(!gpio_valid_get) {
        // error!;
        while(1);
    }

    GPIO_TypeDef  *gpio_port = digital_regs[GPIO2PORT(pin)];

    uint32_t bit = GPIO2BIT(pin);

    gpio_out_init(gpio_port, bit, val);  // 真正初始化的地方

    struct gpio_out g = {.gpio_port=gpio_port,.bit=bit}; //记录并返回对应的GPIO信息

    // GPIO_PIN_10 // 0X0400

    return g;
}

void gpio_out_write(struct gpio_out g, uint32_t val) {

    HAL_GPIO_WritePin(g.gpio_port, g.bit, (GPIO_PinState)val);
}

struct gpio_in
gpio_in_setup(uint32_t pin, uint32_t val) {

    if(gpio_valid(pin)) {
        // error!;
        while(1);
    }

    GPIO_TypeDef  *gpio_port = digital_regs[GPIO2PORT(pin)];

    uint32_t bit = GPIO2BIT(pin);

    gpio_out_init(gpio_port, bit, val);  // 真正初始化的地方

    struct gpio_in g = {.gpio_port=gpio_port,.bit=bit}; //记录并返回对应的GPIO信息

    return g;

}

struct test_io {

	struct gpio_out test_io0;

};

struct test_io test_io_obj;

#define LED_GPIO_PIN		GPIO('H', 11)   //regs_to_pin(GPIOH, 10)

void goio_test_init(void) {

	test_io_obj.test_io0 = gpio_out_setup(LED_GPIO_PIN, 0);
}

void gpio_test_run(void) {

	while(1) {
		gpio_out_write(test_io_obj.test_io0, 0);
        HAL_Delay(500);
		gpio_out_write(test_io_obj.test_io0, 1);
		HAL_Delay(500);
	}
}



