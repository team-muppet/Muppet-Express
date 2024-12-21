#include "Server.hpp"
#include "StaticFileMiddleware.hpp"
#include "RestController.hpp"
#include "IdTraits.hpp"

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
int main(int argc, char** argv) {

	auto exceptionHandler = [](Request& req, Response& res, std::function<void()> routehandler) {
		try {
			// Handle request with middleware
			routehandler();
		}
		catch (const std::exception& e) {
			std::cerr << "Error: " << e.what() << std::endl;

			res.body() = "You dummy";
		}
		catch (...) {
			std::cerr << "Unspecified error occured" << std::endl;
		}
		};

	std::variant<std::string, int> port;

	constexpr int portnr = 2000;

	if constexpr (portnr != NULL) {
		std::cout << "from constexpr Port is 2000" << std::endl;
		port = portnr;
	}
	else {
		for (int i = 1; i < argc; ++i) {
			std::string arg = argv[i];

			if (arg == "-port" && (i + 1 < argc)) {
				port = argv[++i];
			}
		}
	}

	auto server = MuppetExpress::Server(port, exceptionHandler);

	server.Use(MuppetExpress::StaticFileMiddleware("wwwroot"));

	server.Use([](Request& req, Response& res, std::function<void()> next) {
		std::string str;
		for (auto& param : req.params())
		{
			std::cout << param.first + " = " + param.second << std::endl;
		}

		std::cout << "Before 1: " << res.result() << std::endl;
		res.result(http::status::unauthorized);
		next();
		std::cout << "After 1: " << res.result() << std::endl;
		});

	server.Use([](Request& req, Response& res, std::function<void()> next) {
		std::cout << "Before 2: " << res.result() << std::endl;
		next();
		std::cout << "After 2: " << res.result() << std::endl;
		});

	server.MapGet("/test", [](Request& req, Response& res) {
		res.result(http::status::ok);
		res.set(http::field::content_type, "text/plain");
		res.body() = "Hello Test!";
		});

	auto handler = [](Request& req, Response& res, Parameters& params) {
		res.result(http::status::ok);
		res.set(http::field::content_type, "text/plain");

		std::string str;

		for (auto& param : params)
		{
			str += param.first + " = " + param.second + '\n';
		}

		res.body() = str;
		};

	server.MapGet("/fish/{id}/{stupid}", handler);
	server.MapGet("/fish/{id}", handler);
	server.MapGet("/fish/", handler);

	server.MapPost("/echo", EchoFunctor());

	RestController<Pokemon, std::vector> pokemonController(server, "/pokemon", [](std::vector<Pokemon>& datastore){
			try
			{
				datastore.push_back("1,pikachu"_pokemon);
				IdTraits<typename Pokemon::IdType>::generateId();
				datastore.push_back("2,bulbasaur"_pokemon);
				IdTraits<typename Pokemon::IdType>::generateId();
				datastore.push_back("a,charmander"_pokemon);
				IdTraits<typename Pokemon::IdType>::generateId();
			}
			catch (const std::exception& e)
			{
				std::cerr << "Error: " << e.what() << std::endl;
			}
	});

	server.RunServer();

	return 0;
}