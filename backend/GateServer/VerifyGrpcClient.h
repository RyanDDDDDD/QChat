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

class VerifyGrpcClient : public Singleton<VerifyGrpcClient>
{
public:
	GetVerifyRsp GetVerifyCode(std::string email) {
		ClientContext context;
		GetVerifyRsp response;
		GetVerifyReq request;
		request.set_email(email);

		Status status = stub_->GetVerifyCode(&context, request, &response);
		if (status.ok())
			return response;

		response.set_error(ErrorCodes::RPCFailed);
		return response;
	}

	virtual ~VerifyGrpcClient() = default;
private:
	friend class Singleton<VerifyGrpcClient>;
	VerifyGrpcClient() {
		std::shared_ptr<Channel> channel = grpc::CreateChannel("127.0.0.1:50051", grpc::InsecureChannelCredentials());

		stub_ = VerifyService::NewStub(channel);
	};

	std::unique_ptr<VerifyService::Stub> stub_;
};

