#ifndef __uart_h
#define __uart_h

#include "stdint.h" // uint32_t
#include "stdbool.h"
#include "stm32f4xx_hal.h" 

#define RX_BUFFER_SIZE  192
#define TX_BUFFER_SIZE  96

struct uart_data {
    /* UART 句柄*/
    UART_HandleTypeDef *handle;

    /* UART 对应的GPIO */
    GPIO_TypeDef *gpio;
    uint16_t tx_pin;
    uint16_t rx_pin;

    IRQn_Type irq_n;
    uint8_t rx_data;
};

struct uart_device {
    char *name;
    struct uart_data *data;
    int (*dev_uart_init)(struct uart_device *p_dev, uint32_t baud);
    int (*dev_uart_send)(struct uart_device *p_dev, uint8_t data);
    int (*dev_uart_recv)(struct uart_device *p_dev, uint8_t data);
    int (*dev_uart_tx_irq)(struct uart_device *p_dev, bool status);
    int (*dev_uart_rx_irq)(struct uart_device *p_dev, bool status);
    int (*dev_uart_irq_set)(struct uart_device *p_dev, uint32_t PreemptPriority, uint32_t SubPriority);
};



struct uart_device *dev_uart_get(char *name);

void uart_test1(void);




#endif
