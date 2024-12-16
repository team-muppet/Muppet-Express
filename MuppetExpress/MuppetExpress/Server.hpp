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

/*
const express = require('express')
const app = express()
const port = 3000

app.get('/', (req, res) => {
  res.send('Hello World!')
})

app.listen(port, () => {
  console.log(`Example app listening on port ${port}`)
})
*/

namespace MuppetExpress {
	namespace beast = boost::beast;
	namespace http = beast::http;
	namespace net = boost::asio;
	using tcp = net::ip::tcp;
	using MuppetHandler = std::function<void(http::request<http::string_body>& request, http::response<http::string_body>& response)>;

	class Server
	{
	public:
		Server() {

		}

		Server& MapGet(const std::string_view& path, MuppetHandler handler)
		{
			registerHandler(http::verb::get, path, handler);
			return *this;
		}

		Server& MapPost(const std::string_view path, MuppetHandler handler)
		{
			registerHandler(http::verb::post, path, handler);
			return *this;
		}

		Server& MapPut(const std::string_view path, MuppetHandler handler)
		{
			registerHandler(http::verb::put, path, handler);
			return *this;
		}

		Server& MapDelete(const std::string_view path, MuppetHandler handler)
		{
			registerHandler(http::verb::delete_, path, handler);
			return *this;
		}

		void RunServer()
		{
			try {
				net::io_context ioc;

				// Spawn the listener coroutine
				net::co_spawn(ioc, listener(tcp::endpoint(tcp::v4(), 8081)), net::detached);

				// Determine the number of threads
				unsigned int thread_count = std::thread::hardware_concurrency();
				if (thread_count == 0) thread_count = 4; // Default to 4 threads if hardware_concurrency is unavailable

				// Create and run threads
				std::vector<std::thread> threads;
				for (unsigned int i = 0; i < thread_count; ++i) {
					threads.emplace_back([&ioc]() {
						ioc.run();
						});
				}

				std::cout << tcp::endpoint(tcp::v4(), 8081) << std::endl;

				std::cout << "Server is running on http://127.0.0.1:8081 with "
					<< thread_count << " threads..." << std::endl;

				// Join all threads
				for (auto& t : threads) {
					t.join();
				}
			}
			catch (const std::exception& e) {
				std::cerr << "Fatal Error: " << e.what() << std::endl;
			}
		}

	private:

		struct TrieNode {
			std::unordered_map<std::string_view, std::shared_ptr<TrieNode>> children;
			std::unordered_map<http::verb, MuppetHandler> handlers;
		};

		std::shared_ptr<TrieNode> root = std::make_shared<TrieNode>();

		void registerHandler(http::verb method, const std::string_view& path, MuppetHandler handler) {
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

		std::optional<MuppetHandler> resolve(http::verb method, const std::string_view& path) {
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

		// Handle an HTTP request
		net::awaitable<void> handle_request(tcp::socket socket) {
			try {
				beast::flat_buffer buffer;
				http::request<http::string_body> req;
				http::response<http::string_body> res;

				// Read the HTTP request
				co_await http::async_read(socket, buffer, req, net::use_awaitable);

				auto optionalHandler = resolve(req.method(), req.target());

				// Handle the request
				if (optionalHandler) {
					auto& handler = optionalHandler.value();
					handler(req, res);
				}
				else {
					res.result(http::status::not_found);
					res.set(http::field::content_type, "text/plain");
					res.body() = "404 Not Found";
				}

				res.prepare_payload();

				// Send the response
				co_await http::async_write(socket, res, net::use_awaitable);

				// Gracefully close the socket
				beast::error_code ec;
				socket.shutdown(tcp::socket::shutdown_send, ec);
			}
			catch (const std::exception& e) {
				std::cerr << "Error: " << e.what() << std::endl;
			}
		}

		// Listener coroutine
		net::awaitable<void> listener(tcp::endpoint endpoint) {
			auto executor = co_await net::this_coro::executor;
			tcp::acceptor acceptor(executor, endpoint);

			while (true) {
				tcp::socket socket = co_await acceptor.async_accept(net::use_awaitable);
				net::co_spawn(executor, handle_request(std::move(socket)), net::detached);
			}
		}
	};
}