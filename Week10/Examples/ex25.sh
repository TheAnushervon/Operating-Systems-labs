#!/bin/bash

# Create an empty file
touch ex5.txt

# Display initial permissions
echo "Initial permissions:"
ls -l ex5.txt
echo ""

# Remove write permission for everybody
chmod a-w ex5.txt
echo "After removing write permission for everybody:"
ls -l ex5.txt
echo ""

# Grant all permissions to owner and others
chmod uo+rwx ex5.txt
echo "After granting all permissions to owner and others:"
ls -l ex5.txt
echo ""

# Make group permissions equal to user permissions
chmod g=u ex5.txt
echo "After making group permissions equal to user permissions:"
ls -l ex5.txt
echo ""

# Display the meanings of permission values
echo "Meanings of permission values:"
echo "660 means: Read and write permission for the owner and group, no permission for others."
echo "775 means: Read, write, and execute permission for the owner and group, read and execute permission for others."
echo "777 means: Read, write, and execute permission for everyone (owner, group, and others)."
