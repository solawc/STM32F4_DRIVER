#ifndef __uart_h
#define __uart_h

#include "stdint.h" // uint32_t

struct uart
{
    void *tx_gpio_port;
    void *rx_gpio_port;
    uint16_t tx_bit;
    uint16_t rx_bit;
    
    void *uart_port;
    
};



#endif
