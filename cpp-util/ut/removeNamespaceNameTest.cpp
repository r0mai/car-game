#include <boost/test/unit_test.hpp>
#include "removeNamespaceName.hpp"

using namespace util;

BOOST_AUTO_TEST_SUITE( removeNamespaceNameTest )

BOOST_AUTO_TEST_CASE(removeNamespaceName_should_return_class_name_without_namespace)
{
	struct TestClass {};

	BOOST_CHECK_EQUAL(removeNamespaceName<TestClass>(), "TestClass");
}

BOOST_AUTO_TEST_CASE(removeNamespaceName_should_remove_underscore_from_name_end)
{
	struct TestClass__ {};

	BOOST_CHECK_EQUAL(removeNamespaceName<TestClass__>(), "TestClass");
}

BOOST_AUTO_TEST_CASE(removeNamespaceName_should_not_remove_underscore_from_other_places)
{
	struct _Test_Class {};

	BOOST_CHECK_EQUAL(removeNamespaceName<_Test_Class>(), "_Test_Class");
}

namespace templateParameterClasses {
	struct TemplateParameter;

	template <typename T>
	struct TemplateClass {};

	template <typename T, template <class> class U>
	struct TemplateTemplate {};
}

BOOST_AUTO_TEST_CASE(removeNamespaceName_should_return_class_name_with_template_parameters)
{
	using namespace templateParameterClasses;

	typedef TemplateClass<TemplateParameter> Type;

	BOOST_MESSAGE("Original type name = " << type_name<Type>());
	BOOST_CHECK_EQUAL(removeNamespaceName<Type>(),
			"TemplateClass<TemplateParameter>");
}

BOOST_AUTO_TEST_CASE(removeNamespaceName_should_return_class_name_with_more_complicated_template_parameters)
{
	using namespace templateParameterClasses;

	typedef TemplateTemplate<TemplateClass<TemplateParameter>, TemplateClass> Type;

	BOOST_MESSAGE("Original type name = " << type_name<Type>());
	BOOST_CHECK_EQUAL(removeNamespaceName<Type>(),
			"TemplateTemplate<TemplateClass<TemplateParameter>, TemplateClass>");
}

namespace nestedClasses {

struct NestingClass {
	struct NestedClass {};
};

template <typename T, typename U>
struct NestingTemplateClass {
	struct NestedClass {};

	template <typename TT, typename UU>
	struct NestedTemplateClass {};
};

};

BOOST_AUTO_TEST_CASE(removeNamespaceName_should_return_class_name_with_nested_class)
{
	using namespace nestedClasses;

	typedef NestingClass::NestedClass Type;

	BOOST_MESSAGE("Original type name = " << type_name<Type>());
	BOOST_CHECK_EQUAL(removeNamespaceName<Type>(),
			"NestedClass");
}

BOOST_AUTO_TEST_CASE(removeNamespaceName_should_return_class_name_with_nested_class_inside_template)
{
	using namespace nestedClasses;

	typedef NestingTemplateClass<NestingClass::NestedClass, int>::NestedClass Type;

	BOOST_MESSAGE("Original type name = " << type_name<Type>());
	BOOST_CHECK_EQUAL(removeNamespaceName<Type>(),
			"NestedClass");
}

BOOST_AUTO_TEST_CASE(removeNamespaceName_should_return_class_name_with_nested_template_class_inside_template)
{
	using namespace nestedClasses;

	typedef NestingTemplateClass<NestingClass::NestedClass, int>::
			NestedTemplateClass<NestingClass, NestingClass::NestedClass> Type;

	BOOST_MESSAGE("Original type name = " << type_name<Type>());
	BOOST_CHECK_EQUAL(removeNamespaceName<Type>(),
			"NestedTemplateClass<NestingClass, NestedClass>");
}

// TODO make these work
/*
BOOST_AUTO_TEST_CASE(removeNamespaceName_should_return_class_name_with_nested_class_inside_recursive_template)
{
	using namespace nestedClasses;

	typedef NestingTemplateClass<NestingTemplateClass<int, NestingClass>, int>::
			NestedClass Type;

	BOOST_MESSAGE("Original type name = " << type_name<Type>());
	BOOST_CHECK_EQUAL(removeNamespaceName<Type>(),
			"NestedClass");
}

BOOST_AUTO_TEST_CASE(removeNamespaceName_should_return_class_name_with_nested_class_inside_recursive_template2)
{
	using namespace nestedClasses;

	typedef NestingTemplateClass<NestingTemplateClass<int, NestingClass>::NestedClass, int>::
			NestedClass Type;

	BOOST_MESSAGE("Original type name = " << type_name<Type>());
	BOOST_CHECK_EQUAL(removeNamespaceName<Type>(),
			"NestedClass");
}
*/

BOOST_AUTO_TEST_SUITE_END()


