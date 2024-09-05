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

/**
 * @brief repolish
 */
extern std::function<void(QWidget*)> repolish;

enum ReqId{
    ID_GET_VERIFY_CODE  = 1001, // get validation code
    ID_REG_USER         = 1002, // user registeration
};

enum Modules{
    REGISTERMOD         = 0,
};

enum ErrorCodes{
    SUCCESS             = 0,
    ERR_JSON            = 1, // json parsing failed
    ERR_NETWORK         = 2, // network error


};


#endif // GLOBAL_H
