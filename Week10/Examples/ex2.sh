#determine properties of ex1
stat -c "Inode: %i, Blocks: %b, Block Size: %B, Total Size: %s bytes, Permissions: %a" ex1

#copy ex1 to ex2 
cp ex1 ex2

#check the number of links and inode
stat -c "Links: %h, Inode: %i" ex1
stat -c "Links: %h, Inode: %i" ex2
#output will show that inodes are different 
#it means ex2 is seperate copy (not a hard link) of ex1


#identify the files who have
#3 links in /etc
stat -c " %h - %n" /etc/* | grep -3

#it means that 3 hard-links are pointing to
#the same inode for the files listed in the /etc
#directory.These files share the same underlying data on disk, and modifications to one hard link affect the others since they point to the same file.
