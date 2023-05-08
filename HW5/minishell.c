#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h>
#include<sys/wait.h>
#include <pwd.h>
#include<string.h>
#include<sys/errno.h>
#include<signal.h>
#include<sys/signal.h>
#include "sig.h"

//Pranav Yalamla
//I pledge my honor that I have abided by the Stevens Honor System.

int main()
{

    uid_t uid;
    struct passwd *pwd;
    uid = getuid();
    if ((pwd = getpwuid(uid)) < 0){
      fprintf(stderr, "Error: Cannot get passwd entry. %s.\n", strerror(errno));
    }
    char* user;
    char* home;
    user = pwd->pw_name;
    home = pwd->pw_dir;
    int run = 1;
    char* cwd;

    if(signal(SIGINT, sig_int_handler) == SIG_ERR){
      fprintf(stderr, "Error: Cannot register signal handeler. %s.\n", strerror(errno));
    }
    if(signal(SIGCHLD, sig_child_handler) == SIG_ERR){
      fprintf(stderr, "Error: Cannot register signal handeler. %s.\n", strerror(errno));
    }

    //Loop while exit command not given
    while (run==1){
      cwd = getcwd(NULL, 0);
      printf("Msh:%s:%s>", user, cwd);
      free(cwd);
      char input[1000];
      char copy[1000];
      if((fgets(input, 1000, stdin)) < 0){
        fprintf(stderr, "Error: Failed to read from stdin. %s.\n", strerror(errno));
      }
      memcpy(copy, input, 1000);

      int ex_and = 1;

      
      if(input[strlen(input) - 2] == '&'){
        ex_and = 0;
      }

      char* token;
      char s[4] = " \n&";
      token = strtok(input, s);
      
      //Handles cd withotu exec()
      if(token == NULL){
        run = 1;
      }
      else if(strcmp(token, "cd") == 0){
        char* arg = strtok(NULL, s);
        if(arg == NULL){
          if((chdir(home)) < 0){
            fprintf(stderr, "Error: Cannot change directory to %s. %s.\n", home, strerror(errno));
          }
        }
        else if(strcmp(arg, "~") == 0){
          arg = strtok(NULL, s);
          if(arg == NULL){
            if((chdir(home)) < 0){
              fprintf(stderr, "Error: Cannot change directory to %s. %s.\n", home, strerror(errno));
            }
          }
          else{
            fprintf(stderr, "Error: Too many arguments to cd.\n");
          }
        }
        else{
          char *argv = strtok(NULL, s);
          if(argv == NULL){
            if((chdir(arg)) < 0){
              fprintf(stderr, "Error: Cannot change directory to %s. %s.\n", arg, strerror(errno));
            }
          }
          else{
            fprintf(stderr, "Error: Too many arguments to cd.\n");
          }
        }
      }
      //handles exit
      else if(strcmp(token, "exit") == 0){ 
        char* arg = strtok(copy, " \n");
        arg = strtok(NULL, " \n");
        if(arg == NULL){
          run = 0;
          killpg(getpid(), SIGTERM);
        }
        else if (ex_and == 0){
          fprintf(stderr, "Error: Too many arguments to exit.\n");
        }
        else{
          fprintf(stderr, "Error: Too many arguments to exit.\n");
        }
        

      }
      //if not cd or exit, calls exec
      else{
        char* args[1024*sizeof(char*)] = {};
        int num_args = 0;
        while(token != NULL){
          args[num_args] = token;
          num_args++;
          token = strtok(NULL, s);
      }

        if(copy[strlen(copy) -1] == '\n'){
          copy[strlen(copy)-1] = '\0';
        }

        //If there is an & at the end, exec's in the background
        if(copy[strlen(copy)-1] == '&'){
          pid_t pid = fork();
          if (pid < 0){
            fprintf(stderr, "Error: fork() failed. %s.\n", strerror(errno));
          }
          else if(pid == 0){
            char* temp = strtok(copy, "&");
            printf("pid: %d cmd: %s\n", getpid(), temp);
            if(execvp(args[0], args) < 0){
              fprintf(stderr, "Error: exec() failed. %s.\n", strerror(errno));
            }

          }
        }
        //if not & exec's in foreground and waits for task to finish before prompting again
        else{
          pid_t pid = fork();
          if (pid < 0){
            fprintf(stderr, "Error: fork() failed. %s.\n", strerror(errno));
          }
          else if(pid == 0){
            if(execvp(args[0], args) == -1){
              fprintf(stderr, "Error: exec() failed. %s.\n", strerror(errno));
              exit(0);
            }
          }
          else{
            if(waitpid(pid, NULL, 0) < 0){
              fprintf(stderr, "Error: wait() failed. %s.\n", strerror(errno));
            }
          }
        }
      }

    }
    
    return EXIT_SUCCESS;
}