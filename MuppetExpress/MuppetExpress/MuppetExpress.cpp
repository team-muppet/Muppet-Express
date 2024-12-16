#include "Server.hpp";

using namespace MuppetExpress;

struct EchoFunctor {
	void operator()(Request& req, Response& res) {
		std::string body = req.body();

		for (const auto& header : req)
		{
			res.set(header.name_string(), header.value());
		}

		res.result(http::status::ok);
		res.body() = body;
	}
};

// Main function
int main() {

	auto server = MuppetExpress::Server("10");

	server.Use([](Request& req, Response& res, std::function<void()> next) {
		std::cout << "Before:" << res.result() << std::endl;
		return;
		next();
		std::cout << "After:" << res.result() << std::endl;
		});

	server.MapGet("/", [](Request& req, Response& res) {
		res.result(http::status::ok);
		res.set(http::field::content_type, "text/plain");
		res.body() = "Hello World!";
		});

	server.MapGet("/test", [](Request& req, Response& res) {
		res.result(http::status::ok);
		res.set(http::field::content_type, "text/plain");
		res.body() = "Hello Test!";
		});

	server.MapGet("/fish/", [](Request& req, Response& res) {
		res.result(http::status::ok);
		res.set(http::field::content_type, "text/plain");
		res.body() = "Hello Fish!";
		});

	server.MapPost("/echo", EchoFunctor());

	server.RunServer();

	return 0;
}