struct EchoFunctor {
	void operator()(Request& req, Response& res) {
		...
	}
};

auto printParamHandler = [](Request& req, Response& res, Parameters& params) {
	...
};

server.MapPost("/api/echo", EchoFunctor());

server.MapGet("/api/params/{id}/{name}", printParamHandler);
server.MapGet("/api/params/{id}", printParamHandler);




