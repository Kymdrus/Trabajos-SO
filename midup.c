#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#define NAME_MAX 255
#define PATH_MAX 4096
int numero = 0;
int contador_max=4;
#define MAX_ARCHIVOS 1000
typedef struct {
    char nombre[NAME_MAX + 1];
    char ruta[PATH_MAX + 1]; 
    long tamano;
} Registro;
typedef struct {
    char nombre[NAME_MAX + 1];  
    long tamano;                
    int repeticiones;
} Duplicado;
Registro arrayRegistros[MAX_ARCHIVOS];

   
int guardados=0;
void leercarpeta(const char *path,int contador){
    DIR *dir;
    struct dirent *ent;
    struct stat st;
    char subruta[PATH_MAX];
    dir= opendir(path);
    if(dir==NULL)
     return;
    while((ent = readdir(dir)) != NULL){
      if (strcmp(ent->d_name, ".") == 0 ||
        strcmp(ent->d_name, "..") == 0)
        continue;
    
        sprintf(subruta, "%s/%s", path, ent->d_name);
        stat(subruta, &st);
       

        if (S_ISDIR(st.st_mode)) {
            // ¿Qué pasaría si llamamos recorrer() sin limitar la profundidad?
            if (contador < contador_max ) {
                leercarpeta(subruta, contador + 1);
            }
        
        }else if(S_ISREG(st.st_mode)){
                strcpy(arrayRegistros[guardados].nombre, ent->d_name);
                strcpy(arrayRegistros[guardados].ruta, subruta);
                arrayRegistros[guardados].tamano = st.st_size;
                guardados++;

        }else {
                printf("Se está superando el límite del array de 2000!\n");
                break; 
              }
        
    }
    closedir(dir);

}
int main(int argc, char *argv[]){
    const char *dirbase;
    dirbase = argv[1];
    if (argc >= 3)
        contador_max = atoi(argv[2]);
    leercarpeta(dirbase, 0);
      printf("Archivos encontrados:\n");
    for (int i = 0; i < guardados; i++) {
        printf("[%d] Nombre: %s | Ruta: %s | Tamaño: %ld bytes\n",
               i,
               arrayRegistros[i].nombre,
               arrayRegistros[i].ruta,
               arrayRegistros[i].tamano);
    }

    for(int i=0;i<argc;i++){
        if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            
           
        } 
    }
return 0;
}