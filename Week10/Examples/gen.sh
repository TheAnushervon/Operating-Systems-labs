#!/bin/bash
for ((i = 0; i < $1; i++)); do
  echo $RANDOM >> $2
done

#linking process
ln -f ex1.txt ex11.txt
ln -f ex1.txt ex12.txt

#differences 
diff ex1.txt ex11.txt
diff ex1.txt ex12.txt

#Check i-node numbers of all files and save the output to the file output.txt:
stat -c "File: %n, Inode: %i" ex1.txt ex11.txt ex12.txt > output.txt

#check disk usage of ex1.txt
du ex1.txt

# Create a hard link ex13.txt for ex1.txt and move it to the folder /tmp
ln ex1.txt ex13.txt
mv ex13.txt /tmp

# Trace all links to ex1.txt in the current path
find . -inum $(stat -c "%i" ex1.txt)

# Trace all links to ex1.txt in the root path (/)
sudo find / -inum $(stat -c "%i" ex1.txt)

# Check the number of hard links of ex1.txt
stat -c "%h" ex1.txt

# Remove all links from ex1.txt
find . -inum $(stat -c "%i" ex1.txt) -exec rm {} \;
