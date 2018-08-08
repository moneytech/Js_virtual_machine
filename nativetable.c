// Copyright (c) 2013 Zhe Wang
// wzedmund@gmail.com
// version 1.0.3
// date 17/02/2015

//Comment date:  2018/8/8
//function: 本地功能实现（如print） native function implementation


#include "nativetable.h"
#include "object.h"
#include "array.h"


var_t native_print(MicroVM * vm, int argc, var_t * v)
{
    (void)vm;
    var_t var;
    vm_mk_type(&var, T_UNDEFINED);
    if(argc != 1)
        return var;

    if(vm_is_var_boolean(v))
    {
        if(v->val.ival == 0)
            printf("false");
        else
            printf("true");
    }
    else if(vm_is_var_object(v))
    {
        printf("object");
    }
    else if(vm_is_var_undefined(v))
    {
        printf("undefined");
    }
    else if(vm_is_var_number(v))
    {
        printf("%f", v->val.fval);
    }
    else if(vm_is_var_string(v))
    {
        string_t * str = (string_t*)v->val.addr;
        int i;
        printf("\"");
        for(i = 0; i < str->size; i++)
            printf("%c", str->buf[i]);
        printf("\"");
    }
    printf("\r\n");
    return var;
}

var_t native_setInterval(MicroVM * vm, int argc, var_t * v)
{
    (void)vm;
    var_t var;
    vm_mk_type(&var, T_UNDEFINED);

    if(argc != 2)
        return var;
    if(vm_is_var_function(v) && vm_is_var_number(v + 1))
        vm_timeout_register(v, (v+1)->val.fval, 1);

    return var;
}

var_t native_setTimeout(MicroVM * vm, int argc, var_t * v)
{
    (void)vm;
    var_t var;
    vm_mk_type(&var, T_UNDEFINED);
    if(argc != 2)
        return var;
    if(vm_is_var_function(v) && vm_is_var_number(v + 1))
        vm_timeout_register(v, (v+1)->val.fval, 0);


    return var;
}

var_t native_clearTimeout(MicroVM * vm, int argc, var_t * v)
{
    (void)vm;
    var_t var;
    vm_mk_type(&var, T_UNDEFINED);
    if(argc != 1)
        return var;
    if(vm_is_var_function(v))
        vm_timeout_unregister(v);

    return var;
}

string_t str_boolean;
string_t str_object;
string_t str_undefined;
string_t str_number;
string_t str_string;

var_t native_typeof(MicroVM * vm, int argc, var_t * v)
{
    (void)vm;
    var_t var;
    vm_mk_type(&var, T_UNDEFINED);
    if(argc != 1)
        return var;

    if(vm_is_var_boolean(v))
    {
        var.type = T_STRING;
        var.val.addr = &str_boolean;
    }
    else if(vm_is_var_object(v))
    {
        var.type = T_STRING;
        var.val.addr = &str_object;
    }
    else if(vm_is_var_undefined(v))
    {
        var.type = T_STRING;
        var.val.addr = &str_undefined;
    }
    else if(vm_is_var_number(v))
    {
        var.type = T_STRING;
        var.val.addr = &str_number;
    }
    else if(vm_is_var_string(v))
    {
        var.type = T_STRING;
        var.val.addr = &str_string;
    }

    return var;
}

var_t native_require(MicroVM * vm, int argc, var_t * v)
{
    (void)vm;
    return var_undefined;
}

var_t native_gpio_write(MicroVM * vm, int argc, var_t * v)
{
    (void)vm;
    return var_undefined;
}

void native_init(parser_t *p)
{
    str_boolean.buf = "boolean";
    str_boolean.size = strlen("boolean");
    str_object.buf = "object";
    str_object.size = strlen("object");
    str_undefined.buf = "undefined";
    str_undefined.size = strlen("undefined");
    str_number.buf = "number";
    str_number.size = strlen("number");
    str_string.buf = "string";
    str_string.size = strlen("string");

    parser_native_put(p, "print", 1, native_print);
    parser_native_put(p, "Object", 0, native_object);
    parser_native_put(p, "Array", 0, native_array);
    parser_native_put(p, "setInterval", 0, native_setInterval);
    parser_native_put(p, "setTimeout", 0, native_setTimeout);
    parser_native_put(p, "clearTimeout", 0, native_clearTimeout);
    parser_native_put(p, "clearInterval", 0, native_clearTimeout);
    parser_native_put(p, "typeof", 1, native_typeof);
    parser_native_put(p, "require", 1, native_require);
    parser_native_put(p, "gpio_write", 1, native_gpio_write);
}
