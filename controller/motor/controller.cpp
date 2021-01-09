#include "controller.h"
#include "type.h"

void motor::Controller::init()
{
    angleOffset_ = ANGLEOFFSET;

    for (int i =0;i< SPEED_W_SIZE; i++) vFilter_[i]=0;
    vFilterIndex_ = 0;

//    pKP_ = 400; pKI_ = 0; pKD_ = 0;
//    vKP_ = 100; vKI_ = 3;
//    KAff_ = 20; KVff_ = 20;
//    dKP_ = 200; dKI_ = 5;
//    qKP_ = 70; qKI_ = 0;
//    V_ = 200;
//    A_ = 3;

        pKP_ = 900; pKI_ = 0; pKD_ = 0;
        vKP_ = 90; vKI_ = 20;
        KAff_ = 20; KVff_ = 20;
        dKP_ = 800; dKI_ = 80;
        qKP_ = 70; qKI_ = 0;
        V_ = 200;
        A_ = 3;

    calibrate_ = core::Engine::instance().registerTask(&motor::Controller::readAngleOffsetEvent);
    readCurrentOffset_ = core::Engine::instance().registerTask(&motor::Controller::readCurrentOffsetEvent);
    spiNextEvent_ = &motor::Controller::regulateEvent;
    core::Engine::instance().registerTask(&motor::Controller::plotEvent)->start(10);//100Hz
    checkCurrentTask_ = core::Engine::instance().registerTask(&motor::Controller::checkOverCurrentEvent);

    U_COMMAND_REG(console::UpdateField, motor::Controller, updateField);
    U_COMMAND_REG(console::SyncFields, motor::Controller, syncFields);
    U_COMMAND_REG(console::CalibrateMotor, motor::Controller, calibrate);
    U_COMMAND_REG(console::StartMotor, motor::Controller, start);
    U_COMMAND_REG(console::StopMotor, motor::Controller, stop);
    U_COMMAND_REG(console::MoveMotor, motor::Controller, move);
}

U_COMMAND_HANDLER(motor::Controller, start)
{
    pSum_ = 0;
    vSum_ = 0;
    dSum_ = 0;
    qSum_ = 0;
    this->pRef_ = 0;
    this->p_ = 0;
    generator_.reset();
    //readCurrentOffset_->start(10);
    checkCurrentTask_->start(1);
    motorEnable_ = true;
}

U_COMMAND_HANDLER(motor::Controller, stop)
{
    motorEnable_ = false;
    uart::Controller::instance().print("Motor Stopped");
    checkCurrentTask_->stop();
}

U_COMMAND_HANDLER(motor::Controller, move)
{
    if (generator_.moving()) return;
    int32_t s = ((int32_t)data[0] << 24) | ((int32_t)data[1] << 16) | ((int32_t)data[2] << 8) | ((int32_t)data[3]);
    generator_.start(A_, V_, (s<<10));
    uart::Controller::instance().printf("Move to:%d", pRef_);
}

M_EVENT_HANDLER(motor::Controller,readPosition)
{
    core::Engine::instance().lock();
    CLEAR_SCS_AS5047;
    SPI1->DR = CMD_R_ANGLECOM;
}

M_EVENT_HANDLER(motor::Controller,readCurrentOffset)
{
    static uint16_t count = 1;
    static uint32_t iaSum = 0, ibSum = 0;
    if (motorEnable_) return;
    iaSum += ADC_BUFFER[0];
    ibSum += ADC_BUFFER[1];
    if(count++ == 100)
    {
        iaOffset_ = (iaSum/100);
        ibOffset_ = (ibSum/100);
        count = 1;
        iaSum = 0;
        ibSum = 0;
        motorEnable_ = true;
        readCurrentOffset_->stop();
        uart::Controller::instance().printf("Current Offset: ia=%d, ib=%d", iaOffset_, ibOffset_);
        uart::Controller::instance().print("Motor Started");
        checkCurrentTask_->start(1);
    }
}

#include <stdlib.h>
M_EVENT_HANDLER(motor::Controller,checkOverCurrent)
{
    if ((abs(ia_) > overCurrent_) || (abs(ib_) > overCurrent_))
    {
        motorEnable_ = false;
        uart::Controller::instance().print("Over current!");
    }
}
