#ifndef SRC_LAZYARGUMENTMAP_HPP
#define SRC_LAZYARGUMENTMAP_HPP

#include <map>
#include <memory>
#include <string>
#include <boost/preprocessor/cat.hpp>

#define LAZY_ARGUMENT_MAP(Type, name) \
	using BOOST_PP_CAT(name, _type) = std::map<std::string, std::shared_ptr<Type>>;\
	BOOST_PP_CAT(name, _type) BOOST_PP_CAT(create_, name)();\
	std::unique_ptr<BOOST_PP_CAT(name, _type)> \
		BOOST_PP_CAT(name, _map);\
	std::map<std::string, std::shared_ptr<Type>>& name() { \
		if (!BOOST_PP_CAT(name, _map)) { \
			BOOST_PP_CAT(name, _map).reset(new BOOST_PP_CAT(name, _type)(\
					BOOST_PP_CAT(create_, name)())\
				);\
	}\
		return *BOOST_PP_CAT(name, _map);\
	}\
	BOOST_PP_CAT(name, _type) BOOST_PP_CAT(create_, name)()


#endif /* SRC_LAZYARGUMENTMAP_HPP */
