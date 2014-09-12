#ifndef SRC_LUA_LUA_HPP
#define SRC_LUA_LUA_HPP

#include <stdexcept>
#include <string>
#include <ostream>
#include <map>
#include <boost/lexical_cast.hpp>
#include <boost/variant.hpp>

extern "C" {
	struct lua_State;
}

namespace lua {

struct Exception : std::exception {
	std::string errorMessage;

	Exception(std::string errorMessage): errorMessage(std::move(errorMessage)) {}

	virtual const char* what() const noexcept override {
		return errorMessage.c_str();
	}
};

struct RuntimeError : Exception {
	using Exception::Exception;
};

struct FileError : Exception {
	using Exception::Exception;
};

struct SyntaxError : Exception {
	using Exception::Exception;
};

struct UnsupportedType : Exception {
	UnsupportedType(): Exception{"Unsupported Lua type"} {}
};

struct Nil {};

inline bool operator==(Nil, Nil) { return true; }
inline bool operator<(Nil, Nil) { return false; }
inline bool operator<=(Nil, Nil) { return true; }
inline bool operator>(Nil, Nil) { return false; }
inline bool operator>=(Nil, Nil) { return true; }

using Data = boost::make_recursive_variant<Nil, bool, double, std::string,
		std::map<boost::recursive_variant_, boost::recursive_variant_>>::type;

using Table = std::map<Data, Data>;

std::ostream& operator<<(std::ostream& os, Nil);
std::ostream& operator<<(std::ostream& os, const Table& table);

class Lua {
public:
	Lua();
	~Lua();

	Lua(const Lua&) = delete;
	Lua& operator=(const Lua&) = delete;
	Lua(Lua&&) = default;
	Lua& operator=(Lua&&) = default;

	void loadFile(const char* name);
	void loadFile(const std::string& name) { loadFile(name.c_str()); }

	void loadString(const char* str);
	void loadString(const std::string& str) { loadString(str.c_str()); }

	void callFunction(const char* name, const std::vector<Data>& args, std::vector<Data>* result = nullptr);
	void callFunction(const std::string& name, const std::vector<Data>& args, std::vector<Data>* result = nullptr) {
		return callFunction(name.c_str(), args, result);
	}

private:
	lua_State* handle;

	void pushGlobalVariable(const char* name);
	void pushValue(const Data& data);
	Data getValue(int index);
	void pop(int num);

	void handleError(int returnValue);
};

}


#endif /* SRC_LUA_LUA_HPP */
