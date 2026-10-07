#include "stdio.h"

struct s {
  char b;
  int a;
};

int main() {
  printf("%zu\n", sizeof(struct s));
  return 0;
}
