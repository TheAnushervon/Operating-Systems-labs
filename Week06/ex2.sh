#!/bin/bash
gcc worker.c -o worker
gcc schduler.c -o s && ./s data.txt