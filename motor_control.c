#include <motor_control.h>
#include "uart.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "stm32f407xx.h"
extern uint16_t speed  ;
void PWM_Init(void) {
   //GPIOA and TIM1 clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

    //  Configure PA8, PA9, PA10, PA11 as AF for TIM1
    // Pins: PA8 = TIM1_CH1, PA9 = TIM1_CH2, PA10 = TIM1_CH3, PA11 = TIM1_CH4
    GPIOA->MODER &= ~((3 << (8 * 2)) | (3 << (9 * 2)) | (3 << (10 * 2)) | (3 << (11 * 2)));
    GPIOA->MODER |=  ((2 << (8 * 2)) | (2 << (9 * 2)) | (2 << (10 * 2)) | (2 << (11 * 2)));  // AF mode
    GPIOA->AFR[1] &= ~((0xF << ((8 - 8) * 4)) | (0xF << ((9 - 8) * 4)) | (0xF << ((10 - 8) * 4)) | (0xF << ((11 - 8) * 4)));
    GPIOA->AFR[1] |=  ((1 << ((8 - 8) * 4)) | (1 << ((9 - 8) * 4)) | (1 << ((10 - 8) * 4)) | (1 << ((11 - 8) * 4))); // AF1 = TIM1


    TIM1->PSC = 160 - 1;   // Prescaler
    TIM1->ARR = 200 - 1;   // Auto reload

    TIM1->CCMR1 = (6 << 4) | TIM_CCMR1_OC1PE | (6 << 12) | TIM_CCMR1_OC2PE;  // CH1, CH2 PWM mode 1
    TIM1->CCMR2 = (6 << 4) | TIM_CCMR2_OC3PE | (6 << 12) | TIM_CCMR2_OC4PE;  // CH3, CH4 PWM mode 1

    TIM1->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E;

    TIM1->BDTR |= TIM_BDTR_MOE;

    // 7. Enable timer
    TIM1->CR1 |= TIM_CR1_CEN;
}

void SetMotors(uint8_t motor, uint8_t dir, uint16_t speed) {
    if (speed > 200) speed = 200;

    if (motor == 0) {
        if (dir == 1) {
            TIM1->CCR1 = speed;    // Forward PWM on CH1
            TIM1->CCR2 = 0;        // Backward channel off
        } else {
            TIM1->CCR1 = 0;        // Forward off
            TIM1->CCR2 = speed;    // Backward PWM on CH2
        }
    } else if (motor == 1) {
        if (dir == 1) {
            TIM1->CCR3 = speed;    // Forward PWM on CH3
            TIM1->CCR4 = 0;        // Backward off
        } else {
            TIM1->CCR3 = 0;        // Forward off
            TIM1->CCR4 = speed;    // Backward PWM on CH4
        }
    }
}





void control_car(const char* cmd) {
    char msg[64];
    const uint16_t speed_step = 25;
    const uint16_t max_speed = 200;
    const uint16_t turn_speed = 75;

    if (strcmp(cmd, "F") == 0) {
        if (speed + speed_step > max_speed) speed = max_speed;
        else speed += speed_step;

        SetMotors(0, 1, speed);  // right motor forward
        SetMotors(1, 1, speed);  // left motor forward

        snprintf(msg, sizeof(msg), "Speed increased , new speed: %d\r\n", speed);
        USART2_Send(msg);

    } else if (strcmp(cmd, "S") == 0) {
        if (speed < speed_step) speed = 0;
        else speed -= speed_step;

        SetMotors(0, 1, speed);
        SetMotors(1, 1, speed);

        snprintf(msg, sizeof(msg), "Speed decreased, new speed: %d\r\n", speed);
        USART2_Send(msg);

    } else if (strcmp(cmd, "FFF") == 0) {
        speed = max_speed;

        SetMotors(0, 1, speed);
        SetMotors(1, 1, speed);

        snprintf(msg, sizeof(msg), "Full speed forward: %d\r\n", speed);
        USART2_Send(msg);

    } else if (strcmp(cmd, "X") == 0) {
        speed = 0;

        SetMotors(0, 1, 0);
        SetMotors(1, 1, 0);

        USART2_Send("Full stop\r\n");

    } else if (strcmp(cmd, "R") == 0) {
        // Turn right
        SetMotors(0, 1, 0);
        SetMotors(1, 1, turn_speed);

        snprintf(msg, sizeof(msg), "Turning right  %d\r\n", 1);
        USART2_Send(msg);

    } else if (strcmp(cmd, "L") == 0) {
        // Turn left
        SetMotors(1, 1, 0);
        SetMotors(0, 1, turn_speed);

        snprintf(msg, sizeof(msg), "Turning left %d\r\n", 1);
        USART2_Send(msg);

    }
}






