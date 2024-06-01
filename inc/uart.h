#ifndef __uart_h
#define __uart_h

#include "stdint.h" // uint32_t

struct uart_serial uart_init(uint32_t tx_pin, uint32_t rx_pin, void *uart_port, uint32_t baud);


void serial_dev_init(uint32_t baud);
void serial_test(void);

#endif
