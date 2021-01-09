#include "stm32f103xb.h"
#include "controller.h"

void uart::Controller::initUART_()
{
    // GPIOB Periph clock enable
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;


    //USART1 GPIO Configuration
    //PB6      ------> USART1_TX
    //PB7      ------> USART1_RX

    // Set alternate
    AFIO->MAPR |= AFIO_MAPR_USART1_REMAP;       // remap usart to PB6, PB7

    GPIOB->CRL &= ~(GPIO_CRL_CNF6_Msk);         // clear
    GPIOB->CRL |= (GPIO_CRL_CNF6_1);            // 10: Alternate function output Push-pull
    GPIOB->CRL |= (GPIO_CRL_MODE6_Msk);         // 11:output max speed(Tx)

    GPIOB->CRL &= ~GPIO_CRL_MODE7_Msk;          // 00: Input mode (Rx)
    GPIOB->CRL |= GPIO_CRL_CNF7_0;              // 01: Floating input

    // USART1 clock enable
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    // 115200 Bd @ 48 MHz
    // USARTDIV = 48 MHz / 115200 = 416 = 0x01A0
    // BRR[15:4] = USARTDIV[15:4]
    // When OVER8 = 0, BRR [3:0] = USARTDIV [3:0]
    // USART1->BRR = (uint16_t)(0x01A0);

    //USART1->BRR = (uint16_t)(0x0270);//72 115200
    //USART1->BRR = (uint16_t)(0x0230);//64 115200
    //USART1->BRR = (uint16_t)(0x1380);//48 9600
    //USART1->BRR = (uint16_t)(0x340);//8 9600
    //USART1->BRR = (uint16_t)(0x680);//16 9600
    //USART1->BRR = (uint16_t)(0x787);//36 115200
    //USART1->BRR = (uint16_t)(0xEA0);//36 9600
    //USART1->BRR = (uint16_t)(0x1380);//48 9600
    USART1->BRR = (uint16_t)(0x01A0);//48 115200

    // USART enable
    // Receiver enable
    // Transmitter enable
    USART1->CR1 = (uint32_t)(USART_CR1_UE | USART_CR1_RE | USART_CR1_TE);

    USART1->CR1 |= USART_CR1_RXNEIE; //enable interupt rx

    NVIC_EnableIRQ(USART1_IRQn);
}

extern "C" void USART1_IRQHandler(void)
{

    //check receive
    if(USART1->SR & USART_SR_RXNE)
    {
        //clear USART_SR_RXNE with read data or write 0 USART_SR_RXNE
        uart::CONTROLLER.interupt_update_rx();
        USART1->SR &= ~USART_SR_RXNE;
    }


}
void uart::Controller::interupt_update_rx()
{
      uint8_t c =  read_();
        switch (rxState_)
        {
        case HEADER:
            if (c == HEADER_INDICATOR)
            {
                rxState_ = LENGTH;
                checkSum_ = c;
            }
            break;
        case LENGTH:
            if (c > MAX_PACKET_LENGTH) rxState_ = HEADER;
            else
            {
                checkSum_ += c;
                rxLength_ = c;
                rxIndex_ = 0;
                rxState_ = TYPE1;
            }
            break;
        case TYPE1:
            checkSum_ += c;
            rxType_ = c;
            rxState_ = TYPE2;
            break;
        case TYPE2:
            checkSum_ += c;
            rxType_ = (rxType_<<8) + c;
            if (rxLength_>0) rxState_ = DATA;
            else rxState_ = CrC;
            break;
        case DATA:
            checkSum_ += c;
            rxBuffer_[rxIndex_] = c;
            rxIndex_++;
            if (rxIndex_ == rxLength_) rxState_ = CrC;
            break;
        case CrC:
            if (c == checkSum_) rxState_ = FOOTER;
            else rxState_ = HEADER;
            break;
        case FOOTER:
            if (c == FOOTER_INDICATOR) this->processCommand_();
            //printfMessage("Packet received type:%d, length:%d",rxType_, rxLength_);
            rxState_ = HEADER;
            break;
        default:
            rxState_ = HEADER;
            break;
        }

}
bool uart::Controller::rxReady_()
{
    return (USART1->SR & USART_SR_RXNE);
}

bool uart::Controller::txReady_()
{
    return (USART1->SR & USART_SR_TXE) ;
}

uint8_t uart::Controller::read_()
{
    return (uint8_t)USART1->DR;
}

void uart::Controller::write_(uint8_t c)
{
    USART1->DR = c;
}
