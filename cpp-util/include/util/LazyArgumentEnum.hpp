#ifndef SRC_LAZYARGUMENTENUM_HPP
#define SRC_LAZYARGUMENTENUM_HPP

#include "util/StringEnumValue.hpp"
#include "util/LazyArgumentMap.hpp"
#include "util/PrefixMap.hpp"
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/preprocessor/stringize.hpp>

#define _LAZY_ARGUMENT_ENUM_PREFIX_MAP_HELPER(r, Type, elem) \
	STRING_ENUM_VALUE(Type, elem),
#define _LAZY_ARGUMENT_ENUM_OUTPUT_HELPER(r, Type, elem) \
	case Type::elem: return os << BOOST_PP_STRINGIZE(elem);

#define LAZY_ARGUMENT_ENUM(Type, name, ...) \
	enum class Type { \
		__VA_ARGS__ \
	}; \
	LAZY_ARGUMENT_PREFIX_MAP(Type, name) { \
		return { BOOST_PP_SEQ_FOR_EACH( \
				_LAZY_ARGUMENT_ENUM_PREFIX_MAP_HELPER, \
				Type, \
				BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)) \
		}; \
	} \
	inline ::std::istream& operator>>(::std::istream& is, Type& value) { \
		::std::string s; \
		is >> s; \
		value = name().at(s); \
		return is; \
	} \
	inline ::std::ostream& operator<<(::std::ostream& os, Type value) { \
		switch (value) { \
			BOOST_PP_SEQ_FOR_EACH( \
				_LAZY_ARGUMENT_ENUM_OUTPUT_HELPER, \
				Type, \
				BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)) \
		}; \
		return os; \
	}



#endif /* SRC_LAZYARGUMENTENUM_HPP */
