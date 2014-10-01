#include <boost/test/unit_test.hpp>
#include "LazyArgumentMap.hpp"
#include "PrefixMap.hpp"

using namespace util;

BOOST_AUTO_TEST_SUITE(LazyArgumentMapTest)

LAZY_ARGUMENT_MAP(int, lazyArgumentMapUnderTest) {
	return {{"one", 1}, {"two", 2}};
}

LAZY_ARGUMENT_PREFIX_MAP(int, lazyArgumentPrefixMapUnderTest) {
	return {{"one", 1}, {"two", 2}};
}

BOOST_AUTO_TEST_CASE(LAZY_ARGUMENT_MAP_should_instantiate_std_map) {
	BOOST_CHECK_EQUAL(lazyArgumentMapUnderTest().at("one"), 1);
	BOOST_CHECK_EQUAL(lazyArgumentMapUnderTest().at("two"), 2);
	BOOST_CHECK_THROW(lazyArgumentMapUnderTest().at("o"), std::out_of_range);
	BOOST_CHECK_THROW(lazyArgumentMapUnderTest().at("t"), std::out_of_range);
	BOOST_CHECK_THROW(lazyArgumentMapUnderTest().at("xxx"), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(LAZY_ARGUMENT_PREFIX_MAP_should_instantiate_prefix_map) {
	BOOST_CHECK_EQUAL(lazyArgumentPrefixMapUnderTest().at("one"), 1);
	BOOST_CHECK_EQUAL(lazyArgumentPrefixMapUnderTest().at("two"), 2);
	BOOST_CHECK_EQUAL(lazyArgumentPrefixMapUnderTest().at("o"), 1);
	BOOST_CHECK_EQUAL(lazyArgumentPrefixMapUnderTest().at("t"), 2);
	BOOST_CHECK_THROW(lazyArgumentPrefixMapUnderTest().at("xxx"), ValueNotFound);
}

BOOST_AUTO_TEST_SUITE_END()



