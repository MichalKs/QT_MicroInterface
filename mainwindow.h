#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QSerialPort>

class MainWindow : public QMainWindow, public Ui::MainWindow {

  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow(void);

private slots:
  void serialConnect();
  void serialDisconnect();
  void handleError(QSerialPort::SerialPortError error);
  void readData(void);
  void sendData(void);
  void turnOnLED(void);
  void turnOffLED(void);

private:
  QSerialPort* serial;
  QByteArray rxData; // data received from serial port
  QByteArray txData; // data transmitted to serial port
};

#endif // MAINWINDOW_H
