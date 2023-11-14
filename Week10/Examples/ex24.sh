#!/bin/bash

# Remove the ./tmp folder if it exists and create a symbolic link tmp1 for it
[ -d ./tmp ] && rm -r ./tmp
ln -s ./tmp tmp1

# Display contents with inode numbers before creating ./tmp
echo "Listing contents with inode numbers before creating ./tmp:"
ls -li

# Create the folder ./tmp
mkdir ./tmp

# Display contents with inode numbers after creating ./tmp
echo "Listing contents with inode numbers after creating ./tmp:"
ls -li

# Generate a file ex1.txt using gen.sh and add it to ./tmp
./gen.sh 5 ex1.txt
mv ex1.txt ./tmp/

# Check the content of the ./tmp1 folder
echo "Contents of ./tmp1:"
ls -li ./tmp1

# Create a symbolic link tmp1/tmp2 to the ./tmp folder (symbolic link to itself)
ln -s $(realpath ./tmp) ./tmp1/tmp2

# Generate another file ex1.txt using gen.sh and add it to ./tmp1/tmp2
./gen.sh 5 ex1.txt
mv ex1.txt ./tmp1/tmp2/

# Display the content of the ./tmp1/tmp2 folder
echo "Contents of ./tmp1/tmp2:"
ls -li ./tmp1/tmp2

# Attempt to access the sub-folders ./tmp1/tmp2/tmp2/tmp2/....
echo "Trying to access sub-folders:"
cd ./tmp1/tmp2/tmp2/tmp2/....

# Notice the infinite symbolic link loop
echo "Notice: It creates an infinite symbolic link loop."

# Remove the ./tmp folder and display symbolic links again
rm -r ./tmp
echo "Listing contents with inode numbers after deleting ./tmp:"
ls -li

# Remove all other symbolic links created
rm tmp1 tmp1/tmp2

# Display the final contents
echo "Final listing after deleting symbolic links:"
ls -li
$SHELL
