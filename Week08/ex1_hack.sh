#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <ex1 PID>"
    exit 1
fi

ex1_pid="$1"

address=$(grep -o "pass:.*" /proc/$ex1_pid/maps | cut -f1 -d' ')

sudo xxd -l 8 -p /proc/$ex1_pid/mem | xxd -r -p -s $address

kill -SIGKILL $ex1_pid
