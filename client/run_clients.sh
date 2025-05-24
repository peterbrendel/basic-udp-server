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

    python3 client.py &

    pids+=($!)
done

trap cleanup INT TERM

cleanup() {
    echo "Cleaning up..."

    for pid in "${pids[@]}"; do
        echo "Killing client with PID $pid"
        kill -9 "$pid"
    done

    exit 0
}

while true; do
    sleep 10
done
