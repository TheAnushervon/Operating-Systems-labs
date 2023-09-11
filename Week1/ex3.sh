clear
mkdir ex3root
cd ex3root
date && sleep 3
touch root.txt
cd ..
date && sleep 3
mkdir ex3home
cd ex3home
date && sleep 3
touch home.txt
ls -at ~ > home.txt
cd .. 
cd ex3root
ls -at / > root.txt
cat root.txt
cd .. 
cd ex3home
cat home.txt
ls -at ex3home
cd .. 
ls -at ex3home
ls -at ex3root
history