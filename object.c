#include "object.h"
#include "parser.h"

int object_add_prop(MicroVM * vm, var_t * o, var_t * v, const char *name)
{
    if(!vm_is_var_object(o))
        return 0;

    uint32_t hash = parser_hash(name);
    object_t *obj = (object_t*)o->val.addr;

    plist_t plist = list_hash_find(obj->var_list, hash);
    if(plist)
        return 1;

    list_append(vm, &obj->var_list, *v, hash);
    obj->var_cnt++;
    return 1;
}

var_t * object_get_prop(MicroVM * vm, var_t * o, int hash)
{
    if(!vm_is_var_object(o))
        return NULL;

    object_t *obj = (object_t*)o->val.addr;
    plist_t plist = list_hash_find(obj->var_list, hash);
    if(plist)
        return &plist->data;

    return NULL;
}

var_t object_get_length(MicroVM * vm, int argc, var_t *v)
{
    var_t var;
    if(vm_is_var_object(v))
    {
        object_t *obj = (object_t*)v->val.addr;
        var.val.fval = obj->var_cnt;
        var.type = T_OBJECT;
    }
    else
    {
        var.type = T_UNDEFINED;
    }
    return var;
}

object_t * object_create(MicroVM * vm)
{
    object_t * obj;
    obj = heap_alloc(vm->heap, sizeof(object_t));
    return obj;
}

var_t native_object_length(MicroVM * vm, int argc, var_t * v)
{
    if(!vm_is_var_object(v))
        return var_undefined;
    object_t * obj = (object_t*)v->val.addr;
    var_t cell;
    cell.type = T_NUMBER;
    cell.val.fval = obj->var_cnt - 1;
    return cell;
}

var_t native_object(MicroVM * vm, int argc, var_t * v)
{
    var_t var;
    vm_mk_type(&var, T_UNDEFINED);
    if(argc != 0)
        return var;
    v = v - 1;
    if(!vm_is_var_object(v))
        return var;

    object_t * obj = (object_t*)v->val.addr;

    var_t prop;
    vm_mk_native_func(vm, &prop, 0, native_object_length);
    object_add_prop(vm, v, &prop, "length");
    obj->get_prop = object_get_prop;
    return var;
}

