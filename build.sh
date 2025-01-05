#!/bin/bash

# Build script for MuppetExpress project on Linux
# Ensure this script is located in the root directory: muppet-express

# Variables
BUILD_DIR="build"
PROJECT_DIR="MuppetExpress"
SRC_DIR="MuppetExpress"
EXECUTABLE_NAME="MuppetExpress"

gnome-terminal -- bash -c "./fetchPokemonImages.sh"

sudo apt update
sudo apt install -y cmake gnome-terminal libboost-all-dev

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

# Open the executable in a new terminal and capture its output
gnome-terminal -- bash -c "./$EXECUTABLE_NAME 2>&1 | tee /tmp/muppet_output.log; exec bash" &

# Wait briefly to allow the server to start and print the URL
sleep 2

# Monitor the temporary log file for the URL and open Firefox detached
LOG_FILE="/tmp/muppet_output.log"
tail -F "$LOG_FILE" | while read -r line; do
    echo "$line"
    if [[ $line =~ http://[0-9a-zA-Z.:]+ ]]; then
        URL="${BASH_REMATCH[0]}"
        echo "Opening $URL in Firefox..."
        firefox "$URL" >/dev/null 2>&1 &
        disown
        break
    fi
done