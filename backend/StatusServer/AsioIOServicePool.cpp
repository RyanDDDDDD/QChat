#include "AsioIOServicePool.h"
#include <iostream>
using namespace std;
AsioIOServicePool::AsioIOServicePool(std::size_t size)
	:_ioServices(size),
	_works(size), 
	_nextIOService(0)
{
	//block each io_context before staring threads
	for (std::size_t i = 0; i < size; ++i) {
		_works[i] = std::unique_ptr<Work>(new Work(_ioServices[i]));
	}

	//create multipule threads on different io_service
	for (std::size_t i = 0; i < _ioServices.size(); ++i) {
		_threads.emplace_back([this, i]{
			_ioServices[i].run();
		});
	}
}

AsioIOServicePool::~AsioIOServicePool() {
	std::cout << "AsioIOServicePool destruct" << endl;
	Stop();
}

boost::asio::io_context& AsioIOServicePool::GetIOService() {
	auto& service = _ioServices[_nextIOService++];
	if (_nextIOService == _ioServices.size()) {
		_nextIOService = 0;
	}
	return service;
}

void AsioIOServicePool::Stop(){
	for (auto& work : _works) {
		//stop io_context before releasing it
		work->get_io_context().stop();
		work.reset();
	}

	for (auto& t : _threads) {
		if (t.joinable()) {
			t.join();
		}
	}
}
