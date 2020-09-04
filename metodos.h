#ifndef TP2_METODOS_H
#define TP2_METODOS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <fcntl.h>

int parsear(char *palabras[], char *cadena);

bool background(char *ruta[]);

void searchFile(char *arch, char *paths[], char *invocationPath);

void clear(void);

void outPut(char Path[]);

int testRedirect(char *argv[], char nombre[]);

int testPipe(char *trims[], char *argv1[], char *argv2[]);

#endif //TP2_METODOS_H