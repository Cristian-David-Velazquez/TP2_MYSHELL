#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "metodos.h"
#include "tests.h"
#include "redirect.h"

#define BUFFER 256
#define SIZE 30
#define SIZE2 20

/* Declaración de funciones */
int getPath (char *paths[]);
void HandlerSingal (int32_t signum);
void pipeline (char *comando1[], char *comando2[], char *paths[]);
/* Variables globales */
int cantPalabras;

int main (int argc, char *argv[])
{
  int numComando;
  char comandos[BUFFER];
  char auxComandos[BUFFER];
  char hostname[SIZE], usuario[SIZE];
  char invocationPath[BUFFER];  //Path del proceso que se va a ejecutar.
  char *paths[SIZE2];
  char *trims[SIZE2];
  char *argv1[SIZE2], *argv2[SIZE2];
  int pipe, esPipe, pidHijo, flagRedirect;
  int batchfile = 0;
  int sigAction = 0;
  int comandoInterno;//Cambio solo si se encuentra un comando interno
  pid_t pid;
  bool boolbackground = false;
  int flagHijo;//si existe el hijo se pone en 1, variable global para todos
  struct sigaction mysignal;
  mysignal.sa_handler = HandlerSingal;//Le paso mi handler a la estructura de sigaction
  mysignal.sa_flags = 0;// Lo hace re-ejecutable

  sigAction = sigaction (SIGINT, &mysignal, NULL);//Para capturar y manejar señales
  if (sigAction == -1)
    {
      perror ("Error en sigaction");
      exit (EXIT_FAILURE);
    }
  //Obtengo usuario y nombre de la maquina
  gethostname (hostname, SIZE);

  if (getenv ("USER") != NULL)
    {
      strcpy (usuario, getenv ("USER"));
    }

  //Busco todos los paths que están en la variable de entorno PATH
  getPath (paths);

  if (argc == 2)
    {
      if (!strcmp (argv[1], "batchfile"))
        {
          batchfile = 1;
          printf ("Invocacion de batchfile\n");
        }
      else
        {
          printf ("ERROR al intentar leer el batchfile\n");
          exit (EXIT_FAILURE);
        }
    }

  while (1)
    {
      /*Limpio banderas*/
      pipe = 0;
      esPipe = 0;
      flagHijo = 0;
      pid = 1;
      comandoInterno = 0;
      flagRedirect = 0;

      printf ("%s@%s:%s$ ", usuario, hostname, getcwd (NULL, 4096)); // getcwd copia el path actual
      if (batchfile != 1)
        {
          fgets (comandos, BUFFER, stdin);
        }
      else
        {
          strcpy (comandos, "./batch.sh");
          batchfile = 0;
        }
      memset (auxComandos, '\0', BUFFER);

      eliminarTab (comandos, auxComandos);

      cantPalabras = parsear (trims, auxComandos);

      if (cantPalabras == 0 || !strcmp (comandos, "\n"))
        {
          printf (" ");
          continue;
        }
      char fileName[50];
      pipe = testPipe (trims, argv1, argv2);
      boolbackground = background (trims);
      flagRedirect = testRedirect (trims, fileName);

      //Verifico si el proceso se tiene que ejecutar en Background.
      if (boolbackground == true)
        {
          boolbackground = false;
          trims[cantPalabras - 1] = NULL;//elimino el & para evitar problemas al ejecutar
          trims[cantPalabras] = NULL;
          flagHijo = 1;
          pid = fork ();
          if (pid < 0)
            {
              perror ("fork");
              exit (1);
            }
          if (pid == 0)
            {
              pidHijo = getpid ();
              printf ("PROCESO child %d\n", pidHijo);
            }
        }
      if (flagHijo == 0 || pid == 0)
        {
          if (flagRedirect == 0)
            {//Si no hay un redireccion
              numComando = buscoComando (trims);
              switch (numComando)
                {
                  case 1:
                    funcionCD (trims, cantPalabras);
                    comandoInterno = 1;
                    break;
                  case 2:
                    clear ();
                    comandoInterno = 1;
                    break;
                  case 3:
                    comandoInterno=1;
                    funcionEcho (pid,trims,cantPalabras);
                    break;
                  case 4:
                    funcionSalir ();
                    comandoInterno = 1;
                    return 0;
                  default:
                    comandoInterno = 0;
                }
            }
          searchFile (trims[0], paths, invocationPath);
          if (invocationPath[0] == 'X' && comandoInterno == 0)
            {
              printf ("Comando invalido\n");
            }
          if (invocationPath[0] != 'X' && comandoInterno == 0)
            {//Si no se llamo a ningun comando interno entro
              if (fork () == 0)
                {
                  if (pid < 0)
                    {
                      perror ("Creando el hijo");
                      exit (1);
                    }
                  if (flagRedirect == 2)
                    {
                      outPut (fileName);
                    }
                  else if (flagRedirect == 1)
                    {
                      freopen (fileName, "r", stdin);
                    }
                  else if (pipe == 1)
                    {
                      pipeline (argv1, argv2, paths);
                      esPipe = 1;
                    }
                  if (esPipe != 1)
                    {
                      execv (invocationPath, trims);
                      perror (invocationPath);
                      exit (1);
                    }
                }
            }

          if (pid == 0)
            {
              wait (NULL);//el hijo pid espera que termine su hijo
              pidHijo = getpid ();
              printf ("termine %d\n", pidHijo);
              exit (0);
            }
        }
      wait (NULL);//el padre espera que el hijo pid termine
    }
}

/**
 * Encuentra los Paths de la variable de entorno PATH, y guarda uno por cada elemento del arreglo paths.
 * @param paths Arreglo de punteros a caracter donde se almacena cada path.
 * @return Cantidad de dirreciones encontradas.
 */
int getPath (char *paths[])
{//Usado para invocar un programa
  int counter;
  char *variablePAH = getenv ("PATH");

  paths[0] = strtok (variablePAH, ":");
  for (counter = 1; counter < SIZE2; counter++)
    {
      paths[counter] = strtok (NULL, ":");
      if (paths[counter] == NULL)
        break;
    }

  strtok (NULL, ":");
  return counter + 1;
}

/**
 * Se uitliza el pipe para la comunuciacion de los procesos al ejecutar los comandos
 * @param comando1 Argumentos del primer comando.
 * @param comando2 Argumentos del segundo comando.
 * @param paths Es la direccion donde voy a ejecutar los comandos
 */
void pipeline (char *comando1[], char *comando2[], char *paths[])
{
  char invocationPath[SIZE];
  int fd[2];
  pipe (fd);
  if (fork () == 0)
    { //Hijo
      close (fd[0]);
      if (dup2 (fd[1], 1) < 0)
        {// Se hace una redireccion de la salida al pipe.
          printf ("No se puede duplicar el descriptor de archivo.");
          exit (EXIT_FAILURE);
        }
      close (fd[1]);
      searchFile (comando1[0], paths, invocationPath);
      execv (invocationPath, comando1);
      perror (invocationPath);
      exit (1);
    }
  else
    {
      close (fd[1]); //Padre
      if (dup2 (fd[0], 0))
        {//redirrecion de la entrada al pipe
          printf ("No se puede duplicar el descriptor de archivo.");
          exit (EXIT_FAILURE);
        }
      close (fd[0]);
      searchFile (comando2[0], paths, invocationPath);
      execv (invocationPath, comando2);
      perror (invocationPath);
      exit (1);
    }
}
void HandlerSingal (int32_t signum)
{
  fprintf (stderr, "\nSe cierra myshell, Recibi Ctrl-C (%d)\n", signum);
  exit (1);
}
// echo hola amigo > /home/cristian/Documentos/hola.txt
//./prueba < /home/cristian/Documentos/hola.txt //uso de entrada al txt y prueba lo imprime, prueba esta en tp2
//./prueba > /home/cristian/Documentos/hola.txt --escribo en el archivo lo que sale de prueba
// cat < hola.txt --muestra lo que tiene hola.txt, se lo paso, si fuese alreves escrito en el txt
//ls | more
//ps -a | sort