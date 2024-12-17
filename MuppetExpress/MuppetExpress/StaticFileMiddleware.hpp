#pragma once

#include <boost/beast.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Definitions.hpp"

namespace MuppetExpress {

    namespace http = boost::beast::http;
    namespace fs = boost::filesystem;

    class StaticFileMiddleware {
    public:
        // Constructor that ensures the root directory exists
        StaticFileMiddleware(const std::string& rootDir = "wwwroot") : _rootDir(rootDir) {
            createRootDirectory();
        }

        void operator()(Request& req, Response& res, std::function<void()> next) {
            next();  // Call the next middleware in the chain

            // Check if the response status is 404
            if (res.result() == http::status::not_found) {
                // Construct the base path from the request target
                std::string targetPath = std::string(req.target());
                std::string path = _rootDir + targetPath;

                // List of potential file candidates to check
                std::vector<std::string> candidates = {
                    path,                      // Exact path (e.g., wwwroot/about)
                    path + ".html",            // Path with .html appended (e.g., wwwroot/about.html)
                    path + "/index.html"       // Path as a directory with index.html (e.g., wwwroot/about/index.html)
                };

                // Iterate through the candidates and serve the first valid file found
                for (const auto& candidate : candidates) {
                    fs::path filePath = fs::absolute(fs::path(candidate));
                    if (fs::exists(filePath) && fs::is_regular_file(filePath)) {
                        serveFile(filePath, res);
                        return;
                    }
                }
            }
        }

    private:
        std::string _rootDir;

        void createRootDirectory() {
            if (!fs::exists(_rootDir)) {
                try {
                    fs::create_directory(_rootDir);
                    std::cout << "Created root directory: " << _rootDir << std::endl;
                }
                catch (const fs::filesystem_error& e) {
                    std::cerr << "Failed to create root directory: " << e.what() << std::endl;
                }
            }
        }

        void serveFile(const fs::path& filePath, Response& res) {
            std::ifstream file(filePath.string(), std::ios::binary);
            if (!file) {
                return;
            }

            // Read the file contents into a string
            std::string body((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

            // Set the response details
            res.result(http::status::ok);
            res.set(http::field::content_type, getContentType(filePath));
            res.body() = std::move(body);
        }

        std::string getContentType(const fs::path& filePath) {
            static const std::unordered_map<std::string, std::string> mimeTypes = {
                {".html", "text/html"},
                {".htm", "text/html"},
                {".css", "text/css"},
                {".js", "application/javascript"},
                {".png", "image/png"},
                {".jpg", "image/jpeg"},
                {".jpeg", "image/jpeg"},
                {".gif", "image/gif"},
                {".svg", "image/svg+xml"},
                {".txt", "text/plain"},
                {".json", "application/json"}
            };

            std::string ext = filePath.extension().string();
            auto it = mimeTypes.find(ext);
            return (it != mimeTypes.end()) ? it->second : "application/octet-stream";
        }
    };
}  // namespace MuppetExpress
