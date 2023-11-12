#!/bin/bash

# Step 1: Delete ./tmp folder if it exists and create a symbolic link tmp1 for the folder ./tmp
rm -rf ./tmp
ln -s ./tmp tmp1

# Step 2: Run ls -li
ls -li

# Step 3: Create the folder ./tmp
mkdir ./tmp

# Step 4: Run ls -li
ls -li

# Step 5: Create a file ex1.txt using gen.sh and add it to ./tmp
bash gen.sh 1 ./tmp/ex1.txt

# Step 6: Check the folder ./tmp1
ls -li ./tmp1

# Step 7: Create another symbolic link tmp1/tmp2 to the folder ./tmp (symbolic link to itself)
ln -s ../tmp tmp1/tmp2

# Step 8: Create another file ex1.txt using the same generator gen.sh and add it to the folder ./tmp1/tmp2
bash gen.sh 1 ./tmp1/tmp2/ex1.txt

# Step 9: Check the content of the sub-folders
ls -R ./tmp1

# Step 10: Delete the folder ./tmp and check the symbolic links again
rm -rf ./tmp
ls -li ./tmp1

# Step 11: Delete all other symbolic links you created
rm tmp1 tmp1/tmp2
