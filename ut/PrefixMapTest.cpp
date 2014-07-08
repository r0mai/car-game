#include <boost/test/unit_test.hpp>
#include "PrefixMap.hpp"

using namespace car;

BOOST_AUTO_TEST_SUITE(PrefixMapTest)

BOOST_AUTO_TEST_CASE(one_value) {
	int value = 10;
	PrefixMap<int> prefixMapUnderTest;
	prefixMapUnderTest.insert("value", value);
	BOOST_CHECK_EQUAL(prefixMapUnderTest.at("v"), value);
	BOOST_CHECK_EQUAL(prefixMapUnderTest.at("va"), value);
	BOOST_CHECK_EQUAL(prefixMapUnderTest.at("val"), value);
	BOOST_CHECK_EQUAL(prefixMapUnderTest.at("valu"), value);
	BOOST_CHECK_EQUAL(prefixMapUnderTest.at("value"), value);
	BOOST_CHECK_THROW(prefixMapUnderTest.at("x"), ValueNotFound);
	BOOST_CHECK_THROW(prefixMapUnderTest.at("valuee"), ValueNotFound);
}

BOOST_AUTO_TEST_CASE(distinct_values) {
	int value = 324;
	int something = 238;
	PrefixMap<int> prefixMapUnderTest;
	prefixMapUnderTest.insert("value", value);
	prefixMapUnderTest.insert("something", something);
	BOOST_CHECK_EQUAL(prefixMapUnderTest.at("v"), value);
	BOOST_CHECK_EQUAL(prefixMapUnderTest.at("s"), something);
	BOOST_CHECK_THROW(prefixMapUnderTest.at("x"), ValueNotFound);
}

BOOST_AUTO_TEST_CASE(prefix_values) {
	int value = 12;
	int vaaaa = 74;
	PrefixMap<int> prefixMapUnderTest;
	prefixMapUnderTest.insert("value", value);
	prefixMapUnderTest.insert("vaaaa", vaaaa);
	BOOST_CHECK_EQUAL(prefixMapUnderTest.at("val"), value);
	BOOST_CHECK_EQUAL(prefixMapUnderTest.at("vaa"), vaaaa);
	BOOST_CHECK_THROW(prefixMapUnderTest.at("x"), ValueNotFound);
	BOOST_CHECK_THROW(prefixMapUnderTest.at("v"), AmbiguousValue);
	BOOST_CHECK_THROW(prefixMapUnderTest.at("va"), AmbiguousValue);
}

BOOST_AUTO_TEST_CASE(duplicate_values) {
	std::string key = "value";
	PrefixMap<int> prefixMapUnderTest;
	prefixMapUnderTest.insert(key, 234);
	BOOST_CHECK_THROW(prefixMapUnderTest.insert(key, 745), DuplicateValue);
}

BOOST_AUTO_TEST_SUITE_END()

