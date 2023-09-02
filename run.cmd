gcc -c -o fifo.o fifo.c -I"./Dependencies/freeglut/include"
gcc -o fifo.exe fifo.o -L"./Dependencies/freeglut/lib/x64" -lfreeglut -lopengl32 -lglu32
fifo.exe