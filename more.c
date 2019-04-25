#include "ucode.c"

#define BUFFSIZE 1024

char buf[1];
char tty[32];

int main(int argc, char *argv[]){
  int n;
  int in;
  int lines = 0;
  int flag = 0;



  gettty(tty);
  int in1 = open(tty, O_RDONLY);
  int out1 = open(tty, O_WRONLY);

  printf("argc is %d\n", argc);
  if(argc > 1){
    in = open(argv[1], O_RDONLY);
  }
  else{
    in = 0;
  }

  if(in < 0){
    printf("file not opened\n");
    return 1;
  }

  while((n = read(in, buf, 1))){
    //mputc(*buf);
    write(1, buf, 1);


    if(*buf == '\n' || *buf == '\r'){
      lines++;
      if(lines == 25){
        flag = 1;
      }
      if(flag){
        while(1){
          char c = 0;

          n = read(in1, &c, 1);
          c = (c&0x7F);

          if(c == '\n' || c == '\r'){//if enter the user exits more
            break;
          }
          else if(c == ' '){//if space print next screen
            lines = 0;
            flag = 0;
            break;
          }
        }
      }
      write(out1, "\n\r", 1);
    }
  }
  close(in); close(out1);
}
