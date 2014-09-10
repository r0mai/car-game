#ifndef SRC_LUA_LUA_HPP
#define SRC_LUA_LUA_HPP

#include <stdexcept>

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
private:
	lua_State* handle;
};

}


#endif /* SRC_LUA_LUA_HPP */
