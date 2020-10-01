CFLAGS = -c -Wall -pedantic

myshell: main.o metodos.o tests.o
	gcc -o myshell main.o metodos.o tests.o

main.o: main.c metodos.h
	#cppcheck main.c
	gcc $(CFLAGS) main.c

metodos.o: metodos.c metodos.h
	#cppcheck metodos.c
	gcc $(CFLAGS) metodos.c

tests.o: tests.c tests.h
    #cppcheck metodos.c
	gcc $(CFLAGS) tests.c

clean:
	rm -f myshell *.o
