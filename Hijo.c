#include<stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
int main(int argc,char *argv[]){
   sleep(10);
   execlp("wc", "wc", argv[1], (char*)NULL);
    return 0;
}