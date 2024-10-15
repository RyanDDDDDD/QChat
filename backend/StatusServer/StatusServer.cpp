#include <iostream>
#include <json/json.h>
#include <json/value.h>
#include <json/reader.h>
#include "const.h"
#include "ConfigMgr.h"
#include "hiredis.h"
#include "RedisMgr.h"
#include "MysqlMgr.h"
#include "AsioIOServicePool.h"
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <boost/asio.hpp>
#include "StatusServiceImpl.h"


void RunServer() {
    auto& cfg = ConfigMgr::Instance();
    std::string server_address(cfg["StatusServer"]["Host"] + ":" + cfg["StatusServer"]["Port"]);
    StatusServiceImpl service;
    grpc::ServerBuilder builder;

    // listen port and add service
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    // build and lanuch grpc server
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    // create boost.asio ioservice
    boost::asio::io_context io_context;

    // create signal to capture interupt
    boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);

    // setup signal to wait asynchronously
    signals.async_wait([&server](const boost::system::error_code& error, int signal_number) {
        if (!error) {
            std::cout << "Shutting down server..." << std::endl;
            server->Shutdown();
        }
    });

    // run iocontext in diameon thread
    std::thread([&io_context]() { io_context.run(); }).detach();

    // wait for server to shutdown
    server->Wait();
    io_context.stop();
}

int main(int argc, char** argv) {
    try {
        RunServer();
    }
    catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return 0;
}