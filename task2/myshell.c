#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include "LineParser.h"
extern cmdLine *parseCmdLines(const char *strLine);
extern void freeCmdLines(cmdLine *pCmdLine);
extern int replaceCmdArg(cmdLine *pCmdLine, int num, const char *newString);

char * history[10];
int commands_counter=0;
int flag =0;
void print_history(){
 
  if(flag==0){
        for (int i = 0; i < commands_counter; i++){
            printf("%s", history[i]);
       }
    }
    else{
      for(int j=commands_counter;j<10;j++){
       printf("%s", history[j]);
       }
       for(int j=0;j<commands_counter;j++){
          printf("%s", history[j]);
       }
    }
}
void update_history(char* command){
    char *temp = (malloc(strlen(command)));
    strcpy(temp, command);
    history[commands_counter%10] = temp;
    commands_counter =commands_counter+ 1;
  if(commands_counter>9)
  {
    flag=1;
    commands_counter=commands_counter%10;
  }
}

void execute(cmdLine* pCmdLine){
int cpid_status;
pid_t cpid;
cpid=fork();
if(cpid==-1){
  perror("fork \n");
  exit(EXIT_FAILURE);
  }
if(cpid == 0){
  if(strcmp(pCmdLine->arguments[0],"cd")==0){
  chdir(pCmdLine->arguments[1]);
  perror("cd : ");
  exit(0);
  return;
  }
  if(strcmp(pCmdLine->arguments[0],"history")==0){
    print_history();
     return;
    }
  if(strcmp(pCmdLine->arguments[0],"!")==0){
    int log=atoi(pCmdLine->arguments[1]);
    if(log >commands_counter){
    fprintf(stderr,"! Log index out of bounds. So far , %d , commands initilaized \n",commands_counter);
    }
    else{
     update_history(history[log]);
     cmdLine *cmd;
     cmd=parseCmdLines(history[log]);
     fprintf(stderr,"executing now function %s #number : %d \n",history[log],log);
     execute(cmd);
     freeCmdLines(cmd);
      
    }
    exit(EXIT_SUCCESS);
    return;
  }
  execvp(pCmdLine->arguments[0],pCmdLine->arguments);
  perror("Exec : ");
  exit(EXIT_FAILURE);
}
else{
if(pCmdLine->blocking==1){
   if(waitpid(cpid,NULL,WUNTRACED |WCONTINUED |0 )<0){
     perror("Waitpid : ");
     exit(EXIT_FAILURE);
   }
}
}
}

int main(int argc,char**argv){
  char* buf;
  char* ptr;
  char line[2048];
  cmdLine* cmd;
  while(1){
  if((buf=(char*)malloc((MAX_ARGUMENTS)))!=NULL){
    ptr= getcwd(buf,MAX_ARGUMENTS);
    fprintf(stderr,"The current working directory: %s \n",ptr);
  }
 if(fgets(line,2048,stdin)!=NULL){
    cmd=parseCmdLines(line);
 }
  if(strcmp(cmd->arguments[0],"quit")==0){
    freeCmdLines(cmd);
    break;
  }
  else{
       execute(cmd);
      }
      update_history(line);
      freeCmdLines(cmd);
      }
 
for(int j=0;j<10;j++){
  if(!history[j])
  free(history[j]);
}
return 0;
}