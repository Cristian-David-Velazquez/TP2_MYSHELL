CFLAGS = -Wall -pedantic -Werror -Wextra -Wconversion

myshell: main.o metodos.o tests.o redirect.o
	gcc $(CFLAGS) -o myshell main.o metodos.o tests.o redirect.o

main.o: main.c metodos.h
	gcc $(CFLAGS) -c main.c

metodos.o: metodos.c metodos.h
	gcc $(CFLAGS) -c metodos.c

tests.o: tests.c tests.h
	gcc $(CFLAGS) -c tests.c

redirect.o: redirect.c redirect.h
	gcc $(CFLAGS) -c redirect.c

clean:
	rm -f myshell *.o
