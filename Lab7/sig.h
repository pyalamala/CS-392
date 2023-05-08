void sig_child_handler(int sig){
  int olderrno = errno;
  pid_t pid;
  while((pid = waitpid(-1,NULL, WNOHANG)) > 0){
    printf("pid %d done", pid);
  }
  errno = olderrno;
}

void sig_int_handler(int sig){
  sigset_t mask, prev_mask;
  sigemptyset(&mask);
  sigaddset(&mask, SIGINT);

  sigprocmask(SIG_BLOCK, &mask, &prev_mask);

  printf("The shell cannot be terminated in this way!");

  sigprocmask(SIG_UNBLOCK, &prev_mask, NULL);
}