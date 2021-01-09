#include "controller.h"

void hal::Controller::initDrv8841_()
{
    /*********************************
     * PA15 - BI1       (Sets bridge B current: 00 = 100%, 01 = 71%, 10 = 38%, 11 = 0 Internal pulldown.)
     * PB3  - BI0
     * PB4  - AI1       (Sets bridge A current: 00 = 100%, 01 = 71%, 10 = 38%, 11 = 0 Internal pulldown.)
     * PB5  - AI0
     * PB15 - DECAY     (Low = slow decay, open = mix decay, high = fast decay. Internal pulldown and pullup)
     * PB13 - nSLEEP    (Logic high to enable device, logic low to enter low-power sleep mode. Internal pulldown)
     * PB12 - nRESET    (Active-low reset input initializes internal logic and disables the H-bridge outputs. Internal pulldown)
     *
     * PB14 - nFAULT (Logic low when in fault condition nFAULT (overtemperature, overcurrent))
     * ********************************
    */

    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;     // Enable clock portA
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;     // Enable clock portB
    GPIOB->CRH |= GPIO_CRH_MODE12 |
                  GPIO_CRH_MODE13 |
                  GPIO_CRH_MODE15;          // 11: Output mode, max speed 50 MHz
    GPIOB->CRL |= GPIO_CRL_MODE3 |
                  GPIO_CRL_MODE4 |
                  GPIO_CRL_MODE5;           // 11: Output mode, max speed 50 MHz
    GPIOB->CRH &= ~(GPIO_CRH_CNF12 |
                    GPIO_CRH_CNF13 |
                    GPIO_CRH_CNF15);        // 00: General purpose output push-pull
    GPIOB->CRL &= ~(GPIO_CRL_CNF3 |
                    GPIO_CRL_CNF4 |
                    GPIO_CRL_CNF5);         // 00: General purpose output push-pull
    GPIOA->CRH |= GPIO_CRH_MODE15;          // 11: Output mode, max speed 50 MHz
    GPIOA->CRH &= ~GPIO_CRH_CNF15;          // 00: General purpose output push-pull

    GPIOB->CRH &= ~GPIO_CRH_MODE14;         // 00: Input mode (reset state)
    GPIOB->CRH &= ~GPIO_CRH_CNF14;          // Clear
    GPIOB->CRH |= GPIO_CRH_CNF14_1;         // 10: input with pull-up/pull-down
    //GPIOB->ODR &= ~GPIO_ODR_ODR14;          // 0: pull-down
    GPIOB->ODR |= GPIO_ODR_ODR14;           // 1: pull-up

    enDriver_();
    currentSetDriver_();
    decaySetDriver_();
}

void hal::Controller::enDriver_()
{
    /*On pin PB13, PB12*/
    GPIOB->BSRR |= GPIO_BSRR_BS13;
    GPIOB->BSRR |= GPIO_BSRR_BS12;
}

void hal::Controller::sleepDriver_()
{
    /*Off pin PB13*/
    GPIOB->BSRR |= GPIO_BSRR_BR13;
}

void hal::Controller::resetDriver_()
{
    /*Off pin PB12*/
    GPIOB->BSRR |= GPIO_BSRR_BR12;
}

void hal::Controller::currentSetDriver_()
{
    /*10 = 38%*/
//    GPIOB->BSRR |= GPIO_BSRR_BS5 |      // 1
//                   GPIO_BSRR_BS3;       // 1
//    GPIOB->BSRR |= GPIO_BSRR_BR4;       // 0
//    GPIOA->BSRR |= GPIO_BSRR_BR15;      // 0
    /*01 = 71%*/
//    GPIOB->BSRR |= GPIO_BSRR_BR5 |      // 0
//                   GPIO_BSRR_BR3;       // 0
//    GPIOB->BSRR |= GPIO_BSRR_BS4;       // 1
//    GPIOA->BSRR |= GPIO_BSRR_BS15;      // 1

    /*00 = 100%*/
    GPIOB->BSRR |= GPIO_BSRR_BR5 |      // 0
                   GPIO_BSRR_BR3;       // 0
    GPIOB->BSRR |= GPIO_BSRR_BR4;       // 0
    GPIOA->BSRR |= GPIO_BSRR_BR15;      // 0
}

void hal::Controller::decaySetDriver_()
{
    /*Slow decay PB15 = 0*/
    GPIOB->BSRR |= GPIO_BSRR_BR15;

    /*Fast decay PB15 = 1*/
    //GPIOB->BSRR |= GPIO_BSRR_BS15;
}

bool hal::Controller::isFaultDriver_()
{
    if((GPIOB->IDR & GPIO_IDR_IDR14) == GPIO_IDR_IDR14) {return false;}
    else {return true;}
}
