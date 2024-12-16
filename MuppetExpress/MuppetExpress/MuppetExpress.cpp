#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <iostream>
#include <vector>
#include <thread>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

// Handle an HTTP request
net::awaitable<void> handle_request(tcp::socket socket) {
    try {
        beast::flat_buffer buffer;
        http::request<http::string_body> req;
        http::response<http::string_body> res;

        // Read the HTTP request
        co_await http::async_read(socket, buffer, req, net::use_awaitable);

        // Handle the request
        if (req.target() == "/") {
            res.result(http::status::ok);
            res.set(http::field::content_type, "text/plain");
            res.body() = "Welcome to your Boost.Beast server with C++20!";
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

// Main function
int main() {
    try {
        net::io_context ioc;

        // Spawn the listener coroutine
        net::co_spawn(ioc, listener(tcp::endpoint(tcp::v4(), 8080)), net::detached);

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

        std::cout << "Server is running on http://127.0.0.1:8080 with "
            << thread_count << " threads..." << std::endl;

        // Join all threads
        for (auto& t : threads) {
            t.join();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
    }

    return 0;
}
