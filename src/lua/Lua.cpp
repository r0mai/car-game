#include "Lua.hpp"
#include <lua5.1/lua.hpp>

namespace lua {

Lua::Lua() {
	handle = luaL_newstate();
	if (!handle) {
		throw std::bad_alloc();
	}
	luaL_openlibs(handle);
}

Lua::~Lua() {
	lua_close(handle);
}

void Lua::loadFile(const char* name) {
	handleError(luaL_loadfile(handle, name));
	handleError(lua_pcall(handle, 0, 0, 0));
}

void Lua::loadString(const char* str) {
	handleError(luaL_loadstring(handle, str));
	handleError(lua_pcall(handle, 0, 0, 0));
}

void Lua::pushGlobalVariable(const char* str) {
	lua_getglobal(handle, str);
}

namespace {

class PushVisitor {
public:
	using result_type = void;

	PushVisitor(lua_State* handle): handle(handle) {}

	result_type operator()(Nil) const { lua_pushnil(handle); }
	result_type operator()(bool value) const { lua_pushboolean(handle, value); }
	result_type operator()(double value) const { lua_pushnumber(handle, value); }
	result_type operator()(const std::string& value) const { lua_pushstring(handle, value.c_str()); }
private:
	lua_State* handle;
};

}

void Lua::pushValue(const Data& data) {
	boost::apply_visitor(PushVisitor(handle), data);
}

Data Lua::getValue(int index) {
	switch(lua_type(handle, index)) {
	case LUA_TNONE:
	case LUA_TNIL:
		return Nil{};
	case LUA_TBOOLEAN:
		return static_cast<bool>(lua_toboolean(handle, index));
	case LUA_TNUMBER:
		return lua_tonumber(handle, index);
	case LUA_TSTRING:
		return lua_tostring(handle, index);
	default:
		throw UnsupportedType{};
	}
}

void Lua::pop(int num) {
	lua_pop(handle, num);
}

void Lua::handleError(int returnValue) {
	switch (returnValue) {
	case 0:
		return;
	case LUA_ERRRUN:
	case LUA_ERRERR:
		throw RuntimeError{lua_tostring(handle, -1)};
	case LUA_ERRSYNTAX:
		throw SyntaxError{lua_tostring(handle, -1)};
	case LUA_ERRFILE:
		throw FileError{lua_tostring(handle, -1)};
	case LUA_ERRMEM:
		throw std::bad_alloc{};
	default:
		assert(false && "Invalid return type");
	}
}

}

