#include "stm32f1xx.h"
#include "controller.h"

/*
 * PWM = 40Khz
 * Resolution duty = 1200
 */
void hal::Controller::initPwm_()
{
    /*********************************
     * PA8  (PIN29) - IN1 - A+ (PWM TIMER1-CH1)
     * PA9  (PIN30) - IN2 - A- (PWM TIMER1-CH2)
     * PA10 (PIN31) - IN3 - B+ (PWM TIMER1-CH3)
     * PA11 (PIN32) - IN4 - B- (PWM TIMER1-CH4)
     * ********************************
    */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;//en clock portA
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;//en clock alternate

    AFIO->MAPR &= ~(AFIO_MAPR_TIM1_REMAP_Msk);//00: no remap
    GPIOA->CRH |= (GPIO_CRH_MODE8_Msk | GPIO_CRH_MODE9_Msk | GPIO_CRH_MODE10_Msk | GPIO_CRH_MODE11_Msk);//11: Output mode, max speed 50 MHz
    GPIOA->CRH &= ~(GPIO_CRH_CNF8_Msk | GPIO_CRH_CNF9_Msk | GPIO_CRH_CNF10_Msk | GPIO_CRH_CNF11_Msk);//clear
    GPIOA->CRH |= (GPIO_CRH_CNF8_1 | GPIO_CRH_CNF9_1 | GPIO_CRH_CNF10_1 | GPIO_CRH_CNF11_1);//Alternate

    /*begin init timer3*/
    RCC     ->APB1ENR   |= RCC_APB1ENR_TIM3EN;
    TIM3    ->PSC       = 0;
    TIM3    ->ARR       = 1199;
    TIM3    ->DIER      = TIM_DIER_UIE; // Enable update interrupt (timer level)
    TIM3    ->CR1       = TIM_CR1_CEN;  // Enable timer

    /*Triger adc*/
    TIM3    ->CCMR2     |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_0 | TIM_CCMR2_OC4PE;/* set pwm mode 2 (OCxM = 111) and enable preload (OCxPE = 1) channel 4 */
    TIM3    ->CCMR2     &= ~TIM_CCMR2_OC4FE;/*fast mode disable*/
    TIM3    ->CCR4      = 10;/*pulse*/
    TIM3    ->CCER      &= ~TIM_CCER_CC4E;/*CH polarity*/
    TIM3    ->CR2       &= ~TIM_CR2_OIS4;/*CH idele state LOW*/
    TIM3    ->CR2       |= TIM_CR2_MMS;/*111: Compare - OC4REF signal is used as trigger output (TRGO)*/

    /*begin init timer1*/
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    TIM1 ->PSC = 0;
    TIM1 ->ARR = 1199;
    TIM1 ->DIER = TIM_DIER_UIE; // Enable update interrupt (timer level)
    TIM1 ->CR1 = TIM_CR1_CEN;  // Enable timer

    /* set pwm mode 1 (OCxM = 110) and enable preload (OCxPE = 1) for output/compare channel 1 and channel 2 */
    TIM1    ->CCMR1     |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE;

    /* set pwm mode 1 (OCxM = 110) and enable preload (OCxPE = 1) for output/compare channel 3 and channel 4 */
    TIM1    ->CCMR2     |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC3PE | TIM_CCMR2_OC4PE;

    /* Enable channel outputs */
    TIM1    ->CCER      |= TIM_CCER_CC1E;
    TIM1    ->CCER      |= TIM_CCER_CC2E;
    TIM1    ->CCER      |= TIM_CCER_CC3E;
    TIM1    ->CCER      |= TIM_CCER_CC4E;

    /* output idle state HIGHT */
    TIM1    ->CR2       |= TIM_CR2_OIS1;
    TIM1    ->CR2       |= TIM_CR2_OIS2;
    TIM1    ->CR2       |= TIM_CR2_OIS3;
    TIM1    ->CR2       |= TIM_CR2_OIS4;

    /*dead time 0-255*/
    /*
     *DTG[7:5]=0xx => DT=DTG[7:0]x tdtg with tdtg=tDTS.
     *DTG[7:5]=10x => DT=(64+DTG[5:0])xtdtg with Tdtg=2xtDTS.
     *DTG[7:5]=110 => DT=(32+DTG[4:0])xtdtg with Tdtg=8xtDTS.
     *DTG[7:5]=111 => DT=(32+DTG[4:0])xtdtg with Tdtg=16xtDTS.
     *Example if TDTS=125 ns (8 MHz), dead-time possible values are:
     *0 to 15875 ns by 125 ns steps,
     *16 us to 31750 ns by 250 ns steps,
     *32 us to 63 us by 1 us steps,
     *64 us to 126 us by 2 us steps
    */
    TIM1    ->BDTR      |= 1;

    /* (6) Enable output (MOE = 1)*/
    /* (7) Enable counter (CEN = 1) select Edge-aligned (CMS = 00) */
    /* (8) Force update generation (UG = 1) */
    TIM1    ->BDTR      |= TIM_BDTR_MOE; /* (6) */
    TIM1    ->CR1       |= TIM_CR1_CEN; /* (7) */
    TIM1    ->EGR       |= TIM_EGR_UG; /* (8) */
    /*end init timer1*/

    //setDuties_(500,600,300,400);
}
