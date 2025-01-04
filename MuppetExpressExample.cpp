int main()
{
    const int port = 3001;
    auto server = MuppetExpress::Server(port);

    server.MapGet("/", [](Request& req, Response& res) {
        res.body() = "Hello World!";
        });

    server.RunServer();
}