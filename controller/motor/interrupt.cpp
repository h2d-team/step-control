#include "controller.h"

extern "C" void TIM2_IRQHandler(void)
{
    if(TIM2->SR & TIM_SR_UIF)       // if UIF flag is set
    {
        TIM2->SR &= ~TIM_SR_UIF;    // clear UIF flag
        core::Engine::instance().tick();
    }
}

extern "C" void SPI1_IRQHandler(void) //TODO: check inline for interrupt
{
    if ((SPI1->SR & SPI_SR_RXNE) == SPI_SR_RXNE)
    {
        motor::Controller::instance().spiReady();
    }
}

extern "C" void DMA1_Channel1_IRQHandler(void) //40kHz /*FIXME*/
{
    if( (DMA1->ISR & DMA_ISR_TCIF1) == DMA_ISR_TCIF1  )
    {
        DMA1->IFCR  |= DMA_IFCR_CTCIF1;
        motor::Controller::instance().adcReady();
    }
}

extern "C" void USART1_IRQHandler(void)
{
    if(USART1->SR & USART_SR_RXNE)
    {
        USART1->SR &= ~USART_SR_RXNE;
        uart::Controller::instance().nextReceiveEvent();
    }
}
