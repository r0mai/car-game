#include <boost/test/unit_test.hpp>
#include "lua/Lua.hpp"

using namespace lua;

BOOST_AUTO_TEST_SUITE(LuaTest)

BOOST_AUTO_TEST_CASE(loadString) {
	Lua l;
	l.loadString("a=1");
}

BOOST_AUTO_TEST_CASE(loadString_invalid_expression) {
	Lua l;
	BOOST_CHECK_THROW(l.loadString("this is not a valid lua expression"),
			SyntaxError);
}

BOOST_AUTO_TEST_CASE(loadFile) {
	Lua l;
	l.loadFile("test.lua");
}

BOOST_AUTO_TEST_CASE(loadFile_not_exists) {
	Lua l;
	BOOST_CHECK_THROW(l.loadFile("invalid_file_name.lua"), FileError);
}

BOOST_AUTO_TEST_CASE(loadFile_invalid_expression) {
	Lua l;
	BOOST_CHECK_THROW(l.loadFile("bad.lua"), SyntaxError);
}

BOOST_AUTO_TEST_CASE(runtimeError) {
	Lua l;
	std::string message = "some message";
	BOOST_CHECK_THROW(l.loadString("error(\"" + message + "\")"), RuntimeError);
}

BOOST_AUTO_TEST_CASE(callFunction_returnString) {
	Lua l;
	std::string returnValue = "some return value";
	l.loadString("function f() return \"" + returnValue + "\" end");
	std::vector<Data> result = {{}};
	l.callFunction("f", {}, &result);
	BOOST_CHECK_EQUAL(boost::get<std::string>(result[0]), returnValue);
}

BOOST_AUTO_TEST_CASE(callFunction_returnBool) {
	Lua l;
	l.loadString("function f() return true end");
	std::vector<Data> result = {{}};
	l.callFunction("f", {}, &result);
	BOOST_CHECK_EQUAL(boost::get<bool>(result[0]), true);
}

BOOST_AUTO_TEST_CASE(callFunction_returnNumber) {
	Lua l;
	double returnValue = 43.1;
	l.loadString("function f() return 43.1 end");
	std::vector<Data> result = {{}};
	l.callFunction("f", {}, &result);
	BOOST_CHECK_CLOSE(boost::get<double>(result[0]), returnValue, 0.001);
}

BOOST_AUTO_TEST_CASE(callFunction_arg_number) {
	Lua l;
	double arg1 = 11.3;
	double arg2 = 35.44;
	l.loadString("function add(arg1, arg2) return arg1 + arg2 end");
	std::vector<Data> result = {{}};
	l.callFunction("add", {arg1, arg2}, &result);
	BOOST_CHECK_CLOSE(boost::get<double>(result[0]), arg1 + arg2, 0.001);
}

BOOST_AUTO_TEST_CASE(callFunction_arg_string) {
	Lua l;
	std::string arg1 = "first";
	std::string arg2 = "second";
	l.loadString("function cat(arg1, arg2) return arg1 .. arg2 end");
	std::vector<Data> result = {{}};
	l.callFunction("cat", {arg1, arg2}, &result);
	BOOST_CHECK_EQUAL(boost::get<std::string>(result[0]), arg1 + arg2);
}

BOOST_AUTO_TEST_CASE(callFunction_arg_bool) {
	Lua l;
	bool arg = true;
	l.loadString("function _not(arg) return not arg end");
	std::vector<Data> result = {{}};
	l.callFunction("_not", {arg}, &result);
	BOOST_CHECK_EQUAL(boost::get<bool>(result[0]), false);
}

BOOST_AUTO_TEST_CASE(callFunction_arg_nil) {
	Lua l;
	l.loadString("function _isnil(arg) return arg == Nil end");
	std::vector<Data> result = {{}};
	l.callFunction("_isnil", {Nil{}}, &result);
	BOOST_CHECK_EQUAL(boost::get<bool>(result[0]), true);
}

BOOST_AUTO_TEST_SUITE_END()
