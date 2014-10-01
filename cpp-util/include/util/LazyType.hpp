#ifndef SRC_LAZYTYPE_HPP
#define SRC_LAZYTYPE_HPP

#include <memory>
#include <boost/preprocessor/cat.hpp>

#define LAZY_TYPE(Type, name) \
	using BOOST_PP_CAT(name, _type) = Type;\
	BOOST_PP_CAT(name, _type) BOOST_PP_CAT(create_, name)();\
	::std::unique_ptr<Type> BOOST_PP_CAT(name, _value);\
	Type& name() { \
		if (!BOOST_PP_CAT(name, _value)) { \
			BOOST_PP_CAT(name, _value).reset(new Type(\
					BOOST_PP_CAT(create_, name)())\
				);\
		}\
		return *BOOST_PP_CAT(name, _value);\
	}\
	Type BOOST_PP_CAT(create_, name)()



#endif /* SRC_LAZYTYPE_HPP */
