#pragma once

#include <nlohmann/json.hpp>
#include "Server.hpp"
#include <vector>
#include <algorithm>

namespace MuppetExpress {

    using json = nlohmann::json;

    template <typename DTO>
    concept HasId = requires(DTO dto) {
        { dto.Id } -> std::convertible_to<int>;
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
    
    // template <IsDTO DTO>
    template <typename DTO>
        requires IsDTO<DTO>
    class RestController {
    public:
        RestController(Server& server, const std::string& basePath)
            : server_(server), basePath_(basePath) {
            setupHandlers();
        }

    private:
        Server& server_;
        std::string basePath_;
        std::vector<DTO> dataStore_;
        std::size_t idCounter_;

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
        }

        // GET all items
        void handleGetAll(Request& req, Response& res) {
            res.result(http::status::ok);
            res.set(http::field::content_type, "application/json");

            json responseBody = dataStore_;
            res.body() = responseBody.dump();
        }

        // GET item by ID
        void handleGetById(Request& req, Response& res, Parameters& params) {
            int id = std::stoi(params["id"]);
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

        // POST (Create a new item)
        void handleCreate(Request& req, Response& res) {
            try {
                json jsonBody = json::parse(req.body());
                DTO newItem = jsonBody.get<DTO>();

                newItem.Id = ++idCounter_;

                dataStore_.push_back(newItem);

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
            try {
                int id = std::stoi(params["id"]);
                auto it = std::find_if(dataStore_.begin(), dataStore_.end(), [&](const DTO& dto) {
                    return dto.Id == id;
                    });

                if (it != dataStore_.end()) {
                    json jsonBody = json::parse(req.body());
                    DTO updatedItem = jsonBody.get<DTO>();
                    updatedItem.Id = id;  // Keep the ID the same

                    *it = updatedItem;

                    res.result(http::status::ok);
                    res.set(http::field::content_type, "application/json");
                    res.body() = json(updatedItem).dump();
                }
                else {
                    res.result(http::status::not_found);
                    res.body() = R"({ "error": "Item not found" })";
                }
            }
            catch (const std::exception& e) {
                res.result(http::status::bad_request);
                res.body() = json{ {"error", e.what()} }.dump();
            }
        }

        // DELETE an item by ID
        void handleDelete(Request& req, Response& res, Parameters& params) {
            int id = std::stoi(params["id"]);
            auto it = std::remove_if(dataStore_.begin(), dataStore_.end(), [&](const DTO& dto) {
                return dto.Id == id;
                });

            if (it != dataStore_.end()) {
                dataStore_.erase(it, dataStore_.end());
                res.result(http::status::ok);
                res.body() = R"({ "message": "Item deleted" })";
            }
            else {
                res.result(http::status::not_found);
                res.body() = R"({ "error": "Item not found" })";
            }
        }
    };

}  // namespace MuppetExpress
