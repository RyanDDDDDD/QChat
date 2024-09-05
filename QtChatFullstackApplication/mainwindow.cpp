#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setUpContent();
    setUiConnection();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SlotSwitchReg()
{
    setCentralWidget(_regis);
    _login->hide();
    _regis->show();
}

void MainWindow::setUpContent()
{
    setUpLogin();
    setUpRegister();
    setCentralWidget(_login);
}

void MainWindow::setUpLogin()
{
    _login = new LoginDialog(this);
    _login->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
}

void MainWindow::setUpRegister()
{
    _regis = new RegisterDialog(this);
    _regis->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    _regis->hide();
}

void MainWindow::setUiConnection()
{
    connect(_login, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchReg);
}
