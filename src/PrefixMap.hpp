#ifndef SRC_PREFIXMAP_HPP
#define SRC_PREFIXMAP_HPP

#include <string>
#include <map>
#include <deque>
#include <stdexcept>
#include <boost/optional.hpp>

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


template <typename Char, typename Value>
class BasicPrefixMap {
	struct Node {
		boost::optional<Value> value;
		std::map<Char, Node> children;

		Node() = default;
		Node(const Node&) = delete;
		Node& operator=(const Node&) = delete;
		Node(Node&&) = default;
		Node& operator=(Node&&) = default;
	};

public:
	using string = std::basic_string<Char>;

	void insert(const string& key, const Value& value) {
		insertNode(key).value = value;
	}
	void insert(const string& key, Value&& value) {
		insertNode(key).value = std::move(value);
	}

	const Value& at(const string& key) const {
		return *findNode(key).value;
	}
	Value& at(const string& key) {
		return *const_cast<Node&>(findNode(key)).value;
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
