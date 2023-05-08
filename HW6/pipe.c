#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h>
#include<string.h>
#include<sys/errno.h>
#include <wait.h>


//Pranav Yalamala
//I pledge my honor that I have abided by the Stevens Honor System.

int main(int argc, char *argv[]){

    pid_t pid;
    int fd[2];

    if(argc != 2){
        fprintf(stderr, "Incorrect number of arguments.\n");
        return EXIT_FAILURE;
    }

    if(pipe(fd) < 0){
        fprintf(stderr, "Error: grep failed.\n");
        return EXIT_FAILURE;
    }
    if ((pid = fork()) < 0){
        fprintf(stderr, "Error: grep failed.\n");
        return EXIT_FAILURE;
    }
    else if (pid == 0){
        close(fd[0]);
        dup2(fd[1], 1);
        close(fd[1]);
        if(execlp("ls", "ls", "-l", argv[1], NULL) < 0){
            fprintf(stderr, "Error: grep failed.\n");
            return EXIT_FAILURE;
        }
    }
    else{
        close(fd[1]);
        int fd2[2];

        if(pipe(fd2) < 0){
            fprintf(stderr, "Error: grep failed.\n");
            return EXIT_FAILURE;
        }
        if((pid = fork()) < 0){
            fprintf(stderr, "Error: grep failed.\n");
            return EXIT_FAILURE;
        }
        else if (pid == 0){
            close(fd[1]);
            close(fd2[0]);
            if(dup2(fd[0], 0) < 0){
                fprintf(stderr, "Error: grep failed.\n");
                return EXIT_FAILURE;
            }
            if(dup2(fd2[1], 1) < 0){
                fprintf(stderr, "Error: grep failed.\n");
                return EXIT_FAILURE;
            }
            close(fd[0]);
            close(fd2[1]);
            if(execlp("grep", "grep", "^d", NULL) < 0){
                fprintf(stderr, "Error: grep failed.\n");
                return EXIT_FAILURE;
            }
        }
        else{
            waitpid(pid, NULL, 0);
            close(fd2[1]);
            char buf;
            int count = 0;
            while(read(fd2[0], &buf, 1) != 0){
                printf("%c", buf);
                if(buf == '\n'){
                    count += 1;
                }
            }
            printf("Num_lines: %d\n",count);
        }
        waitpid(pid, NULL, 0);
    }

    return EXIT_SUCCESS;


}