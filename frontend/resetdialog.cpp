#include "resetdialog.h"
#include "ui_resetdialog.h"
#include <QDebug>
#include <QRegularExpression>
#include "global.h"
#include "httpmgr.h"

ResetDialog::ResetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResetDialog)
{
    ui->setupUi(this);

    connect(ui->user_edit,&QLineEdit::editingFinished,this,[this](){
        checkUserValid();
    });

    connect(ui->email_edit, &QLineEdit::editingFinished, this, [this](){
        checkEmailValid();
    });

    connect(ui->pwd_edit, &QLineEdit::editingFinished, this, [this](){
        checkPassValid();
    });


    connect(ui->verify_edit, &QLineEdit::editingFinished, this, [this](){
         checkVerifyValid();
    });

    initHandlers();
    connect(HttpMgr::GetInstance().get(), &HttpMgr::sig_reset_mod_finish, this,
            &ResetDialog::slot_reset_mod_finish);
}


ResetDialog::~ResetDialog()
{
    delete ui;
}

void ResetDialog::on_return_btn_clicked()
{
    qDebug() << "sure btn clicked ";
    emit switchLogin();
}

void ResetDialog::on_verify_btn_clicked()
{
    qDebug()<<"receive verify btn clicked ";
    auto email = ui->email_edit->text();
    auto bcheck = checkEmailValid();
    if(!bcheck){
        return;
    }

    QJsonObject json_obj;
    json_obj["email"] = email;
    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/get_verifycode"),
                                        json_obj, ReqId::ID_GET_VERIFY_CODE, Modules::RESETMOD);
}

void ResetDialog::slot_reset_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if(err != ErrorCodes::SUCCESS){
        showTip(tr("Network request error"),false);
        return;
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    //Json Parsing Error
    if(jsonDoc.isNull()){
        showTip(tr("Json Parsing Error"),false);
        return;
    }

    //Json Parsing Error
    if(!jsonDoc.isObject()){
        showTip(tr("Json Parsing Error"),false);
        return;
    }

    _handlers[id](jsonDoc.object());

    return;
}

bool ResetDialog::checkUserValid()
{
    if(ui->user_edit->text() == ""){
        AddTipErr(TipErr::TIP_USER_ERR, tr("User name cannot be empty"));
        return false;
    }

    DelTipErr(TipErr::TIP_USER_ERR);
    return true;
}


bool ResetDialog::checkPassValid()
{
    auto pass = ui->pwd_edit->text();

    if(pass.length() < 6 || pass.length()>15){
        AddTipErr(TipErr::TIP_PWD_ERR, tr("Password length should be 6-15"));
        return false;
    }

    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*.]{6,15}$");
    bool match = regExp.match(pass).hasMatch();
    if(!match){
        //提示字符非法
        AddTipErr(TipErr::TIP_PWD_ERR, tr("Contain not valid symbol"));
        return false;;
    }

    DelTipErr(TipErr::TIP_PWD_ERR);

    return true;
}



bool ResetDialog::checkEmailValid()
{
    auto email = ui->email_edit->text();

    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch();
    if(!match){
        AddTipErr(TipErr::TIP_EMAIL_ERR, tr("Email address not correct"));
        return false;
    }

    DelTipErr(TipErr::TIP_EMAIL_ERR);
    return true;
}

bool ResetDialog::checkVerifyValid()
{
    auto pass = ui->verify_edit->text();
    if(pass.isEmpty()){
        AddTipErr(TipErr::TIP_VERIFY_ERR, tr("Verification code is empty"));
        return false;
    }

    DelTipErr(TipErr::TIP_VERIFY_ERR);
    return true;
}

void ResetDialog::AddTipErr(TipErr te, QString tips)
{
    _tip_errs[te] = tips;
    showTip(tips, false);
}

void ResetDialog::DelTipErr(TipErr te)
{
    _tip_errs.remove(te);
    if(_tip_errs.empty()){
      ui->err_tip->clear();
      return;
    }

    showTip(_tip_errs.first(), false);
}

void ResetDialog::initHandlers()
{
    _handlers.insert(ReqId::ID_GET_VERIFY_CODE, [this](QJsonObject jsonObj){
        int error = jsonObj["error"].toInt();
        if(error != ErrorCodes::SUCCESS){
            showTip(tr("Parameter error"),false);
            return;
        }
        auto email = jsonObj["email"].toString();
        showTip(tr("Send to mailbox successfully"), true);
        qDebug()<< "email is " << email ;
    });

    _handlers.insert(ReqId::ID_RESET_PWD, [this](QJsonObject jsonObj){
        int error = jsonObj["error"].toInt();
        if(error != ErrorCodes::SUCCESS){
            showTip(tr("Para error"),false);
            return;
        }
        auto email = jsonObj["email"].toString();
        showTip(tr("Reset successfully, return to login"), true);
        qDebug()<< "email is " << email ;
        qDebug()<< "user uuid is " <<  jsonObj["uuid"].toString();
    });
}

void ResetDialog::showTip(QString str, bool b_ok)
{
    if(b_ok){
         ui->err_tip->setProperty("state","normal");
    }else{
        ui->err_tip->setProperty("state","err");
    }

    ui->err_tip->setText(str);

    repolish(ui->err_tip);
}

void ResetDialog::on_sure_btn_clicked()
{
    bool valid = checkUserValid();
    if(!valid){
        return;
    }

    valid = checkEmailValid();
    if(!valid){
        return;
    }

    valid = checkPassValid();
    if(!valid){
        return;
    }

    valid = checkVerifyValid();
    if(!valid){
        return;
    }

    QJsonObject json_obj;
    json_obj["user"] = ui->user_edit->text();
    json_obj["email"] = ui->email_edit->text();
    json_obj["passwd"] = xorString(ui->pwd_edit->text());
    json_obj["verifycode"] = ui->verify_edit->text();
    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/reset_pwd"),
                 json_obj, ReqId::ID_RESET_PWD,Modules::RESETMOD);
}

void ResetDialog::on_cancel_btn_clicked()
{
    emit switchLogin();
}
