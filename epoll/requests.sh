#!/bin/bash

HOST="127.0.0.1"
PORT=5678
CLIENTS=10000

for i in $(seq 1 $CLIENTS); do
    (
        # Send something into telnet and keep it open a bit
        { 
            echo "Hello from client $i"
            sleep 10
        } | telnet $HOST $PORT > /dev/null 2>&1
    ) &
done

wait
echo "All clients finished."
