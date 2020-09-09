//
// Created by przemek on 4/13/19.
//

#ifndef PTP_SERIALPORT_H
#define PTP_SERIALPORT_H


#include <QtSerialPort>

class SerialPort : public QSerialPort{
Q_OBJECT
public:
    SerialPort(QObject* parent = nullptr);
signals:
    void valueChanged(QString str);
private slots:
    void readData();
private:
    QString _temp;
    QList<QString> _list;
};


#endif //PTP_SERIALPORT_H
