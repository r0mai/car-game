#ifndef SRC_LUA_LUA_HPP
#define SRC_LUA_LUA_HPP

#include <stdexcept>
#include <string>

extern "C" {
	struct lua_State;
}

namespace lua {

struct Exception : std::runtime_error {
	using std::runtime_error::runtime_error;
};

class Lua {
public:
	Lua();
	~Lua();

	void loadFile(const char* name);
	void loadFile(const std::string& name) { loadFile(name.c_str()); }

	void loadString(const char* str);
	void loadString(const std::string& str) { loadString(str.c_str()); }
private:
	lua_State* handle;

	void initFunction(const char* name);
};

}


#endif /* SRC_LUA_LUA_HPP */
