#include<stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
void Finalizacion(int signal){
  printf("Lanzando CallBack \n");
}
int  main(int argc, char *argv []) {
     if(argc<2){
      printf("Error No se ha introducido ningún Archivo");
      //falta meter logica de error cuando está mal escrito
     }else{
       for(int i=1;i<argc;i++){
        pid_t pid=fork();
        if(pid==0){
          printf("El hijo se ha creado Tiene el pid%d\n", getpid());
          printf(" y está asociado con%s\n", argv[i]);
          execlp("wc","wc",argv[i],NULL);  
        }else if(pid>=1){
            signal(SIGINT,&Finalizacion);
        }
      }
        int pid=wait(NULL);
       // printf("Soy el padre con el pid%d\n", getppid());
       
       
      
     }
    
      
      return 0;
}