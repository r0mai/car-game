#ifndef SRC_EXPLICITTYPE_HPP
#define SRC_EXPLICITTYPE_HPP

#include <utility>
#include <iostream>

namespace car {

template <typename Tag, typename T>
class ExplicitType {
public:
	ExplicitType() = default;
	explicit ExplicitType(const T& v):v(v) {}
	explicit ExplicitType(T&& v):v(std::move(v)) {}
	ExplicitType(const ExplicitType&) = default;
	ExplicitType(ExplicitType&&) = default;

	ExplicitType& operator=(const ExplicitType&) = default;
	ExplicitType& operator=(ExplicitType&&) = default;

	const T& value() const { return v; }
	T& value() { return v; }
private:
	T v;
};

#define EXPLICIT_TYPE_OP(op) \
template <typename Tag, typename T> \
bool operator op (const ExplicitType<Tag, T>& lhs, const ExplicitType<Tag, T>& rhs) { \
	return lhs.value() op rhs.value();\
}

EXPLICIT_TYPE_OP(==)
EXPLICIT_TYPE_OP(!=)
EXPLICIT_TYPE_OP(<)
EXPLICIT_TYPE_OP(>)
EXPLICIT_TYPE_OP(<=)
EXPLICIT_TYPE_OP(>=)

#undef EXPLICIT_TYPE_OP

template <typename Tag, typename T>
std::ostream& operator<<(std::ostream& os, const ExplicitType<Tag, T>& t) {
	return os << t.value();
}

template <typename Tag, typename T>
std::istream& operator>>(std::istream& is, const ExplicitType<Tag, T>& t) {
	return is >> t.value();
}


}


#endif /* SRC_EXPLICITTYPE_HPP */
