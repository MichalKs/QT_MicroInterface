#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QMessageBox>
#include <QDebug>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

    setupUi(this);

    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

    connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(sendFrameButton, SIGNAL(clicked()), this, SLOT(sendData()));

    serial = new QSerialPort(this);
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));


    serial->setPortName("ttyUSB0");
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadWrite)) {

    } else {

        QMessageBox::critical(this, tr("Error"), serial->errorString());
    }

}

MainWindow::~MainWindow(void) {
    serial->close();
}

void MainWindow::sendData(void) {

    QByteArray a("Hello\n");
    serial->write(a);
    serial->waitForBytesWritten(-1);

}

void MainWindow::readData(void) {
    QByteArray data = serial->readLine();
    rxData.append(data);

    if (*(data.data()) == '\n') {
        std::cout << "Hello: " << rxData.data() << std::endl;
        rxData.clear();
    }

}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        serial->close();
    }
}
