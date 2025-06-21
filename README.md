# STM32 Robot – UART, PWM, and ADC (Bare-Metal)

This is a bare-metal STM32F407 robot control project. The robot is driven by an L298N motor driver and controlled over UART using an HC-05 Bluetooth module. Code is fully modular, with clean separation between UART, PWM, ADC, and control logic.

## 🔧 Features

- ✅ **UART Control** via Bluetooth (HC-05)
- ✅ **Motor Control** using PWM (Timer1, 4 channels)
- ✅ **L298N** control with only IN1-IN4 (no ENA/ENB)
- ✅ **Command-based driving** (e.g., 'F', 'S', 'R', 'L', etc.)
- ✅ **ADC Input** from potentiometer using DMA
- ✅ **Feedback messages** sent over UART
- ✅ Fully **bare-metal**, no HAL, CMSIS only
- ✅All logic is interrupt-driven

## 📁 Code Structure

/Src
├── main.c # Main loop and command parsing
├── uart.c # UART initialization and interrupt handler
├── pwm.c # PWM setup and motor control
├── adc.c # ADC + DMA configuration
/Inc
├── uart.h
├── pwm.h
├── adc.h


