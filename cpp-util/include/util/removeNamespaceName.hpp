#ifndef TYPEMANIP_REMOVENAMESPACENAME_HPP_
#define TYPEMANIP_REMOVENAMESPACENAME_HPP_

#include "demangle.hpp"
#include <boost/range/iterator_range.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

namespace util {

template <typename T>
std::string removeNamespaceName() {

	static const boost::regex regex{"\\w+(<[^>]*>)?(\\([^)]*\\))?::"};

	std::string typeName(type_name<T>());
	std::string result = boost::regex_replace(typeName, regex, "");

	while (result[result.size() - 1] == '_') {
		result.pop_back();
	}

	return result;
}

} // namespace util


#endif /* TYPEMANIP_REMOVENAMESPACENAME_HPP_ */
