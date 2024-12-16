#include "Server.hpp";

using namespace MuppetExpress;

// Main function
int main() {

	auto server = MuppetExpress::Server();

	server.MapGet("/", [](auto& req, auto& res) {
		res.result(http::status::ok);
		res.set(http::field::content_type, "text/plain");
		res.body() = "Hello World!";
		});

	server.MapGet("/test", [](auto& req, auto& res) {
		res.result(http::status::ok);
		res.set(http::field::content_type, "text/plain");
		res.body() = "Hello Test!";
		});

	server.MapGet("/fish/", [](auto& req, auto& res) {
		res.result(http::status::ok);
		res.set(http::field::content_type, "text/plain");
		res.body() = "Hello Fish!";
		});

	server.MapPost("/echo", [](auto& req, auto& res) {
		std::string body = req.body();

		for (const auto& header : req)
		{
			res.set(header.name_string(), header.value());
		}

		res.result(http::status::ok);
		res.body() = body;
		});

	server.RunServer();

	return 0;
}