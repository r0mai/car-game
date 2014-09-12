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

BOOST_AUTO_TEST_CASE(callFunction_return_string) {
	Lua l;
	std::string returnValue = "some return value";
	l.loadString("function f() return \"" + returnValue + "\" end");
	std::vector<Data> result = {{}};
	l.callFunction("f", {}, &result);
	BOOST_CHECK_EQUAL(boost::get<std::string>(result[0]), returnValue);
}

BOOST_AUTO_TEST_CASE(callFunction_return_bool) {
	Lua l;
	l.loadString("function f() return true end");
	std::vector<Data> result = {{}};
	l.callFunction("f", {}, &result);
	BOOST_CHECK_EQUAL(boost::get<bool>(result[0]), true);
}

BOOST_AUTO_TEST_CASE(callFunction_return_number) {
	Lua l;
	double returnValue = 43.1;
	l.loadString("function f() return 43.1 end");
	std::vector<Data> result = {{}};
	l.callFunction("f", {}, &result);
	BOOST_CHECK_CLOSE(boost::get<double>(result[0]), returnValue, 0.001);
}

BOOST_AUTO_TEST_CASE(callFunction_return_table) {
	Lua l;
	l.loadString(R"phi(
		function f()
			result = {}
			result[2] = "a"
			result["foo"] = "b"
			result[true] = "c"
			return result
		end
		)phi");
	std::vector<Data> result = {{}};
	l.callFunction("f", {}, &result);
	Table table = boost::get<Table>(result[0]);
	BOOST_CHECK_EQUAL(boost::get<std::string>(table.at(2.0)), "a");
	BOOST_CHECK_EQUAL(boost::get<std::string>(table.at(std::string("foo"))), "b");
	BOOST_CHECK_EQUAL(boost::get<std::string>(table.at(true)), "c");
}

BOOST_AUTO_TEST_CASE(callFunction_return_table_recursive) {
	Lua l;
	l.loadString(R"phi(
		function f()
			result = {}
			result.foo = {}
			result.foo[1] = "a"
			result.foo["bar"] = "b"
			return result
		end
		)phi");
	std::vector<Data> result = {{}};
	l.callFunction("f", {}, &result);
	Table table = boost::get<Table>(boost::get<Table>(result[0]).at(std::string("foo")));
	BOOST_CHECK_EQUAL(boost::get<std::string>(table.at(1.0)), "a");
	BOOST_CHECK_EQUAL(boost::get<std::string>(table.at(std::string("bar"))), "b");
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

BOOST_AUTO_TEST_CASE(callFunction_arg_table) {
	Lua l;
	l.loadString("function add(arg) return arg[1] + arg[\"foo\"] + arg[false] end");
	std::vector<Data> result = {{}};
	Table table;
	table.emplace(std::string{"foo"}, 10.0);
	table.emplace(1.0, 45.0);
	table.emplace(false, 5.0);
	table.emplace(std::string{"bar"}, 1231.0);
	l.callFunction("add", {table}, &result);
	BOOST_CHECK_CLOSE(boost::get<double>(result[0]),
			boost::get<double>(table.at(1.0)) +
			boost::get<double>(table.at(std::string{"foo"})) +
			boost::get<double>(table.at(false)), 0.001);
}

BOOST_AUTO_TEST_CASE(callFunction_arg_table_recursive) {
	Lua l;
	double value = 123.0;
	l.loadString("function double(arg) return arg.foo.bar * 2 end");
	std::vector<Data> result = {{}};
	Table outerTable;
	Table innerTable;
	innerTable.emplace(std::string("bar"), value);
	outerTable.emplace(std::string("foo"), innerTable);
	l.callFunction("double", {outerTable}, &result);
	BOOST_CHECK_CLOSE(boost::get<double>(result[0]), 2 * value, 0.001);
}

BOOST_AUTO_TEST_SUITE_END()
