#ifndef CONSOLE_TYPE_H
#define CONSOLE_TYPE_H

#define PLOT_PREF_MASK  1
#define PLOT_P_MASK  2
#define PLOT_VREF_MASK  4
#define PLOT_V_MASK  8

#define CMD_ROTOR_ZERO  1

#define MAX_X   1000

#include <QObject>
#define ENUM(name, ...)\
enum class name##_ __VA_ARGS__;\
Q_DECLARE_FLAGS(name,name##_)\
Q_FLAG_NS(name)

namespace console
{
    Q_NAMESPACE
    ENUM(Field,{MODE = 1, PKP, PKI, PKD, VKP, VKI, KAFF, KVFF, DKP, DKI, QKP, QKI, RZERO, OCURRENT, P_MAX, V_MAX, A, V, Measure, PercentDownload, BootModeEnable})
}
namespace console
{
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
    boot_start = 40,
    boot_download,
    boot_end,
    boot_app_mode,
    boot_bootloader_mode_require,
    boot_bootloader_mode,

};

enum ComMode
{
    SD = 0,
    UART,
    CAN
};

enum MeasureMode
{
    PULSE = 0,
    RAD,
    REV,
    ANGLE
};

}

#endif // CONSOLE_TYPE_H
