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
  void sendData(void);
  void handleError(QSerialPort::SerialPortError error);
  void readData(void);

private:
  QSerialPort* serial;
  QByteArray rxData;
};

#endif // MAINWINDOW_H
