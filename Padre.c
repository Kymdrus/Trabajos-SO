#include<stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#define hijosmaximo 100
int num_hijos= 0;
pid_t hijos[hijosmaximo];
volatile sig_atomic_t sigint_recibido = 0;

void manejador_sigint(int sig) {
    sigint_recibido = 1;
}
int  main(int argc, char *argv []) {
     if(argc<=1){
      fprintf(stderr,"Error No se ha introducido ningún Archivo");
      exit(EXIT_FAILURE);
     }
      signal(SIGINT,&manejador_sigint);
     if(argc>=1){
       for(int i=1;i<argc;i++){
        pid_t pid=fork();
        if(pid==0){
         
          printf("El hijo se ha creado Tiene el pid%d\n", getpid());
          printf(" y está asociado con%s\n", argv[i]);
          execl("./Hijo","Hijo",argv[i],(char*)NULL);  
          perror("execl");
          exit(EXIT_FAILURE);
        }else if(pid>=1){
          hijos[num_hijos++]=pid;
        }else{
          perror("fork");
          return EXIT_FAILURE;
        }
      }
       
     }
     while(num_hijos>0){
      pid_t pid =wait(NULL);
     
      if(sigint_recibido){
         printf("Padre recibe singint, termina hijos \n");
         for(int i=0;i<num_hijos;i++){
           kill(hijos[i],SIGTERM);
          }
      }
       if(pid>0){
        printf("Fin hijo %d\n", pid);
        num_hijos--;
      }
     }

     return 0;
}