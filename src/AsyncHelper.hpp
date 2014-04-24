#ifndef ASYNCHELPER_HPP_
#define ASYNCHELPER_HPP_

#include <future>
#include <memory>
#include <type_traits>
#include <utility>

namespace car {

template <typename Function>
class AsyncHelper {
public:
	AsyncHelper(Function function): function(std::move(function)) {}

	std::future<void> getFuture() {
		return promise->get_future();
	}

	void operator()() {
		function();
		promise->set_value();
	}
private:
	Function function;
	std::shared_ptr<std::promise<void>> promise =
			std::make_shared<std::promise<void>>();
};

template <typename Function>
AsyncHelper<Function> asyncHelper(Function function) {
	return AsyncHelper<Function>(std::move(function));
}

}


#endif /* ASYNCHELPER_HPP_ */
