#include "gpio.h"
#include "uart.h"
#include "stm32f4xx_hal.h" 

extern GPIO_TypeDef  * const digital_regs[];

struct uart_serial {
    void *uart_port;
    
    void *tx_gpio;
    void *rx_gpio;

    uint16_t tx_bit;
    uint16_t rx_bit;

    uint32_t uart_af;

    UART_HandleTypeDef huart;
};

static void uart_gpio_reset(struct uart_serial uart) {

    GPIO_InitTypeDef GPIO_Init;

    GPIO_Init.Mode = GPIO_MODE_AF_PP;
    GPIO_Init.Pin = uart.tx_bit | uart.rx_bit;
    GPIO_Init.Pull = GPIO_NOPULL;
    GPIO_Init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_Init.Alternate = uart.uart_af;
    HAL_GPIO_Init(uart.tx_gpio, &GPIO_Init);
}

static void uart_config(struct uart_serial uart) {

    uart.huart.Instance = (USART_TypeDef * )uart.uart_port;
    HAL_UART_Init(&(uart.huart));
}

struct uart_serial uart_init(uint32_t tx_pin, uint32_t rx_pin, void *uart) {

    /* 检查引脚是否正确 */
    if(! gpio_valid(tx_pin)) { while(1); }
    if(! gpio_valid(rx_pin)) { while(1); }

    GPIO_TypeDef  *tx_gpio = digital_regs[GPIO2PORT(tx_pin)];
    GPIO_TypeDef  *rx_gpio = digital_regs[GPIO2PORT(rx_pin)]; 

    uint32_t tx_bit = GPIO2BIT(tx_pin);
    uint32_t rx_bit = GPIO2BIT(rx_pin);

    uint32_t af;

    /* 目前串口暂定选择使用这三个，因为有些可能是USARTx,有些是UARTx */
    if((USART_TypeDef *)uart == USART1)
        af = GPIO_AF7_USART1;
    else if ((USART_TypeDef *)uart == USART2)
        af = GPIO_AF7_USART2;
    else if ((USART_TypeDef *)uart == USART3)
        af = GPIO_AF7_USART3;

    struct uart_serial u = {.tx_gpio    = tx_gpio, 
                            .tx_bit     = tx_bit,
							.rx_gpio    = rx_gpio, 
                            .rx_bit     = rx_bit,
							.uart_port  = uart, 
                            .uart_af    = af 
                            };
    
    uart_gpio_reset(u);

    uart_config(u);

    return u;
}


