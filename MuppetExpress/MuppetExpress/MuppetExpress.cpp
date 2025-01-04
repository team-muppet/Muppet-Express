#include "Server.hpp"
#include "StaticFileMiddleware.hpp"
#include "RestController.hpp"
#include <memory_resource>
#include "StatsResourse.hpp"

#include "IdTraits.hpp"
#include "PokemonModel.hpp"
#include "PersonModel.hpp"

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
	std::array<std::byte, 1096> buffer; // 200 bytes of memory, remember SSO in pmr::string
	StatsResource sr;

	std::pmr::pool_options opts;
	opts.max_blocks_per_chunk = 10;
	opts.largest_required_pool_block = 4096;  // or some smaller/larger value

	std::pmr::monotonic_buffer_resource mbr{ buffer.data(), buffer.size(), std::pmr::null_memory_resource() };
	//std::pmr::monotonic_buffer_resource mbr{ buffer.data(), buffer.size(), &sr };
	std::pmr::synchronized_pool_resource spr(opts, &sr);

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

	constexpr int portnr = 2000; // Assign to 0 to make the argc argv work

	if constexpr (portnr != 0) {
		port = portnr;
	}
	else
	{
		port = "2000";
		if (argc != 0) {
			for (int i = 1; i < argc; ++i) {
				std::string arg = argv[i];
				if (arg == "-port") {
					port = argv[++i];
				}
			}
		}
	}

	auto server = MuppetExpress::Server(port, exceptionHandler);

	server.Use(MuppetExpress::StaticFileMiddleware("wwwroot"));

	server.Use([](Request& req, Response& res, std::function<void()> next) {
		std::cout << "Before endpoint: " << res.result() << std::endl;
		next();
		std::cout << "After endpoint: " << res.result() << std::endl;
	});

	server.MapGet("/api/test", [](Request& req, Response& res) {
		res.result(http::status::ok);
		res.set(http::field::content_type, "text/plain");
		res.body() = "Hello Test!";
	});

	auto printParamHandler = [](Request& req, Response& res, Parameters& params) {
		res.result(http::status::ok);
		res.set(http::field::content_type, "text/plain");

		std::string str;

		for (auto& param : params)
		{
			str += param.first + " = " + param.second + '\n';
		}

		res.body() = str;
	};

	server.MapGet("/api/echo", EchoFunctor());
	server.MapPost("/api/echo", EchoFunctor());
	server.MapPut("/api/echo", EchoFunctor());
	server.MapDelete("/api/echo", EchoFunctor());

	server.MapGet("/api/params/{id}/{name}", printParamHandler);
	server.MapGet("/api/params/{id}", printParamHandler);
	server.MapGet("/api/params/", printParamHandler);

	RestController<PmrPokemon, std::pmr::vector> pokemonController(server, "/api/pokemon", &mbr, [&mbr](std::pmr::vector<PmrPokemon>& datastore, IdTraits<typename PmrPokemon::IdType>& idGenerator) {
		try
		{
			datastore.emplace_back("1,pikachu"_pmrPokemon, &mbr);
			idGenerator.generateId();
			datastore.emplace_back("2,bulbasaur"_pmrPokemon, &mbr);
			idGenerator.generateId();
			datastore.emplace_back("a,charmander"_pmrPokemon, &mbr);
			idGenerator.generateId();
		}
		catch (const std::exception& e)
		{
			std::cerr << "Error: " << e.what() << std::endl;
		}
	});

	RestController<Person, std::list> personController(server, "/api/person");

	server.RunServer();

	return 0;
}

void helloWorldExample()
{
	const int port = 3001;

	auto server = MuppetExpress::Server(port);

	server.MapGet("/", [](Request& req, Response& res) {
		res.body() = "Hello World!";
		});

	server.RunServer();
}