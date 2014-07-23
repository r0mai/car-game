#include <boost/test/unit_test.hpp>
#include "ScreenDimension.hpp"
#include <turtle/mock.hpp>

using namespace car;

namespace {

struct MockVisitor {
	using result_type = void;
	MOCK_METHOD(operator(), 1, void(Meters), call_meters);
	MOCK_METHOD(operator(), 1, void(Percent), call_percent);
	MOCK_METHOD(operator(), 1, void(Pixels), call_pixels);
};

template <typename T, typename ToleranceType>
struct ExplicitTypeValueClose {
	T value;
	ToleranceType tolerance;
	template <typename Tag>
	bool operator()(const ExplicitType<Tag, T> t) {
		return boost::test_tools::check_is_close(t.value(), value,
				boost::test_tools::percent_tolerance(tolerance));
	}

	ExplicitTypeValueClose(T value, ToleranceType tolerance):
		value(value), tolerance(tolerance)
	{}
};

template <typename T, typename ToleranceType>
ExplicitTypeValueClose<T, ToleranceType> explicitTypeValueClose(
		T value, ToleranceType tolerance) {
	return ExplicitTypeValueClose<T, ToleranceType>{value, tolerance};
}

struct Fixture {
	MockVisitor visitor;
};

}

BOOST_FIXTURE_TEST_SUITE(ScreenDimensionTest, Fixture)

BOOST_AUTO_TEST_CASE(pixels) {
	MOCK_EXPECT(visitor.call_pixels).once().with(Pixels{547});
	auto result = parseScreenDimenstion("547px");
	boost::apply_visitor(visitor, result);
}

BOOST_AUTO_TEST_CASE(meters_integer) {
	MOCK_EXPECT(visitor.call_meters).once().with(explicitTypeValueClose(15.f, 0.001f));
	auto result = parseScreenDimenstion("15m");
	boost::apply_visitor(visitor, result);
}

BOOST_AUTO_TEST_CASE(meters_float) {
	MOCK_EXPECT(visitor.call_meters).once().with(explicitTypeValueClose(255.661f, 0.001f));
	auto result = parseScreenDimenstion("255.661m");
	boost::apply_visitor(visitor, result);
}

BOOST_AUTO_TEST_CASE(percent_integer) {
	MOCK_EXPECT(visitor.call_percent).once().with(explicitTypeValueClose(630.f, 0.001f));
	auto result = parseScreenDimenstion("630%");
	boost::apply_visitor(visitor, result);
}

BOOST_AUTO_TEST_CASE(percent_float) {
	MOCK_EXPECT(visitor.call_percent).once().with(explicitTypeValueClose(12.5f, 0.001f));
	auto result = parseScreenDimenstion("12.5%");
	boost::apply_visitor(visitor, result);
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

