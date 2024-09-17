#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/******************************************************************************
 *
 * @file       mainwindow.h
 * @brief      mainWindow
 *
 * @author     Ryan
 * @date       2024/08/31
 * @history
 *****************************************************************************/

#include <QMainWindow>
#include "logindialog.h"
#include "registerdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void SlotSwitchReg();
    void SlotSwitchLogin();
    void SlotSwitchReset();

private:
    Ui::MainWindow* ui;
    LoginDialog*    _login;
    RegisterDialog* _regis;
};
#endif // MAINWINDOW_H
