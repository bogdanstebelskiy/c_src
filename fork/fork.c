#include <stdio.h>
#include <stdlib.h>

int main() {
  pid_t pid;
  char *args[] = {"/bin/ls", "-l", "/", NULL};

  pid = fork();

  int *p = 0;
  return *p;

  if (pid < 0) {
    perror("fork failed");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    printf("Child process: Executing the 'ls' command...\n");
    execvp(args[0], args);

    perror("excevp failed");
    exit(EXIT_FAILURE);
  } else {
    wait(NULL);
    printf("Parent process: Child has finished execution. \n");
  }

  return 0;
}
