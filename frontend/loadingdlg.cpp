#include "loadingdlg.h"
#include "ui_loadingdlg.h"

#include <QMovie>

LoadingDlg::LoadingDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoadingDlg)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    setFixedSize(parent->size());

    QMovie* moive = new QMovie(":/res/loading.gif");
    ui->loading_lb->setMovie(moive);
    moive->start();
}

LoadingDlg::~LoadingDlg()
{
    delete ui;
}
