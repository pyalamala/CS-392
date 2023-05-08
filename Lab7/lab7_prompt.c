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

int main()
{
    /*
      TODO:
      1. Use getuid, getpwuid, getcwd to retrieve the username and home 
      directory of the current user
      2. Change the directory to the user's home directory
      3. Print the prompt as specified in homework 5 and wait for user input
    */

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


    if(signal(SIGINT, sig_int_handler) == SIG_ERR){
      fprintf(stderr, "Error: Cannot register signal handeler. %s.\n", strerror(errno));
    }
    if(signal(SIGCHLD, sig_child_handler) == SIG_ERR){
      fprintf(stderr, "Error: Cannot register signal handeler. %s.\n", strerror(errno));
    }

    while (run==1){
      printf("Msh:%s:%s>", user, getcwd(NULL, 0));

      char input[1000];
      char copy[1000];
      if((fgets(input, 1000, stdin)) < 0){
        fprintf(stderr, "Error: Failed to read from stdin. %s.\n", strerror(errno));
      }
      memcpy(copy, input, 1000);
      //printf("%s", input);

      char* token;
      char s[4] = " \n&";
      token = strtok(input, s);
      //printf("%s!", token);
      

      if(strcmp(token, "cd") == 0){
        char* arg = strtok(NULL, s);
        //printf("%s", token);

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
              fprintf(stderr, "Error: Cannot change directory to %s. %s.\n", home, strerror(errno));
            }
          }
          else{
            fprintf(stderr, "Error: Too many arguments to cd.\n");
          }
        }
      }
      else if(strcmp(token, "exit") == 0){ 
        char* arg = strtok(copy, " \n");
        arg = strtok(NULL, " \n");
        if(arg == NULL){
          run = 0;
        }
        else{
          fprintf(stderr, "Error: Too many arguments to exit. %s.\n", strerror(errno));
          return EXIT_FAILURE;
        }
        

      }
      else{
        char* args[1024*sizeof(char*)] = {};
        int num_args = 0;
        while(token != NULL){
          //printf("%s!", token);
          args[num_args] = token;
          num_args++;
          token = strtok(NULL, s);
      }
        // int i = 0;
        // while( i <= num_args){
        //   printf("Args[%d]= %s\n", i, args[i]);
        //   i++;
        // }

        //printf("%s\n", &copy[strlen(copy) -2]);
        if(copy[strlen(copy) -1] == '\n'){
          //printf("plz");
          copy[strlen(copy)-1] = '\0';
        }

        //printf("%s\n", &(copy[strlen(copy)-1]));

        if(copy[strlen(copy)-1] == '&'){
          //printf("I ran\n");
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
        //printf("Hi\n");
      }

      //printf("Current Directory: %s", getcwd(NULL, 0));
    }
    


    /* 
      TODO:
      1. Handle cd commands entered by the user. Check that a single argument 
      is provided. Ignore other commands, but don't exit. Change the current 
      directory according to the user's input.
      2. Handle the exit command. Ensure that it is followed by no arguments.

      ** If tokenizing is giving you trouble, allow the user to omit cd and
      consider the received string as the target directory. There should be 
      no directory named `exit'
    */
    
    return EXIT_SUCCESS;
}