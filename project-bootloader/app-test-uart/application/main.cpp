#include "stm32f103xb.h"
#include "uart/controller.h"

void initSystem_();
int main()
{
    initSystem_();
    uart::CONTROLLER.init();

    uart::CONTROLLER.printMessage("Chao ong chu");
    uint32_t loopIndex = 0, index = 0;
    while (true)
    {
        uart::CONTROLLER.loop();
        if(++loopIndex % 0xFFFFF == 0)
            uart::CONTROLLER.printfMessage("Test uart %d",index++);

    }
    return 0;
}

void initSystem_()
{
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
}
