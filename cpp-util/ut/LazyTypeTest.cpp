#include <boost/test/unit_test.hpp>
#include "LazyType.hpp"

BOOST_AUTO_TEST_SUITE(LazyTypeTest)

struct Counter {
	int count;
};

static int counterCount = 0;

LAZY_TYPE(Counter, counter) {
	return Counter{++counterCount};
};

BOOST_AUTO_TEST_CASE(LazyType_should_be_instantiated_once_when_called) {
	counterCount = 0;
	BOOST_CHECK_EQUAL(counter().count, 1);
	BOOST_CHECK_EQUAL(counter().count, 1);
}

BOOST_AUTO_TEST_SUITE_END()


