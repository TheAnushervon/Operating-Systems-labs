#!/bin/bash

# Compile the mmu.c and pager.c programs
gcc mmu.c -o mmu
gcc pager.c -o pager

# Set the number of pages and frames
NUM_PAGES=4
NUM_FRAMES=2

# Run the pager program in the background
./pager 4 2 &

# Get the PID of the pager process
PAGER_PID=$!

# Run the mmu program
./mmu 4 "R0 R1 W1 R0 R2 W2 R0 R3 W2" $PAGER_PID
$SHELL
