#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "global.h"
#include "httpmgr.h"

RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);

    ui->password_edit->setEchoMode(QLineEdit::Password);
    ui->confirm_edit->setEchoMode(QLineEdit::Password);

    ui->error_label->setProperty("state", "normal");
    repolish(ui->error_label);
    connect(HttpMgr::GetInstance().get(), &HttpMgr::sig_reg_mod_finish, this, &RegisterDialog::slot_reg_mod_finish);

    initHttpHandlers();
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::initHttpHandlers()
{
    //get validation code callback
    _handlers.insert(ReqId::ID_GET_VERIFY_CODE, [this](const QJsonObject& jsonObj){
        int error = jsonObj["error"].toInt();
        if(error != ErrorCodes::SUCCESS){
            showTip(tr("Argument error"), false);
            return;
        }

        auto email = jsonObj["email"].toString();
        showTip(tr("Validation code sent, Please check"), true);
        qDebug() << "email is: " << email;
    });


}

void RegisterDialog::showTip(QString str, bool ok)
{
    if (ok) {
        ui->error_label->setProperty("state", "normal");
    } else {
        ui->error_label->setProperty("state", "err");
    }

    ui->error_label->setText(str);
    repolish(ui->error_label);
}

void RegisterDialog::on_verify_btn_clicked()
{
    auto email = ui->email_label->text();
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch();

    if (match) {
        //send http validation code

    } else {
        showTip(tr("email address is not correct"), false);
    }
}

void RegisterDialog::slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if (err != ErrorCodes::SUCCESS){
        showTip(tr("Network request failed"), false);
        return;
    }

    //parse json string, res->QByteArray
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    if (jsonDoc.isNull()){
        showTip("Json Parsing failed", false);
        return;
    }

    //json parsing error
    if (!jsonDoc.isObject()){
        showTip("Json Parsing failed", false);
        return;
    }

    jsonDoc.object();
    _handlers[id](jsonDoc.object());

    return;
}

