#check file types 
file /dev/*
file /etc/*
#counting number of directories in /etc

ls -l /etc | grep ^d | wc -l 

#check file type before compil

file ex1.c
gcc -o ex1 ex1.c 
file ex1

sleep 5

#recompile modified ex1.c
gcc -o ex1 ex1.c 
file ex1 
