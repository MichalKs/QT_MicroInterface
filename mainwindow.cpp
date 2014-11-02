#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QMessageBox>
#include <QDebug>
#include <QTextStream>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

  setupUi(this);

  // Quit application
  connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
  // Connect to serial device
  connect(connectButton, SIGNAL(clicked()), this, SLOT(serialConnect()));

  connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));

//  connect(sendFrameButton, SIGNAL(clicked()), this, SLOT(sendData()));

}

MainWindow::~MainWindow(void) {
  if (serial) {
    serial->close();
  }

}

void MainWindow::serialConnect(void) {

  serial = new QSerialPort(this);

  connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
  connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
          SLOT(handleError(QSerialPort::SerialPortError)));


  serial->setPortName("ttyUSB0");
  serial->setBaudRate(QSerialPort::Baud115200);
  serial->setDataBits(QSerialPort::Data8);
  serial->setParity(QSerialPort::NoParity);
  serial->setStopBits(QSerialPort::OneStop);
  serial->setFlowControl(QSerialPort::NoFlowControl);

  if (serial->open(QIODevice::ReadWrite)) {

  } else {

      QMessageBox::critical(this, tr("Error"), serial->errorString());
  }
}
/**
 * @brief MainWindow::readData Reads data from serial device
 * @details Prints out every line to console
 */
void MainWindow::readData(void) {

  QByteArray data = serial->readLine();
  rxData.append(data);

  if (*(data.data()) == '\n') {
      std::cout << "Hello: " << rxData.data() << std::endl;

//        textEdit->setText();
      textEdit->append(rxData.data());

      rxData.clear();
  }

}

//void MainWindow::sendData(void) {

//  QByteArray a("Hello\n");
//  serial->write(a);
//  serial->waitForBytesWritten(-1);

//}



void MainWindow::handleError(QSerialPort::SerialPortError error)
{
  if (error == QSerialPort::ResourceError) {
      QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
      serial->close();
  }
}
