#ifndef MYMODULE_INTERNAL_H
#define MYMODULE_INTERNAL_H

#include "mymodule.h"

struct MyModule {
  int value;
  int internal_counter;
  char *private_buffer;
};

#endif
