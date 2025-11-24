#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#define NAME_MAX 255
#define PATH_MAX 4096
#define MAX_CARPETAS 100
int numero = 0;
#define MAX_ARCHIVOS 1000
typedef struct {
    char nombre[NAME_MAX + 1];
    char ruta[PATH_MAX + 1]; 
    long tamano;
} Registro;
Registro arrayRegistros[MAX_ARCHIVOS];
int contador_max=1000;
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
const char *directorios[MAX_CARPETAS];
int main(int argc, char *argv[]){
    int precision=1;
    int bytes=0;
    int numdir=0;
    for(int i=1;i<argc;i++){
       if (strcmp(argv[i], "-l") == 0 && i + 1 < argc) {
          precision = atoi(argv[i + 1]);
          i++;  
       }
       else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
          contador_max= atoi(argv[i + 1]);
          i++;  
       }
       else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
          bytes = atoi(argv[i + 1]);
          i++;  

       
    }
       
    }
    for (int i=1;i<argc;i++){
         if(strcmp(argv[i], "-l")==0 || strcmp(argv[i], "-p")==0 || strcmp(argv[i], "-s")==0 ){
            i++; 
           continue;
        }
    
        if(numdir<MAX_CARPETAS){
            directorios[numdir++]= argv[i];
        }
       
    }    
    if(numdir==0){
       directorios[numdir++]=".";
    }
    for(int i=0;i<numdir;i++){
        leercarpeta(directorios[i], 0);
    }
    for(int i=0;i<guardados;i++){
    
        Registro aux = arrayRegistros[i];
        for(int j=i+1;j<guardados;j++){
            if(arrayRegistros[i].tamano<bytes||arrayRegistros[j].tamano<bytes)
            continue;
            int dup= 0;
            if(precision==1){
               if(strcmp(aux.nombre, arrayRegistros[j].nombre)==0){
                 dup=1;
               }
            }else if(precision==2 ){
               if(strcmp(aux.nombre, arrayRegistros[j].nombre)==0 && aux.tamano==arrayRegistros[j].tamano){
                  dup=1;
               }
            }else{
              printf("l no es un valor válido");
            }
            if(dup==1){
                printf("\nDUPLICADO:\n");
                printf("  Tamaño: %ld bytes\n", arrayRegistros[i].tamano);
                printf("  %s\n", arrayRegistros[i].ruta);
                printf("  %s\n", arrayRegistros[j].ruta);
                
            }
    }
    }
return 0;
}
