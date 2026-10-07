#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[], char *envp[]) {
  int i = 0;
  const char *shell_env;

  for (int i = 0; envp[i]; i++) {
    if (strncmp(envp[i], "SHELL=", 6) == 0) {
      char *name = strrchr(envp[i], '/');

      if (name != NULL) {
        printf("%s\n", name + 1);
      }
    }
  }
}
