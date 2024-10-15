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
	UidInvalid		= 1010, // Uid invalid
};

#define CODEPREFIX "code_"

class Defer {
public:
	Defer(std::function<void()> func): func_(func) {}

	~Defer() {
		func_();
	}

private:
	std::function<void()> func_;
};
