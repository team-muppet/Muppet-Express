auto printParamHandler = [](Request& req, Response& res, Parameters& params) {
	...
};

server.MapGet("/api/params/{id}/{name}", printParamHandler);
server.MapGet("/api/params/{id}", printParamHandler);
server.MapGet("/api/params/", printParamHandler);



