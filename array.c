//Comment date:  2018/8/8
//function: 数组实现 arry implementation file

#include "array.h"
#include "object.h"

var_t native_array_length(MicroVM * vm, int argc, var_t * v)
{
    if(!vm_is_var_array(v))
        return var_undefined;
    array_t * obj = (array_t*)v->val.addr;
    var_t cell;
    cell.type = T_NUMBER;
    cell.val.fval = obj->var_cnt - 1;
    return cell;
}

var_t * native_get_prop(MicroVM * vm, var_t * v, uint32_t key)
{
    if(!vm_is_var_array(v))
        return &var_undefined;

    array_t * obj = (array_t*)v->val.addr;

    if(key < obj->var_cnt)
    {
        plist_t plist = list_index_find(obj->var_list, key + 1);
        if(plist)
            return &plist->data;
        else
        {
            obj->var_cnt++;
            return list_append(vm, &obj->var_list, var_undefined, 0);
        }
    }
    else
        return &var_undefined;
}

var_t native_array(MicroVM * vm, int argc, var_t * v)
{
    v = v - 1;
    if(!vm_is_var_object(v))
        return var_undefined;
    array_t * obj = (array_t*)v->val.addr;

    var_t prop;
    vm_mk_native_func(vm, &prop, 0, native_array_length);
    object_add_prop(vm, v, &prop, "length");
    obj->get_prop = native_get_prop;
    vm_mk_type(v, T_ARRAY);
    return var_undefined;
}
