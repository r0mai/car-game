#include <boost/test/unit_test.hpp>
#include <map>
#include "StringEnumValue.hpp"

BOOST_AUTO_TEST_SUITE(StringEnumValueTest)

enum class TestEnum { first, second };

std::ostream& operator<<(std::ostream& os, TestEnum e) {
	switch (e) {
		case TestEnum::first: return os << ">>first<<";
		case TestEnum::second: return os << ">>second<<";
		default: return os << ">><<";
	}
};

BOOST_AUTO_TEST_CASE(StringEnumValue_should_attach_string_key_to_enum_value) {
	std::map<std::string, TestEnum> testMap{
			STRING_ENUM_VALUE(TestEnum, first),
			STRING_ENUM_VALUE(TestEnum, second)
		};

	BOOST_CHECK_EQUAL(testMap.at("first"), TestEnum::first);
	BOOST_CHECK_EQUAL(testMap.at("second"), TestEnum::second);
}

BOOST_AUTO_TEST_SUITE_END()




