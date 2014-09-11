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
		return std::string{lua_tostring(handle, index)};
	default:
		throw UnsupportedType{};
	}
}

void Lua::pop(int num) {
	lua_pop(handle, num);
}

void Lua::callFunction(const char* name, const std::vector<Data>& args, std::vector<Data>* result) {
	pushGlobalVariable(name);
	for (const auto& arg: args) {
		pushValue(arg);
	}

	std::size_t numReturn = result ? result->size() : 0;
	handleError(lua_pcall(handle, args.size(), numReturn, 0));

	if (result) {
		std::size_t stackSize = lua_gettop(handle);
		assert(stackSize >= numReturn);

		for (std::size_t i = 0; i < numReturn; ++i) {
			(*result)[i] = getValue(stackSize - numReturn + i + 1);
		}

		pop(numReturn);
	}
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

namespace {

class PrintVisitor {
public:
	using result_type = std::ostream&;

	PrintVisitor(std::ostream& os): os(os) {}

	result_type operator()(Nil) const { return os << "Nil"; }
	template <typename T>
	result_type operator()(T value) const { return os << value; }
private:
	std::ostream& os;
};

}

std::ostream& operator<<(std::ostream& os, const Data& data) {
	return boost::apply_visitor(PrintVisitor{os}, data);
}

}

