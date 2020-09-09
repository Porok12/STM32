//
// Created by przemek on 4/13/19.
//

#ifndef PTP_CHART_H
#define PTP_CHART_H

#include <QtCharts>
#include <QTimer>

class Chart : public QChart{
    Q_OBJECT
public:
    Chart();
    void setRange(int range);
    void setMeasurementRate(int rate);
    void clear();
    float getMeasurementRate();
public slots:
    void addData(QString str);
private:
    std::unique_ptr<QElapsedTimer> _timer;
    float _xRange;
    float _xCurrent;
    float _xMeasurementRate;
    float _yMax;
    std::unique_ptr<QLineSeries> _series;
};


#endif //PTP_CHART_H
