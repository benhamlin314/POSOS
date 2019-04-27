//l2u
#include "ucode.c"

#define BUFFSIZE 1024

char *lower = "abcdefghijklmnopqrstuvwxyz";
char *upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

char buf[BUFFSIZE];
char tty[32];


char twoupper(char c){
  int k = 0;
  while(lower[k]){
    if(lower[k] == c){
      return upper[k];
    }
    k++;
  }
  return -1;//not alpha
}

int main(int argc, char *argv[]){
  int n;
  int in, out;
  char line[128];

  //gettty(tty);
  int out1 = open("/dev/tty0", O_WRONLY);

  if(argc < 3){
    in = 0;
    out = 1;
  }
  else{
    in = open(argv[1], O_RDONLY);
    out = open(argv[2], O_WRONLY);
  }

  int i = 0;;

  while(1){
    n = read(in, buf, 1);
    if(n < 1){
      break;
    }
    line[i] = buf[0];
    char c = twoupper(buf[0]);
    //printf("after upper\n\r");
    if(c != -1){
      line[i] = c;
      buf[0] = c;
    }

    if(!in){
      write(out1, buf, 1);
    }
    if(line[i] == '\n'){
      line[++i] = '\r';
      write(out, line, i);
      write(out1, "\n\r", 2);
      memset(line, 0, 128);//reset lines
      i = 0;
    }
    else if(line[i] == '\r' && !in){
      line[i++] = '\n';
      line[i++] = '\r';
      write(out1, "\r\n", 2);
      write(out, line, i);
      i = 0;
    }
    else{
      i++;
    }
  }
  close(in);
  close(out);
  close(out1);
  exit(0);
}
