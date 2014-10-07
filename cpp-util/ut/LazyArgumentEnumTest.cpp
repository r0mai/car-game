#include <boost/test/unit_test.hpp>
#include <sstream>
#include "LazyArgumentEnum.hpp"
#include "PrefixMap.hpp"

using namespace util;

BOOST_AUTO_TEST_SUITE(LazyArgumentMapTest)

LAZY_ARGUMENT_ENUM(Number, numbers, (zero)(one)(two))

BOOST_AUTO_TEST_CASE(LAZY_ARGUMENT_ENUM_shoud_instantiate_enum) {
	BOOST_CHECK_EQUAL(static_cast<int>(Number::zero), 0);
	BOOST_CHECK_EQUAL(static_cast<int>(Number::one), 1);
	BOOST_CHECK_EQUAL(static_cast<int>(Number::two), 2);
}

BOOST_AUTO_TEST_CASE(LAZY_ARGUMENT_ENUM_shoud_create_getter_function) {
	BOOST_CHECK_EQUAL(numbers().at("zero"), Number::zero);
	BOOST_CHECK_EQUAL(numbers().at("one"), Number::one);
	BOOST_CHECK_EQUAL(numbers().at("two"), Number::two);
}

BOOST_AUTO_TEST_CASE(LAZY_ARGUMENT_ENUM_shoud_create_PrefixMap_getter_function) {
	BOOST_CHECK_EQUAL(numbers().at("z"), Number::zero);
	BOOST_CHECK_EQUAL(numbers().at("o"), Number::one);
	BOOST_CHECK_EQUAL(numbers().at("t"), Number::two);
}

BOOST_AUTO_TEST_CASE(LAZY_ARGUMENT_ENUM_shoud_create_input_function) {
	{
		std::istringstream ss("zero");
		Number n;
		ss >> n;
		BOOST_CHECK_EQUAL(n, Number::zero);
	}
	{
		std::istringstream ss("o");
		Number n;
		ss >> n;
		BOOST_CHECK_EQUAL(n, Number::one);
	}
	{
		std::istringstream ss("tw");
		Number n;
		ss >> n;
		BOOST_CHECK_EQUAL(n, Number::two);
	}
}

BOOST_AUTO_TEST_CASE(LAZY_ARGUMENT_ENUM_shoud_create_output_function) {
	{
		std::ostringstream ss;
		ss << Number::zero;
		BOOST_CHECK_EQUAL(ss.str(), "zero");
	}
	{
		std::ostringstream ss;
		ss << Number::one;
		BOOST_CHECK_EQUAL(ss.str(), "one");
	}
	{
		std::ostringstream ss;
		ss << Number::two;
		BOOST_CHECK_EQUAL(ss.str(), "two");
	}
}

BOOST_AUTO_TEST_SUITE_END()


