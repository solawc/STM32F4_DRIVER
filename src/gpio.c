#include "gpio.h"
#include "stm32f4xx_hal.h" 

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

void gpio_all_clock_init(void) {

    __HAL_RCC_GPIOA_CLK_ENABLE(); 
    __HAL_RCC_GPIOB_CLK_ENABLE(); 
    __HAL_RCC_GPIOC_CLK_ENABLE(); 

#ifdef GPIOD
    __HAL_RCC_GPIOD_CLK_ENABLE();
#endif

#ifdef GPIOE
    __HAL_RCC_GPIOE_CLK_ENABLE();
#endif

#ifdef GPIOF
    __HAL_RCC_GPIOF_CLK_ENABLE();
#endif

#ifdef GPIOG
    __HAL_RCC_GPIOG_CLK_ENABLE();
#endif

#ifdef GPIOH
    __HAL_RCC_GPIOH_CLK_ENABLE();
#endif

#ifdef GPIOI
    __HAL_RCC_GPIOI_CLK_ENABLE();
#endif

#ifdef GPIOJ
    __HAL_RCC_GPIOJ_CLK_ENABLE();
#endif

#ifdef GPIOJ
    __HAL_RCC_GPIOK_CLK_ENABLE();
#endif
}



// 校验GPIO端口和pin引脚是否和芯片复合
static int
gpio_valid(uint32_t pin)
{
    uint32_t port = GPIO2PORT(pin);
    return port < ARRAY_SIZE(digital_regs) && digital_regs[port];
}

void gpio_out_reset(GPIO_TypeDef  *GPIOx ,uint32_t gpio_pin, uint32_t val) {

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

    int gpio_valid_get = gpio_valid(pin);

    if(!gpio_valid_get) {
        // error!;
        while(1);
    }

    GPIO_TypeDef  *gpio_port = digital_regs[GPIO2PORT(pin)];

    uint32_t bit = GPIO2BIT(pin);

    gpio_out_reset(gpio_port, bit, val);  // 真正初始化的地方

    struct gpio_out g = {.gpio_port=gpio_port,.bit=bit}; //记录并返回对应的GPIO信息

    return g;
}

void gpio_out_write(struct gpio_out g, uint32_t val) {

    HAL_GPIO_WritePin(g.gpio_port, g.bit, (GPIO_PinState)val);
}

void gpio_out_toggle_noirq(struct gpio_out g)
{
    HAL_GPIO_TogglePin(g.gpio_port, g.bit);
}

void gpio_out_toggle(struct gpio_out g)
{
    // irqstatus_t flag = irq_save();
    gpio_out_toggle_noirq(g);
    // irq_restore(flag);
}


void gpio_in_reset(GPIO_TypeDef  *GPIOx ,uint32_t gpio_pin, uint32_t val) {

    GPIO_InitTypeDef GPIO_Init;

    HAL_GPIO_WritePin(GPIOx, gpio_pin, (GPIO_PinState)val);

    GPIO_Init.Mode = GPIO_MODE_INPUT;
    GPIO_Init.Pin = gpio_pin;
    GPIO_Init.Pull = GPIO_NOPULL;
    GPIO_Init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOx, &GPIO_Init);
}

struct gpio_in gpio_in_setup(uint32_t pin, uint32_t val) {

    if(!gpio_valid(pin)) { while(1); }  // 使用了不存在的引脚

    GPIO_TypeDef  *gpio_port = digital_regs[GPIO2PORT(pin)];

    uint32_t bit = GPIO2BIT(pin);

    gpio_in_reset(gpio_port, bit, val);  // 真正初始化的地方

    struct gpio_in g = {.gpio_port=gpio_port,.bit=bit}; //记录并返回对应的GPIO信息

    return g;
}

uint8_t gpio_in_read(struct gpio_in g)
{
    return (uint8_t)HAL_GPIO_ReadPin(g.gpio_port, g.bit);
}

/*******************使用demo***********************/

#define LED_R_GPIO_PIN		GPIO('H', 10) 
#define LED_G_GPIO_PIN		GPIO('H', 11)
#define LED_B_GPIO_PIN		GPIO('H', 12) 

struct test_io {
	struct gpio_out test_led_r, test_led_g, test_led_b;
};

struct test_io test_io_obj;

void goio_test_init(void) {

	test_io_obj.test_led_r = gpio_out_setup(LED_R_GPIO_PIN, 0);
    test_io_obj.test_led_g = gpio_out_setup(LED_G_GPIO_PIN, 0);
    test_io_obj.test_led_b = gpio_out_setup(LED_B_GPIO_PIN, 0);
}

void gpio_test_run(void) {

	while(1) {
		gpio_out_write(test_io_obj.test_led_r, 0);
        HAL_Delay(500);
		gpio_out_write(test_io_obj.test_led_r, 1);
		HAL_Delay(500);

        gpio_out_write(test_io_obj.test_led_g, 0);
        HAL_Delay(500);
		gpio_out_write(test_io_obj.test_led_g, 1);
		HAL_Delay(500);

        gpio_out_write(test_io_obj.test_led_b, 0);
        HAL_Delay(500);
		gpio_out_write(test_io_obj.test_led_b, 1);
		HAL_Delay(500);
	}
}



