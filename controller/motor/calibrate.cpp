#include "controller.h"

U_COMMAND_HANDLER(motor::Controller, calibrate)
{
    uint16_t count = 1;
    uint32_t iaSum = 0, ibSum = 0;
    SET_DUTY(0,0,0,0);
    for(count=0; count < 100;count++)
    {
        iaSum += ADC_BUFFER[0];
        ibSum += ADC_BUFFER[1];
    }

    iaOffset_ = (iaSum/100);
    ibOffset_ = (ibSum/100);
    uart::Controller::instance().printf("Current Offset: ia=%d, ib=%d", iaOffset_, ibOffset_);

    if (generator_.moving()) return;
    spiNextEvent_ = nullptr;
    SET_DUTY(ALIGN_DUTY,0,0,0);
    calibrate_->start(3000,1); //wait for 3s
}

M_EVENT_HANDLER(motor::Controller,readAngleOffset)
{
    spiNextEvent_ = &motor::Controller::endCalibrateEvent;
    M_EVENT_POST(motor::Controller, readPosition);
}

M_EVENT_HANDLER(motor::Controller,endCalibrate)
{
    angleOffset_ = (spiData_ & 0x3FFF);
    SET_DUTY(0,0,0,0);
    motorEnable_ = false;
    spiNextEvent_ = &motor::Controller::regulateEvent;
    uart::Controller::instance().printf("Calibrate DONE, angle offset:%d", (spiData_ & 0x3FFF));
    uart::Controller::instance().printf("minA=%d, maxA=%d, iaOffset=%d", minA_, maxA_, (minA_+maxA_)/2);
    uart::Controller::instance().printf("minB=%d, maxB=%d, ibOffset=%d", minB_, maxB_, (minB_+maxB_)/2);
    uart::Controller::instance().printf("B/A=%d", (maxB_ - minB_)*1000/(maxA_ - minA_));
}
