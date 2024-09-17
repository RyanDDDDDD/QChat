#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _login = new LoginDialog(this);
    _login->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    setCentralWidget(_login);

    connect(_login, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchReg);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SlotSwitchReg()
{
    _regis = new RegisterDialog(this);
    _regis->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    connect(_regis, &RegisterDialog::sigSwitchLogin, this, &MainWindow::SlotSwitchLogin);
    setCentralWidget(_regis);

    _login->hide();
    _regis->show();
}

void MainWindow::SlotSwitchLogin()
{
    _login = new LoginDialog(this);
    _login->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    setCentralWidget(_login);

    _regis->hide();
    _login->show();

    connect(_login, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchReg);
    // connect(_login, &LoginDialog::switchReset, this, &MainWindow::SlotSwitchReset);
}

void MainWindow::SlotSwitchReset()
{

}
