#ifndef TP2_METODOS_H
#define TP2_METODOS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>

int getPath (char *paths[]);

void eliminarTab (char comandos[], char auxComandos[]);

int parsear (char *palabras[], char *cadena);

void funcionCD (char *trims[], int cantPalabras);

void funcionEcho (pid_t pid, char *trims[], int cantPalabras);

void funcionSalir ();

void searchFile (char *arch, char *paths[], char *invocationPath);

void clear (void);

void imprimirTexto (int cantPalabras, char *trims[]);

void pipeline (char *comando1[], char *comando2[], char *paths[]);

#endif //TP2_METODOS_H