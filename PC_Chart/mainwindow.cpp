//
// Created by przemek on 4/7/19.
//

#include "mainwindow.h"
#include <QGridLayout>

/**
 * @brief Sets up basic parameters
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent)
{
    QWidget *window = new QWidget();
    QGridLayout *mainLayout = setupMainLayout();
    window->setLayout(mainLayout);
    setCentralWidget(window);
    setFixedSize(800, 600);
    setMaximumSize(1920, 1080);
    setMinimumSize(800, 600);
    setWindowTitle("STM32 Project");

    initActionsConnections();
}

/**
 * @brief Destroys all pointers
 */
MainWindow::~MainWindow()
{
    delete _chart;
    delete _serial;
    delete _connectBtn;
    delete _refreshBtn;
    delete _plain;
    delete _chartView;
    delete _portBox;
    delete _baudRateBox;
    delete _parityBox;
    delete _stopBitsBox;
    delete _flowControlBox;
    delete _measurementBox;
    delete _spinBox;
    delete _label;
    delete _lineEdit;
    delete _autoScrollBox;
}

/**
 *  @brief Tries to connect to serial port
 */
void MainWindow::openSerialPort() {
    setupSerial();

    if (_serial->open(QIODevice::ReadWrite)) {
        _connectBtn->setText("Disconnect");
        _label->setText("Connected");
        _lineEdit->setEnabled(true);
        QThread::msleep(60);
        writeData("stop\n");
        QByteArray array;
        switch(_measurementBox->currentIndex()+1) {
            case 1:
                array.append("vol2");
                break;
            case 2:
                array.append("vol");
                break;
            case 3:
                array.append("power");
                break;
            case 4:
                array.append("curr");
        }
        array.append(" -1,100\n");
        writeData(array);
    }
    else {
        closeSerialPort();
    }
}

/**
 * @brief Closes connection to serial port
 */
void MainWindow::closeSerialPort() {
    if(_serial->isOpen()) {
        _serial->close();
    }

    _connectBtn->setText("Connect");
    _label->setText("Disconnected");
    _lineEdit->setEnabled(false);
}

/**
 * @brief Sends data through serial port
 * @param data data to be send
 */
void MainWindow::writeData(const QByteArray &data) {
    if(_serial->isOpen()) {
        QRegularExpressionMatch match;
        match = QRegularExpression("([a-zA-Z]+).*?(\\d+),.*?(\\d+)").match(data);
        if(match.hasMatch()) {
            _chart->setMeasurementRate(match.captured(3).toInt());
        }
        _serial->write(data);
    }
}

/**
 * @brief Search for ports
 */
void MainWindow::searchForPorts() {
    _portBox->clear();
    QList<QSerialPortInfo> info = QSerialPortInfo::availablePorts();
    for(QList<QSerialPortInfo>::iterator i = info.begin(); i != info.end(); i++) {
        _portBox->addItem(i->portName());
    }
}

/**
 * @brief Sends data
 */
void MainWindow::lineEditEnterText() {
    if(_serial->isOpen()) {
        QByteArray data;
        data.append(_lineEdit->text().append('\n'));
        writeData(data);
    }
    _lineEdit->clear();
}

/**
 * @brief Changes measurement goal
 * @param index
 */
void MainWindow::measurementChanged(int index) {
    uint8_t cmd = index+1;
    if(_serial->isOpen()) {
        writeData("stop\n");
        QByteArray array;
        switch(cmd) {
            case 1:
                array.append("vol2");
                break;
            case 2:
                array.append("vol");
                break;
            case 3:
                array.append("power");
                break;
            case 4:
                array.append("curr");
        }
        array.append(QString(" ").append(QString::number(-1)).append(',').append(QString::number(100)).append('\n'));
        writeData(array);
        _chart->clear();
    }
}

/**
 * @brief Display all errors
 * @param error
 */
void MainWindow::handleError(QSerialPort::SerialPortError error) {
    if (error != QSerialPort::NoError) {
        _serial->error(error);
        QMessageBox::critical(this, tr("Critical Error"),
                              QString("%1\nError code: %2").arg(_serial->errorString()).arg(error));
    }
}

/**
 * @brief Initializes all connections
 */
void MainWindow::initActionsConnections() {
    //connect(_connectBtn, SIGNAL(clicked()), SLOT(openSerialPort()));
    connect(_connectBtn, SIGNAL(clicked()), SLOT(buttonClicked()));
    connect(_refreshBtn, SIGNAL(clicked()), SLOT(searchForPorts()));
    connect(_lineEdit, SIGNAL(returnPressed()), SLOT(lineEditEnterText()));
    connect(_measurementBox, SIGNAL(currentIndexChanged(int)), this, SLOT(measurementChanged(int)));
    connect(_plain, SIGNAL(textChanged()), this, SLOT(plainTextChanged()));
    connect(_serial, &QSerialPort::errorOccurred, this, &MainWindow::handleError);
    connect(_serial, SIGNAL(valueChanged(QString)), _chart, SLOT(addData(QString)));
    connect(_serial, &SerialPort::valueChanged, _plain, &QPlainTextEdit::insertPlainText);
    connect(this, &MainWindow::destroyed, this, &MainWindow::closeSerialPort);
    //connect(_serial, &QSerialPort::readyRead, this, &MainWindow::readData);
}

/**
 * @brief Handles connection's button
 */
void MainWindow::buttonClicked() {
    if(_serial->isOpen()) {
        closeSerialPort();
    }
    else {
        openSerialPort();
    }
}

/**
 * @brief Scrolls to bottom when required
 */
void MainWindow::plainTextChanged() {
    if(_autoScrollBox->isChecked()) {
        _plain->verticalScrollBar()->setValue(_plain->verticalScrollBar()->maximum());
    }
}

/**
 * @brief Sets up all required values for serial connection
 */
void MainWindow::setupSerial() {
    _serial->setPortName(_portBox->currentText());
    _serial->setBaudRate(static_cast<QSerialPort::BaudRate >(
                                 _baudRateBox->itemData(_baudRateBox->currentIndex()).toInt()));
    switch(_spinBox->value()) {
        case 5:
            _serial->setDataBits(QSerialPort::Data5);
            break;
        case 6:
            _serial->setDataBits(QSerialPort::Data6);
            break;
        case 7:
            _serial->setDataBits(QSerialPort::Data7);
            break;
        case 8:
            _serial->setDataBits(QSerialPort::Data8);
            break;
    }
    _serial->setParity(static_cast<QSerialPort::Parity >(
                               _parityBox->itemData(_parityBox->currentIndex()).toInt()));
    _serial->setStopBits(static_cast<QSerialPort::StopBits >(
                                 _stopBitsBox->itemData(_stopBitsBox->currentIndex()).toInt()));
    _serial->setFlowControl(static_cast<QSerialPort::FlowControl>(
                                    _flowControlBox->itemData(_flowControlBox->currentIndex()).toInt()));
}

/**
 * @brief Sets up whole layout
 * @return main QGridLayout
 */
QGridLayout * MainWindow::setupMainLayout() {
    _connectBtn = new QPushButton("Connect");
    _refreshBtn = new QPushButton("Refresh");

    _label = new QLabel("Disconnected");

    _chart = new Chart();
    _chartView = new QChartView(_chart);
    _chartView->setRenderHint(QPainter::Antialiasing);

    _portBox = new QComboBox;
    searchForPorts();

    _baudRateBox = new QComboBox;
    _baudRateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    _baudRateBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    _baudRateBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    _baudRateBox->addItem(QStringLiteral("57600"), QSerialPort::Baud57600);
    _baudRateBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    _baudRateBox->setCurrentIndex(2);

    _parityBox = new QComboBox;
    _parityBox->addItem(tr("None"), QSerialPort::NoParity);
    _parityBox->addItem(tr("Even"), QSerialPort::EvenParity);
    _parityBox->addItem(tr("Odd"), QSerialPort::OddParity);
    _parityBox->setCurrentIndex(0);

    _stopBitsBox = new QComboBox;
    _stopBitsBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    _stopBitsBox->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
    _stopBitsBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);
    _stopBitsBox->setCurrentIndex(0);
    _spinBox = new QSpinBox();
    _spinBox->setRange(5, 8);
    _spinBox->setValue(8);

    _flowControlBox = new QComboBox;
    _flowControlBox->addItem(tr("None"), QSerialPort::NoFlowControl);
    _flowControlBox->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    _flowControlBox->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);
    _flowControlBox->setCurrentIndex(0);

    _measurementBox = new QComboBox;
    _measurementBox->addItem(tr("Shunt Voltage"));
    _measurementBox->addItem(tr("Bus Voltage"));
    _measurementBox->addItem(tr("Power"));
    _measurementBox->addItem(tr("Current"));
    _measurementBox->setCurrentIndex(3);

    _serial = new SerialPort(this);

    _plain = new QPlainTextEdit();

    _lineEdit = new QLineEdit;
    _lineEdit->setEnabled(false);

    _autoScrollBox = new QCheckBox();
    _autoScrollBox->setChecked(true);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(tr("Measurement:"), _measurementBox);
    formLayout->addRow(_connectBtn);
    formLayout->addRow(tr("Status:"), _label);
    formLayout->addRow(tr("Port:"), _portBox);
    formLayout->addRow(_refreshBtn);
    formLayout->addRow(tr("Baud Rate:"), _baudRateBox);
    formLayout->addRow(tr("Word Length:"), _spinBox);
    formLayout->addRow(tr("Parity:"), _parityBox);
    formLayout->addRow(tr("Stop Bits:"), _stopBitsBox);
    formLayout->addRow(tr("Flow Control:"), _flowControlBox);
    formLayout->addRow(_lineEdit);
    formLayout->addRow(tr("Auto scroll:"), _autoScrollBox);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(_chartView, 0, 1, -1, 1);
    mainLayout->addWidget(_plain, 1, 0);
    mainLayout->addLayout(formLayout, 0, 0);
    mainLayout->setColumnStretch(0, 0);
    mainLayout->setColumnStretch(1, 1);
    mainLayout->setColumnMinimumWidth(1, 640);

    return mainLayout;
}
