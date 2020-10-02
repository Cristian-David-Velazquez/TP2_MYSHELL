#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>

int buscoComando(char *trims[]);

bool background (char *ruta[]);

int testRedirect (char *argv[], char nombre[]);

int testPipe (char *trims[], char *argv1[], char *argv2[]);