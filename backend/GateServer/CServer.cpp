#include "CServer.h"

#include "HttpConnection.h"

CServer::CServer(boost::asio::io_context& ioc, unsigned short& port)
	:_ioc(ioc),
	_acceptor(ioc, tcp::endpoint(tcp::v4(), port)),
	_socket(ioc)
{

};

void CServer::Start() {
	auto self = shared_from_this();
	_acceptor.async_accept(_socket, [self](beast::error_code ec) {
		try {
			//error in connection, try listen other connection
			if (ec) {
				self->Start();
				return;
			}
			
			//handle current http request
			std::make_shared<HttpConnection>(std::move(self->_socket))->Start();
			
			//listen next request
			self->Start();
		}
		catch (std::exception& exp) {
			std::cerr << "exception is " << exp.what() << std::endl;
		}
	});
}