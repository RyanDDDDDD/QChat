#pragma once

#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include "const.h"
#include "Singleton.h"

using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;

using message::GetVerifyReq;
using message::GetVerifyRsp;
using message::VerifyService;

class RPConPool {
public:
	RPConPool(size_t poolsize, std::string host, std::string port);
	~RPConPool();
		
	void Close();
	std::unique_ptr<VerifyService::Stub> getConnection();
	void returnConnection(std::unique_ptr<VerifyService::Stub> context);

private:
	std::atomic<bool> b_stop_;
	size_t poolSize_;
	std::string host_;
	std::string port_;
	std::queue<std::unique_ptr<VerifyService::Stub>> connections_;
	std::condition_variable cond_;
	std::mutex mutex_;
};

class VerifyGrpcClient : public Singleton<VerifyGrpcClient>
{
public:
	GetVerifyRsp GetVerifyCode(std::string email);
	virtual ~VerifyGrpcClient() = default;

private:
	friend class Singleton<VerifyGrpcClient>;
	VerifyGrpcClient();

private:
	std::unique_ptr<RPConPool> pool_;
};

