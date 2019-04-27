//cat
#include "ucode.c"

#define BLKSIZE 1024

//my 360 stuff used to adapt 460 equivalent
/*int my_read(int fd, char * buf, int nbytes){
  printf("In Read\n");
  MINODE *mip, *mip2;
  mip = running->cwd;
  int count = 0, blk;
  if(nbytes > running->fd[fd]->mptr->INODE.i_size){
    nbytes = running->fd[fd]->mptr->INODE.i_size;//if nbytes is more than size set it to size
  }
  int available = running->fd[fd]->mptr->INODE.i_size - running->fd[fd]->offset;
  char  *cp = buf, readbuf[BLKSIZE];
  mip2 = running->fd[fd]->mptr;

  while(nbytes && available){
    //maps logic block to physical block
    int lbk = running->fd[fd]->offset / BLKSIZE;
    int startbyte = running->fd[fd]->offset % BLKSIZE;

    if(lbk < 12){
      printf("Direct blocks\n");
      blk = mip2->INODE.i_block[lbk];
    }
    else if(lbk >= 12 && lbk < 256 + 12){
      printf("indirect blocks\n");
      int indirect[256];
      get_block(mip2->dev,mip2->INODE.i_block[12],indirect);
      blk = indirect[lbk-12];
    }
    else{
      printf("double indirect blocks\n");
      int dbl_indirect[256];
      get_block(mip2->dev,mip2->INODE.i_block[13],dbl_indirect);
      for(int j = 1; j<257; j++){
        if(lbk >= 256*j+12 && lbk < 256*(j+1) + 12)
        {
          int block_storage[256];
          get_block(mip2->dev,dbl_indirect[j-1],block_storage);
          blk = block_storage[lbk-(256*j+12)];
          break;
        }
      }
    }
    if(blk == 0){
      return count;
    }
    //end mapping

    get_block(mip->dev, blk, readbuf);

    char *store = readbuf + startbyte;
    int remain = BLKSIZE - startbyte;
    while(remain > 0){
      if(nbytes-BLKSIZE > 0){
        printf("BLKSIZE to read\n");
        strncpy(buf, store, BLKSIZE);
        count += BLKSIZE;
        nbytes -= BLKSIZE;
        available -= BLKSIZE;
        remain -= BLKSIZE;
        running->fd[fd]->offset += BLKSIZE;

        if(nbytes <= 0 || available <= 0){
          break;
        }
      }
      else{
        //printf("less than BLKSIZE to read\n");
        strncpy(buf, store, nbytes);
        count += nbytes;
        available -= nbytes;
        remain -= nbytes;
        running->fd[fd]->offset += nbytes;
        nbytes -= nbytes;
        if(nbytes <= 0 || available <= 0){
          break;
        }
      }
    }
  }
  return count;
}

int cat_file(char * file){
  char buf[BLKSIZE];
  int n, i = 0;

  printf("in cat\n")
  int fd = open(file, 0);
  if(fd > -1){
    while( n = my_read(fd, buf, BLKSIZE)){
      printf("read block\n");
      for(i=0;i<n;i++){
        if(buf[i] == '\\'){
          if(buf[i+1] == 'n'){//if new line print new line
            putchar('\n');
            i++;
          }
        }else{
          putchar(buf[i]);
        }
      }
    }
    printf("\n");//print new line
    close_file(fd);
  }
  else{
    printf("ERROR: couldn't open file to read\n");
  }
}*/

int main(int argc, char *argv[]){
  //360 code attempt of course it doesn't work
  /*if(argc > 1){
    cat_file(argv[1]);
  }
  else{
    int n = 0;
    char tty[32];
    char *buf;
    char c;
    buf = &c;
    gettty(tty);
    int in = open(tty, O_RDONLY);
    int out = open(tty, O_WRONLY);

    while(1){
      n = read(in, buf, 1);
      if(n>1){
        write(out, buf, 1);
      }
    }
  }*/

  char tty[64];
  char buf[12];
  char line[128];

  int n, in, out, i;

  //gettty(tty);
  int out1 = open("/dev/tty0", O_WRONLY);

  if(argc < 2){
    in = 0;
    out = 1;
  }
  else{
    close(0);
    in = open(argv[1], O_RDONLY);
    out = 1;
  }

  i = 0;

  while(1){
    n = read(in, buf, 1);
    if(n<1){
      break;
    }
    line[i] = buf[0];
    if(!in){
      write(out1, buf[0], 1);
    }
    if(line[i] == '\n'){
      line[++i] = '\r';
      write(out, line, i);
      write(out1, "\r", 1);
      memset(line, 0, 128);
      i = 0;
    }
    else if (line[i] == '\r' && !in) {
      line[i++] = '\n';
      line[i++] = '\r';
      write(out1, "\r", 2);
      write(out,line,  i);
      memset(line, 0, 128);
      i = 0;
    }
    else{
      i++;
    }
  }
  close(in); close(out); close(out1);
  exit(0);
}
