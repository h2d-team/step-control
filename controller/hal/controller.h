#ifndef STM32F1_CONTROLLER_H_
#define STM32F1_CONTROLLER_H_
#include "core/engine.h"

COMPONENT(hal, Controller)
public:
    void init()
    {
        initClock_();
        initUart_();
        initSpi_();
        initAdc_();
        initPwm_();
        initTick_();
        initDrv8841_();
    }
private:
    void initUart_();
    void initClock_();
    void initSpi_();
    void initPwm_();
    void initAdc_();
    void initTick_();

    /*Drv8841*/
     void initDrv8841_();
     void enDriver_();
     void sleepDriver_();
     void resetDriver_();
     void currentSetDriver_();
     void decaySetDriver_();
     bool isFaultDriver_();
COMPONENT_END

#endif /* UART_CONTROLLER_H_ */
