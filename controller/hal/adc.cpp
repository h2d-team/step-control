#include "stm32f1xx.h"
#include "controller.h"

volatile uint16_t ADC_BUFFER[2];

void hal::Controller::initAdc_()
{
    /*********************************
    * ADC
    * ADC -> IA         (readADC[1])(PA0)
    * ADC -> IB         (readADC[2])(PA1)
    * Conversion starts when ADON bit is set for a second time by software after ADC power-up time (tSTAB)
    * Clock 12Mhz; Sample time 7.5 cycles; conversion time 1.17us
    * -> 1 channel = 1.795us -> 2 channel = 3.59us -> f = 279khz
    * ********************************
    */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;//en clock portA
    GPIOA->CRL &= ~(GPIO_CRL_MODE0_Msk | GPIO_CRL_MODE1_Msk);//00: Input mode
    GPIOA->CRL &= ~(GPIO_CRL_CNF0_Msk | GPIO_CRL_CNF1_Msk);//00: Analog mode

    /*ADC Clock selection 12Mhz*/
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    ADC1->CR2 |= ADC_CR2_ADON | // turn on ADC
                 //ADC_CR2_CONT | // enable continuos mode
                 ADC_CR2_DMA;   // enable DMA mode
    ADC1->CR1 |= ADC_CR1_SCAN;  // enable scan mode

    ADC1->SQR1 |= ADC_SQR1_L_0; // 2 channels in sequence (0001: 2 conversions)

    ADC1->SQR3 &= ~ADC_SQR3_SQ1;  // channel 0 is first in sequence
    ADC1->SQR3 |= ADC_SQR3_SQ2_0; // channel 1 is second in sequence

    ADC1->SMPR2 |= ADC_SMPR2_SMP0_0 | // sample time for first channel in sequence (001: 7.5 cycles)
                   ADC_SMPR2_SMP1_0 ; // sample time for second channel in sequence (001: 7.5 cycles)

    /*DMA circular mode*/
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    DMA1_Channel1->CPAR  = (uint32_t)(&(ADC1->DR)); // peripheral (source) address
    DMA1_Channel1->CMAR  = (uint32_t)ADC_BUFFER;  // memory (desination) address
    DMA1_Channel1->CNDTR = 2; // 2 transfers

    DMA1_Channel1->CCR |= DMA_CCR_CIRC |    // circular mode enable
                          DMA_CCR_MINC |    // memory increment mode enable
                          DMA_CCR_MSIZE_0 | // memory size 16 bits
                          DMA_CCR_PSIZE_0 | // peripheral size 16 bits
                          DMA_CCR_TCIE;     // 1: TC interrupt enabled

    DMA1_Channel1->CCR |= DMA_CCR_EN ; // Enable channel
    NVIC_EnableIRQ(DMA1_Channel1_IRQn);
    NVIC_SetPriority(DMA1_Channel1_IRQn,0);

    /*Triger from Timer*/
    ADC1->CR2 |= ADC_CR2_EXTTRIG; //1: Conversion on external event enable
    ADC1->CR2 |= ADC_CR2_EXTSEL_2;//100: Timer 3 TRGO event

    ADC1->CR2 |= ADC_CR2_ADON; // Turn on conversion

    /*Calibration module ADC*/
    ADC1->CR2 |= ADC_CR2_CAL;// Enable calibration
    while((ADC1->CR2 & ADC_CR2_CAL) == ADC_CR2_CAL){;}
}
