#ifndef MYMODULE_H
#define MYMODULE_H

struct MyModule;

struct MyModule *mymodule_create(const int val);
void mymodule_destroy(struct MyModule *const mod);
int mymodule_get_value(struct MyModule *const mod);
void mymodule_set_value(struct MyModule *const mod, const int val);

#endif
