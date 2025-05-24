# Simple UDP Server

## Goal
Listen to UDP datagrams and dispatch potentially heavy-weight process to workers in different threads while keeping the idea of a "connection", meaning that only one worker is responsible for one client.

Note: The current approach isn't fair and does not balance work across workers evenly

## Requirements

- cmake 3.25 or later
- clang or gcc with c++20 support
- (optional) Docker

## Docker
```bash
docker build . --tag sparz-server:latest
docker run --rm sparz-server
```

## How to compile
Hopefully the bash script will help
```bash
./compile <flags>
```

### Compilation flags
```
release   # build in release mode
clean     # clean cmake build
```

## Flags
```
--port    | -p # port server listens on
--workers | -w # number of worker threads
```
