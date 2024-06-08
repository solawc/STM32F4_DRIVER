#include "gpio.h"
#include "uart.h"

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

void uart_irq_enable(struct uart_serial uart) {

    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
}

void uart_rx_irq_enable(struct uart_serial uart) {

    __HAL_UART_ENABLE_IT(&uart.huart, UART_IT_RXNE);
}

void uart_tx_irq_enable(struct uart_serial uart) {

    __HAL_UART_ENABLE_IT(&uart.huart, UART_IT_TXE);
}

void uart_rx_irq_disable(struct uart_serial uart) {

    __HAL_UART_DISABLE_IT(&uart.huart, UART_IT_RXNE);
}

void uart_tx_irq_disable(struct uart_serial uart) {

    __HAL_UART_DISABLE_IT(&uart.huart, UART_IT_TXE);
}


/*****************************************************/

/*****************demo********************************/
#define UART1_TX    GPIO('A', 9) 
#define UART1_RX    GPIO('A', 10) 
#define UART1_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define UART1_CLK_ENABLE()          __HAL_RCC_USART1_CLK_ENABLE()

struct uart_serial test_uart;

void serial_test_init(uint32_t baud) {

    UART1_GPIO_CLK_ENABLE();
    UART1_CLK_ENABLE();

    test_uart = uart_init(UART1_TX, UART1_RX, USART1, baud);
}

HAL_StatusTypeDef STATUS = HAL_ERROR;

void serial_test_send(uint8_t data) {

    STATUS = HAL_UART_Transmit(&(test_uart.huart), &data, 1, 1000);

    if(STATUS != HAL_OK) {

        while(1);
    }
}

void serial_test(void) {

    uint8_t str[9] = "GRBL DEBUG\r\n";

    uart_write(test_uart, str, 9);

    HAL_Delay(500);
}
/***********************************************************************************************************************/

UART_HandleTypeDef huart1 = {
    .Instance = USART1,
    .Init.BaudRate = 115200,
    .Init.HwFlowCtl = UART_HWCONTROL_NONE,
    .Init.Mode = UART_MODE_TX_RX,
    .Init.OverSampling = UART_OVERSAMPLING_16,
    .Init.Parity = UART_PARITY_NONE,
    .Init.StopBits = UART_STOPBITS_1,
    .Init.WordLength = UART_WORDLENGTH_8B,
};

struct uart_data {
    /* UART 句柄*/
    UART_HandleTypeDef *handle;

    /* UART 对应的GPIO */
    GPIO_TypeDef *gpio;
    uint16_t tx_pin;
    uint16_t rx_pin;
};

struct uart_device {
    char *name;
    struct uart_data *data;
    int (*dev_uart_init)(struct uart_device *p_dev, uint32_t baud);
    int (*dev_uart_send)(struct uart_device *p_dev, uint8_t data);
    int (*dev_uart_recv)(struct uart_device *p_dev, uint8_t data);
    int (*dev_uart_tx_irq)(struct uart_device *p_dev, bool status);
    int (*dev_uart_rx_irq)(struct uart_device *p_dev, bool status);
};

static int dev_uart_gpio_init(struct uart_device *p_dev) {

    GPIO_InitTypeDef GPIO_Init;
    GPIO_Init.Mode = GPIO_MODE_AF_PP;
    GPIO_Init.Pin = p_dev->data->tx_pin | p_dev->data->rx_pin;
    GPIO_Init.Pull = GPIO_NOPULL;
    GPIO_Init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_Init.Alternate = (uint8_t)0x07; 
    HAL_GPIO_Init(p_dev->data->gpio, p_dev->data->handle);
}

static int dev_uart_init(struct uart_device *p_dev, uint32_t baud)
{
    struct uart_data *data = p_dev->data;

    dev_uart_gpio_init(p_dev);

    if (HAL_UART_Init(data->handle) != HAL_OK)
    {
		return -1;
    }

    return 0;
}

static int dev_uart_send(struct uart_device *p_dev, uint8_t data)
{
    HAL_UART_Transmit(p_dev->data->handle, data, 1, 100);

    return 0;
}

static int dev_uart_recv(struct uart_device *p_dev, uint8_t *data)
{
    *data = p_dev->data->handle->Instance->DR;
    return 0;
}

int dev_uart_tx_irq(struct uart_device *p_dev, bool status)
{
    if(status)
        __HAL_UART_ENABLE_IT(p_dev->data->handle, UART_IT_TXE);
    else 
        __HAL_UART_DISABLE_IT(p_dev->data->handle, UART_IT_TXE);

    return 0;
}

int dev_uart_rx_irq(struct uart_device *p_dev, bool status)
{
    if(status)
        __HAL_UART_ENABLE_IT(p_dev->data->handle, UART_IT_RXNE);
    else 
        __HAL_UART_DISABLE_IT(p_dev->data->handle, UART_IT_RXNE);
    
    return 0;
}

struct uart_data g_stm32_uart1_data = {
    &huart1,
    GPIOA,
    GPIO_PIN_9,
    GPIO_PIN_10,
};

struct uart_device uart1 = {
    "stm32_uart1",          /* 设备名字：MCU厂家名_外设号 */
    &g_stm32_uart1_data,    /* 私有数据 */
    dev_uart_init,
    dev_uart_send,
    dev_uart_recv,
    dev_uart_tx_irq,
    dev_uart_rx_irq,
};

