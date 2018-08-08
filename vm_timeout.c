#include "vm_timeout.h"

vm_timeout_t vm_timeout_queue[VM_TIMEOUT_MAX];
uint32_t vm_system_ticks_count = 0;

vm_timeout_t * vm_timeout_register(var_t * v, uint64_t msec, uint8_t repeat)
{
    int i ;
    vm_timeout_t * timeout = NULL;
    for(i = 0; i < VM_TIMEOUT_MAX; i++)
    {
        timeout = &vm_timeout_queue[i];
        if(timeout->handle.type == T_UNKNOWN)
        {
            timeout->handle.type = v->type;
            timeout->handle.val = v->val;
            timeout->period = msec;
            timeout->repeat = repeat;
            break;
        }
    }
    return timeout;
}

void vm_timeout_unregister(var_t * v)
{
    int i ;
    vm_timeout_t * timeout = NULL;
    for(i = 0; i < VM_TIMEOUT_MAX; i++)
    {
        timeout = &vm_timeout_queue[i];
        if(timeout->handle.val.addr == v->val.addr && timeout->handle.type == v->type)
        {
            timeout->handle.type = 0;
            timeout->handle.val.addr = 0;
            break;
        }
    }
}

void vm_timeout_proc(MicroVM * vm)
{
    int i = 0;
    for (i = 0; i < VM_TIMEOUT_MAX; i++)
    {
        vm_timeout_t *timeout = &vm_timeout_queue[i];
        if (timeout->handle.type != T_UNKNOWN && vm_system_ticks_count - timeout->count >= timeout->period)
        {
            vm_callback(vm, &timeout->handle, 0, NULL);
            if (timeout->repeat)
            {
                timeout->count = vm_system_ticks_count;
            }
            else
            {
                vm_timeout_unregister(&timeout->handle);
            }
        }
    }
}
