#ifndef STM32F1_HAL_H
#define STM32F1_HAL_H
#include <stdint.h>

extern volatile uint16_t ADC_BUFFER[2];

/*UART*/
#define UART_TX_READY   (USART1->SR & USART_SR_TXE)
#define UART_RX_READY   (USART1->SR & USART_SR_RXNE)
#define UART_READ       (uint8_t)USART1->DR
#define UART_WRITE(c)   USART1->DR = c

/*PWM*/
#define SET_DUTY(ah, al, bh, bl)\
{\
    TIM1->CCR1 = ah;\
    TIM1->CCR2 = al;\
    TIM1->CCR3 = bh;\
    TIM1->CCR4 = bl;\
}\

/*SPI*/
#define CLEAR_SCS_AS5047    (GPIOA->BSRR |= GPIO_BSRR_BR4)
#define SET_SCS_AS5047      (GPIOA->BSRR |= GPIO_BSRR_BS4)

#define OP_WRITE 0x0000
#define OP_READ  0x4000

#define PARITY_EVEN     0x0000
#define PARITY_ODD      0x8000

#define ADDR_NOP          0x0000
#define ADDR_ERRFL        0x0001
#define ADDR_PROG         0x0003
#define ADDR_DIAAGC       0x3FFC
#define ADDR_MAG          0x3FFD
#define ADDR_ANGLEUNC     0x3FFE
#define ADDR_ANGLECOM     0x3FFF
#define ADDR_SETTINGS1    0x0018
#define ADDR_SETTINGS2    0x0019

#define CMD_R_NOP       (PARITY_ODD  | OP_READ | ADDR_NOP)
#define CMD_R_ERRFL     (PARITY_EVEN | OP_READ | ADDR_ERRFL)
#define CMD_R_PROG      (PARITY_ODD  | OP_READ | ADDR_PROG)
#define CMD_R_DIAAGC    (PARITY_ODD  | OP_READ | ADDR_DIAAGC)
#define CMD_R_MAG       (PARITY_EVEN | OP_READ | ADDR_MAG)
#define CMD_R_ANGLEUNC  (PARITY_EVEN | OP_READ | ADDR_ANGLEUNC)
#define CMD_R_ANGLECOM  (PARITY_ODD  | OP_READ | ADDR_ANGLECOM)
#define CMD_R_SETTINGS1 (PARITY_ODD  | OP_READ | ADDR_SETTINGS1)
#define CMD_R_SETTINGS2 (PARITY_EVEN | OP_READ | ADDR_SETTINGS2)


#endif // UART_H
