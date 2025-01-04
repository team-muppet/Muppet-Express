#pragma once

#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <unordered_map>
#include <string>
#include <memory>
#include <optional>
#include <variant>
#include <functional>

#include "Definitions.hpp"

namespace MuppetExpress {

	class Router {
	public:
		void registerHandler(http::verb method, const std::string_view& path, Handler handler) {
			auto node = traversePathToAddNodes(path);
			node->handlers[method] = std::move(handler);
		}

		std::optional<HandlerWithoutParameters> resolve(Request& request) {
			std::unordered_map<std::string, std::string> params;
			auto node = traversePathForLookup(request.target(), params);
			if (!node) return std::nullopt;

			parseQueryParameters(request.target(), params);

			request.params(params);

			if (auto it = node->handlers.find(request.method()); it != node->handlers.end()) {
				/*return std::visit([&](auto&& handlerVariant) -> std::optional<HandlerWithoutParameters> {
					if constexpr (std::is_same_v<std::decay_t<decltype(handlerVariant)>, HandlerWithoutParameters>) {
						return handlerVariant;
					}
					else {
						return std::bind(handlerVariant, std::placeholders::_1, std::placeholders::_2, params);
					}
					},
					it->second
				);*/

				auto& handlerVariant = it->second;

				if (std::holds_alternative<HandlerWithoutParameters>(handlerVariant))
				{
					return get<HandlerWithoutParameters>(handlerVariant);
				}
				else
				{
					return std::bind(std::get<HandlerWithParameters>(handlerVariant), std::placeholders::_1, std::placeholders::_2, params);
				}
			}
			return std::nullopt;
		}

	private:
		// Hash and equality functors to support heterogeneous lookup.
		struct StringViewHash {
			using is_transparent = void;
			std::size_t operator()(const std::string_view sv) const noexcept {
				return std::hash<std::string_view>{}(sv);
			}
			std::size_t operator()(const std::string& s) const noexcept {
				return std::hash<std::string_view>{}(s);
			}
		};

		struct StringViewEqual {
			using is_transparent = void;
			bool operator()(std::string_view lhs, std::string_view rhs) const noexcept {
				return lhs == rhs;
			}
			bool operator()(const std::string& lhs, const std::string& rhs) const noexcept {
				return lhs == rhs;
			}
			bool operator()(std::string_view lhs, const std::string& rhs) const noexcept {
				return lhs == rhs;
			}
			bool operator()(const std::string& lhs, std::string_view rhs) const noexcept {
				return lhs == rhs;
			}
		};

		struct TrieNode {
			// Children with literal segments
			//template<class Key, class T, class Hash = std::hash<Key>, class KeyEqual = std::equal_to<Key>...
			std::unordered_map<std::string, std::shared_ptr<TrieNode>, StringViewHash, StringViewEqual> children;

			// Optional parameter node. If this is set, this node will match any one segment and record its value.
			std::shared_ptr<TrieNode> paramChild;
			std::string paramName; // Name of the parameter if paramChild is used.

			std::unordered_map<http::verb, Handler> handlers;
		};

		std::shared_ptr<TrieNode> root = std::make_shared<TrieNode>();

		// Modified traversePath for registration
		// If a segment starts with "{" and ends with "}", it's a parameter segment.
		std::shared_ptr<TrieNode> traversePathToAddNodes(const std::string_view& path) {
			auto node = root;
			size_t start = 0;

			while (start < path.size()) {
				size_t end = path.find('/', start);
				if (end == std::string_view::npos) {
					end = path.size();
				}

				std::string_view segment(path.data() + start, end - start);

				if (!segment.empty()) {
					if (segment.front() == '{' && segment.back() == '}') {
						// Parameter segment
						// Use the paramChild if it exists, otherwise create it
						if (!node->paramChild) {
							node->paramChild = std::make_shared<TrieNode>();
							node->paramChild->paramName = std::string(segment.substr(1, segment.size() - 2));
						}
						node = node->paramChild;
					}
					else {
						// Literal segment
						auto it = node->children.find(segment);
						if (it == node->children.end()) {
							auto newNode = std::make_shared<TrieNode>();
							node->children.emplace(std::string(segment), newNode);
							node = newNode;
						}
						else {
							node = it->second;
						}
					}
				}

				start = end + 1;
			}

			return node;
		}

		// Modified traverse for lookup.
		// This time we attempt to match either a literal child or the paramChild.
		// If paramChild matches, we record the segment in params.
		std::shared_ptr<TrieNode> traversePathForLookup(const std::string_view& path, Parameters& params) const {
			auto node = root;
			size_t start = 0;

			const std::string seperators = "?&#";

			while (start < path.size() && (start == 0 || seperators.find(path[start - 1]) == std::string_view::npos)) {
				size_t end = path.find_first_of("/?&#", start);
				if (end == std::string_view::npos) {
					end = path.size();
				}

				std::string_view segment(path.data() + start, end - start);

				if (!segment.empty()) {
					// Try literal match first
					auto it = node->children.find(segment);
					if (it != node->children.end()) {
						node = it->second;
					}
					else if (node->paramChild) {
						// Parameter match
						node = node->paramChild;
						params[node->paramName] = segment;
					}
					else {
						// No match
						return nullptr;
					}
				}

				start = end + 1;
			}

			return node;
		}

		void parseQueryParameters(const std::string_view& path, Parameters& Parameter)
		{
			size_t start = path.find('?');
			if (start == std::string_view::npos) {
				return;
			}

			while (start < path.size()) {
				std::string key, value;
				size_t end = path.find('=', start);
				if (end != std::string_view::npos) {
					key = path.substr(start + 1, (end - start) - 1);
					start = end;
				}

				end = path.find_first_of("&#", start);
				if (end != std::string_view::npos) {
					value = path.substr(start + 1, (end - start) - 1);
				}
				else
				{
					value = path.substr(start + 1);
				}

				if (!key.empty() && !value.empty())
				{
					Parameter[key] = value;
				}

				start = end;
			}
		}
	};
} // namespace MuppetExpress
