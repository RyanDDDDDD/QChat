#include "LogicSystem.h"
#include <iostream>
#include <csignal>
#include <thread>
#include <mutex>
#include "AsioIOServicePool.h"
#include "CServer.h"
#include "ConfigMgr.h"
#include "RedisMgr.h"
#include "ChatServiceImpl.h"

bool bstop = false;
std::condition_variable cond_quit;
std::mutex mutex_quit;

int main()
{
	auto& cfg = ConfigMgr::Instance();
	auto serverName = cfg["SelfServer"]["Name"];

	try {
		auto pool = AsioIOServicePool::GetInstance();
		RedisMgr::GetInstance()->HSet(LOGIN_COUNT, serverName, "0");
		
		std::string serverAddress(cfg["SelfServer"]["Host"] + ":" + cfg["SelfServer"]["RPCPort"]);

		//launch grpc service
		ChatServiceImpl service;
		grpc::ServerBuilder builder;
		builder.AddListeningPort(serverAddress, grpc::InsecureServerCredentials());
		builder.RegisterService(&service);

		std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
		std::cout << "RPC server listening on" << serverAddress << std::endl;

		std::thread grpcServerThread([&server] {
			server->Wait();
		});

		boost::asio::io_context io_context;
		boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
		signals.async_wait([&server, &io_context, pool](auto, auto) {
			io_context.stop();
			pool->Stop();
			server->Shutdown();
		});

		auto port_str = cfg["SelfServer"]["Port"];

		unsigned short port = atoi(port_str.c_str());
		CServer server(io_context, port);

		io_context.run();

		RedisMgr::GetInstance()->HDel(LOGIN_COUNT, serverName);
		RedisMgr::GetInstance()->Close();

		grpcServerThread.join();
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}
}

