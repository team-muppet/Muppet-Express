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

#include "Request.hpp"

namespace MuppetExpress {
	namespace beast = boost::beast;
	namespace http = beast::http;
	namespace net = boost::asio;
	using tcp = net::ip::tcp;
	using Response = http::response<http::string_body>;

	using HandlerWithoutParameters = std::function<void(Request& request, Response& response)>;
	using HandlerWithParameters = std::function<void(Request& request, Response& response, Parameters& parameters)>;
	using Handler = std::variant<HandlerWithoutParameters, HandlerWithParameters>;

	using Middleware = std::function<void(Request& request, Response& response, std::function<void()> next)>;
	
	using GlobalExceptionHandler = std::function<void(Request& req, Response& res, std::function<void()> routehandler)>;
};