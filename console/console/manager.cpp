#include "manager.h"
#include "core/views.h"
#include "serial/controller.h"
#include <QXYSeries>

void console::Manager::init()
{
    qmlRegisterUncreatableMetaObject(console::staticMetaObject,"console", 1, 0, "Type","Cannot create namespace Type in QML");
    core::Views::instance()->registerComponent("Console", this);
    connect(serial::Controller::instance(), &serial::Controller::eventReceived, this, &console::Manager::serialReceived_, Qt::DirectConnection);
    pause_ = false;

    crc_CreateLookUpTable();
}

void console::Manager::initChart(QVariant p, QVariant pRef, QVariant v, QVariant vRef)
{
    p_ = new PlotLine(qvariant_cast<QXYSeries*>(p));
    pRef_ = new PlotLine(qvariant_cast<QXYSeries*>(pRef));
    v_ = new PlotLine(qvariant_cast<QXYSeries*>(v));
    vRef_ = new PlotLine(qvariant_cast<QXYSeries*>(vRef));
    chartReady_ = true;
}

void console::Manager::start()
{

}

void console::Manager::showMessage(QString text)
{
    QDateTime now = QDateTime::currentDateTime();
    emit printMessage(now.toString("hh:mm:ss"), text);
}

#define PULSE_PER_REV   (1<<14)  //~1 revolution
#define POS_MIN         500.0
#define POS_MAX         1000.0
#define POS_SHIFT       4

qreal console::Manager::convertPosition_(int32_t v)
{
    auto s = POS_MIN + 250 + ( (qreal)v /(10 * PULSE_PER_REV))*((POS_MAX - POS_MIN)/2);
    //auto s = POS_MIN + 250 + ( (qreal)v /(pMax * PULSE_PER_REV))*((POS_MAX - POS_MIN)/2);
    return s;
}

qreal console::Manager::convertVelocity_(int32_t v)
{
    return 250 + ((qreal)v/2500) * POS_MIN/2; // 1000 ms
    //return 250 + ((qreal)v/vMax) * POS_MIN/2; // test
    //return v;
}

qreal console::Manager::convertPositionRad(qreal v)
{
    //return ((v - (POS_MIN + 250))*pMax*2* M_PI) / ((POS_MAX - POS_MIN)/2); //[rad]
    return v;
}

qreal console::Manager::convertPositionRev(qreal v)
{
    //return ((v - (POS_MIN + 250))*pMax) / ((POS_MAX - POS_MIN)/2); //[revolution]
    return v;
}

qreal console::Manager::convertPositionPulse(qreal v)
{
    //return ((v - (POS_MIN + 250))*pMax* PULSE_PER_REV) / ((POS_MAX - POS_MIN)/2); //[pulse]
    return v;
}

qreal console::Manager::convertVelocityRad(qreal v)
{
    //return ((v - 250) * (vMax * 2* M_PI / PULSE_PER_REV)) / (1000 * POS_MIN/2); // Rad/1000ms
    //return ((v - 250) * (vMax * 2* M_PI / PULSE_PER_REV)) / (POS_MIN/2); // test
    return v;
}

void console::Manager::displayPlot_(uint8_t* data, uint16_t length)
{
    if (!chartReady_) return;
    int index = 0;
    int32_t v_P = 0;
    int16_t v_V  = 0;

    v_P =  ((int32_t)data[index++]<<24) + ((int32_t)data[index++]<<16) + ((int32_t)data[index++]<<8) + data[index++];
    pRef_->addValue(currentIndex_, convertPosition_(v_P));

    v_P =  ((int32_t)data[index++]<<24) + ((int32_t)data[index++]<<16) + ((int32_t)data[index++]<<8) + data[index++];
    p_->addValue(currentIndex_, convertPosition_(v_P));

    v_V =  ((int16_t)data[index++]<<8) + data[index++];
    vRef_->addValue(currentIndex_, convertVelocity_(v_V));

    v_V =  ((int16_t)data[index++]<<8) + data[index++];
    v_->addValue(currentIndex_, convertVelocity_(v_V));

    if (flushCount_++ > 10 && !pause_)
    {
        flushCount_ = 0;
        p_->flush(currentIndex_);
        pRef_->flush(currentIndex_);
        v_->flush(currentIndex_);
        vRef_->flush(currentIndex_);
    }
    if(++currentIndex_ >= MAX_X) currentIndex_ = 0;
}
