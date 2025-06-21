#include "uart.h"
#include "stm32f407xx.h"
#include <stdio.h>
#include <string.h>
extern command_ready;
extern uint16_t speed;
uint32_t SystemCoreClock = 16000000;


volatile uint8_t uart2_rx_buffer[UART2_RX_BUFFER_SIZE];
volatile uint32_t uart2_rx_index = 0;

// Initialize UART2 (PA2 = TX, PA3 = RX)
void UART2_Init(uint32_t baudrate) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    GPIOA->MODER &= ~(GPIO_MODER_MODER2 | GPIO_MODER_MODER3);
    GPIOA->MODER |= (0x02 << GPIO_MODER_MODER2_Pos) | (0x02 << GPIO_MODER_MODER3_Pos);
    GPIOA->AFR[0] |= (0x07 << GPIO_AFRL_AFSEL2_Pos) | (0x07 << GPIO_AFRL_AFSEL3_Pos);

    USART2->BRR = (SystemCoreClock + (baudrate / 2)) / baudrate;
    USART2->CR1 = USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
    USART2->CR1 |= USART_CR1_RXNEIE;
    USART2->CR1 |= USART_CR1_IDLEIE;

    NVIC_EnableIRQ(USART2_IRQn);
}

void UART2_SendChar(uint8_t ch) {
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = ch;
}

void USART2_Send(const char* str) {
    while (*str) {
        UART2_SendChar(*str++);
    }
}
void USART2_IRQHandler(void) {
    static uint16_t j = 0;

    if (USART2->SR & USART_SR_RXNE) {
        uint8_t data = USART2->DR;

        if (data != '\r' && data != '\n') {
            if (j < UART2_RX_BUFFER_SIZE - 1) {
                uart2_rx_buffer[j++] = data;
            }
        } else {

            uart2_rx_buffer[j] = '\0';


        }
    }

    if (USART2->SR & USART_SR_IDLE) {
        volatile uint32_t tmp;
        tmp = USART2->SR;
        tmp = USART2->DR;

        if (j > 0) {
            uart2_rx_buffer[j] = '\0';
            command_ready = 1;
            j = 0;
        }
    }
}

