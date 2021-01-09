#include "stm32f1xx.h"
#include "controller.h"

void hal::Controller::initTick_()
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->PSC = 1000;
    TIM2->ARR = 47;               // 1khz
    TIM2->DIER = TIM_DIER_UIE;      // Enable update interrupt (timer level)
    TIM2->CR1 = TIM_CR1_CEN;        // Enable timer
    NVIC_EnableIRQ(TIM2_IRQn);      // Enable interrupt from TIM2 (NVIC level)
}
