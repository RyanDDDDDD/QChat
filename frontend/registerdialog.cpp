#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "global.h"
#include "httpmgr.h"

RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterDialog)
    ,_countdown(5)
{
    ui->setupUi(this);

    ui->password_edit->setEchoMode(QLineEdit::Password);
    ui->confirm_edit->setEchoMode(QLineEdit::Password);

    ui->error_label->setProperty("state", "normal");
    repolish(ui->error_label);
    connect(HttpMgr::GetInstance().get(), &HttpMgr::sig_reg_mod_finish, this, &RegisterDialog::slot_reg_mod_finish);

    initHttpHandlers();
    ui->error_label->clear();

    connect(ui->user_edit,&QLineEdit::editingFinished,this,[this]{checkUserValid();});
    connect(ui->email_edit, &QLineEdit::editingFinished, this, [this]{checkEmailValid();});
    connect(ui->password_edit, &QLineEdit::editingFinished, this, [this]{checkPassValid();});
    connect(ui->confirm_edit, &QLineEdit::editingFinished, this, [this]{checkConfirmValid();});
    connect(ui->code_edit, &QLineEdit::editingFinished, this, [this]{checkVerifyValid();});

    ui->password_visible->setCursor(Qt::PointingHandCursor);
    ui->confirm_visible->setCursor(Qt::PointingHandCursor);

    ui->password_visible->SetState("unvisible","unvisible_hover","","visible",
                               "visible_hover","");

    ui->confirm_visible->SetState("unvisible","unvisible_hover","","visible",
                                  "visible_hover","");

    connect(ui->password_visible, &ClickedLabel::clicked, this, [this]{
        auto state = ui->password_visible->GetCurState();
        if (state == ClickedState::Normal) {
            ui->password_edit->setEchoMode(QLineEdit::Password);
        } else {
            ui->password_edit->setEchoMode(QLineEdit::Normal);
        }
        qDebug() << "Label was clicked!";
    });

    connect(ui->confirm_visible, &ClickedLabel::clicked, this, [this]{
        auto state = ui->confirm_visible->GetCurState();
        if (state == ClickedState::Normal) {
            ui->confirm_edit->setEchoMode(QLineEdit::Password);
        } else {
            ui->confirm_edit->setEchoMode(QLineEdit::Normal);
        }

        qDebug() << "Label was clicked!";
    });

    _countdown_timer = new QTimer(this);

    connect(_countdown_timer, &QTimer::timeout, [this]{
        if (_countdown == 0) {
            _countdown_timer->stop();
            emit sigSwitchLogin();
            return;
        }

        --_countdown;
        auto str = QString("Register successfully, return to login after %1 s").arg(_countdown);
        ui->tip_label->setText(str);
    });
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

    //user registration callback
    _handlers.insert(ReqId::ID_REG_USER, [this](const QJsonObject& jsonObj){
        int error = jsonObj["error"].toInt();
        if(error != ErrorCodes::SUCCESS){
            showTip(tr("Parameter error"),false);
            return;
        }


        auto email = jsonObj["email"].toString();
        showTip(tr("User register successfully"), true);
        qDebug() << "user uuid is " << jsonObj["uid"].toString();
        qDebug() << "email is " << email ;
        qDebug() << "registration end";
        ChangeTipPage();
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

void RegisterDialog::AddTipErr(TipErr te, QString tips)
{
    _tip_errs[te] = tips;
    showTip(tips, false);
}

void RegisterDialog::DelTipErr(TipErr te)
{
    _tip_errs.remove(te);
    if(_tip_errs.empty()){
        ui->error_label->clear();
        return;
    }

    showTip(_tip_errs.first(), false);
}

bool RegisterDialog::checkUserValid()
{
    if(ui->user_edit->text() == ""){
        AddTipErr(TipErr::TIP_USER_ERR, tr("uesr name is empty!!!"));
        return false;
    }

    DelTipErr(TipErr::TIP_USER_ERR);
    return true;
}

bool RegisterDialog::checkEmailValid()
{
    auto email = ui->email_edit->text();

    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch();
    if(!match){
        AddTipErr(TipErr::TIP_EMAIL_ERR, tr("email format error"));
        return false;
    }

    DelTipErr(TipErr::TIP_EMAIL_ERR);
    return true;
}

bool RegisterDialog::checkPassValid()
{
    auto pass = ui->confirm_edit->text();
    auto confirm = ui->confirm_edit->text();

    if(pass.length() < 6 || pass.length()>15){
        AddTipErr(TipErr::TIP_PWD_ERR, tr("Password length should be 6 to 15"));
        return false;
    }

    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*]{6,15}$");
    bool match = regExp.match(pass).hasMatch();
    if(!match){
        AddTipErr(TipErr::TIP_PWD_ERR, tr("Contains invalid char"));
        return false;;
    }

    DelTipErr(TipErr::TIP_PWD_ERR);

    if (pass != confirm) {
        AddTipErr(TipErr::TIP_PWD_CONFIRM, tr("Password and confirm not matched"));
        return false;
    } else {
        DelTipErr(TipErr::TIP_PWD_CONFIRM);
    }

    return true;
}

bool RegisterDialog::checkConfirmValid()
{
    auto pass = ui->confirm_edit->text();
    auto confirm = ui->confirm_edit->text();

    if(confirm.length() < 6 || confirm.length()>15){
        AddTipErr(TipErr::TIP_PWD_ERR, tr("Password length should be 6 to 15"));
        return false;
    }

    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*]{6,15}$");
    bool match = regExp.match(pass).hasMatch();
    if(!match){
        AddTipErr(TipErr::TIP_PWD_ERR, tr("Contains invalid char"));
        return false;;
    }

    DelTipErr(TipErr::TIP_PWD_CONFIRM);

    if (pass != confirm) {
        AddTipErr(TipErr::TIP_PWD_CONFIRM, tr("Password and confirm not matched"));
        return false;
    } else {
        DelTipErr(TipErr::TIP_PWD_CONFIRM);
    }

    return true;
}

bool RegisterDialog::checkVerifyValid()
{
    auto pass = ui->code_edit->text();
    if(pass.isEmpty()){
        AddTipErr(TipErr::TIP_VERIFY_ERR, tr("auth code is empty!!!"));
        return false;
    }

    DelTipErr(TipErr::TIP_VERIFY_ERR);
    return true;
}

void RegisterDialog::ChangeTipPage()
{
    _countdown_timer->stop();
    ui->stackedWidget->setCurrentWidget(ui->page_2);

    _countdown_timer->start(1000);
}

void RegisterDialog::on_verify_btn_clicked()
{
    auto email = ui->email_edit->text();
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch();

    if (match) {
        //send http validation code
        QJsonObject jsonObj;
        jsonObj["email"] = email;
        HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix + "/get_verifycode"), jsonObj, ReqId::ID_GET_VERIFY_CODE, Modules::REGISTERMOD);


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


void RegisterDialog::on_confirm_btn_clicked()
{
    if(ui->user_edit->text() == ""){
        showTip(tr("User name is empty"), false);
        return;
    }
    if(ui->email_edit->text() == ""){
        showTip(tr("Email is empty"), false);
        return;
    }
    if(ui->password_edit->text() == ""){
        showTip(tr("Password is empty"), false);
        return;
    }
    if(ui->confirm_edit->text() == ""){
        showTip(tr("Password confirm is empty"), false);
        return;
    }
    if(ui->confirm_edit->text() != ui->password_edit->text()){
        showTip(tr("Password and Password confirm does not match"), false);
        return;
    }
    if(ui->code_edit->text() == ""){
        showTip(tr("Verification code is empty"), false);
        return;
    }

    //send http request to gate server
    QJsonObject json_obj;
    json_obj["user"] = ui->user_edit->text();
    json_obj["email"] = ui->email_edit->text();
    json_obj["passwd"] = xorString(ui->password_edit->text());
    json_obj["confirm"] = xorString(ui->confirm_edit->text());
    json_obj["verifycode"] = ui->code_edit->text();
    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/user_register"),
                                        json_obj, ReqId::ID_REG_USER, Modules::REGISTERMOD);
}

void RegisterDialog::on_pushButton_clicked()
{
    _countdown_timer->stop();
    emit sigSwitchLogin();
}

