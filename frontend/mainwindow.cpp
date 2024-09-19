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
    connect(_login, &LoginDialog::switchReset, this, &MainWindow::SlotSwitchReset);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SlotSwitchReg()
{
    _regis = new RegisterDialog(this);
    _regis->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(_regis);

    _login->hide();
    _regis->show();

    connect(_regis, &RegisterDialog::sigSwitchLogin, this, &MainWindow::SlotSwitchLogin);
}

void MainWindow::SlotSwitchLogin()
{
    _login = new LoginDialog(this);
    _login->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(_login);

    _regis->hide();
    _login->show();

    connect(_login, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchReg);
    connect(_login, &LoginDialog::switchReset, this, &MainWindow::SlotSwitchReset);
}

void MainWindow::SlotSwitchReset()
{
    _reset = new ResetDialog(this);
    _reset->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(_reset);

    _login->hide();
    _reset->show();

    connect(_reset, &ResetDialog::switchLogin, this, &MainWindow::SlotSwitchLogin2);
}

void MainWindow::SlotSwitchLogin2()
{
    _login = new LoginDialog(this);
    _login->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(_login);

    _reset->hide();
    _login->show();

    connect(_login, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchReg);
    connect(_login, &LoginDialog::switchReset, this, &MainWindow::SlotSwitchReset);
}
