// Copyright (c) 2013 Zhe Wang
// wzedmund@gmail.com

// version 1.0.3
// date 17/02/2015
#ifndef INSTRUCTION_H
#define INSTRUCTION_H

enum Opcode   //the might operation. this is to be showed in terminal
{
    Nop = 0,  //0
    Stop,

    LoadVar,
    LoadLocalVar,
    LoadFunc,
    LoadNative,
    LoadNull,
    LoadUndef,
    LoadTrue,
    LoadFalse,
    LoadNum,
    LoadStr, //7

    Store,
    StoreAdd,
    StoreSub,
    StoreMul,
    StoreDiv,
    StoreMod,
    StoreAnd,
    StoreOr,
    StoreXor,
    StoreLShift,
    StoreRShift, //18

    LoadProp,
    LoadArray,

    Eq,
    NotEq,
    Gt,
    GtEq,
    Lt,
    LtEq,  //24

    Mul,
    Div,
    Mod,
    Add,
    Sub,
    Or,
    Xor,
    And,
    Not,
    Neg,
    ShiftL,
    ShiftR,
    Inc,
    Dec, //38

    Pop,
    Dup,
    Sdn,
    Sup,
    Swap, //41

    Jump,
    JumpNotZero,
    JumpZero,
    JumpIntEq,
    JumpIntNotEq,
    JumpIntGt,
    JumpIntGtEq,
    JumpIntLt,
    JumpIntLtEq, //50

    SJump,
    SJumpNotZero,
    SJumpZero,
    SJumpIntEq,
    SJumpIntNotEq,
    SJumpIntGt,
    SJumpIntGtEq,
    SJumpIntLt,
    SJumpIntLtEq,

    Call,
    CallProp,

    Return,
    ReturnVoid,

    New,
};



#endif
