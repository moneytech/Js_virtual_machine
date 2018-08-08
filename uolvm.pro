#-------------------------------------------------
#
# Project created by QtCreator 2016-07-13T10:53:40
#
#------------------------------------------------

TARGET = uolvm
TEMPLATE = app
DEFINES += __UNIX__
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES +=\
    vm.c \
    nativetable.c \
    main.c \
    parser.c \
    heap.c \
    vm_timeout.c \
    object.c \
    array.c \
    gc.c

HEADERS  += \
    errcode.h \
    instruction.h \
    vm.h \
    font.h \
    parser.h \
    heap.h \
    nativetable.h \
    vm_timeout.h \
    object.h \
    array.h \
    gc.h
