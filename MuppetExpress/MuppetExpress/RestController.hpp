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

    //// template <IsDTO DTO>
    //template <typename DTO, template <typename> typename Datastore>
    //    requires IsDatastore<Datastore, DTO> && IsDTO<DTO>
    //class RestController {
    //public:
    //    RestController(Server& server, const std::string& basePath, std::pmr::memory_resource* mr = std::pmr::get_default_resource(), std::optional<std::function<void(Datastore<DTO>& datastore)>> seedFunction = std::nullopt)
    //        : server_(server), basePath_(basePath) _mr(mr) {
    //        setupHandlers();
    //        if (seedFunction)
    //        {
				//seedFunction.value()(dataStore_);
    //        }
    //    }
    template <typename DTO, template <typename> typename Datastore>
        requires IsDatastore<Datastore, DTO>&& IsDTO<DTO>
    class RestController 
    {
    public:
        // The constructor
        RestController(Server& server,
            const std::string& basePath,
            std::pmr::memory_resource* mr = std::pmr::get_default_resource(),
            std::optional<std::function<void(Datastore<DTO>&)>> seedFunction = std::nullopt)
            : server_(server)
            , basePath_(basePath)
            , _mr(mr)
            , dataStore_(makeDataStore())  // Construct dataStore_ appropriately
        {
            setupHandlers();
            if (seedFunction) {
                seedFunction.value()(dataStore_);
            }
        }

    private:
        Server& server_;
        std::string basePath_;
        Datastore<DTO> dataStore_;
        std::pmr::memory_resource* _mr;

        //ChatCode
        Datastore<DTO> makeDataStore() {
            // If it has a constructor that takes pmr::memory_resource*, call it.
            // Otherwise, do a normal default constructor.
            if (_mr) {
                return Datastore<DTO>(_mr);
            }
            else {
                return Datastore<DTO>{};
            }
        }



        void setupHandlers() {
            server_.MapGet(basePath_, [this](Request& req, Response& res) {
                handleGetAll(req, res);
                });

            server_.MapGet(basePath_ + "/{id}", [this](Request& req, Response& res, Parameters& params) {
                handleGetById(req, res, params);
                });

            server_.MapPost(basePath_, [this](Request& req, Response& res) {
                handleCreate(req, res);
                });

            server_.MapPut(basePath_ + "/{id}", [this](Request& req, Response& res, Parameters& params) {
                handleUpdate(req, res, params);
                });

            server_.MapDelete(basePath_ + "/{id}", [this](Request& req, Response& res, Parameters& params) {
                handleDelete(req, res, params);
                });
			server_.MapGet(basePath_ + "/reset", [this](Request& req, Response& res) {
				handleResetPmr(req, res);
				});
        }

		// GET all items pmr safe
        void handleGetAll(Request& req, Response& res) {
            res.result(http::status::ok);
            res.set(http::field::content_type, "application/json");

            json responseBody = dataStore_;
            res.body() = responseBody.dump();
        }

        // GET item by ID
        void handleGetById(Request& req, Response& res, Parameters& params) {
            // Keep this so we can trigger global exception handler
            auto id = IdTraits<typename DTO::IdType>::convert(params["id"]);

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
        //void handleCreate(Request& req, Response& res) {
        //    try {
        //        json jsonBody = json::parse(req.body());
        //        DTO newItem = jsonBody.get<DTO>();

        //        newItem.Id = IdTraits<typename DTO::IdType>::generateId();

        //        dataStore_.push_back(newItem);

        //        res.result(http::status::created);
        //        res.set(http::field::content_type, "application/json");
        //        res.body() = json(newItem).dump();
        //    }
        //    catch (const std::exception& e) {
        //        res.result(http::status::bad_request);
        //        res.set(http::field::content_type, "application/json");
        //        res.body() = json{ {"error", e.what()} }.dump();
        //    }
        //}

        // PUT (Update an item by ID) not pmr safe
        //void handleUpdate(Request& req, Response& res, Parameters& params) {
        //    try {
        //        auto id = IdTraits<typename DTO::IdType>::convert(params["id"]);

        //        auto it = std::find_if(dataStore_.begin(), dataStore_.end(), [&](const DTO& dto) {
        //            return dto.Id == id;
        //            });

        //        if (it != dataStore_.end()) {
        //            json jsonBody = json::parse(req.body());
        //            DTO updatedItem = jsonBody.get<DTO>();
        //            updatedItem.Id = id;  // Keep the ID the same

        //            *it = updatedItem;

        //            res.result(http::status::ok);
        //            res.set(http::field::content_type, "application/json");
        //            res.body() = json(updatedItem).dump();
        //        }
        //        else {
        //            res.result(http::status::not_found);
        //            res.set(http::field::content_type, "application/json");
        //            res.body() = R"({ "error": "Item not found" })";
        //        }
        //    }
        //    catch (const std::exception& e) {
        //        res.result(http::status::bad_request);
        //        res.set(http::field::content_type, "application/json");
        //        res.body() = json{ {"error", e.what()} }.dump();
        //    }
        //}

        // DELETE an item by I, pmr safe
        void handleDelete(Request& req, Response& res, Parameters& params) {
           auto id = IdTraits<typename DTO::IdType>::convert(params["id"]);

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

        //pmr specifik stuff
        // Example of manually parsing JSON into a pmr-aware DTO
        void handleCreate(Request& req, Response& res) {
            try {
                auto j = json::parse(req.body());


                if constexpr (std::is_same_v<DTO, PmrPokemon>) {
                 
                    auto name = j.at("Name").get<std::string>();
					auto id = IdTraits<typename DTO::IdType>::generateId(); // remember to count this down if it does not work


                    DTO newItem{ id, name, _mr };

                    dataStore_.push_back(newItem);
                }
                else {

                    DTO newItem = j.get<DTO>();
                    newItem.Id = IdTraits<typename DTO::IdType>::generateId();
                    dataStore_.push_back(newItem);
                }

               res.result(http::status::created);
                res.set(http::field::content_type, "application/json");
                res.body() = json(dataStore_.back()).dump();
            }
            catch (const std::exception& e) {
            res.result(http::status::bad_request);
            res.set(http::field::content_type, "application/json");
            res.body() = json{ {"error", e.what()} }.dump();
            }
        }

        // PUT (Update an item by ID) trying pmr safe
        void handleUpdate(Request& req, Response& res, Parameters& params) {
            try {
                auto id = IdTraits<typename DTO::IdType>::convert(params["id"]);
                auto j = json::parse(req.body());

                auto it = std::find_if(dataStore_.begin(), dataStore_.end(), [&](const DTO& dto) {
                    return dto.Id == id;
                    });

                auto name = j.at("Name").get<std::string>();

                DTO newItem{ id, name, _mr };

                if ( it != dataStore_.end())
                {
				    dataStore_.erase(it);
                    dataStore_.push_back(newItem);
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

        void handleResetPmr(Request& req, Response& res)
        {
			if (auto* mbr = dynamic_cast<std::pmr::monotonic_buffer_resource*>(_mr)) 
            {
				mbr->release();
			}
        }

    };

}  // namespace MuppetExpress
