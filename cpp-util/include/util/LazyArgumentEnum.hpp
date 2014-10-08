#ifndef SRC_LAZYARGUMENTENUM_HPP
#define SRC_LAZYARGUMENTENUM_HPP

#include "util/StringEnumValue.hpp"
#include "util/LazyArgumentMap.hpp"
#include "util/PrefixMap.hpp"
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <istream>
#include <ostream>

#define _LAZY_ARGUMENT_ENUM_ENUM_HELPER(r, Type, elem) \
	elem,
#define _LAZY_ARGUMENT_ENUM_PREFIX_MAP_HELPER(r, Type, elem) \
	STRING_ENUM_VALUE(Type, elem),
#define _LAZY_ARGUMENT_ENUM_OUTPUT_HELPER(r, Type, elem) \
	case Type::elem: return os << BOOST_PP_STRINGIZE(elem);

#define LAZY_ARGUMENT_ENUM(Type, name, values) \
	enum class Type { \
			BOOST_PP_SEQ_FOR_EACH( \
				_LAZY_ARGUMENT_ENUM_ENUM_HELPER, \
				Type, values) \
	}; \
	LAZY_ARGUMENT_PREFIX_MAP(Type, name) { \
		return { BOOST_PP_SEQ_FOR_EACH( \
				_LAZY_ARGUMENT_ENUM_PREFIX_MAP_HELPER, \
				Type, values) \
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
				Type, values) \
		}; \
		return os; \
	}



#endif /* SRC_LAZYARGUMENTENUM_HPP */
