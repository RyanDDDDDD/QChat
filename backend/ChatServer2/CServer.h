#pragma once

#include <boost/asio.hpp>
#include "CSession.h"
#include <memory>
#include <map>
#include <mutex>

using namespace std;
using boost::asio::ip::tcp;

class CServer : public std::enable_shared_from_this<CServer>
{
public:
	CServer(boost::asio::io_context& io_context, unsigned short& port);
	~CServer();
	void ClearSession(std::string session_id);

private:
	void HandleAccept(shared_ptr<CSession>, const boost::system::error_code& error);
	void StartAccept();

private:	
	boost::asio::io_context& _io_context;
	short _port;
	tcp::acceptor _acceptor;
	std::map<std::string, shared_ptr<CSession>> _sessions;
	std::mutex _mutex;
};

