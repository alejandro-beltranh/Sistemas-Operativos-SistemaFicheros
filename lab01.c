
/********************************************************************************************
 * Pontificia Universidad Javeriana
 * Alejandro Beltrán Huertas
 * 11/11/2025
 * Descripción: Programa en C que lee de teclado el nombre de un directorio y muestra
 *              en pantalla el nombre y tamaño de los ficheros que contiene.
 ********************************************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
int main() {
    DIR *d;      // se declara un puntero al directorio
    char nomdir[90], nomfich[90]; // se declaran buffers para el nombre del directorio y fichero
    struct stat datos;          // se declara estructura para guardar los datos del fichero
    struct dirent *direc;       // se declara estructura para leer los nombres dentro del directorio

    // se solicita el nombre del directorio
    printf("Introduzca el nombre de un directorio: ");
    fgets(nomdir, sizeof(nomdir), stdin);

    // Eliminar el salto de línea al final del nombre por si el usuario lo puso
    nomdir[strlen(nomdir) - 1] = '\0';

    // Intentar abrir el directorio si hay error imporime mensaje
    if ((d = opendir(nomdir)) == NULL) {
        printf("El directorio no existe o no se puede abrir.\n");
        return -1;
    }

    // Leer cada entrada del directorio 
    while ((direc = readdir(d)) != NULL) {
        // Ignorar las entradas "." y ".."
        if (strcmp(direc->d_name, ".") == 0 || strcmp(direc->d_name, "..") == 0)
            continue;

        // Crear la ruta completa al archivo
        strcpy(nomfich, nomdir);
        strcat(nomfich, "/");
        strcat(nomfich, direc->d_name);

        // extrae  la información del archivo
        if (stat(nomfich, &datos) == 0) {
            // Verificar que sea un archivo regular
            if (S_ISREG(datos.st_mode)) {
                printf("Nombre: %-25s Tamaño: %ld bytes\n", direc->d_name, datos.st_size);
            }
        }
    }

    // Cierra el directorio
    closedir(d);
    return 0;
}

