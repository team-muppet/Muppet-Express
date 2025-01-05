struct TrieNode {
	// Children with literal segments
	//template<class Key, class T, class Hash = std::hash<Key>, class KeyEqual = std::equal_to<Key>...
	std::unordered_map<std::string, std::shared_ptr<TrieNode>, StringViewHash, StringViewEqual> children;

	// Optional parameter node. If this is set, this node will match any one segment and record its value.
	std::shared_ptr<TrieNode> paramChild;
	std::string paramName; // Name of the parameter if paramChild is used.

	std::unordered_map<http::verb, Handler> handlers;
};



