//cp
#include "ucode.c"

int main(int argc, char *argv[]){
  char buf;

  int in, out;
  if(argc == 3 && strcmp(argv[1], argv[2]) != 0){//makes sure there are source and destination and that source isnt destination
    in = open(argv[1], O_RDONLY);
    out = open(argv[2], O_WRONLY);
  }
  else{
    exit(-1);
  }
  while(read(in, &buf, 1)){
    write(out, &buf, 1);
  }
  exit(0);
}
