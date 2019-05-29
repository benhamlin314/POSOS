//more
#include "ucode.c"

#define BUFFSIZE 1024

char buf[2];
char tty[64];

int main(int argc, char *argv[]){
  int n;
  int in, out;
  int lines = 0;
  int flag = 0;



  gettty(tty);
  int in1 = open("/dev/tty0", O_RDONLY);
  int out1 = open("/dev/tty0", O_WRONLY);

  //printf("argc is %d\n", argc);
  if(argc > 1){
    in = open(argv[1], O_RDONLY);
    out = 1;
  }
  else{
    in = in1;
    out = 1;
  }

  if(in < 0){
    write(out1, "file not opened\n", 16);
    exit(-1);
  }

  while((n = read(in, buf, 1)) == 1){
    /*if(n < 1){
      printf("DONE\n");
      break;
    }*/
    //mputc(*buf);
    //putchar(buf[0]);
    if(argc > 1){//file out through tty
      write(out, buf, 1);
    }
    else{//stdout
      write(out1, buf, 1);
    }


    if(*buf == '\n' || *buf == '\r'){
      lines++;
      if(lines == 25){
        flag = 1;
      }
      if(flag){
        while(1){
          char c = 0;

          n = read(in1, &c, 1);
          if(n==0||c==4||c==0){
            continue;
          }
          c = (c & 0x7F);

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
  exit(0);
}
