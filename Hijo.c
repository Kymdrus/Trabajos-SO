#include<stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
int main(){
    srand(getpid());
    int r= rand()%5+1;
    sleep(r);
    if(rand()%2){
       printf("No lanzo nada ,soy el hijo con el Pid %d \n", getpid()); 
    }else{
       printf("Lanzando SIGUSR1 con el pid%d\n", getpid());
        kill(getppid(),SIGUSR1);
    }

    return 0;
}