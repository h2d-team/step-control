#ifndef SERIAL_CONTROLLER_H
#define SERIAL_CONTROLLER_H
#include "core/component.h"
#include <QtSerialPort>
#include "core/define.h"

#define HEADER_INDICATOR	0xFE
#define FOOTER_INDICATOR	0xFD
#define MAX_PACKET_LENGTH	250

COMPONENT(serial,Controller)
    Q_OBJECT
    PROPERTY(quint32, bandwidth)
public:
    void init();
    void stop();

    Q_INVOKABLE bool open(QString port);
    Q_INVOKABLE void close();
    Q_INVOKABLE QVariant ports();
    Q_INVOKABLE bool isOpen();
    Q_SIGNAL void eventReceived(quint8 length, quint16 type, quint8* data);
    void send(int type, int length, uint8_t* data);
    void readData();
private:
    QSerialPort serial_;
    //void eventReceived_();
    void readyRead_();
    void decode_(uint8_t c);
     void updateTimer_();

    enum RxState{HEADER = 0, LENGTH, TYPE1, TYPE2, DATA, CRC, FOOTER};
    uint8_t* rxBuffer_;
    uint8_t rxLength_;
    uint8_t rxState_;
    uint8_t rxIndex_;
    uint32_t rxType_;
    uint8_t checkSum_;

    QTimer *timer;
    quint32 bandwidth_ = 0;
COMPONENT_END

#endif
