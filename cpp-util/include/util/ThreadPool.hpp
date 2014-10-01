#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <thread>
#include <memory>
#include <vector>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>

namespace util {

class ThreadPool: public boost::noncopyable {
private:
	typedef std::shared_ptr<std::thread> ThreadPtr;

	boost::asio::io_service ioService;
	std::unique_ptr<boost::asio::io_service::work> work;
	std::vector<ThreadPtr> threads;
	bool running;
	std::size_t numThreads;

	void runInThread();
public:
	boost::asio::io_service& getIoService() { return ioService; }
	std::size_t getNumThreads() const { return numThreads; }
	void setNumThreads(std::size_t value);
	bool isRunning() const { return running; }
	void start();
	void wait();

	ThreadPool(std::size_t numThreads = 1):
		running(false),
		numThreads(numThreads)
	{
	}
};

class ThreadPoolRunner {
	ThreadPool& threadPool_;
public:
	ThreadPoolRunner(ThreadPool& threadPool):
		threadPool_(threadPool)
	{
		threadPool_.start();
	}
	~ThreadPoolRunner()
	{
		threadPool_.wait();
	}
};

} // namespace util

#endif /* THREADPOOL_H_ */
