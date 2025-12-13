#include<stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
void callback(int signal){
  printf("Lanzando CallBack \n");
}
int  main() {
      for(int i=0;i<3;i++){
        pid_t pid=fork();
        if(pid==0){
          printf("Soy Hijo\n");
          execl("./Hijo","Hijo",NULL); 
            
        }else if(pid>=1){
            signal(SIGUSR1,&callback);
        }

      }
      for(int i=0;i<3;i++){
        int pid=wait(NULL);
        printf("Soy el padre con el pid%d\n", getppid());
        printf("y ha acabado el hijo con el pid%d\n",getpid());
      }
      
      return 0;
}