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

BOOST_AUTO_TEST_SUITE_END()
