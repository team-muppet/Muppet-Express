#pragma once

#include <nlohmann/json.hpp>
#include "Server.hpp"
#include <vector>
#include <algorithm>
#include "IdTraits.hpp"
#include "PokemonModel.hpp"

namespace MuppetExpress {
	using json = nlohmann::json;

    template <typename DTO>
    concept HasId = requires(DTO dto) {
        typename DTO::IdType;
        { dto.Id } -> std::same_as<typename DTO::IdType>;
    };

	template <typename DTO>
	concept Serializable = requires(DTO obj, json j) {
		{ to_json(j, obj) } -> std::same_as<void>;
		{ from_json(j, obj) } -> std::same_as<void>;
	};

    template <typename DTO>
    concept IsDTO = requires {
        HasId<DTO> && Serializable<DTO>;
    };

	template <template<typename> typename DataStore, typename DTO>
	concept IsDatastore = requires(DataStore<DTO> store, DTO dto) {
		{ store.begin() } -> std::input_iterator;
		{ store.end() } -> std::sentinel_for<decltype(store.begin())>; // Sentinel checks if its a valid end marker for store.begin()
		{ store.push_back(dto) } -> std::same_as<void>;
		{ store.erase(store.begin(), store.end()) } -> std::same_as<typename DataStore<DTO>::iterator>;
		{ json(store) } -> std::same_as<json>;
	};

    template <typename DTO, template <typename> typename Datastore>
        requires IsDatastore<Datastore, DTO>&& IsDTO<DTO>
    class RestController 
    {
    public:
        using allocator_type = std::pmr::polymorphic_allocator<std::byte>;

        // The constructor with pmr
        RestController(
            Server& server,
            const std::string& basePath,
            allocator_type alloc,
            std::optional<std::function<void(Datastore<DTO>& datastore, IdTraits<typename DTO::IdType>& idGenerator)>> seedFunction = std::nullopt)
            : _alloc(alloc)
        {
            if constexpr (requires { Datastore<DTO>(_alloc); }) {
                dataStore_ = Datastore<DTO>(_alloc);
            }
            else {
                dataStore_ = Datastore<DTO>{};
            }

            setupHandlers(server, basePath);

            if (seedFunction) {
                seedFunction.value()(dataStore_, idGenerator_);
            }
        }

        RestController(
            Server& server,
            const std::string& basePath,
            std::optional<std::function<void(Datastore<DTO>& datastore, IdTraits<typename DTO::IdType>& idGenerator)>> seedFunction = std::nullopt)
        {
            dataStore_ = Datastore<DTO>{};
 
            setupHandlers(server, basePath);

            if (seedFunction) {
                seedFunction.value()(dataStore_, idGenerator_);
            }
        }

    private:
        Datastore<DTO> dataStore_;
        allocator_type _alloc;
        IdTraits<typename DTO::IdType> idGenerator_ = IdTraits<typename DTO::IdType>();
		std::mutex mtx;

        void setupHandlers(Server& server, const std::string& basePath) {
            server.MapGet(basePath, [this](Request& req, Response& res) {
                handleGetAll(req, res);
                });

            server.MapGet(basePath + "/{id}", [this](Request& req, Response& res, Parameters& params) {
                handleGetById(req, res, params);
                });

            server.MapPost(basePath, [this](Request& req, Response& res) {
                handleCreate(req, res);
                });

            server.MapPut(basePath + "/{id}", [this](Request& req, Response& res, Parameters& params) {
                handleUpdate(req, res, params);
                });

            server.MapDelete(basePath + "/{id}", [this](Request& req, Response& res, Parameters& params) {
                handleDelete(req, res, params);
                });
        }

		// GET all items
        void handleGetAll(Request& req, Response& res) {
            std::scoped_lock lock(mtx);
            res.result(http::status::ok);
            res.set(http::field::content_type, "application/json");

			json responseBody = dataStore_;
			res.body() = responseBody.dump();
		}

        // GET item by ID
        void handleGetById(Request& req, Response& res, Parameters& params) {
			std::scoped_lock lock(mtx);
            // Keep this so we can trigger global exception handler
            auto id = idGenerator_.convert(params["id"]);

            auto it = std::find_if(dataStore_.begin(), dataStore_.end(), [&](const DTO& dto) {
                return dto.Id == id;
                });

			if (it != dataStore_.end()) {
				res.result(http::status::ok);
				res.set(http::field::content_type, "application/json");
				res.body() = json(*it).dump();
			}
			else {
				res.result(http::status::not_found);
				res.set(http::field::content_type, "application/json");
				res.body() = R"({ "error": "Item not found" })";
			}
		}

        //// POST (Create a new item)
      void handleCreate(Request& req, Response& res) {
            std::scoped_lock lock(mtx);
            try {
                json jsonBody = json::parse(req.body());
                DTO newItem = jsonBody.get<DTO>();

                newItem.Id = idGenerator_.generateId();

                if constexpr (requires { Datastore<DTO>(_alloc); })
				{
                    DTO newItemCopy(newItem, _alloc);
					dataStore_.push_back(std::move(newItemCopy));
				}
				else
				{
                    dataStore_.push_back(newItem);
				}

				res.result(http::status::created);
				res.set(http::field::content_type, "application/json");
				res.body() = json(newItem).dump();
			}
			catch (const std::exception& e) {
				res.result(http::status::bad_request);
				res.set(http::field::content_type, "application/json");
				res.body() = json{ {"error", e.what()} }.dump();
			}
		}

        // PUT (Update an item by ID)
        void handleUpdate(Request& req, Response& res, Parameters& params) {
            std::scoped_lock lock(mtx);
            try {
                auto id = idGenerator_.convert(params["id"]);

                auto it = std::find_if(dataStore_.begin(), dataStore_.end(), [&](const DTO& dto) {
                    return dto.Id == id;
                    });

				if (it != dataStore_.end()) {
					json jsonBody = json::parse(req.body());
					DTO updatedItem = jsonBody.get<DTO>();
					updatedItem.Id = id;

					*it = updatedItem;

					res.result(http::status::ok);
					res.set(http::field::content_type, "application/json");
					res.body() = json(updatedItem).dump();
				}
				else {
					res.result(http::status::not_found);
					res.set(http::field::content_type, "application/json");
					res.body() = R"({ "error": "Item not found" })";
				}
			}
			catch (const std::exception& e) {
				res.result(http::status::bad_request);
				res.set(http::field::content_type, "application/json");
				res.body() = json{ {"error", e.what()} }.dump();
			}
		}

        // DELETE an item by Id
        void handleDelete(Request& req, Response& res, Parameters& params) {
            std::scoped_lock lock(mtx);
            auto id = idGenerator_.convert(params["id"]);

            auto it = std::remove_if(dataStore_.begin(), dataStore_.end(), [&](const DTO& dto) {
                return dto.Id == id;
                });

            if (it != dataStore_.end()) {
                dataStore_.erase(it, dataStore_.end());
                res.result(http::status::ok);
                res.set(http::field::content_type, "application/json");
                res.body() = R"({ "message": "Item deleted" })";
            }
            else {
                res.result(http::status::not_found);
                res.set(http::field::content_type, "application/json");
                res.body() = R"({ "error": "Item not found" })";
            }
        }

    };

}  // namespace MuppetExpress
