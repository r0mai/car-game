#include "Lua.hpp"
#include <lua5.1/lua.hpp>

namespace lua {

Lua::Lua() {
	handle = lua_open();
	if (!handle) {
		throw std::bad_alloc();
	}
	luaL_openlibs(handle);
}

Lua::~Lua() {
	lua_close(handle);
}

}

