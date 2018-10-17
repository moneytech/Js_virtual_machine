// Copyright (c) 2013 Zhe Wang
// wzedmund@gmail.com

// version 1.0.3
// date 17/02/2015

#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#include "errcode.h"

// includes
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "heap.h"

enum VarTypes
{
    T_UNKNOWN = 0,
    T_VAR,
    T_NULL,
    T_NUMBER,
    T_BOOLEAN,
    T_FUNCTION,
    T_NATIVE_FUNCTION,
    T_ARRAY,
    T_OBJECT,
    T_STRING,
    T_BUFFER,
    T_NAN,
    T_UNDEFINED,
    T_NATIVE_STRING,
    T_FOREIGN_STRING,
};

typedef union
{
    int32_t ival;
    float  fval;
    void*   addr; //pionter of function to execute
}CELL;

typedef struct VAR
{
    enum VarTypes type;
    CELL val;
}var_t;

struct 	MICROVM;
typedef int32_t	 error_t;
typedef var_t (*NativeMethod)(struct MICROVM* vm, int argc, var_t * v);

typedef struct LIST
{
    var_t data;
    uint32_t hash;
    uint8_t index;
    struct LIST *next_node;
}list_t, *plist_t;

typedef struct NATIVE
{
    uint8_t arg_cnt;
    NativeMethod func;
}native_t;

typedef struct FUNCTION
{
    uint8_t var_cnt;   //varible count
    uint8_t arg_cnt;   //argument count
    plist_t var_list;
    uint16_t code_index;
}function_t;

typedef struct STRING
{
    uint16_t size;
    char* buf;
}string_t;

typedef struct OBJECT
{
    uint16_t var_cnt;
    plist_t var_list;
    var_t * (*get_prop)(struct MICROVM* vm, var_t * v, uint32_t key);
}object_t;

typedef object_t array_t;

//vm structure
typedef struct MICROVM
{
    uint8_t* pc;           //program counter
    uint8_t* pc_base;

    var_t* sp_base;
    var_t* sp;            //statck pointer
    uint8_t error;

    plist_t func_list;   //function list
    uint16_t func_cnt;

    plist_t native_func_list;  //native function list
    uint16_t native_func_cnt;

    uint16_t var_cnt;
    plist_t var_list;

    uint16_t str_cnt;
    plist_t str_list;

    uint16_t num_cnt;
    plist_t num_list;

    heap_t * heap;
    heap_t * copy;
}MicroVM;


extern CELL zeroCell;       // Cell constant for false, 0, 0.0f, and NULL
extern CELL oneCell;        // Cell constant for true, 1
extern CELL negOneCell;     // Cell constant for -1
#define nullCell  zeroCell  // Cell constant for NULL
#define falseCell zeroCell  // Cell constant for false
#define trueCell  oneCell   // Cell constant for true

//check if the native method is valid
int isNativeValid(int id1, int id2);

//load codes and initialize the vm
error_t vmStart(MicroVM* vm, char *filename);
//start running the vm
error_t vm_run(MicroVM* vm, uint8_t stop);
//initialize the vm
error_t vmInit(MicroVM* vm);
void vm_callback(MicroVM * vm, var_t * v, uint8_t argc, var_t * args);
//load codes
int vm_load_file(MicroVM* vm, char *filename);
var_t * vm_check_var(var_t * v);
void vm_mk_str(MicroVM * vm, var_t * var, const char * str);
void vm_mk_native_str(MicroVM * vm, var_t * var, const char * str);
void vm_mk_func(MicroVM * vm, var_t * var);
void vm_mk_native_func(MicroVM * vm, var_t * var, uint8_t argc, NativeMethod method);
void vm_mk_type(var_t * var, int type);
uint8_t vm_is_var_number(var_t * v);
uint8_t vm_is_var_integer(var_t * v);
uint8_t vm_is_var_string(var_t * v);
uint8_t vm_is_var_function(var_t * v);
uint8_t vm_is_var_native(var_t * v);
uint8_t vm_is_var_boolean(var_t * v);
uint8_t vm_is_var_undefined(var_t * v);
uint8_t vm_is_var_object(var_t * v);
uint8_t vm_is_var_array(var_t *v);
void vm_mk_var(var_t * sv, var_t * tv);

plist_t list_create(MicroVM *vm, var_t data, uint32_t hash, uint8_t index);
var_t * list_append(MicroVM *vm, plist_t *list, var_t data, uint32_t hash);
plist_t list_hash_find(plist_t list, uint32_t hash);
plist_t list_index_find(plist_t list, uint8_t index);

extern var_t var_undefined;
extern var_t var_null;
extern var_t var_false;
extern var_t var_true;

#endif
