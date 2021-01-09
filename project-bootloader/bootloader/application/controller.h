#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "stm32f103xb.h"
#include "type.h"
#include "config.h"

#define DECODE_VAL ((int32_t)rxBuffer_[0] <<24)| ((int32_t)rxBuffer_[1] << 16)| ((int32_t)rxBuffer_[2] <<8) | (int32_t)rxBuffer_[3]


namespace application
{

class Controller
{
public:
    void init()
    {
        initSystem_();
    }
    void loop();
    /*FLASH*/
    int writeF_(uint32_t Address,void * values, uint16_t size);
    void readF_(uint32_t SectorStartAddress, void * values, uint16_t size);
    void eraseSector_(uint32_t SectorStartAddress);

private:
    /*init System clock*/
    void initSystem_();
    /*FLASH*/
    void initF_();
};

extern application::Controller CONTROLLER;

}

#endif // CONTROLLER_H

