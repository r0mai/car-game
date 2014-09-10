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

void Lua::loadFile(const char* name) {
	if (luaL_dofile(handle, name)) {
		throw Exception{lua_tostring(handle, -1)};
	}
}

void Lua::loadString(const char* str) {
	if (luaL_dostring(handle, str)) {
		throw Exception{lua_tostring(handle, -1)};
	}
}


}

