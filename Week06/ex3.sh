#!/bin/bash
gcc worker.c -o worker
gcc schduler_sjf.c -o s && ./s data.txt