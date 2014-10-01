#include <boost/test/unit_test.hpp>
#include "demangle.hpp"

using namespace util;

namespace foo {
namespace bar {
struct Baz {};
}
}

BOOST_AUTO_TEST_SUITE( demangleTest )

BOOST_AUTO_TEST_CASE(type_name_should_return_demangled_type_name)
{
	BOOST_CHECK_EQUAL(type_name<foo::bar::Baz>(), "foo::bar::Baz");
}

BOOST_AUTO_TEST_SUITE_END() // demangleTest


