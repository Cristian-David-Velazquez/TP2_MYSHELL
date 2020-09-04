CFLAGS = -c -Wall -pedantic

makefile: main.o metodos.o
	gcc -o myshell main.o metodos.o

main.o: main.c metodos.h
	#cppcheck main.c
	gcc $(CFLAGS) main.c

metodos.o: metodos.c metodos.h
	#cppcheck metodos.c
	gcc $(CFLAGS) metodos.c

clean:
	rm -f myshell *.o
