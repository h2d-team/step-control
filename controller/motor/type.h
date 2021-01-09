#ifndef MCU_TYPE_H
#define MCU_TYPE_H

#define FIELD_NUM 19

namespace console
{

enum Field
{
    MODE = 1,
    PKP,
    PKI,
    PKD,
    VKP,
    VKI,
    KAFF,
    KVFF,
    DKP,
    DKI,
    QKP,
    QKI,
    RZERO,
    OCURRENT,
    P_MAX,
    V_MAX,
    A,
    V,
    Measure,
};

enum Command
{
    MessageReceived = 1,
    PlotReceived,
    FieldReceived,
    MotorStarted,
    MotorStoped,
    MotorCalibrated,
    MotorMoved,
    OverCurrent,
    UpdateField,
    CalibrateMotor,
    StartMotor,
    StopMotor,
    MoveMotor,
    SyncFields,
};

}

#endif
