# Simple UDP Server

## Goal
Listen to UDP datagrams and dispatch potentially heavy-weight process to workers in different threads while keeping the idea of a "connection", meaning that only one worker is responsible for one client.

Note: The current approach isn't fair and does not balance work across workers evenly

## Requirements
```bash
cmake
clang | gcc
```

## How to compile
Hopefully
```bash
cmake . && cmake --build .
```

## Flags
```
--port    | -p # port server listens on
--workers | -w # number of worker threads
```
