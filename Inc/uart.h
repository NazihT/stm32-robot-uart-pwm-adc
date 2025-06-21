#ifndef UART_H
#define UART_H

#include <stdint.h>


#define UART2_RX_BUFFER_SIZE 128


// Global  buffer
extern volatile uint8_t uart2_rx_buffer[UART2_RX_BUFFER_SIZE];
extern volatile uint32_t uart2_rx_index;


void UART2_Init(uint32_t baudrate);
void UART2_SendChar(uint8_t ch);
void USART2_Send(const char* str);

#endif
