#include "metodos.h"
#include <stdbool.h>

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
  for (counter = 1; counter < 20; counter++)
    {
      paths[counter] = strtok (NULL, ":");
      if (paths[counter] == NULL)
        break;
    }

  strtok (NULL, ":");
  return counter + 1;
}

/**
 * Lee la cadena ingresada por el usuario y se eliminan los espacios generados por tab
 * @param comandos es la cadena ingresada por el usuario
 * @param auxComandos es la cadena donde se guardara los cambios
 */
void eliminarTab (char comandos[], char auxComandos[])
{
  for (int a = 0; a < (int) strlen (comandos); a++)
    {//elimino tab
      if (comandos[a] != '\t')
        {
          auxComandos[a] = comandos[a];
        }
      if (comandos[a] == '\t')
        { //reemplazo los tab por espacios para que no queden juntas las palabras
          auxComandos[a] = '\n';
        }
    }
}

/**
 * Lee la cadena ingresada por el teclado, divide la cadena en una serie de tokens y se guardan en
 * palabras[] y devuelve la cantidad de divisiones obtenidas.
 * @param palabras Arreglo de punteros a los argumentos obtenidos.
 * @param cadena Cadena ingresada por el usuario
 * @return Número de divisiones encontradas en la cadena.
 */
int parsear (char *palabras[], char *cadena)
{
  int count = 0;
  char *delimiters = " \n";
  palabras[0] = strtok (cadena, delimiters);
  for (count = 1; count < 30; count++)
    {
      palabras[count] = strtok (NULL, delimiters);
      //Encuentro null salgo
      if (palabras[count] == NULL)
        break;
    }
  return count;
}

/**
 * Esta funcion es para desplazarnos entre directorios
 * En el caso que no se indique la direccion, se queda en el mismoo path
 * @param trims Arreglo de punteros a los argumentos obtenidos.
 * @param cantPalabras es el numero de trims obtenidos
 */
void funcionCD (char *trims[], int cantPalabras)
{
  if (cantPalabras > 1)
    {
      if ((chdir (trims[1])) != 0)
        {
          printf ("\nNo existe la ruta ingresada, NO SUCH FILE OR DIRECTORY \n");
        }
    }
  else printf (" ");
}
/**
 * Proceso para imprmir en pantalla el texto que viene luego del comando echo
 * @param pid id del proceso padre o hijo
 * @param trims arreglo de punteros a los argumentos obtenidos.
 * @param cantPalabras numero de trims
 */

void funcionEcho (pid_t pid, char *trims[], int cantPalabras)
{
  if (pid == 0)
    {//Solo entra el hijo
      char *argVhijo[60];
      argVhijo[0] = "/bin/echo";
      printf ("%s", argVhijo[0]);
      for (int i = 1; i < cantPalabras; i++)
        {
          argVhijo[i] = trims[i];
          printf ("%s", argVhijo[i]);
        }
      argVhijo[cantPalabras - 1] = NULL;

      if (execv (argVhijo[0], argVhijo) < 0)
        {
          perror ("execv error");
        }
      exit (1);
    }
  if (cantPalabras > 1)
    {//si echo esta vacio no entra
      if (pid != 0)
        {// Solo si es el padre
          imprimirTexto (cantPalabras, trims);
        }
    }
  else printf ("\n");
}

/**
 * Cuando se recibe el comando quit, se llama a esta funcion
 * para terminar imprimir un mensaje
 */
void funcionSalir ()
{
  printf ("---------------------------------------------------------------------\n");
  printf ("QUIT MYSHELL\n");
}

/**
 * Lee la cadena almacenada en arch, busca el ejecutable, y muestra el PATH donde se encuentra
 * @param arch Es el path donde se encuentra el archivo o directorio que se quiere buscar
 * @param paths Arreglo con todos los directorios en los que se quiere buscar
 * @param execPath El path completo al archivo que se encontro. Si no se encuentra el archivo, se pone 'X' en execPath[0]
 */

void searchFile (char *arch, char *paths[], char *invocationPath)
{//editar nombress de variables""""""""""""""""""""""!!!!!!!
  char returnPath[50];
  int result;
  char searchDir[50] = "";
  char *archivo;
  strcpy (returnPath, arch);

  if (arch[0] == '/' || (arch[0] == '.' && arch[1] == '.' && arch[2] == '/'))
    {
      char *dir;
      char *nextDir;
      int pathReady = 0; //Bandera que controla si ya se obtuvo el path completo

      if (arch[0] == '/')
        searchDir[0] = '/';
      dir = strtok (arch, "/");
      nextDir = strtok (NULL, "/");

      if (nextDir != NULL) //Si el comando no es de la forma /archivo
        strcat (searchDir, dir);
      else
        {
          nextDir = dir; //Si archivo si es NULL, entonces pongo en archivo lo que hay despues de la ultima /
          pathReady = 1;
        }

      while ((nextDir != NULL) && !pathReady)
        {
          dir = nextDir;
          nextDir = strtok (NULL, "/");
          strcat (searchDir, "/");
          if (nextDir != NULL)
            strcat (searchDir, dir);
        }
      archivo = dir;
    }
  else if (arch[0] == '.' && arch[1] == '/')
    { //Es un path relativo, tomando como path el directorio actual.
      getcwd (searchDir, 50);
      strcat (searchDir, "/");
      archivo = strtok (arch, "/");
      archivo = strtok (NULL, "/");
    }
  else
    {
      //Tiene que buscar en todos los directorios del path.
      int i;
      char aux[50];
      for (i = 0; i < 20; i++)
        {
          if (paths[i] == NULL)
            break;
          strcpy (aux, paths[i]);
          strcat (aux, "/");
          strcat (aux, arch);
          result = access (aux, F_OK);
          if (!result)
            {
              strcpy (invocationPath, aux);
              return;
            }
        }
      invocationPath[0] = 'X';
      return;
    }

  strcat (searchDir, archivo);
  result = access (searchDir, F_OK);
  if (!result)
    strcpy (invocationPath, searchDir);
  else
    invocationPath[0] = 'X';
}

/**
 * Limpia la pantalla produciendo un desplazamiento
 */

void clear (void)
{
  int n;
  for (n = 0; n < 11; n++)
    {
      printf ("\n\n\n\n\n\n\n\n\n\n");
    }
}

/**
 * Se imprime en pantalla lo que ingreso el usuario al escribir el comando echo, solo es utilizado por el proceso padre
 * @param cantPalabras Contiene el numero de palabras obtenidas en el paseado por espacios.
 * @param trims Puntero de cada palabra ingresada por el usuario.
 */
void imprimirTexto (int cantPalabras, char *trims[])
{
  char *tab = " \t";
  for (int i = 1; i < cantPalabras; i++)
    {
      printf ("%s ", strtok (trims[i], tab));
    }
  printf ("\n");
}

/**
 * Se uitliza el pipe para la comunuciacion de los procesos al ejecutar los comandos
 * @param comando1 Argumentos del primer comando.
 * @param comando2 Argumentos del segundo comando.
 * @param paths Es la direccion donde voy a ejecutar los comandos
 */
void pipeline (char *comando1[], char *comando2[], char *paths[])
{
  char invocationPath[30];
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