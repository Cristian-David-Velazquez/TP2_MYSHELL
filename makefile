CFLAGS = -c -Wall -pedantic

myshell: main.o metodos.o tests.o redirect.o
	gcc -o myshell main.o metodos.o tests.o redirect.o

main.o: main.c metodos.h
	#cppcheck main.c
	gcc $(CFLAGS) main.c

metodos.o: metodos.c metodos.h
	#cppcheck metodos.c
	gcc $(CFLAGS) metodos.c

tests.o: tests.c tests.h
    #cppcheck tests.c
	gcc $(CFLAGS) tests.c

redirect.o: redirect.c redirect.h
    #cppcheck redirect.c
	gcc $(CFLAGS) redirect.c

clean:
	rm -f myshell *.o
