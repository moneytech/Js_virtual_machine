// Copyright (c) 2013 Zhe Wang
// wzedmund@gmail.com

// version 1.0.3
// date 17/02/2015

//Comment date:  2018/8/8
//function: 虚拟机实现 virtual machine implementation

#include "vm.h"
#include "instruction.h"
#include "parser.h"
#include "nativetable.h"
#include "object.h"

var_t var_undefined;
var_t var_null;
var_t var_false;
var_t var_true;

void list_init(plist_t * list)
{
    *list = NULL;
}

plist_t list_create(MicroVM *vm, var_t val, uint32_t hash, uint8_t index)
{
    plist_t pNewNode = (plist_t)heap_alloc(vm->heap, sizeof(list_t));
    if (NULL != pNewNode)
    {
        pNewNode->data = val;
        pNewNode->hash = hash;
        pNewNode->index = index;
        pNewNode->next_node = NULL;
    }
    return pNewNode;
}

var_t * list_append(MicroVM *vm, plist_t *list, var_t val, uint32_t hash)
{
    plist_t pNode = NULL;
    plist_t pNewNode = NULL;
    uint8_t index = 0;
    if ((*list) == NULL)
    {
        *list = list_create(vm, val, hash, index);
        pNode = *list;
        return &pNode->data;
    }
    else
    {
        pNode = *list;
        index++;
        while (NULL != pNode->next_node)
        {
            pNode = pNode->next_node;
            index++;
        }
        pNewNode = list_create(vm, val, hash, index);
        pNode->next_node = pNewNode;
        return &pNewNode->data;
    }
}

plist_t list_index_find(plist_t list, uint8_t index)
{
    plist_t pNode = list;
    while (NULL != pNode)
    {
        if (pNode->index == index)
        {
            return pNode;
        }
        else
        {
            pNode = pNode->next_node;
        }
    }
    return NULL;
}

plist_t list_hash_find(plist_t list, uint32_t hash)
{
    plist_t pNode = list;
    while (NULL != pNode)
    {
        if (pNode->hash == hash)
        {
            return pNode;
        }
        else
        {
            pNode = pNode->next_node;
        }
    }
    return NULL;
}

void list_remove(plist_t* pHead, plist_t pos)
{
    plist_t pCurNode = *pHead;
    plist_t pPerNode = *pHead;
    if (NULL == pos)
        return;

    while ((pCurNode != pos) && (pCurNode != NULL))
    {
        pPerNode = pCurNode;
        pCurNode = pCurNode->next_node;
    }
    pPerNode->next_node = pPerNode->next_node->next_node;
    free(pCurNode);
    pCurNode = NULL;
}

void vm_mk_native_func(MicroVM * vm, var_t * var, uint8_t argc, NativeMethod method)
{
    native_t * func = (native_t*)heap_alloc(vm->heap, sizeof(native_t));
    func->arg_cnt = argc;
    func->func = method;
    var->val.addr = func;
    var->type = T_NATIVE_FUNCTION;
}

var_t * vm_check_var(var_t * v)
{
    if(v->type == T_VAR)
        return (var_t*)v->val.addr;
    else
        return v;
}

void vm_mk_func(MicroVM * vm, var_t * var)
{
    var->type = T_FUNCTION;
    var->val.addr = heap_alloc(vm->heap, sizeof(function_t));
}

void vm_mk_str(MicroVM * vm, var_t * var, const char * str)
{
    int size = strlen(str);
    if(size < 0)
        return;
    var->type = T_STRING;
    var->val.addr = heap_alloc(vm->heap, size);
    memcpy(var->val.addr, str, size);
}

void vm_mk_native_str(MicroVM * vm, var_t * var, const char * str)
{
    int size = strlen(str);
    if(size < 0)
        return;
    var->type = T_NATIVE_STRING;
    var->val.addr = heap_alloc(vm->heap, size);
    memcpy(var->val.addr, str, size);
}

void vm_mk_type(var_t * var, int type)
{
    var->type = type;
}

void vm_callback(MicroVM * vm, var_t * v, uint8_t argc, var_t * args)
{
    if(v->val.addr == NULL)
        return;

    if(vm_is_var_native(v))
    {
        native_t * native = (native_t*)v->val.addr;
        NativeMethod method = native->func;
        method(vm, argc, args);
    }
    else if(vm_is_var_function(v))
    {
        function_t * func = (function_t*)v->val.addr;
        if(func == NULL)
            return;

        MicroVM local_vm;
        memcpy(&local_vm, vm, sizeof(MicroVM));
        local_vm.pc = local_vm.pc_base + func->code_index;
        int i;
        for(i = 0; i < argc; i++)
        {
            var_t * local_param = (var_t*)list_index_find(func->var_list, i);
            if(local_param)
                *local_param = *(args + i);
        }
        vm_run(&local_vm, 1);
    }
}

var_t * vm_get_var(MicroVM * vm, uint16_t id1)
{
    list_t *list = list_index_find(vm->var_list, id1);
    if(list == NULL)
        return NULL;
    return &list->data;
}

var_t * vm_get_local_var(MicroVM * vm, uint8_t id1, uint8_t id2)
{
    list_t *list = list_index_find(vm->func_list, id1);
    if(list)
    {
        function_t *func = (function_t*)list->data.val.addr;
        list = list_index_find(func->var_list, id2);
        if(list == NULL)
            return NULL;
        return &list->data;
    }
    return NULL;
}

void vm_op_load_native(MicroVM * vm, uint16_t id1)
{
    list_t *list = list_index_find(vm->native_func_list, id1);
    vm->sp++;
    if(list)
        *vm->sp = list->data;
    else
        *vm->sp = var_undefined;
}

void vm_op_load_func(MicroVM * vm, uint16_t id1)
{
    list_t *list = list_index_find(vm->func_list, id1);
    vm->sp++;
    if(list)
        *vm->sp = list->data;
    else
        *vm->sp = var_undefined;
}

void vm_op_load(MicroVM * vm, uint16_t id1)
{
    var_t * v = vm_get_var(vm, id1);
    vm->sp++;
    if(v)
    {
        vm->sp->val.addr = v;
        vm->sp->type = T_VAR;
    }
    else
        *vm->sp = var_undefined;
}

void vm_op_load_local(MicroVM * vm, uint8_t id1, uint8_t id2)
{
    var_t * v = vm_get_local_var(vm, id1, id2);
    vm->sp++;
    if(v)
    {
        vm->sp->val.addr = v;
        vm->sp->type = T_VAR;
    }
    else
        *vm->sp = var_undefined;
}

void vm_op_add(var_t * op1, var_t * op2, var_t * res)
{
    switch (op1->type)
    {
        case T_NUMBER:
        {
            if(op2->type == T_NUMBER)
            {
                res->type = T_NUMBER;
                res->val.fval = op1->val.fval + op2->val.fval;
            }
            else
                res->type = T_UNDEFINED;
            break;
        }
        default: res->type = T_UNDEFINED;break;
    }
}

void vm_op_sub(var_t * op1, var_t * op2, var_t * res)
{
    if(op1->type == T_NUMBER && op2->type == T_NUMBER)
    {
        res->type = T_NUMBER;
        res->val.fval = op1->val.fval - op2->val.fval;
    }
    else
        res->type = T_UNDEFINED;
}

void vm_op_mul(var_t * op1, var_t * op2, var_t * res)
{
    if(op1->type == T_NUMBER && op2->type == T_NUMBER)
    {
        res->type = T_NUMBER;
        res->val.fval = op1->val.fval * op2->val.fval;
    }
    else
        res->type = T_UNDEFINED;
}

void vm_op_div(var_t * op1, var_t * op2, var_t * res)
{
    if(op1->type == T_NUMBER && op2->type == T_NUMBER)
    {
        res->type = T_NUMBER;
        res->val.fval = op1->val.fval / op2->val.fval;
    }
    else
        res->type = T_UNDEFINED;
}

void vm_op_or(var_t * op1, var_t * op2, var_t * res)
{
    int32_t v1, v2;
    if(op1->type == T_NUMBER && op2->type == T_NUMBER)
    {
        res->type = T_NUMBER;
        v1 = op1->val.fval;
        v2 = op2->val.fval;
        res->val.fval =  v1 | v2;
    }
    else
        res->type = T_UNDEFINED;
}

void vm_op_xor(var_t * op1, var_t * op2, var_t * res)
{
    int32_t v1, v2;
    if(op1->type == T_NUMBER && op2->type == T_NUMBER)
    {
        res->type = T_NUMBER;
        v1 = op1->val.fval;
        v2 = op2->val.fval;
        res->val.fval =  v1 ^ v2;
    }
    else
        res->type = T_UNDEFINED;
}

void vm_op_and(var_t * op1, var_t * op2, var_t * res)
{
    int32_t v1, v2;
    if(op1->type == T_NUMBER && op2->type == T_NUMBER)
    {
        res->type = T_NUMBER;
        v1 = op1->val.fval;
        v2 = op2->val.fval;
        res->val.fval =  v1 & v2;
    }
    else
        res->type = T_UNDEFINED;
}

void vm_op_shift_l(var_t * op1, var_t * op2, var_t * res)
{
    int32_t v1, v2;
    if(op1->type == T_NUMBER && op2->type == T_NUMBER)
    {
        res->type = T_NUMBER;
        v1 = op1->val.fval;
        v2 = op2->val.fval;
        res->val.fval =  v1 << v2;
    }
    else
        res->type = T_UNDEFINED;
}

void vm_op_mod(var_t * op1, var_t * op2, var_t * res)
{
    int32_t v1, v2;
    if(op1->type == T_NUMBER && op2->type == T_NUMBER)
    {
        res->type = T_NUMBER;
        v1 = op1->val.fval;
        v2 = op2->val.fval;
        res->val.fval =  v1 % v2;
    }
    else
        res->type = T_UNDEFINED;
}

void vm_op_shift_r(var_t * op1, var_t * op2, var_t * res)
{
    int32_t v1, v2;
    if(op1->type == T_NUMBER && op2->type == T_NUMBER)
    {
        res->type = T_NUMBER;
        v1 = op1->val.fval;
        v2 = op2->val.fval;
        res->val.fval =  v1 >> v2;
    }
    else
        res->type = T_UNDEFINED;
}

void vm_op_loadstr(MicroVM *vm, uint8_t id1, var_t * res)
{
    list_t *list;
    list = list_index_find(vm->str_list, id1);
    if(list == NULL)
        vm_mk_type(res, T_UNDEFINED);
    else
        *res = list->data;
}

void vm_op_not(MicroVM *vm)
{
    var_t * op1 = vm_check_var(vm->sp);
    vm->sp->type=T_BOOLEAN;
    vm->sp->val.fval = !op1->val.fval;
}

void vm_op_neg(MicroVM *vm)
{
    var_t * op1 = vm_check_var(vm->sp);
    vm->sp->type=T_BOOLEAN;
    vm->sp->val.fval = -op1->val.fval;
}

void vm_op_inc(MicroVM *vm)
{
    var_t * op1 = vm_check_var(vm->sp);
    if(vm_is_var_number(op1))
        op1->val.fval++;
    vm->sp--;
}

void vm_op_dec(MicroVM *vm)
{
    var_t * op1 = vm_check_var(vm->sp);
    if(vm_is_var_number(op1))
        op1->val.fval--;
    vm->sp--;
}

void vm_op_arthmetic(MicroVM *vm, uint8_t sign)
{
    --vm->sp;
    var_t *op1 = vm_check_var(vm->sp);
    var_t *op2 = vm_check_var(vm->sp + 1);
    int32_t oprand1 = op1->val.fval;
    int32_t oprand2 = op2->val.fval;
    switch (sign) {
        case Add:     vm->sp->val.fval=op1->val.fval + op2->val.fval;break;
        case Sub:     vm->sp->val.fval=op1->val.fval - op2->val.fval;break;
        case Mul:     vm->sp->val.fval=op1->val.fval * op2->val.fval;break;
        case Div:     vm->sp->val.fval=op1->val.fval / op2->val.fval;break;
        case Mod:     vm->sp->val.fval=oprand1 % oprand2;break;
        case Or:      vm->sp->val.fval=oprand1 | oprand2;break;
        case Xor:     vm->sp->val.fval=oprand1 ^ oprand2;break;
        case And:     vm->sp->val.fval=oprand1 & oprand2;break;
        case ShiftL:  vm->sp->val.ival=oprand1 << oprand2;break;
        case ShiftR:  vm->sp->val.ival=oprand1 >> oprand2;break;
        default:      break;
    }
    vm->sp->type=T_NUMBER;
}

void vm_op_compare(MicroVM *vm, uint8_t sign)
{
    --vm->sp;
    var_t * op1 = vm_check_var(vm->sp);
    var_t * op2 = vm_check_var(vm->sp + 1);
    switch (sign) {
        case Eq:
        {
            if( (op1->type == T_STRING || op1->type == T_NATIVE_STRING) &&
                (op2->type == T_STRING || op2->type == T_NATIVE_STRING))
            {
               string_t * str1 = (string_t *)op1->val.addr;
               string_t * str2 = (string_t *)op2->val.addr;

               if(str1->size != str2->size)
                   vm->sp->val.ival = 0;
               if( strncmp(str1->buf, str2->buf, str1->size) == 0)
                   vm->sp->val.ival = 1;
               else
                   vm->sp->val.ival = 0;
            }
            else
            {
                vm->sp->val.ival=(op1->val.fval==op2->val.fval);
            }
        }break;
        case NotEq: vm->sp->val.ival=(op1->val.fval!=op2->val.fval);break;
        case Gt:    vm->sp->val.ival=(op1->val.fval> op2->val.fval);break;
        case GtEq:  vm->sp->val.ival=(op1->val.fval>=op2->val.fval);break;
        case Lt:    vm->sp->val.ival=(op1->val.fval< op2->val.fval);break;
        case LtEq:  vm->sp->val.ival=(op1->val.fval<=op2->val.fval);break;
        default:    break;
    }
    vm->sp->type=T_BOOLEAN;
}

void vm_mk_var(var_t * sv, var_t * tv)
{
    sv->type = T_VAR;
    sv->val.addr = tv;
}

void vm_op_load_array(MicroVM *vm)
{
    var_t * op1 = vm_check_var(vm->sp - 1);
    var_t * op2 = vm_check_var(vm->sp);
    vm->sp--;
    if(vm_is_var_array(op1) && vm_is_var_integer(op2))
    {
        array_t * obj = (array_t*)op1->val.addr;
        vm_mk_var(vm->sp, obj->get_prop(vm, op1, op2->val.fval));
    }
    else
        *vm->sp = var_undefined;
}

void vm_op_load_prop(MicroVM *vm, uint32_t hash)
{
    var_t * v = vm_check_var(vm->sp);
    if(vm_is_var_object(v) || vm_is_var_array(v))
    {
        object_t * obj = (object_t*)v->val.addr;
        plist_t plist = list_hash_find(obj->var_list, hash);
        if(plist)
        {
            if(vm_is_var_native(&plist->data))
            {
                vm->sp++;
                vm_mk_var(vm->sp, &plist->data);
            }
            else
                vm_mk_var(vm->sp, &plist->data);
        }
        else
        {
            vm_mk_var(vm->sp, list_append(vm, &obj->var_list, var_undefined, hash));
            obj->var_cnt++;
        }
    }
    else
        *vm->sp = var_undefined;
}

void vm_op_store(MicroVM *vm, uint8_t sign)
{
    var_t * rhs = vm_check_var(vm->sp--);
    var_t * lhs = vm_check_var(vm->sp--);

    if (lhs && rhs)
    {
        switch (sign) {
        case Store:         *lhs = *rhs;break;
        case StoreAdd:      vm_op_add(lhs, rhs, lhs);break;
        case StoreSub:      vm_op_sub(lhs, rhs, lhs);break;
        case StoreMul:      vm_op_mul(lhs, rhs, lhs);break;
        case StoreDiv:      vm_op_div(lhs, rhs, lhs);break;
        case StoreMod:      vm_op_mod(lhs, rhs, lhs);break;
        case StoreOr:       vm_op_or(lhs, rhs, lhs);break;
        case StoreXor:      vm_op_xor(lhs, rhs, lhs);break;
        case StoreAnd:      vm_op_and(lhs, rhs, lhs);break;
        case StoreLShift:   vm_op_shift_l(lhs, rhs, lhs);break;
        case StoreRShift:   vm_op_shift_r(lhs, rhs, lhs);break;
        default:
            break;
        }

    }
}

uint8_t * vm_op_jump(MicroVM *vm, uint8_t opcode, uint8_t *pc)
{
    var_t * op1 = vm_check_var(vm->sp);
    var_t * op2 = vm_check_var(vm->sp-1);
    if( opcode != Jump && opcode != SJump)
        if( !(vm_is_var_boolean(op1) || vm_is_var_number(op1)) )
            return pc+1;

    if((opcode >= SJumpIntEq && opcode <= SJumpIntLtEq) ||
       (opcode >= JumpIntEq && opcode <= JumpIntLtEq))
        if(!(vm_is_var_boolean(op2) || vm_is_var_number(op2)))
            return pc+1;

    switch (opcode) {
    case Jump:          return vm->pc_base+(*(uint32_t*)(pc));
    case JumpNotZero:   vm->sp--;if(op1->val.fval != 0) return vm->pc_base+(*(uint32_t*)pc) - 1;else return pc+4;
    case JumpZero:      vm->sp--;if(op1->val.fval == 0) return vm->pc_base+(*(uint32_t*)pc) - 1;else return pc+4;
    case JumpIntEq:     vm->sp-=2;if(op2->val.fval == op1->val.fval) return vm->pc_base+(*(uint32_t*)pc) - 1;else return pc+4;
    case JumpIntNotEq:  vm->sp-=2;if(op2->val.fval != op1->val.fval) return vm->pc_base+(*(uint32_t*)pc) - 1;else return pc+4;
    case JumpIntGt:     vm->sp-=2;if(op2->val.fval >  op1->val.fval) return vm->pc_base+(*(uint32_t*)pc) - 1;else return pc+4;
    case JumpIntGtEq:   vm->sp-=2;if(op2->val.fval >= op1->val.fval) return vm->pc_base+(*(uint32_t*)pc) - 1;else return pc+4;
    case JumpIntLt:     vm->sp-=2;if(op2->val.fval <  op1->val.fval) return vm->pc_base+(*(uint32_t*)pc) - 1;else return pc+4;
    case JumpIntLtEq:   vm->sp-=2;if(op2->val.fval <= op1->val.fval) return vm->pc_base+(*(uint32_t*)pc) - 1;else return pc+4;

    case SJump:         return pc+(*(int8_t*)(pc));
    case SJumpNotZero:  vm->sp--;if(op1->val.fval != 0) return pc+(*(int8_t*)(pc));else return pc+1;
    case SJumpZero:     vm->sp--;if(op1->val.fval == 0) return pc+(*(int8_t*)(pc));else return pc+1;
    case SJumpIntEq:    vm->sp-=2;if(op2->val.fval == op1->val.fval) pc=vm->pc_base+(*(int8_t*)(pc+1));else return pc+1;
    case SJumpIntNotEq: vm->sp-=2;if(op2->val.fval != op1->val.fval) pc=vm->pc_base+(*(int8_t*)(pc+1));else return pc+1;
    case SJumpIntGt:    vm->sp-=2;if(op2->val.fval >  op1->val.fval) pc=vm->pc_base+(*(int8_t*)(pc+1));else return pc+1;
    case SJumpIntGtEq:  vm->sp-=2;if(op2->val.fval >= op1->val.fval) pc=vm->pc_base+(*(int8_t*)(pc+1));else return pc+1;
    case SJumpIntLt:    vm->sp-=2;if(op2->val.fval <  op1->val.fval) pc=vm->pc_base+(*(int8_t*)(pc+1));else return pc+1;
    case SJumpIntLtEq:  vm->sp-=2;if(op2->val.fval <= op1->val.fval) pc=vm->pc_base+(*(int8_t*)(pc+1));else return pc+1;
    default:
        return 0;
    }
    return 0;
}

uint8_t vm_is_var_number(var_t * v)
{
    if(v->type == T_NUMBER)
        return 1;
    return 0;
}

uint8_t vm_is_var_integer(var_t *v)
{
    if(v->type == T_NUMBER)
    {
        int32_t i = v->val.fval;
        if( (v->val.fval - i) == 0)
            return 1;
        return 0;
    }
    return 0;
}

uint8_t vm_is_var_string(var_t * v)
{
    if((v->type == T_STRING) | (v->type == T_NATIVE_STRING))
        return 1;
    return 0;
}

uint8_t vm_is_var_function(var_t * v)
{
    if(v->type == T_FUNCTION)
        return 1;
    return 0;
}

uint8_t vm_is_var_native(var_t * v)
{
    if(v->type == T_NATIVE_FUNCTION)
        return 1;
    return 0;
}

uint8_t vm_is_var_boolean(var_t * v)
{
    if(v->type == T_BOOLEAN)
        return 1;
    return 0;
}

uint8_t vm_is_var_object(var_t * v)
{
    if(v->type == T_OBJECT)
        return 1;
    return 0;
}

uint8_t vm_is_var_array(var_t *v)
{
    if(v->type == T_ARRAY)
        return 1;
    return 0;
}

uint8_t vm_is_var_undefined(var_t * v)
{
    if(v->type == T_UNDEFINED)
        return 1;
    return 0;
}

uint8_t vm_is_var(var_t * v)
{
    if(v->type == T_VAR)
        return 1;
    return 0;
}

int vm_opcode_parse(MicroVM *vm, const uint8_t opcode, uint8_t * pc) //print the compiling information
{
    printf("[sp: %lx, pc: %lx] ", (intptr_t)vm->sp, (intptr_t)pc);
    switch(opcode)
    {
    case Nop:       printf("Nop\n");return 1;
    case Stop:      printf("Stop\n");return 1;
    //load
    case LoadVar:   printf("LoadVar, %d, %d\n", *pc, *(pc+1));return 3;
    case LoadLocalVar:   printf("LoadLocalVar, %d, %d\n", *pc, *(pc+1));return 3;
    case LoadFunc:  printf("LoadFunc, %d\n", *pc);return 3;
    case LoadNative:  printf("LoadNative, %d\n", *pc);return 3;
    case LoadNull:  printf("LoadNull\n");return 1;
    case LoadTrue:  printf("LoadTrue\n");return 1;
    case LoadFalse: printf("LoadFalse\n");return 1;
    case LoadNum:   printf("LoadNum, %f\n", *(float*)pc);return 5;
    case LoadStr:   printf("LoadStr, %d\n", *(uint16_t*)pc);return 3;
    case LoadProp:  printf("LoadProp, %d\n", *(uint32_t*)pc);return 5;
    case LoadArray:  printf("LoadArray\n");return 1;
    case LoadUndef:  printf("LoadUndef\n");return 1;
    //store
    case Store:     printf("Store\n");return 1;
    case StoreAdd:  printf("StoreAdd\n");return 1;
    case StoreSub:  printf("StoreSub\n");return 1;
    case StoreMul:  printf("StoreMul\n");return 1;
    case StoreDiv:  printf("StoreDiv\n");return 1;
    case StoreMod:  printf("StoreMod\n");return 1;
    case StoreAnd:  printf("StoreAnd\n");return 1;
    case StoreOr:   printf("StoreOr\n");return 1;
    case StoreXor:  printf("StoreXor\n");return 1;
    case StoreLShift:printf("StoreLShift\n");return 1;
    case StoreRShift:printf("StoreRShift\n");return 1;
    //compare
    case Eq:        printf("Eq\n");return 1;
    case NotEq:     printf("NotEq\n");return 1;
    case Gt:        printf("Gt\n");return 1;
    case GtEq:      printf("GtEq\n");return 1;
    case Lt:        printf("Lt\n");return 1;
    case LtEq:      printf("LtEq\n");return 1;
    //arithmetic
    case Mul:       printf("Mul\n");return 1;
    case Div:       printf("Div\n");return 1;
    case Mod:       printf("Mod\n");return 1;
    case Add:       printf("Add\n");return 1;
    case Sub:       printf("Sub\n");return 1;
    case Or:        printf("Or\n");return 1;
    case Xor:       printf("Xor\n");return 1;
    case And:       printf("And\n");return 1;
    case Not:       printf("Not\n");return 1;
    case Neg:       printf("Neg\n");return 1;
    case ShiftL:    printf("ShiftL\n");return 1;
    case ShiftR:    printf("ShiftR\n");return 1;
    case Inc:       printf("Inc\n");return 1;
    case Dec:       printf("Dec\n");return 1;
    //stack operation
    case Pop:       printf("Pop\n");return 1;
    case Dup:       printf("Dup\n");return 1;
    case Sdn:       printf("Sdn\n");return 1;
    case Sup:       printf("Sup\n");return 1;
    case Swap:      printf("Swap\n");return 1;
    //jump
    case Jump:          printf("Jump, %x\n", *(uint32_t*)pc);return 5;
    case JumpNotZero:   printf("JumpNotZero, %x\n", *(uint32_t*)pc);return 5;
    case JumpZero:      printf("JumpZero, %x\n", *(uint32_t*)pc);return 5;
    case JumpIntEq:     printf("JumpIntEq, %x\n", *(uint32_t*)pc);return 5;
    case JumpIntNotEq:  printf("JumpIntNotEq, %x\n", *(uint32_t*)pc);return 5;
    case JumpIntGt:     printf("JumpIntGt, %x\n", *(uint32_t*)pc);return 5;
    case JumpIntGtEq:   printf("JumpIntGtEq, %x\n", *(uint32_t*)pc);return 5;
    case JumpIntLt:     printf("JumpIntLt, %x\n", *(uint32_t*)pc);return 5;
    case JumpIntLtEq:   printf("JumpIntLtEq, %x\n", *(uint32_t*)pc);return 5;

    case SJump:         printf("SJump, %d\n", *(int8_t*)pc);return 2;
    case SJumpNotZero:  printf("SJumpNotZero, %d\n", *(int8_t*)pc);return 2;
    case SJumpZero:     printf("SJumpZero, %d\n", *(int8_t*)pc);return 2;
    case SJumpIntEq:    printf("SJumpIntEq, %d\n", *(int8_t*)pc);return 2;
    case SJumpIntNotEq: printf("SJumpIntNotEq, %d\n", *(int8_t*)pc);return 2;
    case SJumpIntGt:    printf("SJumpIntGt, %d\n", *(int8_t*)pc);return 2;
    case SJumpIntGtEq:  printf("SJumpIntGtEq, %d\n", *(int8_t*)pc);return 2;
    case SJumpIntLt:    printf("SJumpIntLt, %d\n", *(int8_t*)pc);return 2;
    case SJumpIntLtEq:  printf("SJumpIntLtEq, %d\n", *(int8_t*)pc);return 2;
    case CallProp:      printf("CallProp, %d\n", *(uint8_t*)pc);return 2;
    case Call:          printf("Call, %d\n", *(uint8_t*)pc);return 2;
    case Return:        printf("Return\n");return 1;
    case ReturnVoid:    printf("ReturnVoid\n");return 1;
    case New:           printf("New\n");return 1;
    default:            printf("Unknown\n");return 1;
    }
}

error_t vm_run(MicroVM* vm, uint8_t stop)
{
    uint8_t opcode;
    int32_t error = vm_err_ok;
    uint8_t * pc = vm->pc;
	while(1)
	{ 	
        //todo:check stack overflow
        opcode = *pc++;
        vm_opcode_parse(vm, opcode, pc);
        switch(opcode)
	 	{
        case Nop:break;
        case Stop:goto VM_END;
        //load
        case LoadVar:       vm_op_load(vm, *(uint16_t*)(pc+1));pc+=2;break;
        case LoadLocalVar:  vm_op_load_local(vm, *(uint8_t*)(pc), *(uint8_t*)(pc+1));pc+=2;break;
        case LoadFunc:      vm_op_load_func(vm, *(uint16_t*)(pc));pc+=2;break;
        case LoadNative:    vm_op_load_native(vm, *(uint16_t*)(pc));pc+=2;break;
        case LoadNull:  ++vm->sp;*vm->sp = var_null;break;
        case LoadTrue:  ++vm->sp;*vm->sp = var_true;break;
        case LoadFalse: ++vm->sp;*vm->sp = var_false;break;
        case LoadNum:   ++vm->sp;vm->sp->val.fval=*(float*)pc;pc+=4;vm->sp->type=T_NUMBER;break;
        case LoadStr:
            ++vm->sp;
            vm_op_loadstr(vm, *(uint16_t*)pc, vm->sp);
            pc+=2;
            break;
        case LoadProp:vm_op_load_prop(vm,*(uint32_t*)pc);pc+=4;break;
        case LoadArray:vm_op_load_array(vm);break;
        case LoadUndef:++vm->sp;*vm->sp = var_undefined;break;
        //store
        case Store:
        case StoreAdd:
        case StoreSub:
        case StoreMul:
        case StoreDiv:
        case StoreMod:
        case StoreAnd:
        case StoreOr:
        case StoreXor:
        case StoreLShift:
        case StoreRShift:   vm_op_store(vm, opcode);break;
        //compare
        case Eq:
        case NotEq:
        case Gt:
        case GtEq:
        case Lt:
        case LtEq:   vm_op_compare(vm, opcode);break;
        //arithmetic
        case Mul:
        case Div:
        case Mod:
        case Add:
        case Sub:
        case Or:
        case Xor:
        case And:
        case ShiftL:
        case ShiftR: vm_op_arthmetic(vm, opcode);break;
        case Not:    vm_op_not(vm);break;
        case Neg:    vm_op_neg(vm);break;
        case Inc:    vm_op_inc(vm);break;
        case Dec:    vm_op_dec(vm);break;
        //stack operation
        case Pop:   *(vm->sp-1) = *vm->sp;vm->sp--;break;
        case Dup:   *(vm->sp+1) = *vm->sp;vm->sp++;break;
        case Sdn:   vm->sp--;break;
        case Sup:   vm->sp++;break;
        case Swap:  break;
        //jump
        case Jump:
        case JumpNotZero:
        case JumpZero:
        case JumpIntEq:
        case JumpIntNotEq:
        case JumpIntGt:
        case JumpIntGtEq:
        case JumpIntLt:
        case JumpIntLtEq:

        case SJump:
        case SJumpNotZero:
        case SJumpZero:
        case SJumpIntEq:
        case SJumpIntNotEq:
        case SJumpIntGt:
        case SJumpIntGtEq:
        case SJumpIntLt:
        case SJumpIntLtEq:  pc = vm_op_jump(vm, opcode, pc);break;
        case CallProp:
        case Call:
        {
            uint8_t paramIndex;
            uint8_t argc = *(uint8_t*)pc;
            var_t * v = vm_check_var(vm->sp - argc);
            if(vm_is_var_function(v))
            {
                function_t * func = (function_t*)v->val.addr;

                if(argc != func->arg_cnt)
                {
                    pc = pc + 1;
                    vm->sp-=argc;
                    *vm->sp = var_undefined;
                }
                else
                {
                    vm->sp -= argc;
                    for(paramIndex = 0; paramIndex < argc; paramIndex++)
                    {
                        var_t * local_param = (var_t*)list_index_find(func->var_list, paramIndex);
                        if(local_param)
                            *local_param = *vm_check_var(vm->sp + paramIndex + 1);
                    }

                    vm->sp->val.addr = pc + 1;
                    pc = vm->pc_base + func->code_index;
                }
            }
            else if(vm_is_var_native(v))
            {
                native_t * native = (native_t*)v->val.addr;
                NativeMethod method = native->func;
                vm->sp -= argc;
                for(paramIndex = 0; paramIndex < argc; paramIndex++)
                {
                    var_t *local_sp = vm->sp + paramIndex;
                    *local_sp = *vm_check_var(local_sp + 1);
                }
                if(opcode == CallProp)
                {
                    vm->sp--;
                    *vm->sp = *vm_check_var(vm->sp);
                }
                *vm->sp = method(vm, argc, vm->sp);
                pc++;
            }
            else
            {
                *vm->sp = var_undefined;
                pc++;
            }

        }
        break;
        case Return:
            if(stop)
                goto VM_END;
            pc = (uint8_t*)(vm->sp-1)->val.addr;
            vm->sp--;
            *vm->sp = *(vm->sp+1);
            break;
        case ReturnVoid:
            if(stop)
                goto VM_END;
            pc = (uint8_t*)vm->sp->val.addr;
            vm->sp--;
            *vm->sp = var_undefined;
            break;
        case New:
            vm->sp++;
            vm->sp->val.addr = object_create(vm);
            vm->sp->type = T_OBJECT;
            break;
        default:
            goto VM_END;
	   	}
	}
    VM_END:
	return error;
}

extern NativeMethod* nativeTable[];
uint8_t code_buffer[1000];
extern uint32_t vm_system_ticks_count;
//customize your way to load executable file
int vm_load_file(MicroVM* vm, char *filename)
{
	FILE *file;
	size_t result;
	uint32_t lSize;
    char *buffer;

    var_undefined.type = T_UNDEFINED;
    var_false.type = T_BOOLEAN;
    var_false.val.fval = 0;
    var_true.type = T_BOOLEAN;
    var_true.val.fval = 1;
    var_null.type = T_NULL;
	
	//load opcodes
    file = fopen(filename,"rt");
	if (file == NULL) 
	{
		printf("can't open file\r\n");
        return vm_err_unable_to_open_file;
	}

	//get opcode size
	fseek (file , 0 , SEEK_END);
    lSize = ftell (file);
	rewind (file);
    buffer = (char*) malloc (sizeof(uint8_t)*lSize + 1);
    memset(buffer, 0, lSize);
    // read file into memory
    result = fread (buffer,1,lSize,file);
    printf("code size = %d bytes\n",lSize);
    if (!result)
    {
        fclose(file);
        return vm_err_failed_to_read_file;
    }
    buffer[lSize] = 0;
    fclose(file);

    vm->heap = malloc(sizeof(heap_t));
    heap_init(vm->heap, malloc(10*1000), 10*1000);
    list_init(&vm->var_list);
    list_init(&vm->func_list);
    list_init(&vm->str_list);
    list_init(&vm->native_func_list);

    parser_t * parser = (parser_t*)malloc(sizeof(parser_t));
    memset(parser, 0, sizeof(parser_t));
    parser->vm = vm;
    parser->input = buffer;
    parser->codes = code_buffer;
    parser->code_size = lSize;
    memset(parser->codes, 0 , 1000);
    native_init(parser);
    parser_run(parser);

    if(parser->error != vm_err_ok)
    {
        printf("vm error code = %d\r\n", parser->error);
        return parser->error;
    }

    vm->pc = parser->codes;
    vm->pc_base = parser->codes;
    vm->sp = malloc(sizeof(var_t) * 1000);
    vm->sp_base = vm->sp;


    printf("Compiled code area ===========\n");
    uint32_t i;
    uint8_t * pc = parser->codes;
    for(i = 0; i < parser->opcodeIndex;)
    {
        i += vm_opcode_parse(vm, parser->codes[i], pc + i + 1);
    }
    printf("Executed code area ===========\n");
    vm_run(vm, 0);

    for(;;)
    {
        for(i = 0;i < 100;++i);
        vm_system_ticks_count++;
        vm_timeout_proc(vm);
    }

    return vm_err_ok;
}



