//date:  2018/8/8
//function: 垃圾回收 garbbage collection

#include "gc.h"

void gc_copy_var_list(MicroVM * vm, plist_t * target_list, plist_t var_list, uint16_t cnt)
{
    int i;
    for(i = 0; i < cnt; i++)
    {
        plist_t plist = list_index_find(var_list, i);
        list_append(vm, target_list, plist->data, plist->hash);
    }
}

void gc_copy_vars(MicroVM * vm)
{
    int i;
    plist_t var_list;
    for(i = 0; i < vm->var_cnt; i++)
    {
        plist_t plist = list_index_find(vm->var_list, i);
        switch (plist->data.type) {
            case T_STRING:
            {
                var_t* var = list_append(vm, &var_list, plist->data, plist->hash);
                var->val.addr = heap_alloc(vm->heap, sizeof(string_t));
                string_t * src = (string_t*)plist->data.val.addr;
                string_t * dst = (string_t*)var->val.addr;
                memcpy(dst, src, sizeof(string_t));
                dst->buf = heap_alloc(vm->heap, src->size);
                memcpy(dst->buf, src->buf, src->size);
            }break;
            case T_ARRAY:
            {
                var_t* var = list_append(vm, &var_list, plist->data, plist->hash);
                var->val.addr = heap_alloc(vm->heap, sizeof(array_t));
                array_t * src = (array_t*)plist->data.val.addr;
                array_t * dst = (array_t*)var->val.addr;
                memcpy(dst, src, sizeof(array_t));
                gc_copy_var_list(vm, dst->var_list, src->var_list, src->var_cnt);
            }break;
            case T_OBJECT:break;
            default:break;
        }
    }
    vm->var_list = var_list;
}

void gc_copy_functions(MicroVM * vm)
{
    int i;
    plist_t func_list;
    for(i = 0; i < vm->func_cnt; i++)
    {
        plist_t plist = list_index_find(vm->func_list, i);
        function_t * func = (function_t*)plist->data.val.addr;
        var_t * var = list_append(vm, &func_list, plist->data, plist->hash);
        var->val.addr = heap_alloc(vm->heap, sizeof(function_t));
        function_t * new_func = (function_t*)var->val.addr;
        gc_copy_var_list(vm, &new_func->var_list, func->var_list, func->var_cnt);
    }
    vm->func_list = func_list;
}

void gc_copy_strings(MicroVM * vm)
{
    int i;
    plist_t str_list;
    for(i = 0; i < vm->str_cnt; i++)
    {
        plist_t plist = list_index_find(vm->str_list, i);
        var_t * var = list_append(vm, &str_list, plist->data, plist->hash);
        string_t * str = (string_t*)plist->data.val.addr;
        var->val.addr = heap_alloc(vm->heap, sizeof(string_t));
        string_t * new_str = (string_t*)var->val.addr;
        memcpy(new_str, str, sizeof(string_t));
        new_str->buf = heap_alloc(vm->heap, str->size);
        memcpy(new_str->buf, str->buf, str->size);
    }
    vm->str_list = str_list;
}

void gc_copy_native_functions(MicroVM * vm)
{
    int i;
    plist_t native_func_list;
    for(i = 0; i < vm->native_func_cnt; i++)
    {
        plist_t plist = list_index_find(vm->native_func_list, i);
        var_t * var = list_append(vm, &native_func_list, plist->data, plist->hash);
        native_t * native = (native_t*)plist->data.val.addr;
        var = heap_alloc(vm->heap, sizeof(native_t));
        native_t * new_native = (native_t*)var->val.addr;
        new_native->arg_cnt = native->arg_cnt;
        new_native->func = native->func;
    }
    vm->native_func_list = native_func_list;
}

void gc_collect(MicroVM * vm)
{
    heap_t * copy = vm->copy;
    vm->copy = vm->heap;
    vm->heap = copy;

    gc_copy_functions(vm);
    gc_copy_strings(vm);
    gc_copy_native_functions(vm);
    gc_copy_vars(vm);
}
