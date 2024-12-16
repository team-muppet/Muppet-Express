#pragma once

#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <iostream>
#include <vector>
#include <thread>

#include "MuppetExpressDefinitions.hpp"

namespace MuppetExpress {

	class Router {
	public:
		void registerHandler(http::verb method, const std::string_view& path, Handler handler) {
			auto node = root;
			size_t start = 0;
			size_t end = 0;

			while ((end = path.find('/', start)) != std::string_view::npos) {
				std::string_view segment = path.substr(start, end - start);
				if (!segment.empty()) {
					if (node->children.find(segment) == node->children.end()) {
						node->children[segment] = std::make_shared<TrieNode>();
					}
					node = node->children[segment];
				}
				start = end + 1;
			}

			std::string_view lastSegment = path.substr(start);

			if (!lastSegment.empty()) {
				if (node->children.find(lastSegment) == node->children.end()) {
					node->children[lastSegment] = std::make_shared<TrieNode>();
				}
				node = node->children[lastSegment];
			}

			node->handlers[method] = handler;
		}

		std::optional<Handler> resolve(http::verb method, const std::string_view& path) {
			auto node = root;
			size_t start = 0;
			size_t end = 0;

			while ((end = path.find('/', start)) != std::string_view::npos) {
				std::string_view segment = path.substr(start, end - start);
				if (!segment.empty() && node->children.find(segment) != node->children.end()) {
					node = node->children[segment];
				}
				start = end + 1;
			}

			std::string_view lastSegment = path.substr(start);

			if (!lastSegment.empty() && node->children.find(lastSegment) != node->children.end()) {
				node = node->children[lastSegment];

				if (node->handlers.find(method) != node->handlers.end())
				{
					return node->handlers[method];
				}
			}

			if (lastSegment.empty() && node->handlers.find(method) != node->handlers.end())
			{
				return node->handlers[method];
			}

			return std::nullopt;
		}
	private:

		struct TrieNode {
			std::unordered_map<std::string_view, std::shared_ptr<TrieNode>> children;
			std::unordered_map<http::verb, Handler> handlers;
			/*Signal signal*/
		};

		std::shared_ptr<TrieNode> root = std::make_shared<TrieNode>();
	};
};