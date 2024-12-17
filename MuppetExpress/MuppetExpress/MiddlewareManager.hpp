#pragma once

#include <vector>
#include "MuppetExpressDefinitions.hpp"

namespace MuppetExpress {

	class MiddlewareManager {
	public:
		void addMiddleware(Middleware middleware) {
			_middlewareChain.push_back(std::move(middleware));
		}

		void runChain(Request& req, Response& res, std::function<void()> endpointHandler) {
			std::size_t current = 0;

			// [&] Captures the variables in the scope by reference so they can be used in the lambda
			std::function<void()> next = [&]() {
				if (current < _middlewareChain.size()) {
					Middleware& mw = _middlewareChain[current++]; // Get the current mw, then increment current so when next() is called it will be the next in line
					mw(req, res, next); // Looks kind of like recursion, however its more like a callback for next() to be called in the mw to continue the chain
				}
				else { 
					endpointHandler(); // If there are no more middleware, call the endpointHandler
				}
			};

			next(); // Initial next() call to start the chain
		}

		void runChainBind(Request& req, Response& res, std::function<void()> endpointHandler) {
			std::size_t current = 0;

			auto next = std::bind(&MiddlewareManager::runNext, this, std::ref(current), std::ref(req), std::ref(res), endpointHandler);

			next();
		}

	private:
		std::vector<Middleware> _middlewareChain;

		void runNext(std::size_t current, Request& req, Response& res, std::function<void()> endpointHandler) {
			if (current < _middlewareChain.size()) {
				Middleware& mw = _middlewareChain[current++];
				auto next = std::bind(&MiddlewareManager::runNext, this, std::ref(current), std::ref(req), std::ref(res), endpointHandler);
				mw(req, res, next);
			}
			else
			{
				endpointHandler();
			}
		}
	};
}