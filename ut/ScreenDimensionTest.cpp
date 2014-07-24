#include <boost/test/unit_test.hpp>
#include "ScreenDimension.hpp"

using namespace car;

#define CHECK_VARIANT_EXPLICIT_TYPE(variant, Type, expectedValue) do {\
	auto actualValue = boost::get<Type>(&(variant));\
	BOOST_REQUIRE(actualValue);\
	BOOST_CHECK_EQUAL(actualValue->value(), expectedValue);\
} while (false)

#define CHECK_VARIANT_EXPLICIT_TYPE_CLOSE(variant, Type, expectedValue) do {\
	auto actualValue = boost::get<Type>(&(variant));\
	BOOST_REQUIRE(actualValue);\
	BOOST_CHECK_CLOSE(actualValue->value(), expectedValue, 0.0001);\
} while (false)

BOOST_AUTO_TEST_SUITE(ScreenDimensionTest)

BOOST_AUTO_TEST_CASE(pixels) {
	auto result = parseScreenDimenstion("547px");
	CHECK_VARIANT_EXPLICIT_TYPE(result, Pixels, 547);
}

BOOST_AUTO_TEST_CASE(meters_integer) {
	auto result = parseScreenDimenstion("15m");
	CHECK_VARIANT_EXPLICIT_TYPE_CLOSE(result, Meters, 15.f);
}

BOOST_AUTO_TEST_CASE(meters_float) {
	auto result = parseScreenDimenstion("255.661m");
	CHECK_VARIANT_EXPLICIT_TYPE_CLOSE(result, Meters, 255.661);
}

BOOST_AUTO_TEST_CASE(percent_integer) {
	auto result = parseScreenDimenstion("630%");
	CHECK_VARIANT_EXPLICIT_TYPE_CLOSE(result, Percent, 630.f);
}

BOOST_AUTO_TEST_CASE(percent_float) {
	auto result = parseScreenDimenstion("12.5%");
	CHECK_VARIANT_EXPLICIT_TYPE_CLOSE(result, Percent, 12.5);
}

BOOST_AUTO_TEST_CASE(error_no_unit) {
	BOOST_CHECK_THROW(parseScreenDimenstion("123"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(error_bad_unit) {
	BOOST_CHECK_THROW(parseScreenDimenstion("123u"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(error_too_long_unit_percent) {
	BOOST_CHECK_THROW(parseScreenDimenstion("123%x"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(error_too_long_unit_meters) {
	BOOST_CHECK_THROW(parseScreenDimenstion("4.5ma"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(error_too_long_unit_pixels) {
	BOOST_CHECK_THROW(parseScreenDimenstion("123pxx"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(error_bad_number_meter) {
	BOOST_CHECK_THROW(parseScreenDimenstion("123.456.789m"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(error_bad_number_percent) {
	BOOST_CHECK_THROW(parseScreenDimenstion("55a55%"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(error_float_pixels) {
	BOOST_CHECK_THROW(parseScreenDimenstion("12.3px"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(error_bad_number_pixels) {
	BOOST_CHECK_THROW(parseScreenDimenstion("54x2px"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(error_prefix_percent) {
	BOOST_CHECK_THROW(parseScreenDimenstion("xcv345.0%"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(error_prefix_meters) {
	BOOST_CHECK_THROW(parseScreenDimenstion("kl23m"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(error_prefix_pixels) {
	BOOST_CHECK_THROW(parseScreenDimenstion("a53px"), std::logic_error);
}



BOOST_AUTO_TEST_SUITE_END()

