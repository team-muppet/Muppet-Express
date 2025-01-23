# MuppetExpress

MuppetExpress is a lightweight C++ framework that brings the familiar ExpressJS-like syntax to C++. It allows developers to easily set up web servers with support for routes, middleware, and basic RESTful endpoints.

![Simple architecture image](https://github.com/user-attachments/assets/d225a918-e9f6-4422-951a-5dffe0e7388c)


## Key Features

- **ExpressJS-like Syntax**: Set up routes and endpoints with an intuitive, JavaScript-inspired API.
- **Middleware Support**: Seamlessly register and manage middleware for your web server.
- **RESTful CRUD Operations**: Use the `RestController` class to quickly add CRUD endpoints for your data types.

## Example

Here’s how simple it is to get started:

```cpp
Server server(2000);

// Add middleware
server.use([](const Request& req, Response& res, Next next) {
    std::cout << "Middleware hit!" << std::endl;
    next();
});

// Register routes
server.map("/hello", [](const Request& req, Response& res) {
    res.send("Hello, World!");
});

// Start the server
server.run();
```

## Getting Started

Clone the repository and run the build script to set up everything:

```bash
git clone https://gitlab.au.dk/team-muppet/muppet-express && cd muppet-express
./build.sh
```

This will launch a test server and open a browser with example endpoints.

---

MuppetExpress makes it easy to write web servers in C++ with modern features and a syntax you already know.
