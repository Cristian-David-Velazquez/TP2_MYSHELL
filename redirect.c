#include "redirect.h"

/**
 * La salida estándar para el comando echo se redigire a una dirección ingresada por el usuario.
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
      exit (1);
    }
  close (STDOUT_FILENO);
  if (dup (file) < 0)
    {//la salida STDOUT ahora apunta al archivo
      perror ("Error dup");
      exit (1);
    }
  close (file);
}