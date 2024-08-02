#include "gpio_a.h"
#include "stm32f4xx_hal.h" 
#include "board_pins.h"

int stepper_gpio_init(void) {

    GPIO_InitTypeDef GPIO_Init; 

	GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    /* stepper 使能引脚初始化 */
    #ifdef STEP_X_EN_PORT
        GPIO_Init.Pin = STEP_X_EN_PIN;
        HAL_GPIO_Init(STEP_X_EN_PORT, &GPIO_Init);
    #endif

    #ifdef STEP_Y_EN_PORT
        GPIO_Init.Pin = STEP_Y_EN_PIN;
        HAL_GPIO_Init(STEP_Y_EN_PORT, &GPIO_Init);
    #endif

    #ifdef STEP_Z_EN_PORT
        GPIO_Init.Pin = STEP_Z_EN_PIN;
        HAL_GPIO_Init(STEP_Z_EN_PORT, &GPIO_Init);
    #endif

    /* stepper 方向引脚初始化 */
        GPIO_Init.Pin = MOTOR_X_DIR_PIN;
        HAL_GPIO_Init(MOTOR_X_DIR_PORT, &GPIO_Init);

        GPIO_Init.Pin = MOTOR_Y_DIR_PIN;
        HAL_GPIO_Init(MOTOR_Y_DIR_PORT, &GPIO_Init);
    #ifdef MOTOR_Z_DIR_PIN
        GPIO_Init.Pin = MOTOR_Z_DIR_PIN;
        HAL_GPIO_Init(MOTOR_Z_DIR_PORT, &GPIO_Init);
    #endif

    /* stepper step引脚初始化 */
    GPIO_Init.Pin = MOTOR_X_AXIS_PIN;
    HAL_GPIO_Init(MOTOR_X_AXIS_PORT, &GPIO_Init);

	GPIO_Init.Pin = MOTOR_Y_AXIS_PIN;
    HAL_GPIO_Init(MOTOR_Y_AXIS_PORT, &GPIO_Init);
#ifdef MOTOR_Z_AXIS_PIN
	GPIO_Init.Pin = MOTOR_Z_AXIS_PIN;
    HAL_GPIO_Init(MOTOR_Z_AXIS_PORT, &GPIO_Init);
#endif

    return 0;
}

int stepper_en(bool status) {
    if(status) { 
	#ifdef STEP_EN_PORT
		HAL_GPIO_WritePin(STEP_EN_PORT, STEP_EN_PIN, GPIO_PIN_SET); 
	#endif

	#ifdef STEP_X_EN_PORT
		HAL_GPIO_WritePin(STEP_X_EN_PORT, STEP_X_EN_PIN, GPIO_PIN_SET); 
	#endif

	#ifdef STEP_Y_EN_PORT
		HAL_GPIO_WritePin(STEP_Y_EN_PORT, STEP_Y_EN_PIN, GPIO_PIN_SET); 
	#endif

	#ifdef STEP_Z_EN_PORT
		HAL_GPIO_WritePin(STEP_Z_EN_PORT, STEP_Z_EN_PIN, GPIO_PIN_SET); 
	#endif 
	}
	else { 
	#ifdef STEP_EN_PORT
		HAL_GPIO_WritePin(STEP_EN_PORT, STEP_EN_PIN, GPIO_PIN_RESET); 
	#endif

	#ifdef STEP_X_EN_PORT
		HAL_GPIO_WritePin(STEP_X_EN_PORT, STEP_X_EN_PIN, GPIO_PIN_RESET); 
	#endif

	#ifdef STEP_Y_EN_PORT
		HAL_GPIO_WritePin(STEP_Y_EN_PORT, STEP_Y_EN_PIN, GPIO_PIN_RESET); 
	#endif

	#ifdef STEP_Z_EN_PORT
		HAL_GPIO_WritePin(STEP_Z_EN_PORT, STEP_Z_EN_PIN, GPIO_PIN_RESET); 
	#endif 
	}
	
	return 0;
}

int stepper_dir(uint8_t mask) {
    if(mask & 0x01) {HAL_GPIO_WritePin(MOTOR_X_DIR_PORT, MOTOR_X_DIR_PIN, GPIO_PIN_SET);}
	else {HAL_GPIO_WritePin(MOTOR_X_DIR_PORT, MOTOR_X_DIR_PIN, GPIO_PIN_RESET);}

	if(mask & 0x02) {HAL_GPIO_WritePin(MOTOR_Y_DIR_PORT, MOTOR_Y_DIR_PIN, GPIO_PIN_SET);}
	else {HAL_GPIO_WritePin(MOTOR_Y_DIR_PORT, MOTOR_Y_DIR_PIN, GPIO_PIN_RESET);}

#ifdef MOTOR_Z_AXIS_PIN
	if(mask & 0x04) {HAL_GPIO_WritePin(MOTOR_Z_DIR_PORT, MOTOR_Z_DIR_PIN, GPIO_PIN_SET);}
	else {HAL_GPIO_WritePin(MOTOR_Z_DIR_PORT, MOTOR_Z_DIR_PIN, GPIO_PIN_RESET);}
#endif
	return 0;
}

int stepper_step(uint8_t mask) {
    if(mask & 0x01) {HAL_GPIO_WritePin(MOTOR_X_AXIS_PORT, MOTOR_X_AXIS_PIN, GPIO_PIN_SET);}
	else {HAL_GPIO_WritePin(MOTOR_X_AXIS_PORT, MOTOR_X_AXIS_PIN, GPIO_PIN_RESET);}

	if(mask & 0x02) {HAL_GPIO_WritePin(MOTOR_Y_AXIS_PORT, MOTOR_Y_AXIS_PIN, GPIO_PIN_SET);}
	else {HAL_GPIO_WritePin(MOTOR_Y_AXIS_PORT, MOTOR_Y_AXIS_PIN, GPIO_PIN_RESET);}

	#ifdef MOTOR_Z_AXIS_PIN
		if(mask & 0x04) {HAL_GPIO_WritePin(MOTOR_Z_AXIS_PORT, MOTOR_Z_AXIS_PIN, GPIO_PIN_SET);}
		else {HAL_GPIO_WritePin(MOTOR_Z_AXIS_PORT, MOTOR_Z_AXIS_PIN, GPIO_PIN_RESET);}
	#endif
	return 0;
}

uint8_t stepper_get_axis_mask(uint8_t axis) {
    if(axis == 0) return 0x01;
	else if(axis == 1)	return 0x02;
	else return 0x04;
}


int limit_gpio_init(void) {

    GPIO_InitTypeDef GPIO_Init = {0};
	GPIO_Init.Mode = GPIO_MODE_IT_RISING;
	GPIO_Init.Pull = GPIO_NOPULL;

#ifdef LIMIT_X_PIN
	GPIO_Init.Pin = LIMIT_X_PIN;
    HAL_GPIO_Init(LIMIT_X_PORT, &GPIO_Init);
#endif

#ifdef LIMIT_Y_PIN
	GPIO_Init.Pin = LIMIT_Y_PIN;
    HAL_GPIO_Init(LIMIT_Y_PORT, &GPIO_Init);
#endif

#ifdef LIMIT_Z_PIN 
	GPIO_Init.Pin = LIMIT_Z_PIN;
    HAL_GPIO_Init(LIMIT_Z_PORT, &GPIO_Init);
#endif
    return 0;
}

uint8_t limit_gpio_get_state(uint8_t bit_select) {

    uint8_t limit_status = 0x00;
	
	limit_status |= HAL_GPIO_ReadPin(LIMIT_X_PORT, LIMIT_X_PIN) << 0;
	limit_status |= (HAL_GPIO_ReadPin(LIMIT_Y_PORT, LIMIT_Y_PIN)) << 1;
#ifdef LIMIT_Z_PIN
	limit_status |= (HAL_GPIO_ReadPin(LIMIT_Y_PORT, LIMIT_Y_PIN)) << 2;
#endif
	return limit_status;
}

int limit_gpio_irq_set(bool state) {

    if(state) {
        HAL_NVIC_SetPriority(LIMIT_IRQn, 0, 0);
	    HAL_NVIC_EnableIRQ(LIMIT_IRQn);
    }else {
        HAL_NVIC_DisableIRQ(LIMIT_IRQn);
    }

    return 0;
}
/*********************************************************************** */

GPIO_TypeDef * const digital_regs[] = {
    ['A' - 'A'] = GPIOA, GPIOB, GPIOC,
#ifdef GPIOD
    ['D' - 'A'] = GPIOD,
#endif
#ifdef GPIOE
    ['E' - 'A'] = GPIOE,
#endif
#ifdef GPIOF
    ['F' - 'A'] = GPIOF,
#endif
#ifdef GPIOG
    ['G' - 'A'] = GPIOG,
#endif
#ifdef GPIOH
    ['H' - 'A'] = GPIOH,
#endif
#ifdef GPIOI
    ['I' - 'A'] = GPIOI,
#endif
};

// 实现 ffs() 函数
static int ffs(int i) {
    if (i == 0) return 0;
    int position = 1;
    while ((i & 1) == 0) {
        i >>= 1;
        position++;
    }
    return position;
}

void gpio_peripheral(uint32_t gpio, uint32_t mode, int pullup, uint32_t Alternate) 
{

	GPIO_TypeDef *regs = digital_regs[GPIO2PORT(gpio)];
	uint32_t pin = GPIO2BIT(gpio);

	GPIO_InitTypeDef GPIO_Init; 
	GPIO_Init.Alternate = Alternate; /* 没有配置复用，这个不起作用 */
	GPIO_Init.Mode = mode;
	GPIO_Init.Pull = GPIO_NOPULL;	
	GPIO_Init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_Init.Pin = pin;

	HAL_GPIO_Init(regs, &GPIO_Init);
}

// Convert a register and bit location back to an integer pin identifier
static int
regs_to_pin(GPIO_TypeDef *regs, uint32_t bit)
{
    int i;
    for (i=0; i<ARRAY_SIZE(digital_regs); i++)
        if (digital_regs[i] == regs)
            return GPIO('A' + i, ffs(bit)-1);
    return 0;
}

// Verify that a gpio is a valid pin
static int gpio_valid(uint32_t pin)
{
    uint32_t port = GPIO2PORT(pin);
    return port < ARRAY_SIZE(digital_regs) && digital_regs[port];
}

struct gpio_out gpio_out_setup(uint32_t pin, uint32_t val)
{
    if (!gpio_valid(pin)) {
		while(1);
	}

    GPIO_TypeDef *regs = digital_regs[GPIO2PORT(pin)];
    // gpio_clock_enable(regs); // 时钟初始化
    struct gpio_out g = { .regs=regs, .bit=GPIO2BIT(pin) };
    gpio_out_reset(g, val);
    return g;
}

void gpio_out_reset(struct gpio_out g, uint32_t val)
{
    GPIO_TypeDef *regs = g.regs;

    int pin = regs_to_pin(regs, g.bit);

    if (val)
        regs->BSRR = g.bit;
    else
        regs->BSRR = g.bit << 16;
    gpio_peripheral(pin, GPIO_MODE_OUTPUT_PP, 0, 0x00);
}

void
gpio_out_toggle_noirq(struct gpio_out g)
{
    GPIO_TypeDef *regs = g.regs;
	HAL_GPIO_TogglePin(regs, g.bit);
}

void
gpio_out_toggle(struct gpio_out g)
{
    // irqstatus_t flag = irq_save();
    gpio_out_toggle_noirq(g);
    // irq_restore(flag);
}

void
gpio_out_write(struct gpio_out g, uint32_t val)
{
    GPIO_TypeDef *regs = g.regs;
	HAL_GPIO_WritePin(regs, g.bit, (GPIO_PinState)val);
}

