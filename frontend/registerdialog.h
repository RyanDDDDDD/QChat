#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include "global.h"

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

private:
    void initHttpHandlers();
    void showTip(QString str, bool ok);

    void AddTipErr(TipErr te, QString tips);
    void DelTipErr(TipErr te);

    bool checkUserValid();
    bool checkEmailValid();
    bool checkPassValid();
    bool checkConfirmValid();
    bool checkVerifyValid();

    void ChangeTipPage();

signals:
    void sigSwitchLogin();

private slots:
    void on_verify_btn_clicked();

    void slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err);

    void on_confirm_btn_clicked();
    void on_return_btn_clicked();
    void on_cancel_btn_clicked();

private:
    Ui::RegisterDialog *ui;
    QMap<ReqId, std::function<void(const QJsonObject&)>> _handlers;
    QMap<TipErr, QString> _tip_errs;

    QTimer *_countdown_timer;
    int _countdown;
};

#endif // REGISTERDIALOG_H
