#include "metodos.h"
#include <stdbool.h>

/**
 * Lee la cadena ingresada por el teclado, divide la cadena en una serie de tokens y se guardan en
 * palabras[] y devuelve la cantidad de divisiones obtenidas.
 * @param palabras Arreglo de punteros a los argumentos obtenidos.
 * @param cadena Cadena ingresada por el usuario
 * @return Número de divisiones encontradas en la cadena.
 */
int parsear(char *palabras[], char *cadena) {
    int count = 0;
    char *  delimiters=" \n";
    palabras[0] = strtok(cadena, delimiters);
    for (count = 1; count < 30; count++) {
        palabras[count] = strtok(NULL, delimiters);
        //Encuentro null salgo
        if (palabras[count] == NULL)
            break;
    }
    return count;
}

/**
 * Se comparara la ultima cadena para verificar si se lanzo el comando "&" para background.
 * @param ruta es un array que contiene el path ingresado por el usuario.
 * @return Devuelve un booleano true si debe ejecutarse en background, y false en caso contrario.
 */
bool background(char *ruta[]) {
    int i=0,j;
    bool boolBackground=false;
    char * aux;
    //Recorro porque es un puntero y me desplazo a la ultima posicion
    for (j = 0; j < 30; j++) {
        if (ruta[j] == NULL)
            break;
    }
    aux = ruta[j - 1];

    while(i!=strlen(aux)){// Detecto & parseado con espacio
       // strcpy(aux,ultimaPalabra[i]);
        if (!strcmp( aux, "&")){
           boolBackground=true;
           break; }
            i++;
    }
    return boolBackground;

}


/**
 * Lee la cadena almacenada en arch, busca el ejecutable, y muestra el PATH donde se encuentra
 * @param arch Es el path donde se encuentra el archivo o directorio que se quiere buscar
 * @param paths Arreglo con todos los directorios en los que se quiere buscar
 * @param execPath El path completo al archivo que se encontro. Si no se encuentra el archivo, se pone 'X' en execPath[0]
 */

void searchFile(char *arch, char *paths[], char *invocationPath) {//editar nombress de variables""""""""""""""""""""""!!!!!!!
    char returnPath[50];
    int result;
    char searchDir[50] = "";
    char *archivo;
    strcpy(returnPath, arch);

    if (arch[0] == '/' || (arch[0] == '.' && arch[1] == '.' && arch[2] == '/')) {
        char *dir;
        char *nextDir;
        int pathReady = 0; //Bandera que controla si ya se obtuvo el path completo

        if (arch[0] == '/')
            searchDir[0] = '/';
        dir = strtok(arch, "/");
        nextDir = strtok(NULL, "/");

        if (nextDir != NULL) //Si el comando no es de la forma /archivo
            strcat(searchDir, dir);
        else {
            nextDir = dir; //Si archivo si es NULL, entonces pongo en archivo lo que hay despues de la ultima /
            pathReady = 1;
        }

        while ((nextDir != NULL) && !pathReady) {
            dir = nextDir;
            nextDir = strtok(NULL, "/");
            strcat(searchDir, "/");
            if (nextDir != NULL)
                strcat(searchDir, dir);
        }
        archivo = dir;
    } else if (arch[0] == '.' && arch[1] == '/') { //Es un path relativo, tomando como path el directorio actual.
        getcwd(searchDir, 50);
        strcat(searchDir, "/");
        archivo = strtok(arch, "/");
        archivo = strtok(NULL, "/");
    } else {
        //Tiene que buscar en todos los directorios del path.
        int i;
        char aux[50];
        for (i = 0; i < 20; i++) {
            if (paths[i] == NULL)
                break;
            strcpy(aux, paths[i]);
            strcat(aux, "/");
            strcat(aux, arch);
            result = access(aux, F_OK);
            if (!result) {
                strcpy(invocationPath, aux);
                return;
            }
        }
        invocationPath[0] = 'X';
        return;
    }

    strcat(searchDir, archivo);
    result = access(searchDir, F_OK);
    if (!result)
        strcpy(invocationPath, searchDir);
    else
        invocationPath[0] = 'X';
}

/**
 * Limpia la pantalla produciendo un desplazamiento
 */

void clear(void)
{
    int n;
    for (n = 0; n < 11; n++){
        printf( "\n\n\n\n\n\n\n\n\n\n" );
    }
}

/**
 * La salida estándar para el comando echo se redigire a una dirección ingresada por el usuario.
 * @param Path dirección del archivo donde se envía la salida.
 */
void outPut(char Path[]) {
    int file;
    file = open(Path,  O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR);//abro para escribir, si no existe lo creo y  le doy permisos
    if (file < 0) {
        perror("Cannot open output file\n");
        exit(1);
    }
    close(STDOUT_FILENO);
    if (dup(file) < 0) {//la salida STDOUT ahora apunta al archivo
        perror("Error dup");
        exit(1);
    }
    close(file);
}

/**
 * Testeo si debe redireccionar la entrada o la salida estandar y se guarda la direccion con el nombre del archivo en caso positivo.
 * @param trims Cadena que contiene la linea de comando/s
 * @param nombre Almacena la direccion y el nombre del archivo
 * @return Devuelve 0 si no hay que redireccionar, 1 para redireccionar la entrada, y 2 para redireccionar la salida.
 */
int testRedirect(char *trims[], char nombre[]) {
    int i;
    for (i = 0; i < 20; i++) {
        if (trims[i] == NULL) {
            nombre = NULL;
            return 0;
        } else if (!strcmp(trims[i], "<")) {
            strcpy(nombre, trims[i + 1]);
            trims[i] = NULL;
            trims[i + 1] = NULL;
            return 1;
        } else if (!strcmp(trims[i], ">")) {
            strcpy(nombre, trims[i + 1]);
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
int testPipe(char *trims[], char *argv1[], char *argv2[]) {
    int indexArg;
    int aux;
    int indexArg2;

    for (indexArg = 0; trims[indexArg] != NULL; indexArg++) {
        aux = strcmp("|", trims[indexArg]);
        if (aux == 0)//Si lo encuentro salgo
            break;
        argv1[indexArg] = (char *) malloc(strlen(trims[indexArg] + 1));//asigno memoria y devuelvo un puntero
        strcpy(argv1[indexArg], trims[indexArg]);
    }
    argv1[indexArg] = '\0';

    //Si no encontró "|", aux es distinto de 0 por lo que se devuelve un 0
    if (aux != 0){
        return 0; }
    //Si no es asi incremento para leer el resto de los caracteres que siguen del "|"
    indexArg++;

    for (indexArg2 = 0; trims[indexArg] != NULL; indexArg2++) {

        if (trims[indexArg] == NULL)
            break;
        argv2[indexArg2] = (char *) malloc(strlen(trims[indexArg] + 1));
        strcpy(argv2[indexArg2], trims[indexArg]);
        indexArg++;
    }
    argv2[indexArg2] = '\0';

    return 1;
}