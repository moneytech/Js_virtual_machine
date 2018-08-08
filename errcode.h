// Copyright (c) 2013 Zhe Wang
// wzedmund@gmail.com

// version 1.0.3
// date 17/02/2015

#ifndef ERRCODE_H
#define ERRCODE_H

enum VM_ERR
{
    vm_err_ok = 0,
    vm_err_undefined_var,
    vm_err_redefined_var,
    vm_err_unable_to_open_file,
    vm_err_failed_to_read_file,

    vm_err_not_enough_memory,
};

#endif
