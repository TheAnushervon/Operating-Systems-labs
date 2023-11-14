#!/bin/bash

# Generate ex1.txt with 10 random numbers using gen.sh
./gen.sh 10 ex1.txt

# Create hard links ex11.txt and ex12.txt for ex1.txt
ln ex1.txt ex11.txt
ln ex1.txt ex12.txt

# Compare file content using diff
echo "Comparing file content using diff:"
diff ex1.txt ex11.txt
diff ex1.txt ex12.txt

# Check and save i-node numbers to output.txt
echo "Checking and saving i-node numbers:"
ls -i ex1.txt ex11.txt ex12.txt > output.txt

# Confirm if i-node numbers are different
echo "Do i-node numbers differ? Yes, they are different."

# Display disk usage of ex1.txt
echo "Disk usage of ex1.txt:"
du ex1.txt

# Create a hard link ex13.txt and move it to /tmp
ln ex1.txt ex13.txt
mv ex13.txt /tmp/

# Trace links to ex1.txt in the current path
echo "Tracing links to ex1.txt in the current path:"
find . -inum $(ls -i ex1.txt | awk '{print $1}')

# Trace links to ex1.txt in the root path (/)
echo "Tracing links to ex1.txt in the root path (/):"
find / -samefile $(realpath ex1.txt)

# Explain the difference between current and root path searches
echo "Searching in the current path explores links in the current directory and its subdirectories. Searching in the root path (/) examines links across the entire file system."

# Display the number of hard links for ex1.txt
echo "Number of hard links for ex1.txt:"
stat -c "%h" ex1.txt

# Remove all links from ex1.txt
echo "Removing all links from ex1.txt:"
find . -inum $(ls -i ex1.txt | awk '{print $1}') -exec rm {} \;