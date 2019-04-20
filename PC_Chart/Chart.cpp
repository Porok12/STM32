//
// Created by przemek on 4/13/19.
//

#include "Chart.h"

/**
 * @brief Sets up basic parameters
 */
Chart::Chart() :
    QChart(){
    this->setTitle("");
    this->createDefaultAxes();
    this->legend()->hide();
    this->setTheme(QChart::ChartThemeLight);
    this->setAnimationOptions(QtCharts::QChart::NoAnimation);

    _yMax = 0;
    _xCurrent = 0;
    _xRange = 5;
    _xMeasurementRate = 100 / 1000.0f;
    _series = std::unique_ptr<QLineSeries>(new QLineSeries());
    _timer = std::unique_ptr<QElapsedTimer>(new QElapsedTimer());

    this->addSeries(_series.get());
    this->createDefaultAxes();
    this->axisX()->setRange(_xCurrent - _xRange, _xCurrent);
}

/**
 * @brief Receives values and add them
 * @param str it's QString with received value
 */
void Chart::addData(QString str) {
    if(!this->series().isEmpty()) {
        //this->removeSeries(_series.get());
    }

    QRegExp exp = QRegExp("(\\d+\\.\\d+)");
    QRegExp unt = QRegExp("(\\[\\w+\\])");
    exp.indexIn(str);
    unt.indexIn(str);

    bool ok = false;
    QString(exp.capturedTexts().first()).toFloat(&ok);
    if(ok) {
        if(_series->count() > (int)(_xRange / (_timer->elapsed() / 1000.0f))) { // (_xRange / _xMeasurementRate)
            _series->remove(0);
            _yMax = 0;
            for(int i = 0; i < _series->count(); i++) {
                if(_yMax < _series->at(i).y()) {
                    _yMax = _series->at(i).y();
                }
            }
        }
        _series->append(_xCurrent, exp.capturedTexts().first().toFloat());
        _xCurrent += _timer->elapsed() / 1000.0f; //_xMeasurementRate;
        if(_yMax < exp.capturedTexts().first().toFloat()) {
            _yMax = exp.capturedTexts().first().toFloat();
        }
    }

    _timer->start();

    //this->addSeries(_series.get());
    //this->createDefaultAxes();
    this->axisY()->setRange(0, _yMax * 1.1f);
    this->axisX()->setRange(_xCurrent - _xRange, _xCurrent);
    if(this->axisY()->titleText().compare(unt.capturedTexts().first()) != 0
       && unt.capturedTexts().first() != "") {
        this->axisY()->setTitleText(unt.capturedTexts().first());
    }
}

/**
 * @brief Sets range
 * @param range
 */
void Chart::setRange(int range) {
    _xRange = range;
}

/**
 * @brief Sets measurement rate
 * @param rate
 */
void Chart::setMeasurementRate(int rate) {
    _xMeasurementRate = rate / 1000.0f;
}

/**
 * @brief Clears series and set (x,y) to zero
 */
void Chart::clear() {
    _xCurrent = 0;
    _yMax = 0;
    _series->clear();
    if(!this->series().isEmpty()) {
        //this->removeSeries(_series.get());
    }
}

/**
 * @brief Currently doesn't matter
 * @return returns set value of measurement rate
 */
float Chart::getMeasurementRate() {
    return _xMeasurementRate;
}
