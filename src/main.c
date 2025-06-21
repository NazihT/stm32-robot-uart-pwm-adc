#include <motor_control.h>
#include "stm32f407xx.h"
#include "uart.h"
#include <stdio.h>
#include <string.h>
#include "adc.h"
 uint16_t speed;
void shittydelay()        // will implement a timer interrupt instead of this soon 
{
	for (volatile int i=0;i<1000000;i++)
	{

	}
}
volatile uint8_t command_ready = 0;
int main()
{


        UART2_Init(9600);
        PWM_Init();
        ADC1_Init();
        while (1) {

        	if (command_ready) {
        	        control_car((char*)uart2_rx_buffer);
        	        uart2_rx_buffer[0] = '\0';  
        	        command_ready = 0;          
        	    }
        	 char adc_value[20];
                 snprintf(adc_value, sizeof(adc_value), "ADC: %u\r\n", ADC_Value);
        	 USART2_Send(adc_value);
		 shittydelay();       
		





	
}
}


