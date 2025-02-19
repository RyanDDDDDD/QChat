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
    ID_SEARCH_USER_REQ = 1007,  // user search request
    ID_SEARCH_USER_RSP = 1008,  // user search response
    ID_ADD_FRIEND_REQ = 1009,   // add friend request
    ID_ADD_FRIEND_RSP = 1010,   // add friend response
    ID_NOTIFY_ADD_FRIEND_REQ = 1011,  //notify user request
    ID_AUTH_FRIEND_REQ = 1013,  //auth friend request
    ID_AUTH_FRIEND_RSP = 1014,  //auth friend response
    ID_NOTIFY_AUTH_FRIEND_REQ = 1015, //notify add friend auth request
    ID_TEXT_CHAT_MSG_REQ  = 1017,  //chat msg request
    ID_TEXT_CHAT_MSG_RSP  = 1018,  //chat msg response
    ID_NOTIFY_TEXT_CHAT_MSG_REQ = 1019, //notify chat msg request
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

enum ChatUIMode{
    SearchMode,
    ChatMode,
    ContactMode,
};

enum ListItemType {
    CHAT_USER_ITEM,
    CONTACT_USER_ITEM,
    SEARCH_USER_ITEM,
    ADD_USER_TIP_ITEM,
    INVALID_ITEM,
    GROUP_TIP_ITEM,
    LINE_ITEM,
    APPLY_FRIEND_ITEM
};

const int CHAT_COUNT_PER_PAGE = 13;

const int MIN_APPLY_LABEL_ED_LEN = 40;

const QString add_prefix = "add alias ";

const int  tip_offset = 5;

const std::vector<QString>  strs ={"hello world !",
                                   "nice to meet u",
                                   "New year，new life",
                                   "You have to love yourself",
                                   "My love is written in the wind ever since the whole world is you"};

const std::vector<QString> heads = {
    ":/res/head_1.jpg",
    ":/res/head_2.jpg",
    ":/res/head_3.jpg",
    ":/res/head_4.jpg",
    ":/res/head_5.jpg"
};

const std::vector<QString> names = {
    "HanMeiMei",
    "Lily",
    "Ben",
    "Androw",
    "Max",
    "Summer",
    "Candy",
    "Hunter"
};

enum class ChatRole
{
    Self,
    Other
};

struct MsgInfo{
    QString msgFlag;
    QString content;
    QPixmap pixmap;
};

#endif // GLOBAL_H
