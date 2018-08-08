#ifndef PARSER_H
#define PARSER_H

#include "vm.h"

#define FLAG_MAIN         1
#define FLAG_FUNC         2
#define FLAG_IGNORE       4
#define FLAG_ASSIGN       8

enum PARSER_TOKEN
{
    TOKEN_INVALID = 0,
    TOKEN_ADD,      // +
    TOKEN_SUB,      // -
    TOKEN_MUL,      // *
    TOKEN_DIV,      // /
    TOKEN_MOD,
    TOKEN_LSHIFT,   // <<
    TOKEN_RSHIFT,   // >>
    TOKEN_NEG,      // -
    TOKEN_EQ,       // ==
    TOKEN_TYPE_EQ,  // ===
    TOKEN_NOTEQ,    // !=
    TOKEN_TYPE_NOTEQ, // !==
    TOKEN_GT,       // >
    TOKEN_GTEQ,     // >=
    TOKEN_LT,       // <
    TOKEN_LTEQ,     // <=
    TOKEN_AND,      // &
    TOKEN_OR,       // |
    TOKEN_XOR,      // ^
    TOKEN_NOT,      // !
    TOKEN_LOGIC_AND, // &&
    TOKEN_LOGIC_OR,  // ||
    TOKEN_INC,
    TOKEN_DEC,

    TOKEN_DELETE,
    TOKEN_TYPEOF,

    TOKEN_COMMENT,
    TOKEN_VAR,
    TOKEN_FUNC,
    TOKEN_NEW,
    TOKEN_memberExpressionSuffix,
};

typedef struct parser_var
{
    var_t val;
    uint32_t hash;
}parser_var_t;

typedef struct parser_varmap
{
    uint16_t size;           // Symbal hash table size
    uint16_t count;
    parser_var_t * table;
    uint8_t id;
}parser_varmap_t;

typedef struct parser_t {
	uint8_t  error;
	uint16_t line;
	uint16_t col;
	char * input;
	uint32_t index;
    uint16_t id1;
    uint16_t id2;
    uint32_t opcodeIndex;
    uint8_t op;
    uint8_t* codes;
    uint32_t code_size;
    parser_varmap_t map;

    function_t* currFunc;
    uint8_t curr_func_id;
    CELL num;
    MicroVM* vm;
    uint8_t flag;
    uint8_t num_of_args;
    char id_name[100];
    uint8_t token;
}parser_t;

uint32_t parser_hash(const void *key);
int parser_run(parser_t * p);
native_t * parser_native_put(parser_t* p, const char* name, int argc, NativeMethod method);
#endif
