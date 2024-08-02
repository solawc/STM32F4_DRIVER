#include "gpio_a.h"
#include "uart.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"


static int dev_uart_irq_enable(struct uart_device *p_dev, uint32_t PreemptPriority, uint32_t SubPriority) {
    HAL_NVIC_SetPriority(p_dev->data->irq_n, PreemptPriority, SubPriority);
    HAL_NVIC_EnableIRQ(p_dev->data->irq_n);
	return 0;
}

static int dev_uart_gpio_init(struct uart_device *p_dev) {

    GPIO_InitTypeDef GPIO_Init;
    GPIO_Init.Mode = GPIO_MODE_AF_PP;
    GPIO_Init.Pin = p_dev->data->tx_pin | p_dev->data->rx_pin;
    GPIO_Init.Pull = GPIO_NOPULL;
    GPIO_Init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_Init.Alternate = (uint8_t)0x07; 
    HAL_GPIO_Init(p_dev->data->gpio, &GPIO_Init);
	
	
	return 0;
}

static int dev_uart_init(struct uart_device *p_dev, uint32_t baud)
{
    struct uart_data *data = p_dev->data;

    dev_uart_gpio_init(p_dev);

    data->handle->Init.BaudRate = baud; 

    if (HAL_UART_Init(data->handle) != HAL_OK)
    {
		return -1;
    }

    return 0;
}

static int dev_uart_send(struct uart_device *p_dev, uint8_t data)
{
    // 这里应该要实现串口中断发送，发送队列中的数据

    HAL_StatusTypeDef status = HAL_OK;

    status = HAL_UART_Transmit(p_dev->data->handle, &data, 1, 100); // 阻塞型发送，效率低不好

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

static int dev_uart_tx_irq(struct uart_device *p_dev, bool status)
{
    if(status)
        __HAL_UART_ENABLE_IT(p_dev->data->handle, UART_IT_TXE);
    else 
        __HAL_UART_DISABLE_IT(p_dev->data->handle, UART_IT_TXE);
    return 0;
}

static int dev_uart_rx_irq(struct uart_device *p_dev, bool status)
{
    if(status)
        HAL_UART_Receive_IT(p_dev->data->handle, &p_dev->data->rx_data, 1); // 启动中断接收
    else 
        __HAL_UART_DISABLE_IT(p_dev->data->handle, UART_IT_RXNE);

    return 0;
}

/*****************************************************
 *                  UART handler define
*****************************************************/
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


/*****************************************************
 *                  UART Regiest 
*****************************************************/
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

/*****************************************************
 *                  对外释放的接口 
*****************************************************/
struct uart_device *g_uart_dev_num[] = {
    &uart1,
};

struct uart_device *dev_uart_get(char *name) {

    for(int i=0; i< sizeof(g_uart_dev_num) / sizeof(g_uart_dev_num[0]); i++) {

        if( 0 == strcmp(name, g_uart_dev_num[i]->name)) {
            return g_uart_dev_num[i];
        }
    }

    return NULL;
}

/*****************************************************
 *            UART IRQHandler Function
*****************************************************/
extern struct uart_device *serial_connect;
void USART1_IRQHandler(void) {

    HAL_UART_IRQHandler(&huart1);
}

/*****************************************************
 *                  UART Driver Test
*****************************************************/

void uart_test1(void) {

    // 时钟初始化
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART1_CLK_ENABLE();

    struct uart_device *p_dev;

    p_dev = dev_uart_get("stm32_uart1");

    p_dev->dev_uart_init(p_dev, 115200);

    while (1)
    {
        p_dev->dev_uart_send(p_dev, 'B');
        HAL_Delay(500);
    }
}
