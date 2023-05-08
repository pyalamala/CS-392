#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>


int main(int argc, char *argv[]) {

    /*
      TODO:
      Use fork() and pipe(), but NOT popen().

      Create a pipe through which a child process will send two integers to 
      the parent. The parent will add the integers and print the result to 
      the terminal. 

      OPTIONAL:
      Make a second child, which will do the addition. In this case, the parent
      waits for the children to terminate. (See Lab 6 or 7.)
    */
    pid_t pid;
    int fd[2];
    

    if (pipe(fd) < 0){
      fprintf(stderr, "Pipe Error. %s\n", strerror(errno));
    }
    if ((pid = fork()) < 0){
      fprintf(stderr, "Fork Error. %s\n", strerror(errno));
    }
    else if (pid == 0){
      close(fd[0]);
      int n = 6;
      int m = 7;
      write(fd[1], &n, sizeof(n));
      write(fd[1], &m, sizeof(m));
    }
    else{
      close(fd[1]);
      if ((pid = fork()) < 0){
        fprintf(stderr, "Fork Error. %s\n", strerror(errno));
      }
      else if (pid == 0){
        int i;
        int j;
        int written1;
        int written2;
        written1 = read(fd[0], &i, sizeof(i));
        written2 = read(fd[0], &j, sizeof(j));
        int k = i+j;
        printf("Ints: %d, %d: Sum: %d, Bytes Read: %d, %d\n", i, j, k, written1, written2);
      }
      else{
        waitpid(pid, NULL, 0);
      }
      waitpid(pid, NULL, 0);
    }

    
    return EXIT_SUCCESS;
}
