# Simple UDP Server

## Goal
Listen to UDP datagrams and dispatch potentially heavy-weight process to workers in different threads while keeping the idea of a "connection", meaning that only one worker is responsible for one client.

Notes:
- This approach isn't fair and does not balance work across workers evenly, although it ensures that only one worker is responsible for a specific user.
- It heavily relies on constant heap allocation, it's slow and inconsistent. To improve performance a memory pool could be used instead.


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

## Test clients
under `/client` there's a client script and a shell script to manage those scripts

```bash
sh client/run_clients.sh 10 # to run 10 clients

# ^C will kill all processes
```

## TODO
- Handle server interrupts by stopping and joining workers
