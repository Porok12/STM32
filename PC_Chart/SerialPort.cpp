//
// Created by przemek on 4/13/19.
//

#include "SerialPort.h"

/**
 * @brief Sets up basic stuff
 * @param parent
 */
SerialPort::SerialPort(QObject *parent)
        : QSerialPort(parent) {
    connect(this, &QSerialPort::readyRead, this, &SerialPort::readData);
}

/**
 * @brief Gathers data bytes to commands
 */
void SerialPort::readData() {
    QByteArray data = this->readAll();
    _list = QString(data).split("\r\n");
    for(int i = 0; i < _list.size(); i++) {
        _temp += _list[i];
    }

    while(_temp.contains('\n')) {
        emit valueChanged(QRegularExpression("^(.*?\\n)").match(_temp).capturedTexts().first());
        qInfo() << "_temp = " << _temp;
        _temp.remove(QRegularExpression("^(.*?\\n)"));
    }
}
