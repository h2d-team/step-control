#include "controller.h"
#include "uart/controller.h"

void application::Controller::initF_()
{
    FLASH->ACR &= ~FLASH_ACR_LATENCY;   //clear
    FLASH->ACR |= FLASH_ACR_LATENCY_0;  //001 One wait state, if 24 MHz < SYSCLK ≤ 48 MHz
}

/*
output: +) 0 Write flash success
        +) -1 Flash not erased to begin
        +) -2 Write protect error
        +) other Write flash error
*/
int application::Controller::writeF_(uint32_t Address,void * values, uint16_t size)
{
    uint16_t *AddressPtr;
    uint16_t *valuePtr;
    AddressPtr = (uint16_t *)Address;
    valuePtr = (uint16_t *)values;
    size = size / 2;  // incoming value is expressed in bytes, not 16 bit words
    while(size)
    {
        // unlock the flash
        // Key 1 : 0x45670123
        // Key 2 : 0xCDEF89AB
        FLASH->KEYR = FLASH_KEY1;
        FLASH->KEYR = FLASH_KEY2;
        FLASH->CR &= ~FLASH_CR_PER;// ensure PER is low
        FLASH->CR |= FLASH_CR_PG;// set the PG bit
        *(AddressPtr) = *(valuePtr);
        while(FLASH->SR & FLASH_SR_BSY); // wait while busy
        if (FLASH->SR & FLASH_SR_PGERR)
            return -1; // flash not erased to begin with
        if (FLASH->SR & FLASH_SR_WRPRTERR)
            return -2; // write protect error
        AddressPtr++;
        valuePtr++;
        size--;
    }
    return 0;
}

void application::Controller::readF_(uint32_t SectorStartAddress, void *values, uint16_t size)
{
    uint16_t *AddressPtr;
    uint16_t *valuePtr;
    AddressPtr = (uint16_t *)SectorStartAddress;
    valuePtr = (uint16_t *)values;
    size = size/2; // incoming value is expressed in bytes, not 16 bit words
    while(size)
    {
        *((uint16_t *)valuePtr) = *((uint16_t *)AddressPtr);
        valuePtr++;
        AddressPtr++;
        size--;
    }
}

void application::Controller::eraseSector_(uint32_t SectorStartAddress)
{
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;
    FLASH->CR &= ~FLASH_CR_PG;  // Ensure PG bit is low
    FLASH->CR |= FLASH_CR_PER; // set the PER bit
    FLASH->AR = SectorStartAddress;
    FLASH->CR |= FLASH_CR_STRT; // set the start bit
    while(FLASH->SR & FLASH_SR_BSY); // wait while busy
}
