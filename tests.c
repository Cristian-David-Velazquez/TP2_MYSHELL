#include "tests.h"

/**
 *Verifico que comando es y retorno un numero para identificarlo
 * @param trims son las palabras que ingreso el usuario
 * @return 0 para el caso que no coincida con ningun comando y de 1 a 4 cuando conincide
 */
int buscoComando (char *trims[])
{
  int num = 0;
  if (!strcmp (trims[0], "cd"))
    {
      num = 1;
    }
  if (!strcmp (trims[0], "clr"))
    {
      num = 2;
    }
  if (!strcmp (trims[0], "echo"))
    {
      num = 3;
    }
  if (!strcmp (trims[0], "quit"))
    {
      num = 4;
    }
  return num;
}

/**
 * Se comparara la ultima cadena para verificar si se lanzo el comando "&" para background.
 * @param ruta es un array que contiene el path ingresado por el usuario.
 * @return Devuelve un booleano true si debe ejecutarse en background, y false en caso contrario.
 */
bool background (char *ruta[])
{
  int i = 0, j;
  bool boolBackground = false;
  char *aux;
  //Recorro porque es un puntero y me desplazo a la ultima posicion
  for (j = 0; j < 30; j++)
    {
      if (ruta[j] == NULL)
        break;
    }
  aux = ruta[j - 1];

  while (i != (int) strlen (aux))
    {// Detecto & parseado con espacio
      // strcpy(aux,ultimaPalabra[i]);
      if (!strcmp (aux, "&"))
        {
          boolBackground = true;
          break;
        }
      i++;
    }
  return boolBackground;

}

/**
 * Testeo si debe redireccionar la entrada o la salida estandar y se guarda la direccion con el nombre del archivo en caso positivo.
 * @param trims Cadena que contiene la linea de comando/s
 * @param nombre Almacena la direccion y el nombre del archivo
 * @return Devuelve 0 si no hay que redireccionar, 1 para redireccionar la entrada, y 2 para redireccionar la salida.
 */
int testRedirect (char *trims[], char nombre[])
{
  int i;
  for (i = 0; i < 20; i++)
    {
      if (trims[i] == NULL)
        {
          nombre = NULL;
          return 0;
        }
      else if (!strcmp (trims[i], "<"))
        {
          strcpy (nombre, trims[i + 1]);
          trims[i] = NULL;
          trims[i + 1] = NULL;
          return 1;
        }
      else if (!strcmp (trims[i], ">"))
        {
          strcpy (nombre, trims[i + 1]);
          trims[i] = NULL;
          trims[i + 1] = NULL;
          return 2;
        }
    }
  return 0;
}

/**
 * Verifica si debe realizarse una comunicacion mediante pipeline.
 * @param trims Argumentos de lo/s comando/s ingresados por el usuario.
 * @param argv1 Array donde se almacenan los parametros del comando 1.
 * @param argv2 Array donde se almacenan los parametros del comando 2.
 * @return Devuelve 1 si se debe crear el pipeline. 0 en caso contrario.
 */
int testPipe (char *trims[], char *argv1[], char *argv2[])
{
  int indexArg;
  int aux;
  int indexArg2;

  for (indexArg = 0; trims[indexArg] != NULL; indexArg++)
    {
      aux = strcmp ("|", trims[indexArg]);
      if (aux == 0)//Si lo encuentro salgo
        break;
      argv1[indexArg] = (char *) malloc (strlen (trims[indexArg] + 1));//asigno memoria y devuelvo un puntero
      strcpy (argv1[indexArg], trims[indexArg]);
    }
  argv1[indexArg] = '\0';

  //Si no encontró "|", aux es distinto de 0 por lo que se devuelve un 0
  if (aux != 0)
    {
      return 0;
    }
  //Si no es asi incremento para leer el resto de los caracteres que siguen del "|"
  indexArg++;

  for (indexArg2 = 0; trims[indexArg] != NULL; indexArg2++)
    {

      if (trims[indexArg] == NULL)
        break;
      argv2[indexArg2] = (char *) malloc (strlen (trims[indexArg] + 1));
      strcpy (argv2[indexArg2], trims[indexArg]);
      indexArg++;
    }
  argv2[indexArg2] = '\0';

  return 1;
}