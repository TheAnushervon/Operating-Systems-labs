#!/bin/bash

# Check file types in /dev and /etc
echo "Checking file types in /dev:"
file /dev/*
echo "Checking file types in /etc:"
file /etc/*

# Count the number of directories in /etc
num_directories=$(ls -l /etc | grep ^d | wc -l)
echo "Number of directories in /etc: $num_directories"

# Write a hello world program ex1.c
echo -e "#include <stdio.h>\n\nint main() {\n\tprintf(\"Привет, мир!\");\n\treturn 0;\n}" > ex1.c

# Check file type before compilation
echo "File type of ex1.c before compilation:"
file ex1.c

# Compile the hello world program
gcc -o ex1 ex1.c

# Check file type after compilation
echo "File type of ex1 after compilation:"
file ex1

echo -e "#include <stdio.h>\n\nint main() {\n\tprintf(\"Hello world!\");\n\t return 0;\n}" >ex1.c
echo "Running after adding eng-words"
# Run the program and print non-English words
gcc -o ex1 ex1.c
./ex1

# Check file type again after running the program
echo "File type of ex1 after running:"
file ex1