#ifndef VM_TIMEOUT_H
#define VM_TIMEOUT_H

#include "vm.h"

#define VM_TIMEOUT_MAX          10

typedef struct vm_timeout {
    uint8_t  repeat;
    uint16_t period;
    uint16_t count;
    var_t    handle;
} vm_timeout_t;

vm_timeout_t * vm_timeout_register(var_t * v, uint64_t msec, uint8_t repeat);
void vm_timeout_unregister(var_t * v);
void vm_timeout_proc(MicroVM * vm);


#endif // VM_TIMEOUT_H
