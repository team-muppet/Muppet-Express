cmake_minimum_required(VERSION 3.10)
project(MuppetExpress)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find Boost libraries (Filesystem and System)
find_package(Boost REQUIRED COMPONENTS filesystem system)

# Source files
set(SOURCES
    MuppetExpress.cpp
)

# Add executable
add_executable(MuppetExpress ${SOURCES})

# Include directories for headers (lib directory for nlohmann/json and the current directory for your headers)
target_include_directories(MuppetExpress PRIVATE ${CMAKE_SOURCE_DIR}/../lib)

# Link Boost libraries
target_link_libraries(MuppetExpress Boost::filesystem Boost::system)
