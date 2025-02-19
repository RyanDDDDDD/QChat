#pragma once
#include <boost/beast/http.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>

#include <hiredis/hiredis.h>

#include <atomic>
#include <memory>
#include <mutex>
#include <functional>
#include <map>
#include <unordered_map>
#include <string>
#include <queue>
#include <condition_variable>
#include <cassert>

#include "Singleton.h"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>


#define MAX_LENGTH  1024*2
//header length
#define HEAD_TOTAL_LEN 4
//header id length
#define HEAD_ID_LEN 2
//header data length
#define HEAD_DATA_LEN 2
#define MAX_RECVQUE  10000
#define MAX_SENDQUE 1000


enum MSG_IDS {
	MSG_CHAT_LOGIN = 1005, //login msg
	MSG_CHAT_LOGIN_RSP = 1006, //login response
	ID_SEARCH_USER_REQ = 1007, //search user id request
	ID_SEARCH_USER_RSP = 1008, //search user id response
	ID_ADD_FRIEND_REQ = 1009, //add friend request
	ID_ADD_FRIEND_RSP = 1010, //add friend response
	ID_NOTIFY_ADD_FRIEND_REQ = 1011,  // notify add friend request
	ID_AUTH_FRIEND_REQ = 1013,  //auth friend request
	ID_AUTH_FRIEND_RSP = 1014,  //auth friend response
	ID_NOTIFY_AUTH_FRIEND_REQ = 1015, //notify auth friend request
	ID_TEXT_CHAT_MSG_REQ = 1017, //chat msg request
	ID_TEXT_CHAT_MSG_RSP = 1018, //chat msg response
	ID_NOTIFY_TEXT_CHAT_MSG_REQ = 1019, // notify chat msg request
};

enum ErrorCodes {
	Success			= 0,
	Error_Json		= 1001,	//	Json parsing error
	RPCFailed		= 1002,	//	RRC request failed
	VerifyExpired	= 1003, //	authentication code expired
	VerifyCodeErr	= 1004, //	authentication code error
	UserExist		= 1005, //	user exist
	PasswdErr		= 1006, //	password error
	EmailNotMatch	= 1007, //	email not matched
	PasswdUpFailed	= 1008, //	Password update failed
	PasswdInvalid	= 1009, //	Password invalid
	TokenInvalid = 1010,	//Token invalid
	UidInvalid = 1011,		//uid invalid
};

class Defer {
public:
	Defer(std::function<void()> func): func_(func) {}

	~Defer() {
		func_();
	}

private:
	std::function<void()> func_;
};

#define CODEPREFIX "code_"
#define USERIPPREFIX  "uip_"
#define USERTOKENPREFIX  "utoken_"
#define IPCOUNTPREFIX  "ipcount_"
#define USER_BASE_INFO "ubaseinfo_"
#define LOGIN_COUNT  "logincount"
#define NAME_INFO  "nameinfo_"
