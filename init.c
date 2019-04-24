#include "ucode.c"
#define BUFFSIZE 1024

int in1, in2, in3, out1, out2, out3, err1, err2, err3;

int s0, s1, cons;
char buf[BUFFSIZE];


int main(int argc, char * argv[]){
  in1 = open("/dev/tty0", 0);
  out1 = open("/dev/tty0", 1);
  err1 = open("/dev/tty0", 2);
  in2 = open("/dev/ttyS0", 0);
  out2 = open("/dev/ttyS0", 1);
  err2 = open("/dev/ttyS0", 2);
  in3 = open("/dev/ttyS1", 0);
  out3 = open("/dev/ttyS1", 1);
  err3 = open("/dev/ttyS1", 2);

  int status;

  cons = fork();

  while(1){
    if(cons){
      s0 = fork();
      if(s0){
        s1 = fork();
        if(s1){
          int pid, status;
          while(1){
            printf("Init: waiting for zombie\n");
            pid = wait(&status);

            if(pid==cons){
              cons = fork();
              if(cons){
                continue;
              }
              else{
                char * line = "Init- Login on console:\n\r";
                write(out1, line, strlen(line));
                exec("login /dev/tty0");
              }
            }
            else if(pid == s0){
              s0 = fork();
              if(s0){
                continue;
              }
              else{
                char * line = "Init- Login on s0:\n\r";
                write(out2, line, strlen(line));
                exec("login /dev/ttyS0");
              }
            }
            else if(pid == s1){
              s1 = fork();
              if(s1){
                continue;
              }
              else{
                char * line = "Init- Login on s1:\n\r";
                write(out3, line, strlen(line));
                exec("login /dev/ttyS1");
              }
            }
          }
        }
        else{
          char * line = "Init- Login on s1:\n\r";
          write(out3, line, strlen(line));
          exec("login /dev/ttyS1");
        }
      }
      else{
        char * line = "Init- Login on s0:\n\r";
        write(out2, line, strlen(line));
        exec("login /dev/ttyS0");
      }
    }
    else{
      char * line = "Init- Login on console:\n\r";
      write(out1, line, strlen(line));
      exec("login /dev/tty0");
    }
  }
  return 0;
}
