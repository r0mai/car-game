#ifndef SRC_LUA_LUA_HPP
#define SRC_LUA_LUA_HPP

#include <stdexcept>
#include <string>
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

using Data = boost::variant<Nil, bool, double, std::string>;

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
