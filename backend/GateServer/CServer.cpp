#include "CServer.h"
#include "HttpConnection.h"
#include "AsioIOServicePool.h"

CServer::CServer(boost::asio::io_context& ioc, unsigned short& port)
	:_ioc(ioc),
	_acceptor(ioc, tcp::endpoint(tcp::v4(), port))
{

};

void CServer::Start() {
	auto self = shared_from_this();
	auto& io_context = AsioIOServicePool::GetInstance()->GetIOService();
	std::shared_ptr<HttpConnection> new_con = std::make_shared<HttpConnection>(io_context);

	_acceptor.async_accept(new_con->GetSocket(), [self, new_con](beast::error_code ec) {
		try {
			//error in connection, try listen other connection
			if (ec) {
				self->Start();
				return;
			}
			
			//handle current http request
			new_con->Start();
			
			//listen next request
			self->Start();
		}
		catch (std::exception& exp) {
			std::cerr << "exception is " << exp.what() << std::endl;
		}
	});
}