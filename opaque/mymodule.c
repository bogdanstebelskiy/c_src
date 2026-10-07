#include "mymodule_internal.h"
#include <stdlib.h>

struct MyModule *mymodule_create(const int val) {
  struct MyModule *mod = malloc(sizeof(struct MyModule));

  if (mod != NULL) {
    mod->value = val;
    mod->internal_counter = 0;
    mod->private_buffer = NULL;
  }

  return mod;
}

void mymodule_destroy(struct MyModule *const mod) {
  free(mod->private_buffer);
  free(mod);
}

int mymodule_get_value(struct MyModule *const mod) { return mod->value; }

void mymodule_set_value(struct MyModule *const mod, const int val) {
  mod->value = val;
  mod->internal_counter++;
}
