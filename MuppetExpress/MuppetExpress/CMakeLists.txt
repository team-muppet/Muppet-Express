cmake_minimum_required(VERSION 3.10)
project(MuppetExpress)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)

# Find Boost libraries (Filesystem and System)
find_package(Boost REQUIRED COMPONENTS filesystem system)

# Source files
set(SOURCES
    MuppetExpress.cpp
    Server.hpp
    Router.hpp
    Request.hpp
    MiddlewareManager.hpp
    StaticFileMiddleware.hpp
    Definitions.hpp
)

# Add executable
add_executable(MuppetExpress ${SOURCES})

# Link Boost libraries
target_link_libraries(MuppetExpress Boost::filesystem Boost::system)
