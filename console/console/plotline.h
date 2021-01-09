#ifndef CONSOLE_PLOTLINE_H
#define CONSOLE_PLOTLINE_H

#include <QObject>
#include <QVector>
#include <QList>
#include <QXYSeries>
#include <QDebug>
#include "type.h"

using namespace QtCharts;

class PlotLine
{
public:
    PlotLine(QXYSeries* series): series_(series)
    {
        plotData_.resize(MAX_X);
        for (int i=0;i<MAX_X;i++)
        {
            plotData_[i].setX(i);
            plotData_[i].setY(0);
        }
    }

    void addValue(uint32_t lastIndex, qreal y)
    {
        data_[lastIndex] = y;
    }

    void flush(uint32_t lastIndex)
    {
        for (int i=0; i<MAX_X; i++)
        {
            plotData_[i].setX(i);
            auto index = i + lastIndex + 1;
            if (index >= MAX_X) index -= MAX_X;
            plotData_[i].setY(data_[index]);
        }
        series_->replace(plotData_);
    }
private:
    QVector<QPointF> plotData_;
    qreal data_[MAX_X];
    QXYSeries* series_;
};

#endif // PLOTLINE_H
