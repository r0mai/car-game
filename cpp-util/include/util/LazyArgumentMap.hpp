#ifndef SRC_LAZYARGUMENTMAP_HPP
#define SRC_LAZYARGUMENTMAP_HPP

#include <map>
#include <string>
#include "LazyType.hpp"
#include <boost/preprocessor/cat.hpp>

#define LAZY_ARGUMENT_MAP(Type, name) \
	using BOOST_PP_CAT(name, _map_type) = ::std::map<::std::string, Type>;\
	LAZY_TYPE(BOOST_PP_CAT(name, _map_type), name)

#define LAZY_ARGUMENT_PREFIX_MAP(Type, name) \
	LAZY_TYPE(::util::PrefixMap<Type>, name)


#endif /* SRC_LAZYARGUMENTMAP_HPP */
