#ifndef SRC_PREFIXMAP_HPP
#define SRC_PREFIXMAP_HPP

#include <string>
#include <map>
#include <deque>
#include <stdexcept>
#include <memory>

namespace car {

struct PrefixMapError: std::logic_error {
	using std::logic_error::logic_error;
};

struct AmbiguousValue: PrefixMapError {
	AmbiguousValue(): PrefixMapError{"Ambiguous value"} {}
};

struct ValueNotFound: PrefixMapError {
	ValueNotFound(): PrefixMapError{"Value not found"} {}
};

struct DuplicateValue: PrefixMapError {
	DuplicateValue(): PrefixMapError{"Duplicate value or prefix of existing value"} {}
};


// TODO make it more std container compliant
template <typename Char, typename Value>
class BasicPrefixMap {
public:

	using string = std::basic_string<Char>;
	using value_type = std::pair<const string, Value>;

private:

	struct Node {
		std::unique_ptr<value_type> value;
		using MapType = std::map<Char, Node>;
		MapType children;

		Node() = default;
		Node(const Node&) = default;
		Node& operator=(const Node&) = default;
		Node(Node&&) = default;
		Node& operator=(Node&&) = default;
	};

public:

	BasicPrefixMap() = default;
	BasicPrefixMap(const BasicPrefixMap&) = default;
	BasicPrefixMap& operator=(const BasicPrefixMap&) = default;
	BasicPrefixMap(BasicPrefixMap&&) = default;
	BasicPrefixMap& operator=(BasicPrefixMap&&) = default;

	BasicPrefixMap(std::initializer_list<value_type> values) {
		insert(values);
	}

	template <typename InputIterator>
	BasicPrefixMap(InputIterator begin, InputIterator end) {
		insert(begin, end);
	}

	template <typename... Args>
	void emplace(Args&&... args) {
		std::unique_ptr<value_type> value{new value_type{std::forward<Args>(args)...}};
		insertNode(value->first).value = std::move(value);
	}

	void insert(const value_type& value) {
		emplace(value);
	}

	template <typename P, typename = typename std::enable_if<
		std::is_constructible<value_type, P&&>::value>::type>
	void insert(P&& value) {
		emplace(std::forward<P>(value));
	}

	template <typename InputIterator>
	void insert(InputIterator begin, InputIterator end) {
		for (; begin != end; ++begin) {
			insert(*begin);
		}
	}

	void insert(std::initializer_list<value_type> values) {
		insert(values.begin(), values.end());
	}

	const Value& at(const string& key) const {
		return findNode(key).value->second;
	}
	Value& at(const string& key) {
		return const_cast<Node&>(findNode(key)).value->second;
	}
private:
	Node rootNode;

	struct NodePosition {
		const Node* node;
		std::size_t depth = 0;

		NodePosition(const Node& node):node(&node) {}
	};

	NodePosition findBestNode(const string& key) const {
		NodePosition result{rootNode};

		for (const auto& character: key) {
			assert(result.node);
			auto it = result.node->children.find(character);
			if (it == result.node->children.end()) {
				return result;
			}

			result.node = &it->second;
			++result.depth;
		}

		return result;
	}

	Node& insertNode(const string& key) {
		auto nodePosition = findBestNode(key);
		assert(nodePosition.node);

		if (nodePosition.node->value) {
			throw DuplicateValue{};
		}

		Node* node = const_cast<Node*>(nodePosition.node);
		for (std::size_t i = nodePosition.depth; i < key.size(); ++i) {
			node = &node->children[key[i]];
			assert(node);
			assert(!node->value);
			assert(node->children.empty());
		}

		return *node;
	}

	const Node& findNode(const string& key) const {
		auto nodePosition = findBestNode(key);

		if (nodePosition.depth != key.size()) {
			throw ValueNotFound{};
		}

		const Node* result = nullptr;
		std::deque<const Node*> queue{nodePosition.node};

		while (!queue.empty()) {
			auto node = queue.front();
			if (node->value) {

				if (result) {
					throw AmbiguousValue{};
				}
				result = node;

			}
			queue.pop_front();
			for (const auto& child: node->children) {
				queue.push_back(&child.second);
			}
		}

		assert(result);
		return *result;
	}
};

template <typename Value>
using PrefixMap = BasicPrefixMap<char, Value>;

} // namespace car



#endif /* SRC_PREFIXMAP_HPP */
