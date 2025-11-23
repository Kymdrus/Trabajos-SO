#include <stdio.h>      // printf
#include <string.h>     // strcopy
#include <stdlib.h>     // no me acuerdo
#include <dirent.h>     // opendir y readdir
#include <sys/stat.h>   // lstat y struct stat

struct Archivo { //Para el array donde los ordeno, filtro, etc...
    char nombre[256];      
    char ruta_completa[4096];
    long tamano;           
}tabla_archivos[2000]; //Numero grande para asegurarme que no se llebna

int contador_archivos_guardados = 0; 


// Fucnion recursiva que mira todos los directorios
void analizarArchivos(char *ruta_actual, int profundidad) {
    
    DIR *d = opendir(ruta_actual);
    if (d == NULL) {return;}// Si no existe o algo, sale.

    struct dirent *entrada;
    struct stat datos_archivo;
    char ruta_completa[4096];

    // Bucle que lee todos los archivos
    while ((entrada = readdir(d)) != NULL) {
        // Se salta los ".." y "." 
        if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0) {
            continue;
        }

        //Meto en "ruta_completa" la ruta actual + nombre del archivo.
        snprintf(ruta_completa, 4096, "%s/%s", ruta_actual, entrada->d_name);

        // guardar todos los datos de "ruta completa" a "datos_archivo"
        lstat(ruta_completa, &datos_archivo); 

        // Si es directorio.....->
        if (S_ISDIR(datos_archivo.st_mode)) {
            //Si no llegamos al final, entonces sigue recursivamente.
            if (profundidad != 0) {
                analizarArchivos(ruta_completa, profundidad - 1);
            }
        }

        //Si es un archivo normal...->
        else if (S_ISREG(datos_archivo.st_mode)) {
            // Controlo no haber llegado al final del array así no peta.
            if (contador_archivos_guardados < 2000) {

                // Guardo la información en mi array con todos los directorios
                strcpy(tabla_archivos[contador_archivos_guardados].nombre, entrada->d_name);
                strcpy(tabla_archivos[contador_archivos_guardados].ruta_completa, ruta_completa);
                tabla_archivos[contador_archivos_guardados].tamano = datos_archivo.st_size;

                contador_archivos_guardados++;

            } else {
                printf("Se está superando el límite del array de 2000!\n");
                break; 
            }
        }
    }

    closedir(d);
}

int main(int argc, char *argv[]){



    int profundidad = 0;
    int precision = 0;
    int bytes = 0;
    char directorio[4096] = "";

    int contador = 1;

    while(contador <= argc-1){
        
        char *argumentoActual = argv[contador];

        // --------parsear p-------
        if (!strcmp(argumentoActual, "-p"))
        {
            if ((contador+1) == (argc)){break;}
            if (argv[contador+1][0] == '-')
            {
                printf("El argumento %s espera un valor.\nSaliendo...\n", argumentoActual);
                exit(0);
            }
            
            profundidad = atoi(argv[contador+1]);
            contador+=2;
            continue;
        }

        // --------parsear s-------
        else if (!strcmp(argumentoActual, "-l"))
        {
            if ((contador+1) == (argc)){break;}
            if (argv[contador+1][0] == '-')
            {
                printf("El argumento %s espera un valor.\nSaliendo...\n", argumentoActual);
                exit(0);
            }
            
            precision = atoi(argv[contador+1]);
            contador+=2;
            continue;
        }

        // --------parsear b-------
        else if (!strcmp(argumentoActual, "-s"))
        {
            if ((contador+1) == (argc)){break;}
            if (argv[contador+1][0] == '-')
            {
                printf("El argumento %s espera un valor.\nSaliendo...\n", argumentoActual);
                exit(0);
            }
            
            bytes = atoi(argv[contador+1]);
            contador+=2;
            continue;
        }

        // Tomar el directorio
        // El directorio no va por argumento, sino que simplemente aparece, así que las validaciones son distintas
        // Si el usuario pone más de un directorio, simplemente se sobreescribe
        else
        {
            //stringcopy -> copia lo de argv a directorio
            strcpy(directorio, argv[contador]);
            if ((contador+1) == (argc)){break;}
            contador++;
            continue;
        }

        // Como todos los valores llevan a continue, si estoy aquí es que algo pasó mal.
        printf("El argumento %s no existe.\n", argumentoActual);
        exit(0);
    }


    printf("Profundidad : %d\nPrecision : %d\nBytes : %d\nDirectorio : %s\n", profundidad, precision, bytes, directorio);


    //Compruebo si se especificó un directorio
    if (!strcmp(directorio, ""))
    {
        strcpy(directorio, ".");
    }

    //Funcion que toma todos los directorios y los guardará en un array gigante que ya cree previamente
    analizarArchivos(directorio, profundidad);

    ///------------------------------------------------///
    ///                      SORT!!!!!!                ///
    ///------------------------------------------------///

    struct Archivo auxiliar; 

    // Bubble sort que se repite la cantidad de archivos que haya
    for (int i = 0; i < contador_archivos_guardados - 1; i++) {
        
        // recorre todo el array haciendo comparaciones en pareja.
        for (int j = 0; j < contador_archivos_guardados - 1; j++) {
            // strcmp devuelve un int que indica si el primero va después alfabéticamente (compara los valores ascii simplemente. Creo)
            if (strcmp(tabla_archivos[j].nombre, tabla_archivos[j+1].nombre) > 0) {
                //Los cambio de lugar
                auxiliar = tabla_archivos[j]; 
                tabla_archivos[j] = tabla_archivos[j+1]; 
                tabla_archivos[j+1] = auxiliar;
            }
        }
    }

    ///------------------------------------------------///
    ///                    Fin del SORT                ///
    ///------------------------------------------------///




    ///------------------------------------------------///
    ///     Encontrar e imprimir los duplicados        ///
    ///------------------------------------------------///

    //Bucle for en la cantidad de archivos que guardé.
    for (int i = 0; i < contador_archivos_guardados - 1; i++) {

        // Si la precisión es 1, solo compara los nombres
        if (precision == 1) {

           //Si el nombre del actual y el siguiente son iguales
            if (strcmp(tabla_archivos[i].nombre, tabla_archivos[i+1].nombre) == 0) {
                
                // Comprueba que no se pase de los bytes especificados
                if (tabla_archivos[i].tamano >= bytes) {
                    
                    printf("Se encontraron archivos duplicados!\n");
                    
                    //Imprime los dos elementos
                    printf("%s\tbytes: %ld\t%s\n", 
                           tabla_archivos[i].nombre, 
                           tabla_archivos[i].tamano, 
                           tabla_archivos[i].ruta_completa);
                    
                    printf("%s\tbytes: %ld\t%s\n\n", 
                           tabla_archivos[i+1].nombre, 
                           tabla_archivos[i+1].tamano, 
                           tabla_archivos[i+1].ruta_completa);
                }
            }
        }

        // Si es 2, compara nombres y tamaño
        else if (precision == 2) {
            // Compara nombres y tamaño.
            if (strcmp(tabla_archivos[i].nombre, tabla_archivos[i+1].nombre) == 0 && 
                tabla_archivos[i].tamano == tabla_archivos[i+1].tamano) {
                
                //Exactamente lo mismo que antes.
                if (tabla_archivos[i].tamano >= bytes) {
                    printf("Se encontraron archivos duplicados tanto por nombre como por tamaño!\n");

                    printf("%s\tbytes: %ld\t%s\n", 
                           tabla_archivos[i].nombre, 
                           tabla_archivos[i].tamano, 
                           tabla_archivos[i].ruta_completa);

                    printf("%s\tbytes: %ld\t%s\n\n", 
                           tabla_archivos[i+1].nombre, 
                           tabla_archivos[i+1].tamano, 
                           tabla_archivos[i+1].ruta_completa);
                }
            }
        }
    }

    return 0;
}
