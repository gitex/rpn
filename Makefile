main: main.c
	gcc main.c -o main

debug: main.c
	gcc -g -O0 main.c -o main

run:
	./main

gdb: debug
	gdb --ex "tty /dev/null" ./main

