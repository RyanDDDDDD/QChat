#include "logindialog.h"
#include "ui_logindialog.h"
#include "httpmgr.h"

#include <QPixmap>
#include <QPainter>
#include <QPainterPath>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    connect(ui->regist_btn, &QPushButton::clicked, this, &LoginDialog::switchRegister);

    ui->forget_pwd_label->SetState("normal", "hover", "", "selected", "selected_hover", "");
    connect(ui->forget_pwd_label, &ClickedLabel::clicked, this, &LoginDialog::slot_forget_pwd);

    initHead();
    initHttpHandler();
    connect(HttpMgr::GetInstance().get(), &HttpMgr::sig_login_mod_finish,
            this, &LoginDialog::slot_login_mod_finish);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::slot_forget_pwd()
{
    qDebug() << "slot forget pwd";
    emit switchReset();
}

void LoginDialog::initHttpHandler()
{
    _handlers.insert(ReqId::ID_LOGIN_USER, [this](QJsonObject jsonObj){
        int error = jsonObj["error"].toInt();
        if (error != ErrorCodes::SUCCESS) {
            showTip(tr("Param error"), false);
            enableBtn(true);
            return;
        }

        auto email = jsonObj["email"].toString();

        ServerInfo si;
        si.Uid = jsonObj["uid"].toInt();
        si.Host = jsonObj["host"].toString();
        si.Port = jsonObj["port"].toString();
        si.Token = jsonObj["token"].toString();

        _uid = si.Uid;
        _token = si.Token;
        qDebug() << "email is " << email << " uid is " << si.Uid << " host is "
                 << si.Host << " Port is " << si.Port << " Token is " << si.Token;

        emit sig_connect_tcp(si);
    });
}

void LoginDialog::initHead()
{
    QPixmap originalPixmap(":/res/head_1.jpg");

    qDebug() << originalPixmap.size() << ui->head_label->size();
    originalPixmap = originalPixmap.scaled(ui->head_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QPixmap roundedPixmap(originalPixmap.size());
    roundedPixmap.fill(Qt::transparent);

    QPainter painter(&roundedPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    QPainterPath path;
    path.addRoundedRect(0, 0, originalPixmap.width(), originalPixmap.height(), 10, 10);
    painter.setClipPath(path);

    painter.drawPixmap(0, 0, originalPixmap);

    ui->head_label->setPixmap(roundedPixmap);
}

bool LoginDialog::checkUserValid()
{
    auto email = ui->email_edit->text();
    if (email.isEmpty()) {
        qDebug() << "email mepty";
        AddTipErr(TipErr::TIP_EMAIL_ERR, tr("email can not be empty"));
        return false;
    }

    DelTipErr(TipErr::TIP_EMAIL_ERR);
    return true;
}

bool LoginDialog::checkPwdValid()
{
    auto pwd = ui->password_edit->text();

    if (pwd.length() < 6 || pwd.length() > 15) {
        qDebug() << "Pass length invalid";
        AddTipErr(TipErr::TIP_PWD_ERR, tr("Password length should be 6-15"));
        return false;
    }

    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*]{6, 15}$");
    bool match = regExp.match(pwd).hasMatch();
    if (!match) {
        AddTipErr(TipErr::TIP_PWD_ERR, tr("Contains invalid symbol"));
        return false;
    }

    DelTipErr(TipErr::TIP_PWD_ERR);
    return true;
}

void LoginDialog::AddTipErr(TipErr te, QString tips)
{
    _tip_errs[te] = tips;
    showTip(tips, false);
}

void LoginDialog::DelTipErr(TipErr te)
{
    _tip_errs.remove(te);
    if (_tip_errs.empty()) {
        ui->err_tip->clear();
        return;
    }

    showTip(_tip_errs.first(), false);
}

void LoginDialog::showTip(QString str, bool b_ok)
{
    if (b_ok) {
        ui->err_tip->setProperty("state", "normal");
    } else {
        ui->err_tip->setProperty("state", "err");
    }

    ui->err_tip->setText(str);
    repolish(ui->err_tip);
}

bool LoginDialog::enableBtn(bool enabled)
{
    ui->login_btn->setEnabled(enabled);
    ui->regist_btn->setEnabled(enabled);
    return true;
}

void LoginDialog::on_login_btn_clicked()
{
    qDebug() << "login btn clicked";
    if (checkUserValid() == false) {
        return;
    }

    if (checkPwdValid() == false) {
        return;
    }

    enableBtn(false);
    auto email = ui->email_edit->text();
    auto pwd = ui->password_edit->text();

    //send http request
    QJsonObject json_obj;
    json_obj["email"] = email;
    json_obj["passwd"] = xorString(pwd);

    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix + "user_login"), json_obj,
                                        ReqId::ID_LOGIN_USER, Modules::LOGINMOD);
}

void LoginDialog::slot_login_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if (err != ErrorCodes::SUCCESS){
        showTip(tr("Network request error"), false);
        return;
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());

    if (jsonDoc.isNull()){
        showTip(tr("json parsing error"), false);
        return;
    }

    if (!jsonDoc.isObject()){
        showTip(tr("json parsing error"), false);
        return;
    }

    _handlers[id](jsonDoc.object());
    return;
}

