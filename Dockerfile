# basic dockerfile to run this C++ project with CMake
FROM ubuntu:20.04
# Set the working directory
WORKDIR /app
# Install dependencies
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    cmake

# Copy the source code into the container
COPY . /app
# Create a build directory
RUN mkdir build
# Change to the build directory
WORKDIR /app/build
# Run CMake to configure the project
RUN cmake ..
# Build the project
RUN make
# Set the entry point to run the executable
ENTRYPOINT ["./sparz -"]
    