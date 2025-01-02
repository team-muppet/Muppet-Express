#include "Server.hpp"
#include "StaticFileMiddleware.hpp"
#include "RestController.hpp"
//#include "PmrDatastore.hpp"
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
	std::array<std::byte, 100> buffer;
	StatsResource sr;
	
	std::pmr::monotonic_buffer_resource mbr{ buffer.data(), buffer.size(), std::pmr::null_memory_resource() };
	std::pmr::synchronized_pool_resource spr;

	/*StatsResource sr;

	std::pmr::vector<Pokemon> PMRtest({ "1,pikachu"_pokemon, "2,bulbasaur"_pokemon, "3,charmander"_pokemon }, &sr);

	sr.printStats();

	PMRtest.push_back("4,squirtle"_pokemon);

	sr.printStats();*/

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
		port = portnr;
	}
	else {
		if (argc == 0) {
			port = "2001";
		}
		else {
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

	/*RestController<Pokemon, std::pmr::vector> pokemonController(server, "/pokemon",
		[](std::pmr::vector<Pokemon>& datastore, std::size_t& idCounter) {
		try
		{
			datastore.push_back("1,pikachu"_pokemon);
			++idCounter;
			datastore.push_back("2,bulbasaur"_pokemon);
			++idCounter;
			datastore.push_back("a,charmander"_pokemon);
			++idCounter;
		}
		catch (const std::exception& e)
		{
			std::cerr << "Error: " << e.what() << std::endl;
		}
		});*/

	RestController<PmrPokemon, std::pmr::vector> pokemonController(server, "/pokemon", &mbr);


	//RestController<Pokemon, std::vector> pokemonController(server, "/pokemon", [](std::vector<Pokemon>& datastore) {
	//	try
	//	{
	//		datastore.push_back("1,pikachu"_pokemon);
	//		//++idCounter;
	//		datastore.push_back("2,bulbasaur"_pokemon);
	//		//++idCounter;
	//		datastore.push_back("a,charmander"_pokemon);
	//		//++idCounter;

	//	}
	//	catch (const std::exception& e)
	//	{
	//		std::cerr << "Error: " << e.what() << std::endl;
	//	}

	//	if (!datastore.empty()) {
	//		auto maxIt = std::max_element(
	//			datastore.begin(), datastore.end(),
	//			[](const Pokemon& a, const Pokemon& b) {
	//				return a.Id < b.Id;
	//			}
	//		);
	//		IdTraits<typename Pokemon::IdType>::updateCounter((*maxIt).Id);
	//		}		
	//	});
	/*RestController<Pokemon, std::vector> pokemonController(server, "/pokemon", [](std::vector<Pokemon>& datastore){
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
	});*/

	//RestController<Person, std::list> personController(server, "/person");

	//RestController<Pokemon, std::list> personController(server, "/pokemon");

	server.RunServer();

	return 0;
}