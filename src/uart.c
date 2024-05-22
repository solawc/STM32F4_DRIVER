#include "gpio.h"
#include "uart.h"
#include "stm32f4xx_hal.h" 

extern GPIO_TypeDef * const digital_regs[];

struct uart_serial {

    uint32_t baud;

    UART_HandleTypeDef huart;
};

static void uart_gpio_reset(uint32_t tx_pin, uint32_t rx_pin) {

    GPIO_InitTypeDef GPIO_Init;

    GPIO_TypeDef *tx_port = digital_regs[GPIO2PORT(tx_pin)];
    GPIO_TypeDef *rx_port = digital_regs[GPIO2PORT(rx_pin)];

    uint16_t tx_bit = GPIO2BIT(tx_pin);
    uint16_t rx_bit = GPIO2BIT(rx_pin);

    GPIO_Init.Mode = GPIO_MODE_AF_PP;
    GPIO_Init.Pin = tx_bit;
    GPIO_Init.Pull = GPIO_NOPULL;
    GPIO_Init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_Init.Alternate = (uint8_t)0x07; 
    HAL_GPIO_Init(tx_port, &GPIO_Init);

    GPIO_Init.Mode = GPIO_MODE_AF_PP;
    GPIO_Init.Pin = rx_bit;
    GPIO_Init.Pull = GPIO_NOPULL;
    GPIO_Init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_Init.Alternate = (uint8_t)0x07; 
    HAL_GPIO_Init(rx_port, &GPIO_Init);
}

static struct uart_serial uart_config(void *uart_port, uint32_t baud) {

    struct uart_serial uart;

    uart.huart.Instance = uart_port;
    uart.huart.Init.BaudRate = baud;
    uart.huart.Init.WordLength = UART_WORDLENGTH_8B;
    uart.huart.Init.StopBits = UART_STOPBITS_1;
    uart.huart.Init.Parity = UART_PARITY_NONE;
    uart.huart.Init.Mode = UART_MODE_TX_RX;
    uart.huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    uart.huart.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&uart.huart) != HAL_OK)
    {
			while(1);
    }
		
	return uart;
}

struct uart_serial uart_init(uint32_t tx_pin, uint32_t rx_pin, void *uart_port, uint32_t baud) {

    uart_gpio_reset(tx_pin, rx_pin);

    return uart_config(uart_port, baud);
}


/*****************************************************/

/*****************demo********************************/
#define UART1_TX    GPIO('A', 9) 
#define UART1_RX    GPIO('A', 10) 

struct uart_serial debug_uart;

void serial_init(uint32_t baud) {

    __HAL_RCC_USART1_CLK_ENABLE();

    debug_uart = uart_init(UART1_TX, UART1_RX, USART1, baud);
}

HAL_StatusTypeDef STATUS = HAL_ERROR;

void serial_send(uint8_t data) {

    STATUS = HAL_UART_Transmit(&(debug_uart.huart), &data, 1, 1000);

    if(STATUS != HAL_OK) {

        while(1);
    }
}

void serial_test(void) {

    // serial_send('A');

    uint8_t str[8] = "debug\r\n";

    STATUS = HAL_UART_Transmit(&(debug_uart.huart), str, 8, 1000);

    if(STATUS != HAL_OK) {
        while(1);
    }

    HAL_Delay(500);
}

