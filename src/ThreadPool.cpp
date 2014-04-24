#include "ThreadPool.hpp"
#include <functional>
#include <boost/exception/all.hpp>
#include <iostream>

void ThreadPool::runInThread()
{
	while (true) {
		try {
			ioService.run();
			break;
		} catch (std::exception &e) {
			std::cerr << boost::diagnostic_information(e) << std::endl;
		}
		// retry
	}
}

void ThreadPool::setNumThreads(std::size_t value)
{
	if (numThreads == value) {
		return;
	}
	bool wasRunning = running;
	if (wasRunning) {
		wait();
	}
	numThreads = value;
	if (wasRunning) {
		start();
	}

}

void ThreadPool::start()
{
	if (!running && numThreads > 0) {
		work.reset(new boost::asio::io_service::work(ioService));
		threads.reserve(numThreads);
		while (threads.size() < numThreads) {
			threads.push_back(std::make_shared<std::thread>([this]() {
					runInThread();
				})
			);
		}
		running = true;
	}
}

void ThreadPool::wait()
{
	if (running) {
		work.reset();
		for(auto t: threads) {
			t->join();
		}
		threads.clear();
		ioService.reset();
		running = false;
	}
}

