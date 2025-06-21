#include "adc.h"
#include "stm32f407xx.h"

volatile uint16_t ADC_Value;

void ADC1_Init(void) {
    //  Enable clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_DMA2EN;
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    //  Configure PA0 as analog
    GPIOA->MODER |= GPIO_MODER_MODER0;
    GPIOA->PUPDR &= ~(3 << (0 * 2));


    DMA2_Stream0->CR &= ~DMA_SxCR_EN;

    while (DMA2_Stream0->CR & DMA_SxCR_EN); // Wait until it's disabled

    DMA2_Stream0->PAR = (uint32_t)&ADC1->DR;       // Peripheral address
    DMA2_Stream0->M0AR = (uint32_t)&ADC_Value;     // Memory address
    DMA2_Stream0->NDTR = 1;

    DMA2_Stream0->CR = (0 << DMA_SxCR_CHSEL_Pos) | // Channel 0
                       DMA_SxCR_CIRC |
                       DMA_SxCR_MINC |
                       DMA_SxCR_PSIZE_0 |          // Peripheral size 16 bits
                       DMA_SxCR_MSIZE_0 |          // Memory size 16 bits
                       DMA_SxCR_PL_1;

    DMA2_Stream0->CR |= DMA_SxCR_EN;               //  enable DMA

    //  Configure ADC1
    ADC1->SQR3 = 0;                 // Channel 0
    ADC1->SMPR2 |= (7 << 0);        // Sampling time 480 cycles
    ADC1->CR2 = ADC_CR2_DMA |       // DMA mode
                ADC_CR2_DDS |       // DMA in continuous mode
                ADC_CR2_CONT |      // Continuous conversion
                ADC_CR2_ADON;       // Enable ADC


    ADC1->CR2 |= ADC_CR2_SWSTART;
}
