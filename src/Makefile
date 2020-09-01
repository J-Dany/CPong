main: main.o
	gcc main.c -o main `pkg-config --cflags --libs gtk+-3.0`

main.o: main.c
	gcc -c main.c