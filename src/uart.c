#include "gpio.h"
#include "uart.h"




// struct uart_serial uart_init(uint32_t tx_pin, uint32_t rx_pin, void *uart_port, uint32_t baud) {

//     uart_gpio_reset(tx_pin, rx_pin);

//     return uart_config(uart_port, baud);
// }

// void uart_write_byte(struct uart_serial uart, uint8_t data) {

//     HAL_UART_Transmit(&(uart.huart), &data, 1, 1000);
// }

// void uart_write(struct uart_serial uart, uint8_t *str, uint16_t size) {

//     HAL_UART_Transmit(&(uart.huart), str, size, 0xffff);
// }

// void uart_irq_enable(struct uart_serial uart) {

//     HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
// 	   HAL_NVIC_EnableIRQ(USART1_IRQn);
// }

// void uart_rx_irq_enable(struct uart_serial uart) {

//     __HAL_UART_ENABLE_IT(&uart.huart, UART_IT_RXNE);
// }

// void uart_tx_irq_enable(struct uart_serial uart) {

//     __HAL_UART_ENABLE_IT(&uart.huart, UART_IT_TXE);
// }

// void uart_rx_irq_disable(struct uart_serial uart) {

//     __HAL_UART_DISABLE_IT(&uart.huart, UART_IT_RXNE);
// }

// void uart_tx_irq_disable(struct uart_serial uart) {

//     __HAL_UART_DISABLE_IT(&uart.huart, UART_IT_TXE);
// }
/*

    HAL_NVIC_SetPriority(LaserUART_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(LaserUART_IRQn);


*/


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

static int dev_uart_irq_enable(struct uart_device *p_dev, uint32_t PreemptPriority, uint32_t SubPriority) {
    HAL_NVIC_SetPriority(p_dev->data->irq_n, PreemptPriority, SubPriority);
    HAL_NVIC_EnableIRQ(p_dev->data->irq_n);
}

static int dev_uart_gpio_init(struct uart_device *p_dev) {

    GPIO_InitTypeDef GPIO_Init;
    GPIO_Init.Mode = GPIO_MODE_AF_PP;
    GPIO_Init.Pin = p_dev->data->tx_pin | p_dev->data->rx_pin;
    GPIO_Init.Pull = GPIO_NOPULL;
    GPIO_Init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_Init.Alternate = (uint8_t)0x07; 
    HAL_GPIO_Init(p_dev->data->gpio, &GPIO_Init);
}

static int dev_uart_init(struct uart_device *p_dev, uint32_t baud)
{
    struct uart_data *data = p_dev->data;

    dev_uart_gpio_init(p_dev);

    data->handle->Init.BaudRate = baud; 

    if (HAL_UART_Init(data->handle) != HAL_OK)
    {
        while(1);
		return -1;
    }

    return 0;
}

static int dev_uart_send(struct uart_device *p_dev, uint8_t data)
{
    HAL_StatusTypeDef status = HAL_OK;

    status = HAL_UART_Transmit(p_dev->data->handle, &data, 1, 100);

    if(status != HAL_OK) 
        return -1;
    else 
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
    USART1_IRQn,
};

struct uart_device uart1 = {
    "stm32_uart1",          /* 设备名字：MCU厂家名_外设号 */
    &g_stm32_uart1_data,    /* 私有数据 */
    dev_uart_init,
    dev_uart_send,
    dev_uart_recv,
    dev_uart_tx_irq,
    dev_uart_rx_irq,
    dev_uart_irq_enable,
};

struct uart_device *g_uart_dev_num[] = {
    &uart1,
};

struct uart_device *get_uart_dev(char *name) {

    int i = 0;

    for(i=0; i< sizeof(g_uart_dev_num) / sizeof(g_uart_dev_num[0]); i++) {

        if( 0 == strcmp(name, g_uart_dev_num[i]->name)) {
            return g_uart_dev_num[i];
        }
    }

    return NULL;
}

void uart_test1(void) {

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART1_CLK_ENABLE();

    struct uart_device *p_dev;
    p_dev = get_uart_dev("stm32_uart1");

    p_dev->dev_uart_init(p_dev, 115200);

    while (1)
    {
        p_dev->dev_uart_send(p_dev, 'B');
        HAL_Delay(500);
    }

}
