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

#include "Definitions.hpp"

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
		std::cout << "Portnumber from int: " << portnumber << std::endl;
		portnumber = std::clamp(portnumber, 0, 65535);
		return portnumber;
	}
	int operator()(std::string portnumber) {
		std::cout << "Portnumber from string/param args: " << portnumber << std::endl;
		int portnumberFromString = std::clamp(std::stoi(portnumber), 0, 65535);
		return portnumberFromString;
	}
};

//struct constPortnumberVisitor {
//	constexpr int operator()(int portnumber) const {
//		return portnumber; // Simply return the integer
//	}
//
//	constexpr int operator()(std::string_view portnumber) const {
//		// Parse string_view into int at compile time
//		int result = 0;
//		for (char c : portnumber) {
//			if (c < '0' || c > '9') throw "Invalid character"; // Compile-time check
//			result = result * 10 + (c - '0');
//		}
//		return result;
//	}
//};

namespace MuppetExpress {
	class Server
	{
	public:
		Server(std::variant<std::string, int> portnumber, std::optional<GlobalExceptionHandler> exceptionHandler = std::nullopt) {
			try
			{
				portnumberVisitor visitor;
				portnumber_ = std::visit(visitor, portnumber);
			}
			catch (const std::exception& e)
			{
				std::cerr << "Error: " << e.what() << std::endl;
				exit(1);
				//or throw; to a catch all in main
			}
			//if constexpr (std::is_constant_evaluated()) {
			//	portnumberVisitor visitor;
			//	portnumber_ = std::visit(visitor, portnumber);
			//}
			//else {
			//	constPortnumberVisitor visitor;
			//	portnumber_ = std::visit(visitor, portnumber);
			//}

			_globalExceptionHandler = exceptionHandler;
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
		std::optional<GlobalExceptionHandler> _globalExceptionHandler;

		// Handle an HTTP request
		net::awaitable<void> handle_request(tcp::socket socket) {
			try {
				beast::flat_buffer buffer;
				Request req;
				Response res;

				// Read the HTTP request
				co_await http::async_read(socket, buffer, req, net::use_awaitable);

				auto optionalHandler = router.resolve(req);

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

				auto handler = [&]() {
					_middlewareManager.runChain(req, res, routeHandler);
					};

				if (_globalExceptionHandler)
				{
					_globalExceptionHandler.value()(req, res, handler);
				}
				else
				{
					DefaultGlobalExceptionHandler(req, res, handler);
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

		void setResponseInternalServerError(Response& res) {
			res.result(http::status::internal_server_error);
			res.set(http::field::content_type, "text/plain");
			res.body() = "500 internal server error";
		}

		void DefaultGlobalExceptionHandler(Request& req, Response& res, std::function<void()> routehandler) {
			try {
				// Handle request with middleware
				routehandler();
			}
			catch (const std::exception& e) {
				std::cerr << "Error: " << e.what() << std::endl;
				setResponseInternalServerError(res);
			}
			catch (...) {
				std::cerr << "Unspecified error occured" << std::endl;
				setResponseInternalServerError(res);
			}
		}
	};
}