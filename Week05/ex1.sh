#!/bin/bash
mkdir /tmp/ex1
gcc publisher.c -o publisher
gcc subscriber.c -o subscriber

chmod +x publisher
chmod +x subscriber

n=$1;

for ((i = 1; i <= n; i++)); do 
    gnome-terminal -- ./subscriber $i
done

gnome-terminal -- ./publisher $n