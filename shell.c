#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX 256

int main(void){

  while(1){
    printf("retfut > ");
    fflush(stdout);
    int status = -1;

    char cmds[MAX];
    if(fgets(cmds, sizeof(cmds), stdin) == NULL) return 0;

    char *argv[10];
    int argc = 0;

    char *cmd = strtok(cmds, " \n");

    while(cmd != NULL){
      if(argc >= 9){
        fprintf(stderr, "too many arguments\n");
        argc = 0;
        continue;
      }
      argv[argc++] = cmd;
      cmd = strtok(NULL, " \n");
    }

    if(argc == 0) continue;

    argv[argc] = NULL;

    pid_t pid = fork();
    if(pid < 0) return -1;
    else if(pid == 0){
      if(execvp(argv[0], argv) == -1){
        perror(argv[0]);
        _exit(1);
      }
    }
    else{
      if(waitpid(pid, &status, 0) == -1) perror("waitpid");
    }
    
  }
  return 0;
}
    
