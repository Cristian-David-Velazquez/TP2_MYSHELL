#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int testRedirect(char *argv[], char nombre[]);

int testPipe(char *trims[], char *argv1[], char *argv2[]);