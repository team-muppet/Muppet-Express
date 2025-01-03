#pragma once

#include <vector>
#include <boost/beast/http.hpp>
#include <unordered_map>
#include <string>

namespace MuppetExpress {
	using Parameters = std::unordered_map<std::string, std::string>;

	class Request : public boost::beast::http::request<boost::beast::http::string_body> {
	public:
		const Parameters& params() const
		{
			return parameters_;
		}

		void params(Parameters params)
		{
			parameters_ = params;
		}

		void params(Parameters&& params)
		{
			parameters_ = std::move(params);
		}

	private:
		Parameters parameters_;
	};
};