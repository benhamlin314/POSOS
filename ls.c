#include "ucode.c"

#define BUFFSIZE 1024


char *t1 = "xwrxwrxwr-------";
char *t2 = "-----------------";

char cwd[32];
char buf[BUFFSIZE];

int ls_boi(char *name, STAT *sp){
  //printf("%x ", sp->st_mode);
  //printf("%s name\n", name);
  if(sp->st_mode == 0x41ED){//if directory
    mputc('d');
  }
  else{
    mputc('-');
  }

  //to be fair I got this from a friend from 360 after trying to get my i_mode
  //to print all nicely I understand how it works now and it looks better
  //than the hex code of the mode
  int i = 0;
  for(i = 8; i >= 0; i--){
    if(sp->st_mode & (1<<i)){
      mputc(t1[i]);
    }
    else
    {
      mputc(t2[i]);
    }
  }

  printf("  %d  ", sp->st_nlink);
  printf("%d  ", sp->st_uid);
  printf("%d  ", sp->st_gid);
  printf("%d  ", sp->st_size);
  printf("    %s\n\r", name);
}

int main(int argc, char *argv[]){
  //pwd();//used to help debug
  //printf("in ls\n");
  DIR *dp;
  char * cp;
  STAT statbuf;//not a pointer to have stuff actually stored locally
  int fd;
  int n, flag;

  if(argc > 1){
    //printf("opening pathname\n");
    fd = open(argv[1], O_RDONLY);
  }
  else{
    //printf("opening cwd\n");
    getcwd(cwd);
    fd = open(cwd, O_RDONLY);
  }
  //printf("cwd is: %s\n", cwd);

  if(fd < 0){
    printf("file descriptor failed to open\n");
    close(fd);
    return 1;
  }

  //read inode block
  n = read(fd, buf, BUFFSIZE);
  cp = buf;
  dp = (DIR *)cp;

  if(n < 0){
    printf("read failed\n");
    return 1;
  }
  char temp[32];
  //printf("printing stats\n");
  while(cp < buf + BUFFSIZE){
    memset(temp, 0, 32);//resets temp every iteration to prevent corruption
    strncpy(temp, dp->name, dp->name_len);//copys name to TEMPORARY
    strcat(temp, "\0");
    //printf("temp is: %s\n", temp);

    if((flag = stat(temp, &statbuf)) < 0){
      printf("stat failed\n");
    }
    else{
      //printf("flag is: %d\n", flag);
      ls_boi(temp, &statbuf);
    }

    cp += dp->rec_len;
    dp = (DIR *)cp;
  }
  close(fd);
  return 0;
}
