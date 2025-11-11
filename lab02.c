/********************************************************************************************
 * Pontificia Universidad Javeriana
 * Alejandro Beltrán Huertas
 * 11/11/2025
 * 
 * Descripción: Programa en C que lee el nombre de un directorio y muestra:
 *  - Sus ficheros y permisos de lectura para el propietario.
 *  - Si cada entrada es un archivo o un directorio.
 *  - Ficheros modificados en los últimos 10 días.
 ********************************************************************************************/


#include <stdio.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main() {
    int er;
    char nomdir[100], nomfich[100], resp[30];
    struct stat atr;
    DIR *d;
    struct dirent *rd1;
    time_t fecha;

    // Solicitar el nombre del directorio
    printf("Nombre del directorio:\n");
    fgets(nomdir, sizeof(nomdir), stdin);

    // Eliminar salto de línea final si el usuario lo puso
    nomdir[strlen(nomdir) - 1] = '\0';

    // Obtener fecha actual
    fecha = time(&fecha);

    // se abre el directorio
    if ((d = opendir(nomdir)) == NULL) {
        printf("No existe ese directorio.\n");
        return -1;
    } else {
        // Recorrer los elementos del directorio
        while ((rd1 = readdir(d)) != NULL) {

            // Ignorar "." y ".."
            if ((strcmp(rd1->d_name, ".") != 0) && (strcmp(rd1->d_name, "..") != 0)) {

                // Construir la ruta completa del fichero
                strcpy(nomfich, nomdir);
                strcat(nomfich, "/");
                strcat(nomfich, rd1->d_name);

                printf("Fichero: %s\n", nomfich);

                // Obtener atributos del archivo
                er = stat(nomfich, &atr);
                if (er != 0) {
                    perror("Error al obtener atributos");
                    continue;
                }

                // Mostrar modo (permisos en octal)
                printf("Modo: %o\n", atr.st_mode);

                // Verificar permiso de lectura del usuario
                if ((atr.st_mode & 0400) != 0)
                    printf("→ Permiso R para propietario.\n");
                else
                    printf("→ No permiso R para propietario.\n");

                // Determinar tipo de archivo
                if (S_ISDIR(atr.st_mode))
                    printf("→ Es un directorio.\n");
                if (S_ISREG(atr.st_mode))
                    printf("→ Es un archivo regular.\n");

                // Mostrar archivos modificados en los últimos 10 días
                if ((fecha - 10 * 24 * 60 * 60) < atr.st_mtime) {
                    printf("→ Fichero modificado recientemente:\n");
                    printf("   Nombre: %s\n", rd1->d_name);
                    printf("   Última modificación: %s", ctime(&atr.st_mtime));
                }

                printf("--------------------------------------------------\n");
            }
        }

        closedir(d);
    }

    return 0;
}
