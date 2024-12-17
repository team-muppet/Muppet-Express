#!/bin/bash

# Build script for MuppetExpress project on Linux
# Ensure this script is located in the root directory: muppet-express

# Variables
BUILD_DIR="build"
PROJECT_DIR="MuppetExpress"
SRC_DIR="MuppetExpress"
EXECUTABLE_NAME="MuppetExpress"

sudo apt update
sudo apt install -y cmake
sudo apt install -y libboost-all-dev

cd "$PROJECT_DIR"

# Create the build directory if it doesn't exist
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR" || exit 1

# Run CMake to generate Makefiles
cmake ../"$SRC_DIR" -DCMAKE_BUILD_TYPE=Release

# Check if CMake succeeded
if [ $? -ne 0 ]; then
    echo "CMake configuration failed."
    exit 1
fi

# Build the project
make -j$(nproc)

# Check if Make succeeded
if [ $? -ne 0 ]; then
    echo "Build failed."
    exit 1
fi

# Notify the user of a successful build
echo "Build successful! Executable is located at $PROJECT_DIR/$BUILD_DIR/$EXECUTABLE_NAME"

echo "Copying over wwwroot"

cp -r ../$SRC_DIR/wwwroot .

echo "Running the program"

echo ""

./"$EXECUTABLE_NAME"
