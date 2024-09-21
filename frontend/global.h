#ifndef GLOBAL_H
#define GLOBAL_H

#include <functional>
#include <iostream>
#include <mutex>
#include <memory>

#include <QWidget>
#include <QStyle>
#include <QRegularExpression>
#include <QByteArray>
#include <QNetworkReply>
#include <QJsonObject>
#include <QDir>
#include <QSettings>
#include <QDebug>

/**
 * @brief repolish
 */
extern std::function<void(QWidget*)> repolish;
extern std::function<QString(QString)> xorString;

enum ReqId{
    ID_GET_VERIFY_CODE  = 1001, // get validation code
    ID_REG_USER         = 1002, // user registeration
    ID_RESET_PWD        = 1003, // reset password
    ID_LOGIN_USER       = 1004, // user login
    ID_CHAT_LOGIN       = 1005, // login chat server
    ID_CHAT_LOGIN_RSP   = 1006, // response package from server
};

enum Modules{
    REGISTERMOD         = 0,
    RESETMOD            = 1,
    LOGINMOD            = 2
};

enum ErrorCodes{
    SUCCESS             = 0,
    ERR_JSON            = 1, // json parsing failed
    ERR_NETWORK         = 2, // network error
};

enum TipErr{
    TIP_SUCCESS = 0,
    TIP_EMAIL_ERR = 1,
    TIP_PWD_ERR = 2,
    TIP_CONFIRM_ERR = 3,
    TIP_PWD_CONFIRM = 4,
    TIP_VERIFY_ERR = 5,
    TIP_USER_ERR = 6
};

enum ClickedState{
    Normal = 0,
    Selected = 1
};

extern QString gate_url_prefix;

struct ServerInfo {
    QString Host;
    QString Port;
    QString Token;
    int Uid;
};

#endif // GLOBAL_H
