#include "mymodule.h"

#include <stdio.h>

int main() {
  struct MyModule *mod = mymodule_create(42);
  printf("Initial value: %d\n", mymodule_get_value(mod));

  mymodule_set_value(mod, 100);
  printf("Setted value: %d\n", mymodule_get_value(mod));

  int val = mymodule_get_value(mod);
  printf("Got value: %d\n", val);

  mymodule_destroy(mod);
  printf("Destroyed: %d\n", mymodule_get_value(mod));
  return 0;
}
