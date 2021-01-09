#ifndef CONSOLE_MANAGER_H
#define CONSOLE_MANAGER_H
#include "component.h"
#include <QtSerialPort>
#include "type.h"
#include "core/define.h"
#include "plotline.h"

COMPONENT(console, Manager)
Q_OBJECT
    READONLY(bool, pause_)
public:
    void init();
    void start();
    Q_INVOKABLE void showMessage(QString text);
    Q_SIGNAL void printMessage(QString time, QString text);
    Q_SIGNAL void fieldUpdated(quint16 type, qint32 value);

    Q_INVOKABLE void move(qint32 s); //s = 256 ~~ 1  Revolution
    Q_INVOKABLE void flashFields();
    Q_INVOKABLE void calibrateMotor();
    Q_INVOKABLE void startMotor();
    Q_INVOKABLE void stopMotor();
    Q_INVOKABLE void loadFields();
    Q_INVOKABLE void updateField(quint16 type, QString v);

    Q_INVOKABLE void initChart(QVariant p, QVariant pRef, QVariant v, QVariant vRef);

    Q_INVOKABLE qreal convertPositionRad(qreal v);
    Q_INVOKABLE qreal convertVelocityRad(qreal v);
    Q_INVOKABLE qreal convertPositionRev(qreal v);
    Q_INVOKABLE qreal convertPositionPulse(qreal v);

    Q_INVOKABLE void pause(bool p){set_pause_(p);}
    Q_INVOKABLE void updateFileName(QString fileName){this->fileName = fileName;}
    Q_INVOKABLE void enableBootLoader()
    {
        console::Manager::instance()->showMessage("Please reset board before into boot mode");
        requestJumBootLoader = true;
    }
    Q_INVOKABLE void updateFirmware();

    /* Check crc */

    void crc_ResetSeed(void);
    void crc_Generate(const uint8_t * const buff, const uint16_t length);
    uint32_t crc_Get(void);

private:
    uint8_t parser_AsciiToHex(uint8_t data);
    uint8_t parser_TwoHexToByte(uint8_t const data[]);

    /* check crc */
    void crc_CreateLookUpTable();

    uint32_t crc_LookUpTable[256u];
    uint32_t crc_Value = 0u;

private:

    void fieldUpdated_(uint16_t type, int32_t val);
    void displayPlot_(uint8_t* data, uint16_t length);
    void serialReceived_(quint8 length, quint16 type, quint8* data);

    qreal convertPosition_(int32_t v);
    qreal convertVelocity_(int32_t v);


private:
    int32_t currentIndex_ = 0;
    uint16_t flushCount_ = 0;
    bool chartReady_ = false;

    PlotLine* p_ = nullptr;
    PlotLine* pRef_ = nullptr;
    PlotLine* v_ = nullptr;
    PlotLine* vRef_ = nullptr;

    /*Bootloader*/
    QStringList lines;
    int lines_index = 0;
    QString fileName;
    bool requestJumBootLoader = false;
COMPONENT_END

#endif // MANAGER_H
