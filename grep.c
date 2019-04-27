//grep
#include "ucode.c"

#define BUFFSIZE 1024

char tty[32];
char buf[BUFFSIZE];

int main(int argc, char *argv[]){
  char * pattern;
  char line[128];
  int in, out;
  int i, j;
  int n;
  int flag = 0;

  //gettty(tty);
  int out1 = open("/dev/tty0", O_WRONLY);

  if(argc < 2){
    printf("not enough arguments\n");
    exit(-1);
  }

  if(argc < 3){//stdin
    pattern = argv[1];
    out = 1;
    in = 0;
  }
  else{//file
    pattern = argv[1];
    out = 1;
    in = open(argv[2], O_RDONLY);
    if(in < 0){
      printf("open failed\n");
    }
  }

  if(pattern != 0){
    exit(-1);
  }

  int length = strlen(pattern);

  i = 0;
  memset(line, 0, 128);//sets line to all 0
  while(1){
    n = read(in, buf, 1);
    if(n<1){
      break;
    }
    line[i] = buf[0];
    if(buf[0] == '\n' || buf[0] == '\r'){
      line[++i] == '\r';
      if(flag){
        write(out, line, i);
        write(out1, "\n\r", 1);
      }
      flag = 0;
      i = 0;
      memset(line, 0, 1);
    }
    else{
      /*while(line[i] == pattern[i] && i < length)
      {
        i++;
      }*/
      if(line[i] == pattern[j]){
        j++;
        if(j == length && !flag){
          flag = 1;
        }
      }
      else{
        j = 0;
      }
      /*if(i == length){
        flag = 1;
      }*/
      i++;
    }
  }
  close(in); close(out);
  exit(0);
}
