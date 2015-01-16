#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QMessageBox>
#include <QDebug>
#include <QTextStream>
#include <iostream>

/**
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

  qDebug() << "In MainWindow constructor";
  setupUi(this); // setup form

  // Quit application
  connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
  connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));
  // Connect to serial device
  connect(connectButton, SIGNAL(clicked()), this, SLOT(serialConnect()));
  connect(disconnectButton, SIGNAL(clicked()), this, SLOT(serialDisconnect()));
  connect(sendFrameButton, SIGNAL(clicked()), this, SLOT(sendData()));

  connect(ledOnButton, SIGNAL(clicked()), this, SLOT(turnOnLED()));
  connect(ledOffButton, SIGNAL(clicked()), this, SLOT(turnOffLED()));

  connect(dataToSend, SIGNAL(returnPressed()), sendFrameButton, SLOT(click()));

  // Create the serial port
  serial = new QSerialPort(this);

  connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
  connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
          SLOT(handleError(QSerialPort::SerialPortError)));

}
/**
 * @brief MainWindow::~MainWindow
 */
MainWindow::~MainWindow(void) {
  qDebug() << "In MainWindow desctructor";
  if (serial) {
    serial->close();
  }
}
/**
 * @brief MainWindow::serialConnect This slot connects
 * to a given serial port.
 */
void MainWindow::serialConnect(void) {

  serial->setPortName("ttyUSB0");
  serial->setBaudRate(QSerialPort::Baud115200);
  serial->setDataBits(QSerialPort::Data8);
  serial->setParity(QSerialPort::NoParity);
  serial->setStopBits(QSerialPort::OneStop);
  serial->setFlowControl(QSerialPort::NoFlowControl);

  if (serial->open(QIODevice::ReadWrite)) {
    qDebug() << "Opened serial device";

    disconnectButton->setEnabled(true);
    sendFrameButton->setEnabled(true);
    actionDisconnect->setEnabled(true);
    ledTab->setEnabled(true);

    connectButton->setEnabled(false);
    actionConnect->setEnabled(false);
  } else {
    qDebug() << "Cannot open port";
    QMessageBox::critical(this, tr("Error"), serial->errorString());
  }
}

void MainWindow::serialDisconnect(void) {
  if (serial) {
    serial->close();
  }

  disconnectButton->setEnabled(false);
  sendFrameButton->setEnabled(false);
  actionDisconnect->setEnabled(false);
  ledTab->setEnabled(false);

  connectButton->setEnabled(true);
  actionConnect->setEnabled(true);

  qDebug() << "Closed serial device";
}

/**
 * @brief MainWindow::readData Reads data from serial device
 * @details Prints out every line to console
 */
void MainWindow::readData(void) {

  QByteArray newData = serial->readLine();
  rxData.append(newData);

  textEdit->moveCursor(QTextCursor::End);
  QString str(rxData.data());
  textEdit->insertPlainText(str);
  textEdit->moveCursor(QTextCursor::End);
  rxData.clear();

}

void MainWindow::sendData(void) {

  QByteArray tmp;

  QString str = dataToSend->text();

  tmp.append(str);
  tmp.append("\r");

  qint64 ret = serial->write(tmp);
  if (ret == -1) {
    qDebug() << "Error sending data";
  } else if (ret >= 0 ) {
    qDebug() << "Sent " << ret << " bytes";
  }
//    serial->waitForBytesWritten(-1);
}

void MainWindow::turnOffLED(void) {

  int led = ledCombo->currentIndex();

  QByteArray tmp;
  tmp.append(":LED ");
  tmp.append(QString::number(led));
  tmp.append(" OFF\r");

  qint64 ret = serial->write(tmp);
  if (ret == -1) {
    qDebug() << "Error sending data";
  } else if (ret >= 0 ) {
    qDebug() << "Sent " << ret << " bytes";
  }
}

void MainWindow::turnOnLED(void) {

  int led = ledCombo->currentIndex();

  QByteArray tmp;
  tmp.append(":LED ");
  tmp.append(QString::number(led));
  tmp.append(" ON\r");

  qint64 ret = serial->write(tmp);
  if (ret == -1) {
    qDebug() << "Error sending data";
  } else if (ret >= 0 ) {
    qDebug() << "Sent " << ret << " bytes";
  }
}

void MainWindow::handleError(QSerialPort::SerialPortError error) {
  if (error == QSerialPort::ResourceError) {
      QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
      serial->close();
  }
}
