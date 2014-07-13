#include <boost/test/unit_test.hpp>
#include "PrefixMap.hpp"

using namespace car;

BOOST_AUTO_TEST_SUITE(PrefixMapTest)

BOOST_AUTO_TEST_CASE(one_value) {
	int value = 10;
	PrefixMap<int> prefixMapUnderTest;
	prefixMapUnderTest.emplace("value", value);
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
	prefixMapUnderTest.emplace("value", value);
	prefixMapUnderTest.emplace("something", something);
	BOOST_CHECK_EQUAL(prefixMapUnderTest.at("v"), value);
	BOOST_CHECK_EQUAL(prefixMapUnderTest.at("s"), something);
	BOOST_CHECK_THROW(prefixMapUnderTest.at("x"), ValueNotFound);
}

BOOST_AUTO_TEST_CASE(prefix_values) {
	int value = 12;
	int vaaaa = 74;
	PrefixMap<int> prefixMapUnderTest;
	prefixMapUnderTest.emplace("value", value);
	prefixMapUnderTest.emplace("vaaaa", vaaaa);
	BOOST_CHECK_EQUAL(prefixMapUnderTest.at("val"), value);
	BOOST_CHECK_EQUAL(prefixMapUnderTest.at("vaa"), vaaaa);
	BOOST_CHECK_THROW(prefixMapUnderTest.at("x"), ValueNotFound);
	BOOST_CHECK_THROW(prefixMapUnderTest.at("v"), AmbiguousValue);
	BOOST_CHECK_THROW(prefixMapUnderTest.at("va"), AmbiguousValue);
}

BOOST_AUTO_TEST_CASE(duplicate_values) {
	std::string key = "value";
	PrefixMap<int> prefixMapUnderTest;
	prefixMapUnderTest.emplace(key, 234);
	BOOST_CHECK_THROW(prefixMapUnderTest.emplace(key, 745), DuplicateValue);
}

BOOST_AUTO_TEST_CASE(initializer_list_create) {
	int value = 1;
	int something = 3;
	PrefixMap<int> prefixMapUnderTest{{"value", value}, {"something", something}};
	BOOST_CHECK_EQUAL(prefixMapUnderTest.at("v"), value);
	BOOST_CHECK_EQUAL(prefixMapUnderTest.at("s"), something);
	BOOST_CHECK_THROW(prefixMapUnderTest.at("x"), ValueNotFound);
}

BOOST_AUTO_TEST_CASE(initializer_list_insert) {
	int value = 34;
	int something = 54;
	PrefixMap<int> prefixMapUnderTest;
	prefixMapUnderTest.insert({{"value", value}, {"something", something}});
	BOOST_CHECK_EQUAL(prefixMapUnderTest.at("v"), value);
	BOOST_CHECK_EQUAL(prefixMapUnderTest.at("s"), something);
	BOOST_CHECK_THROW(prefixMapUnderTest.at("x"), ValueNotFound);
}

BOOST_AUTO_TEST_CASE(iterator_create) {
	int value = 1123;
	int something = 56;
	std::vector<PrefixMap<int>::value_type> v{{"value", value}, {"something", something}};
	PrefixMap<int> prefixMapUnderTest(v.begin(), v.end());
	BOOST_CHECK_EQUAL(prefixMapUnderTest.at("v"), value);
	BOOST_CHECK_EQUAL(prefixMapUnderTest.at("s"), something);
	BOOST_CHECK_THROW(prefixMapUnderTest.at("x"), ValueNotFound);
}

BOOST_AUTO_TEST_CASE(iterator_insert) {
	int value = 7;
	int something = 1;
	std::vector<PrefixMap<int>::value_type> v{{"value", value}, {"something", something}};
	PrefixMap<int> prefixMapUnderTest;
	prefixMapUnderTest.insert(v.begin(), v.end());
	BOOST_CHECK_EQUAL(prefixMapUnderTest.at("v"), value);
	BOOST_CHECK_EQUAL(prefixMapUnderTest.at("s"), something);
	BOOST_CHECK_THROW(prefixMapUnderTest.at("x"), ValueNotFound);
}

BOOST_AUTO_TEST_CASE(iterate) {
	std::vector<std::pair<std::string, int>> v{
		{"value1", 12}, {"value2", 42}, {"something", 333}, {"value3", 0}};
	PrefixMap<int> prefixMapUnderTest(v.begin(), v.end());
	std::sort(v.begin(), v.end());

	auto it = prefixMapUnderTest.cbegin();
	BOOST_CHECK_EQUAL(it->first, v[0].first);
	BOOST_CHECK_EQUAL(it->second, v[0].second);
	++it;
	BOOST_CHECK_EQUAL(it->first, v[1].first);
	BOOST_CHECK_EQUAL(it->second, v[1].second);
	++it;
	BOOST_CHECK_EQUAL(it->first, v[2].first);
	BOOST_CHECK_EQUAL(it->second, v[2].second);
	++it;
	BOOST_CHECK_EQUAL(it->first, v[3].first);
	BOOST_CHECK_EQUAL(it->second, v[3].second);
	++it;
	BOOST_CHECK(it == prefixMapUnderTest.cend());
}

struct TestValue {
	bool copied = false;
	bool moved = false;
	int value;

	explicit TestValue(int value):value(value) {}
	TestValue(const TestValue& other):
		copied(true), moved(other.moved), value(other.value)
	{}
	TestValue(TestValue&& other):
		copied(other.copied), moved(true), value(other.value)
	{}

	TestValue& operator=(const TestValue& other) {
		copied = true;
		moved = other.moved;
		value = other.value;
		return *this;
	}
	TestValue& operator=(TestValue&& other) {
		copied = other.copied;
		moved = true;
		value = other.value;
		return *this;
	}
};

BOOST_AUTO_TEST_CASE(insert_lvalue_should_copy) {
	std::string key = "key";
	int value = 12;
	PrefixMap<TestValue>::value_type v{key, TestValue{value}};
	PrefixMap<TestValue> prefixMapUnderTest;
	prefixMapUnderTest.insert(v);
	const auto& storedValue = prefixMapUnderTest.at(key);
	BOOST_CHECK(storedValue.copied);
	BOOST_CHECK_EQUAL(storedValue.value, value);
}

BOOST_AUTO_TEST_CASE(insert_rvalue_should_not_copy) {
	std::string key = "key";
	int value = 12;
	PrefixMap<TestValue>::value_type v{key, TestValue{value}};
	PrefixMap<TestValue> prefixMapUnderTest;
	prefixMapUnderTest.insert(std::move(v));
	const auto& storedValue = prefixMapUnderTest.at(key);
	BOOST_CHECK(!storedValue.copied);
	BOOST_CHECK_EQUAL(storedValue.value, value);
}

BOOST_AUTO_TEST_CASE(emplace_lvalue_should_copy) {
	std::string key = "key";
	int value = 12;
	TestValue v{value};
	PrefixMap<TestValue> prefixMapUnderTest;
	prefixMapUnderTest.emplace(key, v);
	const auto& storedValue = prefixMapUnderTest.at(key);
	BOOST_CHECK(storedValue.copied);
	BOOST_CHECK(!storedValue.moved);
	BOOST_CHECK_EQUAL(storedValue.value, value);
}

BOOST_AUTO_TEST_CASE(emplace_rvalue_should_move) {
	std::string key = "key";
	int value = 12;
	TestValue v{value};
	PrefixMap<TestValue> prefixMapUnderTest;
	prefixMapUnderTest.emplace(key, std::move(v));
	const auto& storedValue = prefixMapUnderTest.at(key);
	BOOST_CHECK(!storedValue.copied);
	BOOST_CHECK(storedValue.moved);
	BOOST_CHECK_EQUAL(storedValue.value, value);
}

BOOST_AUTO_TEST_SUITE_END()

