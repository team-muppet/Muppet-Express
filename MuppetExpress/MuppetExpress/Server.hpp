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
#include "Router.hpp"
#include "MiddlewareManager.hpp"

#include "MuppetExpressDefinitions.hpp"

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

struct portnumberVisitor
{
	int operator()(int portnumber) {
		std::cout << portnumber << std::endl;
		return portnumber;
	}
	int operator()(std::string_view portnumber) {
		std::string portnumberString(portnumber);
		std::cout << portnumber << std::endl;
		return std::stoi(portnumberString);
	}
};

struct constPortnumberVisitor {
	constexpr int operator()(int portnumber) const {
		return portnumber; // Simply return the integer
	}

	constexpr int operator()(std::string_view portnumber) const {
		// Parse string_view into int at compile time
		int result = 0;
		for (char c : portnumber) {
			if (c < '0' || c > '9') throw "Invalid character"; // Compile-time check
			result = result * 10 + (c - '0');
		}
		return result;
	}
};

namespace MuppetExpress {
	class Server
	{
	public:
		Server(std::variant<std::string_view, int> portnumber) {

			if constexpr (std::is_constant_evaluated()) {
				constPortnumberVisitor visitor;
				portnumber_ = std::visit(visitor, portnumber);
			}
			else {
				portnumberVisitor visitor;
				portnumber_ = std::visit(visitor, portnumber);
			}

		}

		Server& MapGet(const std::string_view& path, Handler handler)
		{
			router.registerHandler(http::verb::get, path, handler);
			return *this;
		}

		Server& MapPost(const std::string_view& path, Handler handler)
		{
			router.registerHandler(http::verb::post, path, handler);
			return *this;
		}

		Server& MapPut(const std::string_view& path, Handler handler)
		{
			router.registerHandler(http::verb::put, path, handler);
			return *this;
		}

		Server& MapDelete(const std::string_view& path, Handler handler)
		{
			router.registerHandler(http::verb::delete_, path, handler);
			return *this;
		}

		Server& Use(Middleware mw) {
			_middlewareManager.addMiddleware(std::move(mw));
			return *this;
		}

		void RunServer()
		{
			try {
				net::io_context ioc;

				// Spawn the listener coroutine
				net::co_spawn(ioc, listener(tcp::endpoint(tcp::v4(), portnumber_)), net::detached);

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

				std::cout << "Server is running on http://127.0.0.1:" << portnumber_ << " with "
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

		int portnumber_;

		Router router;
		MiddlewareManager _middlewareManager;

		// Handle an HTTP request
		net::awaitable<void> handle_request(tcp::socket socket) {
			try {
				beast::flat_buffer buffer;
				http::request<http::string_body> req;
				http::response<http::string_body> res;

				// Read the HTTP request
				co_await http::async_read(socket, buffer, req, net::use_awaitable);

				auto optionalHandler = router.resolve(req.method(), req.target());

				// Make an endpoint handler function
				std::function<void()> routeHandler = [&]() {
					if (optionalHandler) {
						auto& handler = optionalHandler.value();
						handler(req, res);
					}
					else {
						res.result(http::status::not_found);
						res.set(http::field::content_type, "text/plain");
						res.body() = "404 Not Found";
					}
				};

				// Handle request with middleware
				_middlewareManager.runChain(req, res, routeHandler);

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