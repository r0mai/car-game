#ifndef TYPEMANIP_DEMANGLE_HPP_
#define TYPEMANIP_DEMANGLE_HPP_

#include <cxxabi.h>
#include <string>
#include <memory>

namespace util {

inline std::string demangle(const char *s) {
	std::shared_ptr<char> result0{abi::__cxa_demangle(s, 0, 0, NULL), free};
	return std::string{result0.get()};
}

template<typename T>
inline std::string type_name() {
	return demangle(typeid(T).name());
}
template<typename T>
inline std::string type_name(const T&) {
	return demangle(typeid(T).name());
}

} // namespace typemanip

#endif /* TYPEMANIP_DEMANGLE_HPP_ */
