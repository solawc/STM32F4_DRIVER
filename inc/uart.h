#ifndef __uart_h
#define __uart_h

#include "stdint.h" // uint32_t
#include "stdbool.h"
#include "stm32f4xx_hal.h" 

#define RX_BUFFER_SIZE  192
#define TX_BUFFER_SIZE  96

// struct uart_serial {
//     uint32_t baud;
//     UART_HandleTypeDef huart;

//     uint8_t receive_buff[RX_BUFFER_SIZE], receive_pos;
//     uint8_t transmit_buff[TX_BUFFER_SIZE], transmit_pos, transmit_max;
// };


// struct uart_serial uart_init(uint32_t tx_pin, uint32_t rx_pin, void *uart_port, uint32_t baud);
// void uart_write_byte(struct uart_serial uart, uint8_t data);
// void uart_irq_enable(struct uart_serial uart);
// void uart_rx_irq_enable(struct uart_serial uart);
// void uart_tx_irq_enable(struct uart_serial uart);
// void uart_rx_irq_disable(struct uart_serial uart);
// void uart_tx_irq_disable(struct uart_serial uart);


struct uart_data {
    /* UART 句柄*/
    UART_HandleTypeDef *handle;

    /* UART 对应的GPIO */
    GPIO_TypeDef *gpio;
    uint16_t tx_pin;
    uint16_t rx_pin;

    IRQn_Type irq_n;
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



struct uart_device *get_uart_dev(char *name);

void uart_test1(void);




#endif
