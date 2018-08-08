#ifndef OBJECT_H
#define OBJECT_H

#include "vm.h"

int object_add_prop(MicroVM * vm, var_t * o, var_t * v, const char *name);
var_t * object_get_prop(MicroVM * vm, var_t * o, int hash);
var_t object_get_length(MicroVM * vm, int argc, var_t *v);
object_t * object_create(MicroVM * vm);
var_t native_object(MicroVM * vm, int argc, var_t * v);

#endif // OBJECT_H
