/*
 * jsduino v1.1
 * 2018-4-1
 */
#include <stdio.h>
#include "vm.h"

int main(int argc, char *argv[])
{
    MicroVM* vm = (MicroVM*)malloc(sizeof(MicroVM));
    memset(vm, 0, sizeof(MicroVM));
    vm_load_file(vm, "D:/led.js");
    return 0;
}
