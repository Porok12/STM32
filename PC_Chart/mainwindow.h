//
// Created by przemek on 4/7/19.
//

#ifndef PTP_MAINWINDOW_H
#define PTP_MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include "Chart.h"
#include "SerialPort.h"

/**
 * @author Przemyslaw Papla
 *
 */
class MainWindow : public QMainWindow{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void openSerialPort();
    void closeSerialPort();
    void writeData(const QByteArray &data);
    void searchForPorts();
    void lineEditEnterText();
    void measurementChanged(int index);
    void plainTextChanged();
    void handleError(QSerialPort::SerialPortError error);
    void buttonClicked();

private:
    QGridLayout* setupMainLayout();
    void setupSerial();
    void initActionsConnections();
    Chart * _chart;
    SerialPort * _serial;
    QPushButton *_connectBtn;
    QPushButton *_refreshBtn;
    QPlainTextEdit * _plain;
    QChartView *_chartView;
    QComboBox * _portBox;
    QComboBox * _baudRateBox;
    QComboBox * _parityBox;
    QComboBox * _stopBitsBox;
    QComboBox * _flowControlBox;
    QComboBox * _measurementBox;
    QSpinBox * _spinBox;
    QLabel * _label;
    QLineEdit * _lineEdit;
    QCheckBox * _autoScrollBox;
};


#endif //PTP_MAINWINDOW_H
