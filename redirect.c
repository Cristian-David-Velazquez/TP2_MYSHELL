#include "redirect.h"

/**
 * Cambia la dirección de entrada de la consola a la direccion del archivo
 * @param Path direccion donde se leera los posibles comandos
 */
void input (char Path[])
{
  int file, flags, permisos;

  flags = O_RDONLY;
  permisos = S_IWUSR|S_IRUSR;

  file = open(Path,flags,permisos);

  if (file < 0)
    {
      perror("Cannot open output file\n");
      exit(EXIT_FAILURE);
    }
  close(STDIN_FILENO);
  if (dup (file) < 0) //cambie la entrada
    {
      perror ("Error dup");
      exit (EXIT_FAILURE);
    }
  close(file);
}

/**
 * La salida estándar se redigire a una dirección ingresada por el usuario.
 * @param Path dirección del archivo donde se envía la salida.
 */
void outPut (char Path[])
{
  int file;
  file = open (Path,
               O_WRONLY | O_CREAT | O_TRUNC,
               S_IWUSR | S_IRUSR);//abro para escribir, si no existe lo creo y  le doy permisos
  if (file < 0)
    {
      perror ("Cannot open output file\n");
      exit (EXIT_FAILURE);
    }
  close (STDOUT_FILENO);
  if (dup (file) < 0)
    {//la salida STDOUT ahora apunta al archivo
      perror ("Error dup");
      exit (EXIT_FAILURE);
    }
  close (file);
}