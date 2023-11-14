#!/bin/bash

# Display the inode of the program ex1
inode_ex1=$(ls -i ex1 | awk '{print $1}')
echo "Inode of ex1: $inode_ex1"

# Gather information about ex1: number of blocks, size of each block, total size, and permissions
blocks_ex1=$(du ex1 | awk '{print $1}')
block_size_ex1=$(stat -c "%o" ex1)
total_size_ex1=$(stat -c "%s" ex1)
permissions_ex1=$(stat -c "%A" ex1)

echo "Number of blocks in ex1: $blocks_ex1"
echo "Size of each block in ex1: $block_size_ex1"
echo "Total size of ex1 in bytes: $total_size_ex1"
echo "Permissions of ex1: $permissions_ex1"

# Duplicate the program ex1 to ex2
cp ex1 ex2

# Investigate the number of links for ex2 and compare i-node numbers
links_ex2=$(stat -c "%h" ex2)
inode_ex2=$(ls -i ex2 | awk '{print $1}')

echo "Number of links for ex2: $links_ex2"
echo "Inode of ex2: $inode_ex2"

# Check if ex1 and ex2 share the same i-node numbers
if [ "$inode_ex1" -eq "$inode_ex2" ]; then
    echo "ex1 and ex2 share the same i-node numbers."
else
    echo "ex1 and ex2 do not share the same i-node numbers."
fi

# Identify files with 3 links in /etc
echo "Files with 3 links in /etc:"
stat -c "%h - %n" /etc/* | grep ^3

# Explain the significance of the number of links (3 links)
echo "The count of links (3 links) denotes the number of hard links associated with each file."
$SHELL

#it means that 3 hard-links are pointing to
#the same inode for the files listed in the /etc
#directory.These files share the same underlying data on disk, and modifications to one hard link affect the others since they point to the same file.
