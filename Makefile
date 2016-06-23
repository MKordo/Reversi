reversi.exe : main.o input.o minmax.o
	gcc main.o input.o minmax.o -o reversi.exe

main.o : main.c
	gcc -c main.c

input.o : input.c
	gcc -c input.c

minmax.o : minmax.c
	gcc -c minmax.c
