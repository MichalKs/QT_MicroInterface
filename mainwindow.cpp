#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

    setupUi(this);

    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

    connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));
    serial = new QSerialPort(this);

}

MainWindow::~MainWindow() {

}
