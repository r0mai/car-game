#include <boost/test/unit_test.hpp>
#include "Finally.hpp"

using namespace util;

BOOST_AUTO_TEST_SUITE( finallyTest )

struct FinallyTester {
	bool called = false;

	void operator()() { called = true; }
};

BOOST_AUTO_TEST_CASE(finally_should_call_function_at_end)
{
	FinallyTester finallyTester;
	{
		auto f = finally(std::ref(finallyTester));
		BOOST_CHECK(!finallyTester.called);
	}
	BOOST_CHECK(finallyTester.called);
}

BOOST_AUTO_TEST_CASE(finally_should_call_function_at_end_even_if_exception_is_thrown)
{
	FinallyTester finallyTester;
	try {
		auto f = finally(std::ref(finallyTester));
		throw 1;
	} catch (...) {}
	BOOST_CHECK(finallyTester.called);
}

BOOST_AUTO_TEST_SUITE_END() // demangleTest



