#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include "uart/controller.h"
#include "core/component.h"
#include "stm32f103xb.h"
#include "generator.h"
#include "define.h"

COMPONENT(motor, Controller)
    /*console*/
    U_COMMAND(updateField);
    U_COMMAND(syncFields);
    M_EVENT(plot);

    /*calibrate*/
    U_COMMAND(calibrate);
    M_EVENT(readAngleOffset);
    M_EVENT(endCalibrate)

    /*motor*/
    U_COMMAND(start);
    M_EVENT(readCurrentOffset);
    M_EVENT(readPosition);
    U_COMMAND(stop);
    U_COMMAND(move);
    M_EVENT(regulate);
    M_EVENT(checkOverCurrent);

public:
    void init();
    inline void spiReady();
    inline void adcReady();

private:
    void fieldUpdated_(uint16_t type, int32_t value);
    core::Task* calibrate_;
    core::Task* readCurrentOffset_;
    core::Task* checkCurrentTask_;
    bool motorEnable_ = false;
    volatile core::Event spiNextEvent_ = nullptr;
    volatile uint16_t spiData_;
    uint16_t cycleCount_ = 0;

    /* Position Control */
    void updatePosition_(int32_t p);
    Generator generator_;
    int32_t p_, pRef_;
    int32_t v_, vRef_, aRef_;
    int16_t vFilter_[SPEED_W_SIZE];
    uint16_t vFilterIndex_ = 0;

    uint16_t pKP_ = 0, pKI_= 0, pKD_= 0, pKV_= 0;
    uint16_t vKP_ = 0, vKI_= 0, KAff_= 0, KVff_= 0;
    uint32_t V_, A_;
    int64_t pSum_ = 0, vSum_ = 0;
    int32_t lastE_;
    uint16_t angle_;
    uint16_t angleOffset_;

    /* Current Control */
    volatile int32_t ia_;
    volatile int32_t ib_;
    volatile int32_t iaFilter_ = 0;
    volatile int32_t ibFilter_ = 0;
    volatile uint32_t iaOffset_ = 2119;
    volatile uint32_t ibOffset_ = 2189;

    uint16_t overCurrent_ = 5000;
    int32_t iqRef_;
    uint16_t dKP_=0, dKI_=0, qKP_=0, qKI_=0;
    int64_t dSum_ = 0, qSum_ = 0;

    int32_t minA_=iaOffset_;int32_t minpA_;
    int32_t maxA_=iaOffset_;int32_t maxpA_;
    int32_t minB_ = ibOffset_;int32_t minpB_;
    int32_t maxB_ = ibOffset_;int32_t maxpB_;

     COMPONENT_END

/*inlines*/
#include "controller_.h"
#include "regulator.h"

#endif // CONTROLLER_H

