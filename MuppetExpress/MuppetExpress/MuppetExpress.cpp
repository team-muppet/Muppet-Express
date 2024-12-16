#include "Server.hpp";


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

	server.RunServer();

	return 0;
}