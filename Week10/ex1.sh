gcc monitor.c -o monitor
gcc ex1.c -o ex1

gnome-terminal -- ./monitor .
./ex1 .

rm monitor
rm ex1
