#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: $0 <number_of_clients>"
    exit 1
fi

NUM_CLIENTS=$1

rm -f pids

echo "Starting $NUM_CLIENTS clients..."
echo "Clients will be terminated gracefully on script exit."

for ((i=1; i<=NUM_CLIENTS; i++))
do
    echo "Starting client $i"
    # store PID of the client in a file
    python3 client.py &
    echo $! >> pids
done

trap cleanup INT TERM EXIT

cleanup() {
    echo "Cleaning up..."
    if [ -f "pids" ]; then
        while read pid; do
            echo "Killing client with PID $pid"
            kill $pid
        done < pids
        rm pids
    fi

    exit 0
}

while true; do
    sleep 10
done
