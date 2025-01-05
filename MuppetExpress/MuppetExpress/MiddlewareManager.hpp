#pragma once

#include <vector>
#include "Definitions.hpp"

namespace MuppetExpress {

	class MiddlewareManager {
	public:
		void addMiddleware(Middleware middleware) {
			// Strong guarantee, ensures that _middlewareChain remains in a valid state if push_back() should fail to alloc
			if (_middlewareChain.capacity() == _middlewareChain.size())
			{
				std::vector<Middleware> tmp(_middlewareChain);
				tmp.push_back(middleware);
				_middlewareChain.swap(tmp);
			}
			else
			{
				_middlewareChain.push_back(middleware);
			}
		}

		void runChain(Request& req, Response& res, std::function<void()> endpointHandler) {
			std::size_t current = 0;

			std::function<void()> next = [&]() {
				if (current < _middlewareChain.size()) {
					Middleware& mw = _middlewareChain[current++];
					mw(req, res, next); // Pass next() to mw as a callback to continue the chain
				}
				else { 
					endpointHandler(); // If its the end of the chain, call the endpointHandler
				}
			};

			next(); // Initial next() call to start the chain
		}

		void runChainBind(Request& req, Response& res, std::function<void()> endpointHandler) {
			std::size_t current = 0;

			runNext(current, req, res, endpointHandler);
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