#include "controller.h"


void application::Controller::initSystem_()
{
#if 1//Clock ben trong max speed 48Mhz system
    /*RCC->CR*/
    RCC->CR |= RCC_CR_HSION;
    while(!(RCC->CR & RCC_CR_HSIRDY)){;}        // check flag HSIRDY = 1

    /*RCC->CFGR*/
    RCC->CFGR &= ~RCC_CFGR_PLLSRC;              // 0: HSI oscillator clock / 2 selected as PLL input clock
    RCC->CFGR |= RCC_CFGR_PLLMULL12;            // Nhan tan trong PLL
    RCC->CFGR |= RCC_CFGR_SW_PLL;               // Clock PLL lam dau vao cho Clock system mux
    while ((RCC->CR & RCC_CFGR_SWS_PLL)){;}

    RCC->CFGR &= ~RCC_CFGR_HPRE;                // He so chia AHB prescaler = 0 la chia 1
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;           // 100: HCLK divided by 2
    RCC->CFGR &= ~RCC_CFGR_PPRE2;               // 0xx: HCLK not divided

    /*ADC prescaler clock 12Mhz*/
    RCC->CFGR |= RCC_CFGR_ADCPRE_0;             // 01: PCLK2 divided by 4

    RCC->CR |= RCC_CR_PLLON;                    // 1: PLL ON
    while(!(RCC->CR & RCC_CR_PLLRDY)){;}        // check flag PLLRDY = 1
#endif

#if 0//Clock ben trong max speed 36Mhz system
    //RCC->CR
    RCC->CR |= RCC_CR_HSION;
    while(!(RCC->CR & RCC_CR_HSIRDY)){;}// check flag HSIRDY = 1

    RCC->CR |= RCC_CR_PLLON;
    while(!(RCC->CR & RCC_CR_PLLRDY)){;}// check flag PLLRDY = 1

    //RCC->CFGR
    RCC->CR &= ~RCC_CR_PLLON;
    while(RCC->CR & RCC_CR_PLLRDY){;}// check flag PLLRDY = 0

    RCC->CFGR &= ~RCC_CFGR_PLLSRC;//0: HSI oscillator clock / 2 selected as PLL input clock
    RCC->CFGR |= RCC_CFGR_PLLMULL9;//nhan tan trong PLL
    RCC->CFGR |= RCC_CFGR_SW_PLL;// clock PLL lam dau vao cho Clock system mux
    while ((RCC->CR & RCC_CFGR_SWS_PLL)){;}
    //he so chia AHB prescaler = 0 la chia 1

    RCC->CR |= RCC_CR_PLLON;
    while(!(RCC->CR & RCC_CR_PLLRDY)){;}// check flag PLLRDY = 1
#endif

#if 0//Clock ben ngoai max speed 72Mhz system
    /*RCC->CR*/
    RCC->CR |= RCC_CR_HSEON;
    while(!(RCC->CR & RCC_CR_HSERDY)){;}// check flag HSIRDY = 1

    /* enable flash prefetch buffer */
    FLASH->ACR |= FLASH_ACR_LATENCY_1;
    FLASH->ACR |= FLASH_ACR_PRFTBE;

    RCC->CR |= RCC_CR_PLLON;
    while(!(RCC->CR & RCC_CR_PLLRDY)){;}// check flag PLLRDY = 1

    /*RCC->CFGR*/
    RCC->CR &= ~RCC_CR_PLLON;
    while(RCC->CR & RCC_CR_PLLRDY){;}// check flag PLLRDY = 0

    RCC->CFGR |= RCC_CFGR_PLLSRC;//1: Clock from PREDIV1 selected as PLL input clock
    RCC->CFGR |= RCC_CFGR_PLLMULL9;//nhan tan trong PLL
    RCC->CFGR |= RCC_CFGR_SW_PLL;// clock PLL lam dau vao cho Clock system mux
    while ((RCC->CR & RCC_CFGR_SWS_PLL)){;}
    //he so chia AHB prescaler = 0 la chia 1
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;//100: HCLK divided by 2

    /*ADC prescaler clock 12Mhz*/
    RCC->CFGR |= RCC_CFGR_ADCPRE_1;//10: PCLK2 divided by 6

    RCC->CR |= RCC_CR_PLLON;
    while(!(RCC->CR & RCC_CR_PLLRDY)){;}// check flag PLLRDY = 1
#endif
}
