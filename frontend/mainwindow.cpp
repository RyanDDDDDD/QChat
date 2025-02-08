#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tcpmgr.h"

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

    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_switch_chatdlg, this, &MainWindow::SlotSwitchChat);

    emit TcpMgr::GetInstance()->sig_switch_chatdlg();
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

void MainWindow::SlotSwitchChat()
{
    _chat = new ChatDialog();
    _chat->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(_chat);
    _login->hide();
    this->setMinimumSize(QSize(1050, 900));
    this->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
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
