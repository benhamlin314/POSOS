//sh
#include "ucode.c"

#define BUFFSIZE 1024

char buf[BUFFSIZE];
char input[128];

int main(int argc, char * argv[]){
  int in = 0;
  int status;
  while(1){
    printf("bhsh #%d $", getpid());
    gets(input);

    if(!strcmp(input, "logout") || !strcmp(input, "exit")){
      break;
    }

    int pid = fork();
    if(pid == 0){
      exec(input);
    }
    else{
      pid = wait(&status);
    }
  }
}
