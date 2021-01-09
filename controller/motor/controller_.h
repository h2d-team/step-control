#ifndef MOTOR_CONTROLLER__H
#define MOTOR_CONTROLLER__H

#define FILTER_SHIFT    3
#include "hal/hal.h"
#include "controller.h"
#include "uart/controller.h"

inline void motor::Controller::adcReady()
{
    iaFilter_ = iaFilter_ - (iaFilter_ >> FILTER_SHIFT) + (ADC_BUFFER[0] - iaOffset_);
    ibFilter_ = ibFilter_ - (ibFilter_ >> FILTER_SHIFT) + (ADC_BUFFER[1] - ibOffset_);

    static int count = 0;
    if (((++count % 8) == 0)) //40kHz:8 = 5kHz
    {
        ia_ = -iaFilter_*2; //Q12.4
        ib_ = ibFilter_; //Q12.4

        if (spiNextEvent_ == &motor::Controller::regulateEvent)
        {
            M_EVENT_POST(motor::Controller, readPosition);
        }
        count = 0;
    }
}

inline void motor::Controller::spiReady()
{
    SET_SCS_AS5047;
    spiData_ = (SPI1->DR  & 0x3FFF);
    core::Engine::instance().post(spiNextEvent_);
    core::Engine::instance().unlock();
}

#endif // CONTROLLER__H
