#include "gpio.h"
#include "uart.h"
#include "stm32f4xx_hal.h" 

struct uart_serial {
    uint32_t baud;
    UART_HandleTypeDef huart;

    uint8_t receive_buff[192], receive_pos;
    uint8_t transmit_buff[96], transmit_pos, transmit_max;
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

void uart_write_byte(struct uart_serial uart, uint8_t data) {

    HAL_UART_Transmit(&(uart.huart), &data, 1, 1000);
}

void uart_write(struct uart_serial uart, uint8_t *str, uint16_t size) {

    HAL_UART_Transmit(&(uart.huart), str, size, 0xffff);
}

void uart_rx_irq_enable(struct uart_serial uart) {

    // ..
}

void uart_tx_irq_enable(struct uart_serial uart) {

    // ..
}

void uart_rx_irq_disable(struct uart_serial uart) {

    // ..
}

void uart_tx_irq_disable(struct uart_serial uart) {

    // ..
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

    uint8_t str[9] = "debug1\r\n";

    uart_write(debug_uart, str, 9);

    HAL_Delay(500);
}


#define CR1_FLAGS (USART_CR1_UE | USART_CR1_RE | USART_CR1_TE   \
                   | USART_CR1_RXNEIE)

void USART1_IRQHandler(void) {

/*
    uint32_t sr = USARTx->SR;
    if (sr & (USART_SR_RXNE | USART_SR_ORE)) {
        // The ORE flag is automatically cleared by reading SR, followed
        // by reading DR.
        serial_rx_byte(USARTx->DR);
    }
    if (sr & USART_SR_TXE && USARTx->CR1 & USART_CR1_TXEIE) {
        uint8_t data;
        int ret = serial_get_tx_byte(&data);
        if (ret)
            USARTx->CR1 = CR1_FLAGS;
        else
            USARTx->DR = data;
    }
*/
    uint32_t sr = debug_uart.huart.Instance->SR;

    if (sr & (USART_SR_RXNE | USART_SR_ORE)) {

    }

    if (sr & USART_SR_TXE && debug_uart.huart.Instance->CR1 & USART_CR1_TXEIE) {
        uint8_t data;
        // int ret = serial_get_tx_byte(&data);
        int ret = -1;
        if (ret)
            debug_uart.huart.Instance->CR1 = CR1_FLAGS;
        else
            debug_uart.huart.Instance->DR = data;
    }
}
