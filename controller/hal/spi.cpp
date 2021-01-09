#include "stm32f1xx.h"
#include "controller.h"

void hal::Controller::initSpi_()
{
    /*********************************
     * SPI
     * PA5 -> SPI_CLK
     * PA6 -> SPI_MISO
     * PA7 -> SPI_MOSI
     * PA4 -> SCS Serial chip select. A logic low on this pin enables serial interface communication.
     **********************************/

    /*Init GPIO*/
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; // Enable clock portA
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; // Enable clock alternate
    AFIO->MAPR &= ~AFIO_MAPR_SPI1_REMAP;// 0: no remap (NSS/PA4, SCK/PA5, MISO/PA6, MOSI/PA7)
    GPIOA->CRL |= GPIO_CRL_MODE5 |      // 11: Output mode, max speed 50 MHz
                  GPIO_CRL_MODE4 |      // 11: Output mode, max speed 50 MHz
                  GPIO_CRL_MODE7;       // 11: Output mode, max speed 50 MHz
    GPIOA->CRL &= ~(GPIO_CRL_CNF5 |     // Clear
                    GPIO_CRL_CNF7);     // Clear
    GPIOA->CRL |= GPIO_CRL_CNF5_1 |     // 10: Alternate function output Push-pull
                  GPIO_CRL_CNF7_1;      // 10: Alternate function output Push-pull
    GPIOA->CRL &= ~GPIO_CRL_CNF4;       // 00: General purpose output push-pull
    GPIOA->CRL &= ~GPIO_CRL_MODE6;      // 00: Input mode (reset state)
    GPIOA->CRL |= GPIO_CRL_CNF6_0;      // 01: Floating input (reset state)

    /*Init SPI*/
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; // Enable clock module SPI1
    SPI1->CR1 |= SPI_CR1_MSTR |         // 1: Master configuration
                 SPI_CR1_DFF |          // 1: 16-bit data frame format is selected for transmission/reception
                 SPI_CR1_CPHA |         // 1: The second clock transition is the first data capture edge
                 SPI_CR1_BR_2 |         // 100: fPCLK/32 (48Mhz/32 = 1.5Mhz)
                 SPI_CR1_SSM ;          // 1: Software slave management enabled
    SPI1->CR1 &= ~SPI_CR1_CPOL;         // 0: CK to 0 when idle
    SPI1->CR2 |= SPI_CR2_RXNEIE |       // 1: RXNE interrupt not masked. Used to generate an interrupt request when the RXNE flag is set
                 SPI_CR2_SSOE;          // 1: SS output is enabled in master mode and when the cell is enabled. The cell cannot work in a multimaster environment

    SPI1->CR1 |= SPI_CR1_SPE ;          // 1: Peripheral enabled

    NVIC_EnableIRQ(SPI1_IRQn);          // Enable interrupt from SPI1 (NVIC level)
}
