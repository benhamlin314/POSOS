//login
#include "ucode.c"

#define BUFFSIZE 1024

char buf[BUFFSIZE], *bp; //buffer and bufferpointer

//returns flag for no more tokens to parse. 1 for being end of string or file
int tokenizer(char* str, char* outstr, char delim, int occurrences) {
  //printf("start tokenizer\n");
  int i = 0, j = 0;
  if(!str){
    return 1;
  }
  if(!delim){
    return 1;
  }

  //printf("clearing outstring\n");
  while(outstr[i] != 0) {
    outstr[i] = 0;
    i++;
  }
  i = 0;

  //printf("finding location in string\n");
  while(str[i] && occurrences > 0) {
    if (str[i] == delim) {
      occurrences--;
    }
    i++;
  }

  //makes sure its not end of string
  if (!str[i]) {
      return 1;
  }

  //printf("copying\n");
  while (str[i] && str[i] != delim) {
    outstr[j] = str[i];
    //printf("%c ", outstr[j]);
    i++;
    j++;//incremented here so the debug prints would work
  }
  outstr[j] = 0;
  return 0;
}


int main(int argc, char *argv[]){
  int in, out, err, passwrdfile,  n=0, i=0, k=1;
  int gid, uid;

  char user[32], pass[32], line[128], *cp;

  close(0); close(1); close(2);
  in = open(argv[1], O_RDONLY);
  out = open(argv[1], 1);
  err = open(argv[1], 2);

  fixtty(argv[1]);

  //printf("opening passwd file\n");
  passwrdfile = open("/etc/passwd", 0);
  if(!passwrdfile){
    printf("Login failed to load password file!!\n");
    close(in); close(out); close(err); close(passwrdfile);
    return -1;
  }
  else{
    printf("Loaded passwd...\n");
  }

  while(1){
    printf("Login: ");
    gets(user);//user login
    printf("password: ");
    gets(pass);//user password
    //printf("user: %s, password: %s\n", user, pass);
    int j = 0;
    n = read(passwrdfile, buf, BUFFSIZE);
    //printf("getting line from file\n");

    tokenizer(buf, line, '\n', j++);
    //line = strtok(buf, '\n');
    //printf("%s\n", line);

    int flag = 0;
    while(line){
      k = 0;
      if(flag){
          printf("Login not found\n");
          return -1;
      }
      else{
        printf("Load a line from password\n");
      }

      char name_file[32];
      tokenizer(line, name_file, ':', k++);
      //printf("name in file: %s\n", name_file);
      //name_file = strtok(line, ':');
      if(!strcmp(name_file, user)){
        char pass_file[32];
        tokenizer(line, pass_file, ':', k++);
        //pass_file = strtok(0, ':');

        if(!strcmp(pass_file, pass)){
          //username and password Found
          char uid[16];
          char gid[16];
          char fullname[32];
          char alias[16];
          char mode[16];
          char direc[16];
          char program[16];

          tokenizer(line, gid, ':', k++);
          //gid = strtok(0, ':');
          tokenizer(line, uid, ':', k++);
          //uid = strtok(0, ':');
          tokenizer(line, fullname, ':', k++);
          //fullname = strtok(0, ':');
          tokenizer(fullname, alias, ' ', 0);
          tokenizer(fullname, mode, ' ', 1);
          tokenizer(line, direc, ':',k++);
          //direc = strtok(0, ':');
          tokenizer(line, program, ':', k++);
          //program = strtok(0, ':');
          //alias = strtok(fullname, ' ');
          //mode = strtok(0, ' ');

          chuid(atoi(uid), atoi(gid));
          chdir(direc);

          //printf("%s %s %s %s %s %s %s %s %s\n", name_file, pass_file, gid, uid, fullname, alias, mode, direc, program);
          close(passwrdfile);
          printf("Welcome %s. Loading %s\n", alias, direc);
          exec(program);
          return 0;
        }

      }
      flag = tokenizer(buf, line, '\n', j++);
      //line = strtok(0, '\n');
    }
    if (line[0] == 0){
      printf("Login Failed\n");
      close(passwrdfile);
      passwrdfile = open("/etc/passwd", 0);//read file again resetting
    }
  }
  printf("Exit\n");
}
