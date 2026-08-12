# Use Ubuntu 26.04 as the base image.

FROM ubuntu:26.04

# Update the package list.
#
# This makes sure that apt knows about the latest
# available packages before we try to install them.

RUN apt-get update

# Install the tools required to build the project.
#
# build-essential -> C/C++ compiler and basic build tools
# cmake          -> Project configuration and build system
# libsdl3-dev     -> SDL3 development libraries and headers

RUN apt-get install -y \
    build-essential \
    cmake \
    libsdl3-dev \
    libsdl3-ttf-dev

# Set the working directory inside the container.
#
# All commands executed after this point will use /app
# as their current directory.

WORKDIR /app

# Copy the project files from the host machine
# into the /app directory inside the container.

COPY . .

# Create the CMake build directory.
#
# This configures the project using the CMakeLists.txt
# copied into the container.

RUN cmake -S . -B build

# Compile the project.
#
# This is equivalent to running:
#
# cmake --build build
#
# on the host machine.

RUN cmake --build build

# Run the game when the container starts.

CMD ["./build/game"]