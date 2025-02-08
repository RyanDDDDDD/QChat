#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp >

#include "StatusServiceImpl.h"
#include "ConfigMgr.h"
#include "const.h"
#include <string>

std::string generate_unique_string() {
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    std::string unique_string = to_string(uuid);
    return unique_string;
}

Status StatusServiceImpl::GetChatServer(ServerContext* context, const GetChatServerReq* request, GetChatServerRsp* reply)
{
    std::string prefix("Status server has received :  ");
    const auto& server = getChatServer();
    reply->set_host(server.host);
    reply->set_port(server.port);
    reply->set_error(ErrorCodes::Success);
    reply->set_token(generate_unique_string());
    insertToken(request->uid(), reply->token());
    return Status::OK;
}

StatusServiceImpl::StatusServiceImpl()
{
    auto& cfg = ConfigMgr::Instance();
    ChatServer server;
    server.port = cfg["ChatServer1"]["Port"];
    server.host = cfg["ChatServer1"]["Host"];
    server.con_count = 0;
    server.name = cfg["ChatServer1"]["Name"];
    _servers[server.name] = server;

    server.port = cfg["ChatServer2"]["Port"];
    server.host = cfg["ChatServer2"]["Host"];
    _servers[server.name] = server;
}

ChatServer StatusServiceImpl::getChatServer() 
{
    //least connection method to return chat server
    std::lock_guard<std::mutex> guard(_server_mtx);
    auto minServer = _servers.begin()->second;
    for (const auto& server : _servers) {
        if (server.second.con_count < minServer.con_count) {
            minServer = server.second;
        }
    }

    return minServer;
}

Status StatusServiceImpl::Login(ServerContext* context, const LoginReq* request, LoginRsp* reply) 
{
    auto uid = request->uid();
    auto token = request->token();
    std::lock_guard<std::mutex> guard(_token_mtx);
    auto iter = _tokens.find(uid);

    if (iter == _tokens.end()) {
        reply->set_error(ErrorCodes::UidInvalid);
        return Status::OK;
    }

    if (iter->second != token) {
        reply->set_error(ErrorCodes::UidInvalid);
        return Status::OK;
    }

    reply->set_error(ErrorCodes::Success);
    reply->set_uid(uid);
    reply->set_token(token);
    return Status::OK;
}

void StatusServiceImpl::insertToken(int uid, std::string token) 
{
    std::lock_guard<std::mutex> guard(_token_mtx);
    _tokens[uid] = token;
}