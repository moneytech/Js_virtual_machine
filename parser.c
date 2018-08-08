//Comment date:  2018/8/8
//function: 语法分析 Gramma analysis

#include "parser.h"
#include "instruction.h"
#include "errcode.h"

int parser_flag_is_main(parser_t * p);
int parser_flag_is_func(parser_t * p);
int parser_flag_is_ignore(parser_t * p);
void parser_flag_set_call_assign(parser_t * p);
int parser_flag_is_call_assign(parser_t * p);
void parser_flag_set_ignore(parser_t * p);
int parser_program(parser_t * p, uint8_t flag);
int parser_sourceElements(parser_t * p, uint8_t token);
int parser_sourceElement(parser_t * p, uint8_t token);
int parser_functionDeclaration(parser_t * p);
int parser_functionExpression(parser_t * p);
int parser_formalParameterList(parser_t * p);
int parser_functionBody(parser_t * p);
int parser_statement(parser_t * p);
int parser_statementBlock(parser_t * p);
int parser_statementList(parser_t * p);
int parser_variableStatement(parser_t * p);
int parser_variableDeclarationList(parser_t * p);
int parser_variableDeclarationListNoIn(parser_t * p);
int parser_variableDeclaration(parser_t * p);
int parser_variableDeclarationNoIn(parser_t * p);
int parser_initialiser(parser_t * p);
int parser_initialiserNoIn(parser_t * p);
int parser_emptyStatement(parser_t * p);
int parser_expressionStatement(parser_t * p);
int parser_ifStatement(parser_t * p);
int parser_doWhileStatement(parser_t * p);
int parser_whileStatement(parser_t * p);
int parser_forStatement(parser_t * p);
int parser_forStatementInitialiserPart(parser_t * p);
int parser_forInStatement(parser_t * p);
int parser_forInStatementInitialiserPart(parser_t * p);
int parser_continueStatement(parser_t * p);
int parser_breakStatement(parser_t * p);
int parser_returnStatement(parser_t * p);
int parser_withStatement(parser_t * p);
int parser_labelledStatement(parser_t * p);
int parser_switchStatement(parser_t * p);
int parser_caseBlock(parser_t * p);
int parser_caseClause(parser_t * p);
int parser_defaultClause(parser_t * p);
int parser_throwStatement(parser_t * p);
int parser_tryStatement(parser_t * p);
int parser_catchClause(parser_t * p);
int parser_finallyClause(parser_t * p);
int parser_expression(parser_t * p);
int parser_expressionNoIn(parser_t * p);
int parser_assignmentExpression(parser_t * p);
int parser_assignmentExpressionNoIn(parser_t * p);
int parser_leftHandSideExpression(parser_t * p);
int parser_newExpression(parser_t * p);
int parser_memberExpression(parser_t * p, uint8_t type);
int parser_memberExpressionSuffix(parser_t * p);
int parser_callExpression(parser_t * p);
int parser_callExpressionSuffix(parser_t * p);
int parser_arguments(parser_t * p);
int parser_indexSuffix(parser_t * p);
int parser_propertyReferenceSuffix(parser_t * p);
int parser_assignmentOperator(parser_t * p);
int parser_conditionalExpression(parser_t * p);
int parser_conditionalExpressionNoIn(parser_t * p);
int parser_logicalORExpression(parser_t * p);
int parser_logicalORExpressionNoIn(parser_t * p);
int parser_logicalANDExpression(parser_t * p);
int parser_logicalANDExpressionNoIn(parser_t * p);
int parser_bitwiseORExpression(parser_t * p);
int parser_bitwiseORExpressionNoIn(parser_t * p);
int parser_bitwiseXORExpression(parser_t * p);
int parser_bitwiseXORExpressionNoIn(parser_t * p);
int parser_bitwiseANDExpression(parser_t * p);
int parser_bitwiseANDExpressionNoIn(parser_t * p);
int parser_equalityExpression(parser_t * p);
int parser_equalityExpressionNoIn(parser_t * p);
int parser_relationalExpression(parser_t * p);
int parser_shiftExpression(parser_t * p);
int parser_additiveExpression(parser_t * p);
int parser_multiplicativeExpression(parser_t * p);
int parser_unaryExpression(parser_t * p);
int parser_postfixExpression(parser_t * p);
int parser_primaryExpression(parser_t * p, uint8_t type);
int parser_arrayLiteral(parser_t * p);
int parser_objectLiteral(parser_t * p);
int parser_propertyNameAndValue(parser_t * p);
int parser_propertyName(parser_t * p);
int parser_literal(parser_t * p);
int parser_StringLiteral(parser_t * p);
int parser_NumericLiteral(parser_t * p);
int parser_Identifier(parser_t * p);
int parser_Comment(parser_t * p);
int parser_LineComment(parser_t * p); 

void parser_push_opcode(parser_t *p, uint8_t code)
{
    if(parser_flag_is_ignore(p))
        return;
    p->codes[p->opcodeIndex++] = code;
}

void parser_push_opcode_1(parser_t *p, uint8_t code, uint8_t oprand1)
{
    if(parser_flag_is_ignore(p))
        return;
    p->codes[p->opcodeIndex++] = code;
    p->codes[p->opcodeIndex++] = oprand1;
}

void parser_push_opcode_2(parser_t *p, uint8_t code, uint8_t oprand1, uint8_t oprand2)
{
    if(parser_flag_is_ignore(p))
        return;
    p->codes[p->opcodeIndex++] = code;
    p->codes[p->opcodeIndex++] = oprand1;
    p->codes[p->opcodeIndex++] = oprand2;
}

void parser_push_opcode_3(parser_t *p, uint8_t code, uint8_t oprand1, uint8_t oprand2, uint8_t oprand3)
{
    if(parser_flag_is_ignore(p))
        return;
    p->codes[p->opcodeIndex++] = code;
    p->codes[p->opcodeIndex++] = oprand1;
    p->codes[p->opcodeIndex++] = oprand2;
    p->codes[p->opcodeIndex++] = oprand3;
}

void parser_push_opcode_4(parser_t *p, uint8_t code, uint8_t oprand1, uint8_t oprand2, uint8_t oprand3, uint8_t oprand4)
{
    if(parser_flag_is_ignore(p))
        return;
    p->codes[p->opcodeIndex++] = code;
    p->codes[p->opcodeIndex++] = oprand1;
    p->codes[p->opcodeIndex++] = oprand2;
    p->codes[p->opcodeIndex++] = oprand3;
    p->codes[p->opcodeIndex++] = oprand4;
}

void parser_append_opcode(parser_t *p, uint8_t * code, int len)
{
    if(parser_flag_is_ignore(p))
        return;
    for(int i = 0; i < len; i++)
        p->codes[p->opcodeIndex++] = code[i];
}

void parser_insert_opcode(parser_t *p, int index, uint8_t * bytes, int len)
{
    if(parser_flag_is_ignore(p))
        return;
    int i, n;

    n = p->opcodeIndex - index;
    for (i = 1; i <= n; i++)
    {
        p->codes[p->opcodeIndex + len - i] = p->codes[p->opcodeIndex - i];
    }

    for (i = 0; i < len; i++)
    {
        p->codes[index + i] = bytes[i];
    }

    p->opcodeIndex += len;
}

uint32_t parser_htbl_key(uint32_t size, uint32_t hash, int i) {
    return (hash + i * (hash * 2 + 1)) % size;
}

uint32_t parser_hash(const void *key)
{
    const char *ptr = key;
    uint32_t val = 0;

    while (*ptr) {
        uint32_t tmp;

        val = (val << 4) + *ptr;
        tmp = val & 0xf0000000;
        if (tmp) {
            val = (val ^ (tmp >> 24)) ^ tmp;
        }

        ptr++;
    }

    return val;
}

native_t * parser_native_get(parser_t* p, const char* name, int* id)
{
    uint32_t hash = parser_hash(name);
    MicroVM *vm = p->vm;
    plist_t list = vm->native_func_list;
    list = list_hash_find(list, hash);
    if(list)
    {
        *id = list->index;
        return (native_t*)list->data.val.addr;
    }
    return NULL;
}

native_t * parser_native_put(parser_t* p, const char* name, int argc, NativeMethod method)
{
    uint32_t hash = parser_hash(name);
    MicroVM *vm = p->vm;
    plist_t list = vm->native_func_list;
    var_t cell;
    list = list_hash_find(list, hash);
    if(list)
        return (native_t*)list->data.val.addr;

    native_t * func = (native_t*)heap_alloc(vm->heap, sizeof(native_t));
    func->arg_cnt = argc;
    func->func = method;
    cell.val.addr = func;
    cell.type = T_NATIVE_FUNCTION;
    list_append(vm, &vm->native_func_list, cell, hash);
    vm->native_func_cnt++;
    return func;
}

function_t * parser_func_put(parser_t* p, const char* name, uint16_t* id)
{
    uint32_t hash = parser_hash(name);
    MicroVM *vm = p->vm;
    plist_t list = vm->func_list;
    var_t cell;
    list = list_hash_find(list, hash);
    if(list)
    {
        *id = list->index;
        return (function_t*)list->data.val.addr;
    }

    function_t * func = (function_t*)heap_alloc(vm->heap, sizeof(function_t));
    func->arg_cnt = 0;
    func->var_cnt = 0;
    cell.val.addr = func;
    cell.type = T_FUNCTION;
    list_append(vm, &vm->func_list, cell, hash);
    *id = vm->func_cnt++;
    p->currFunc = func;
    p->currFunc->var_list = NULL;
    return func;
}

int parser_str_put(parser_t* p, const char* name, int* id)
{
    uint32_t hash = parser_hash(name);
    MicroVM *vm = p->vm;
    plist_t list = vm->str_list;
    var_t cell;
    list = list_hash_find(list, hash);
    if(list)
    {
        *id = list->index;
        return 0;
    }

    string_t * str = (string_t*)heap_alloc(vm->heap, sizeof(string_t));
    str->size = strlen(name);
    str->buf = heap_alloc(vm->heap, str->size);
    cell.val.addr = str;
    cell.type = T_NATIVE_STRING;
    memcpy(str->buf, name, str->size);
    list_append(vm, &vm->str_list, cell, hash);
    *id = vm->str_cnt++;
    return 1;
}

function_t * parser_func_get(parser_t* p, const char* name, uint8_t * func_id)
{
    uint32_t hash = parser_hash(name);
    plist_t plist = list_hash_find(p->vm->func_list, hash);
    if(plist)
    {
        *func_id = plist->index;
        return (function_t*)plist->data.val.addr;
    }
    return NULL;
}

int parser_var_get(parser_t* p, const char* name, uint8_t* func_id, uint8_t* var_id)
{
    uint32_t hash = parser_hash(name);
    MicroVM *vm = p->vm;
    plist_t plist;
    if(p->currFunc)
    {
        plist = list_hash_find(p->currFunc->var_list, hash);
        *func_id = p->curr_func_id;
        if(plist)
        {
            *var_id = plist->index;
            return 2;
        }
    }

    plist = list_hash_find(vm->var_list, hash);
    *func_id = 0;
    if(plist)
    {
        *var_id = plist->index;
        return 1;
    }

    return 0;
}

int parser_var_put(parser_t* p, const char* name, uint8_t* func_id, uint8_t* var_id )
{
    uint32_t hash = parser_hash(name);
    MicroVM *vm = p->vm;
    var_t cell;
    plist_t plist;
    if(p->currFunc)
    {
        plist = list_hash_find(p->currFunc->var_list, hash);
        *func_id = p->curr_func_id;
        if(plist)
        {
            *var_id = plist->index;
            return 0;
        }

        cell.type = T_UNDEFINED;
        list_append(vm, &p->currFunc->var_list, cell, hash);
        *var_id = p->currFunc->var_cnt++;
    }
    else
    {
        plist = list_hash_find(vm->var_list, hash);
        *func_id = 0;
        if(plist)
        {
            *var_id = plist->index;
            return 0;
        }

        cell.type = T_UNDEFINED;
        list_append(vm, &vm->var_list, cell, hash);
        *var_id = vm->var_cnt++;
    }

    return 1;
}

char parser_next_ch(parser_t * p, uint32_t index)
{
    if(index < p->code_size)
        return p->input[index];
    return 0;
}

void parser_eat_token(parser_t * p, int len)
{
	p->index += len;
}

int parser_token_id(parser_t * p, char *str, int len)
{
    char ch;
    int index = p->index;
    int str_index = 0;
    ch = parser_next_ch(p, index);
    do {
        if( ch != str[str_index] )
            return 0;
        index++;
        str_index++;
        ch = parser_next_ch(p, index);
    } while ( isalnum(ch) || ch == '_');

    if(str_index == len)
        return 1;
    return 0;
}

int parser_token(parser_t * p, char *str, int len)
{
	char ch;
    int index = p->index;
    int str_index = 0;
    if(isalpha(parser_next_ch(p, index)) || parser_next_ch(p, index) == '_')
        return parser_token_id(p, str, len);

	do {
		ch = parser_next_ch(p, index);
		
        if( ch != str[str_index] )
			return 0;
		index++;
        str_index++;
    } while ( str_index < len);
    ch = parser_next_ch(p, index);
    if(len == 1 && ch == '=' && parser_next_ch(p, index - 1) == '=')
        return 0;
    return 1;
}

int parser_token_with_eat(parser_t * p, char *str, int len)
{
	int res = parser_token(p, str, len);
	if(res)
		parser_eat_token(p, len);
	return res;
}

int parser_flag_is_main(parser_t * p)
{
    if(p->flag & FLAG_MAIN)
        return 1;
    return 0;
}

int parser_flag_is_func(parser_t * p)
{
    if(p->flag & FLAG_FUNC)
        return 1;
    return 0;
}

int parser_flag_is_ignore(parser_t * p)
{
    if(p->flag & FLAG_IGNORE)
        return 1;
    return 0;
}

void parser_flag_set_assign(parser_t * p)
{
    p->flag |= FLAG_ASSIGN;
}

int parser_flag_is_assign(parser_t * p)
{
    if(p->flag & FLAG_ASSIGN)
        return 1;
    return 0;
}

void parser_flag_set_ignore(parser_t * p)
{
    p->flag &= 0xF8;
    p->flag |= FLAG_IGNORE;
}

int parser_LT_or_semicolon(parser_t * p)
{
    if( parser_token(p, "\n", 1) ||
        parser_token(p, "\r", 1) ||
        parser_token(p, "\t", 1) ||
        parser_token(p, ";", 1))
    {
        parser_eat_token(p, 1);
        return 1;
    }
    return 0;
}

int parser_LT(parser_t * p)
{
    int index = p->index;
    char ch = parser_next_ch(p, index);
    if(parser_token(p, "//", 2)){
        return parser_LineComment(p);
    } else if( parser_token(p, "/*", 1) ) {
        return parser_Comment(p);
    } else if(ch == ' '){
        p->col++;p->index++;return 1;
    } else if(ch == '\n'){
        p->col = 1;p->line++;p->index++;return 1;
    } else if(ch == '\r'){
        p->index++;
        return 1;
    } else if(ch == '\t'){
        p->col += 4;p->index++;return 1;
    }
    return 0;
}

int parser_EOF(parser_t * p)
{
    if( parser_next_ch(p, p->index))
        return 1;
    return 0;
}

int parser_run(parser_t * p)
{
    int res;

    res = parser_program(p, FLAG_MAIN);
    p->index = 0;
    p->line = 1;
    p->col = 1;
    res = parser_program(p, FLAG_FUNC);
    return res;
}

int parser_program(parser_t * p, uint8_t flag)
{
    p->flag = flag;
    while(parser_LT(p));
    parser_sourceElements(p, TOKEN_INVALID);
	while(parser_LT(p));
    p->flag = flag;
    if(parser_flag_is_main(p))
        p->codes[p->opcodeIndex++] = Stop;
    if(parser_EOF(p))
        return 1;
    return 1;
}



int parser_sourceElements(parser_t * p, uint8_t token)
{
	while(1) {
        while(parser_LT(p));
        if( !parser_sourceElement(p, token) ) break;
	}
    return 1;
}

int parser_sourceElement(parser_t * p, uint8_t token)
{
    uint8_t flag = p->flag;
    if( parser_token(p, "function", 8) ) {
		return parser_functionDeclaration(p);
	} else {
        if(token == TOKEN_FUNC && parser_flag_is_main(p))
            parser_flag_set_ignore(p);
        if(token != TOKEN_FUNC && parser_flag_is_func(p))
            parser_flag_set_ignore(p);
        uint8_t res = parser_statement(p);
        p->flag = flag;
        return res;
	}
}

int parser_functionDeclaration(parser_t * p)
{
    uint8_t flag = p->flag;
    uint8_t func_id;
    uint8_t num_of_args;
    if(parser_flag_is_main(p))
        parser_flag_set_ignore(p);
    if( !parser_token(p, "function", 8) ) return 0;
	parser_eat_token(p, 8);
	while(parser_LT(p));
    if( !parser_Identifier(p) ) return 0;
    function_t * func = parser_func_put(p, p->id_name, &p->id1);
    p->currFunc = func;
    func_id = p->curr_func_id;
    p->curr_func_id = p->id1;
    func->code_index = p->opcodeIndex;
    num_of_args = p->num_of_args;
    p->num_of_args = 0;
	if( !parser_formalParameterList(p) ) return 0;
    func->arg_cnt = p->num_of_args;
    p->num_of_args = num_of_args;
	while(parser_LT(p));
	if( !parser_functionBody(p) ) return 0;
    parser_push_opcode(p, ReturnVoid);
    p->curr_func_id = func_id;
    p->flag = flag;
    p->currFunc = NULL;
	return 1;
}

int parser_functionExpression(parser_t * p)
{
    (void)p;
    return 0;
}

int parser_formalParameterList(parser_t * p)
{
    uint8_t id1, id2;
    if( !parser_token(p, "(", 1) ) return 0;
	parser_eat_token(p, 1);
	while(parser_LT(p));
    if( parser_token(p, ")", 1) )
    {
        parser_eat_token(p, 1);
        return 1;
    }

    if( parser_Identifier(p) ) {
        parser_var_put(p, p->id_name, &id1, &id2);
        p->num_of_args++;
		if( parser_token(p, ")", 1) ) {
			parser_eat_token(p, 1);
			return 1;
		} else {
            while(1) {
                while(parser_LT(p));
                if( !parser_token(p, ",", 1) ) break;
                parser_eat_token(p, 1);
                while(parser_LT(p));
                if( !parser_Identifier(p) ) return 0;
                parser_var_put(p, p->id_name, &id1, &id2);
                p->num_of_args++;
            }
            while(parser_LT(p));
            if( parser_token(p, ")", 1) ) {
                parser_eat_token(p, 1);
                return 1;
            } else {
                return 0;
            }
		}
	} else {
        return 0;
	}
}

int parser_functionBody(parser_t * p)
{
	if( !parser_token(p, "{", 1) ) return 0;
	parser_eat_token(p, 1);
	while(parser_LT(p));
    if( !parser_sourceElements(p, TOKEN_FUNC) ) return 0;
	while(parser_LT(p));
	if( !parser_token_with_eat(p, "}", 1) ) return 0;
	return 1;
}

int parser_statement(parser_t * p)
{
    if(parser_token(p, "//", 2)){
        return parser_LineComment(p);
    } else if( parser_token(p, "/*", 1) ) {
        return parser_Comment(p);
    } else if( parser_token(p, "{", 1) ) {
        return parser_statementBlock(p);
	} else if( parser_token(p, "var", 3) ) {
		return parser_variableStatement(p);
	} else if( parser_token(p, ";", 1) ) {
		return parser_emptyStatement(p);
	} else if( parser_token(p, "if", 2) ) {
		return parser_ifStatement(p);
	} else if( parser_token(p, "do", 2) ) {
		return parser_doWhileStatement(p);
	} else if( parser_token(p, "while", 5) ) {
		return parser_whileStatement(p);
	} else if( parser_token(p, "for", 3) ) {
		return parser_forStatement(p);
	} else if( parser_token(p, "continue", 8) ) {
		return parser_continueStatement(p);
	} else if( parser_token(p, "break", 5) ) {
		return parser_breakStatement(p);
	} else if( parser_token(p, "return", 6) ) {
		return parser_returnStatement(p);
	} else if( parser_token(p, "with", 4) ) {
		return parser_withStatement(p);
	} else if( parser_token(p, "switch", 6) ) {
		return parser_switchStatement(p);
	} else if( parser_token(p, "throw", 5) ) {
		return parser_throwStatement(p);
	} else if( parser_token(p, "try", 3) ) {
		return parser_tryStatement(p);
	} else {
        return parser_expressionStatement(p);
	}	
}

int parser_statementBlock(parser_t * p)
{
	if( !parser_token(p, "{", 1) ) return 0;
	parser_eat_token(p, 1);
	while(parser_LT(p));
	parser_statementList(p);
	while(parser_LT(p));
	if( !parser_token_with_eat(p, "}", 1) ) return 0;
	return 1;
}

int parser_statementList(parser_t * p)
{
	if(!parser_statement(p)) return 0;
	
	while(1){
		while(parser_LT(p));
		if(!parser_statement(p)) break;
	}
	return 1;
}

int parser_variableStatement(parser_t * p)
{
	if( !parser_token(p, "var", 3) ) return 0;
	parser_eat_token(p, 3);
	while(parser_LT(p));
	if( !parser_variableDeclarationList(p) ) return 0;
	if( parser_LT_or_semicolon(p) ) return 1;
	return 0;
}

int parser_variableDeclarationList(parser_t * p)
{
	if( !parser_variableDeclaration(p) ) return 0;
	while(1)
	{
		while(parser_LT(p));
        if( !parser_token_with_eat(p, ",", 1) ) return 1;
		while(parser_LT(p));
        if( !parser_variableDeclaration(p) ) return 1;
	}
}

int parser_variableDeclarationListNoIn(parser_t * p)
{
	return 0;
}

int parser_variableDeclaration(parser_t * p)
{
    uint8_t scope;
    if( !parser_Identifier(p) ) return 0;
    scope = parser_var_put(p, p->id_name, &p->id1, &p->id2);
    if( scope == 0 && parser_flag_is_main(p))
    {
        p->error = vm_err_redefined_var;
        return 0;
    }
    if(scope == 1)
        parser_push_opcode_2(p, LoadVar, p->id1,  p->id1 >> 8);
    else
        parser_push_opcode_2(p, LoadLocalVar, p->id1, p->id2);
	while(parser_LT(p));
    if(parser_initialiser(p) == 0)
        parser_push_opcode(p, Sdn);
	return 1;
}

int parser_variableDeclarationNoIn(parser_t * p)
{
	return 0;
}

int parser_initialiser(parser_t * p)
{
    uint8_t flag = p->flag;
    if( !parser_token(p, "=", 1) ) return 0;
    parser_flag_set_assign(p);
	parser_eat_token(p, 1);
	while(parser_LT(p));
	if( !parser_assignmentExpression(p) ) return 0;
    parser_push_opcode(p, Store);
    p->flag = flag;
	return 1;
}

int parser_initialiserNoIn(parser_t * p)
{
	return 0;
}

int parser_emptyStatement(parser_t * p)
{
    return 1;
}

int parser_expressionStatement(parser_t * p)
{
    if( !parser_expression(p) ) return 0;
    if( parser_LT_or_semicolon(p) ) return 1;
    return 1;
}

void parser_direct_jump(parser_t * p, int index)
{
    if(p->opcodeIndex - index < 126)
    {
        int8_t offset1 = p->opcodeIndex - index;
        parser_push_opcode_1(p, SJump, -offset1 - 1);
    }
    else
    {
        int index = p->index + 1;
        parser_push_opcode_4(p, Jump, index, index>>8, index>>16, index>>24);
    }
}

uint8_t parser_insert_jump(parser_t * p, int index)
{
    int8_t code[5];
    if( abs(p->opcodeIndex - index) < 126)
    {
        int8_t offset1 = p->opcodeIndex - index;
        code[0] = SJump;
        code[1] = offset1 + 1;
        parser_insert_opcode(p, index, code, 2);
        return 0;
    }
    else
    {
        int index = p->index + 1;
        parser_push_opcode_4(p, Jump, index, index>>8, index>>16, index>>24);
        return 1;
    }
}

void parser_insert_jumpzero(parser_t * p, int src, int dst)
{
    int8_t code[5];
    if( abs(dst - src) < 126)
    {
        int8_t offset = dst - src;
        code[0] = SJumpZero;
        code[1] = offset + 1;
        parser_insert_opcode(p, src, code, 2);
    }
    else
    {
        code[0] =JumpZero;
        code[1] = dst + 1;
        code[2] = (dst + 1)>> 8;
        code[3] = (dst + 1)>> 16;
        code[4] = (dst + 1)>> 24;
        parser_insert_opcode(p, src, code, 5);
    }
}

void parser_append_jump(parser_t * p, int index)
{
    int8_t code[5];
    if( abs(p->opcodeIndex - index) < 126)
    {
        int8_t offset = index - p->opcodeIndex;
        code[0] = SJump;
        code[1] = offset - 1;
        parser_append_opcode(p, code, 2);
    }
    else
    {
        int index = p->index + 1;
        parser_push_opcode_4(p, Jump, index, index>>8, index>>16, index>>24);
    }
}

int parser_ifStatement(parser_t * p)
{
    int if_index, else_index;
    uint8_t flag = p->flag;
    uint8_t short_or_long_jmp;
    parser_flag_set_assign(p);
    if( !parser_token(p, "if", 2) ) return 0;
	parser_eat_token(p, 2);
	while(parser_LT(p));
	if( !parser_token(p, "(", 1) ) return 0;
	parser_eat_token(p, 1);
	while(parser_LT(p));
	if( !parser_expression(p) ) return 0;
	while(parser_LT(p));
    if( !parser_token(p, ")", 1) ) return 0;
    parser_eat_token(p, 1);
    p->flag = flag;
    if_index = p->opcodeIndex;
    while(parser_LT(p));
	if( !parser_statement(p) ) return 0;
    while(parser_LT(p));
    else_index = p->opcodeIndex;
	if( parser_token(p, "else", 4) ) {
        parser_eat_token(p, 4);
		while(parser_LT(p));
		if( !parser_statement(p) ) return 0;
        short_or_long_jmp = parser_insert_jump(p, else_index);
        if(short_or_long_jmp)
            parser_insert_jumpzero(p, if_index, else_index + 5);
        else
            parser_insert_jumpzero(p, if_index, else_index + 2);
	}
    else
        parser_insert_jumpzero(p, if_index, else_index);
	return 1;
}

void parser_while_jump(parser_t * p, int index1, int index2)
{
    int8_t code[5];
    uint8_t len;
    if( abs(p->opcodeIndex - index2) < 126)
        len = 2;
    else
        len = 5;

    if( abs(p->opcodeIndex - index1) < 126)
    {
        int8_t offset = index1 - p->opcodeIndex;
        code[0] = SJump;
        code[1] = offset - 1 - len;
        parser_append_opcode(p, code, 2);
    }
    else
    {
        code[0] =Jump;
        code[1] = index1 + 1;
        code[2] = (index1 + 1)>> 8;
        code[3] = (index1 + 1)>> 16;
        code[4] = (index1 + 1)>> 24;
        parser_append_opcode(p, code, 5);
    }
    parser_insert_jumpzero(p, index2, p->opcodeIndex);
}

void parser_do_while_jump(parser_t * p, int index)
{
    int8_t code[5];
    if( abs(p->opcodeIndex - index) < 126)
    {
        int8_t offset = index - p->opcodeIndex;
        code[0] = SJumpNotZero;
        code[1] = offset - 1;
        parser_append_opcode(p, code, 2);
    }
    else
    {
        code[0] =JumpNotZero;
        code[1] = index + 1;
        code[2] = (index + 1)>> 8;
        code[3] = (index + 1)>> 16;
        code[4] = (index + 1)>> 24;
        parser_append_opcode(p, code, 5);
    }
}

int parser_doWhileStatement(parser_t * p)
{
    int index1 = p->index;
    if( !parser_token(p, "do", 2) ) return 0;
	parser_eat_token(p, 2);
	while(parser_LT(p));
	if( !parser_statement(p) ) return 0;
	while(parser_LT(p));
	if( !parser_token(p, "while", 5) ) return 0;
	parser_eat_token(p, 5);
	while(parser_LT(p));
	if( !parser_token(p, "(", 1) ) return 0;
	parser_eat_token(p, 1);
	if( !parser_expression(p) ) return 0;
	while(parser_LT(p));
	if( !parser_token(p, ")", 1) ) return 0;
	parser_eat_token(p, 1);
    if( parser_LT_or_semicolon(p) )
    {
        parser_do_while_jump(p, index1);
        return 1;
    }
	return 0;
}

int parser_whileStatement(parser_t * p)
{
    int index1 = p->opcodeIndex;
    if( !parser_token(p, "while", 5) ) return 0;
	parser_eat_token(p, 5);
	while(parser_LT(p));
	if( !parser_token(p, "(", 1) ) return 0;
	parser_eat_token(p, 1);
	if( !parser_expression(p) ) return 0;
	while(parser_LT(p));
	if( !parser_token(p, ")", 1) ) return 0;
    parser_eat_token(p, 1);
    while(parser_LT(p));
    int index2 = p->opcodeIndex;
    if( !parser_statement(p) ) return 0;
    parser_while_jump(p, index1, index2);
    return 1;
}

void parser_for_jump(parser_t * p, int index1, int index2)
{
    int8_t code[5];
    uint8_t len;
    if( abs(p->opcodeIndex - index2) < 126)
        len = 2;
    else
        len = 5;

    if( abs(p->opcodeIndex - index1) < 126)
    {
        int8_t offset = index1 - p->opcodeIndex;
        code[0] = SJump;
        code[1] = offset - 1 - len;
        parser_append_opcode(p, code, 2);
    }
    else
    {
        code[0] =Jump;
        code[1] = index1 + 1;
        code[2] = (index1 + 1)>> 8;
        code[3] = (index1 + 1)>> 16;
        code[4] = (index1 + 1)>> 24;
        parser_append_opcode(p, code, 5);
    }
    parser_insert_jumpzero(p, index2, p->opcodeIndex);
}

int parser_forStatement(parser_t * p)
{
	if( !parser_token(p, "for", 3) ) return 0;
	parser_eat_token(p, 3);
	while(parser_LT(p));
	if( !parser_token(p, "(", 1) ) return 0;
	parser_eat_token(p, 1);
	while(parser_LT(p));
	parser_forStatementInitialiserPart(p);
	while(parser_LT(p));
	if( !parser_token(p, ";", 1) ) return 0;
	parser_eat_token(p, 1);
	while(parser_LT(p));
    int index1 = p->opcodeIndex;
	parser_expression(p);
	while(parser_LT(p));
	if( !parser_token(p, ";", 1) ) return 0;
	parser_eat_token(p, 1);
	while(parser_LT(p));
    int index2 = p->opcodeIndex;
	parser_expression(p);
	while(parser_LT(p));
	if( !parser_token(p, ")", 1) ) return 0;
	parser_eat_token(p, 1);
	while(parser_LT(p));
	if( !parser_statement(p) ) return 0;
    parser_for_jump(p, index1, index2);
	return 1;
}

int parser_forStatementInitialiserPart(parser_t * p)
{
	if( parser_token(p, "var", 3) ) {
		parser_eat_token(p, 3);
		while(parser_LT(p));
		if( !parser_variableDeclarationListNoIn(p) ) return 0;
		return 1;
	} else {
		return parser_expressionNoIn(p);
	}
}

int parser_forInStatement(parser_t * p)
{
	return 0;
}

int parser_forInStatementInitialiserPart(parser_t * p)
{
	return 0;
}

int parser_continueStatement(parser_t * p)
{
	if( !parser_token(p, "continue", 8) ) return 0;
	parser_eat_token(p, 8);
    while(parser_LT(p));
    parser_Identifier(p);
	if( parser_LT_or_semicolon(p) ) return 1;
	return 0;
}

int parser_breakStatement(parser_t * p)
{
	if( !parser_token(p, "break", 5) ) return 0;
	parser_eat_token(p, 5);
    parser_Identifier(p);
	if( parser_LT_or_semicolon(p) ) return 1;
	return 0;
}

int parser_returnStatement(parser_t * p)
{
    uint8_t flag = p->flag;
    if( !parser_token(p, "return", 6) ) return 0;
    parser_flag_set_assign(p);
	parser_eat_token(p, 6);
    while(parser_LT(p));
    if(parser_expression(p))
        parser_push_opcode(p, Return);
    else
        parser_push_opcode(p, ReturnVoid);
    p->flag = flag;
	if( parser_LT_or_semicolon(p) ) return 1;
	return 0;
}

int parser_withStatement(parser_t * p)
{
	return 0;
}

int parser_labelledStatement(parser_t * p)
{
	return 0;
}

int parser_switchStatement(parser_t * p)
{
	return 0;
}

int parser_caseBlock(parser_t * p)
{
	return 0;
}

int parser_caseClause(parser_t * p)
{
	return 0;
}

int parser_defaultClause(parser_t * p)
{
	return 0;
}

int parser_throwStatement(parser_t * p)
{
	return 0;
}

int parser_tryStatement(parser_t * p)
{
	return 0;
}

int parser_catchClause(parser_t * p)
{
	return 0;
}

int parser_finallyClause(parser_t * p)
{
	return 0;
}

int parser_expression(parser_t * p)
{
    if( !parser_assignmentExpression(p) ) return 0;
	while(1) {
		while(parser_LT(p));
		if( !parser_token(p, ",", 1) ) return 1;
		parser_eat_token(p, 1);
		while(parser_LT(p));
		if( !parser_assignmentExpression(p) ) return 1;
    }
    return 1;
}

int parser_expressionNoIn(parser_t * p)
{
    return parser_expression(p);
}

int parser_assignmentExpression(parser_t * p)
{
    uint8_t flag = p->flag;
    uint8_t token = p->token;
    uint8_t id1, id2;
    uint8_t op1;
    int index = p->index;
    int opcodeIndex = p->opcodeIndex;

    if( parser_leftHandSideExpression(p) ){
        id1 = p->id1;
        id2 = p->id2;

        while(parser_LT(p));
        if( parser_assignmentOperator(p) ) {
            parser_flag_set_assign(p);
            while(parser_LT(p));
            op1 = p->op;
            if( !parser_assignmentExpression(p) ) return 0;
            parser_push_opcode(p, op1);
            p->flag = flag;
            return 1;
        }
    }
    p->index = index;
    p->opcodeIndex = opcodeIndex;
    p->token = token;
    if( parser_conditionalExpression(p) )
    {
        return 1;
    }

    return 0;
}

int parser_assignmentExpressionNoIn(parser_t * p)
{
	return 0;
}

int parser_leftHandSideExpression(parser_t * p)
{
    int index = p->index;
    int opcodeIndex = p->opcodeIndex;

    if( parser_callExpression(p) ) return 1;
    p->index = index;
    p->opcodeIndex = opcodeIndex;
    if( parser_newExpression(p) ) return 1;
	return 0;
}

int parser_newExpression(parser_t * p)
{
    uint8_t num_of_args;
    uint8_t flag = p->flag;
    if( parser_token(p, "new", 3) ) {
        parser_push_opcode(p, New);
        parser_eat_token(p, 3);
        while(parser_LT(p));
        if( !parser_Identifier(p) ) return 0;
        if( parser_native_get(p, p->id_name, &p->id1) )
            parser_push_opcode_2(p, LoadNative, p->id1, p->id1 >> 8);
        num_of_args = p->num_of_args;
        p->num_of_args = 0;
        parser_flag_set_assign(p);
        if( !parser_arguments(p) ) return 0;
        p->flag = flag;

        parser_push_opcode_1(p, Call, p->num_of_args);

        p->num_of_args = num_of_args;
        parser_push_opcode(p, Sdn);
        while(parser_LT(p));
        return 1;
	}
    if( parser_memberExpression(p, TOKEN_VAR) ) return 1;
	return 0;
}

int parser_memberExpression(parser_t * p, uint8_t type)
{
    uint8_t token = 1;
    if( parser_primaryExpression(p, type) ||
	parser_functionExpression(p) ) {
		while(1) {
			while(parser_LT(p));
			if( !parser_memberExpressionSuffix(p) ) break;
            token = TOKEN_memberExpressionSuffix;
		}
        return token;
	} else if( parser_token(p, "new", 3) ){
		parser_eat_token(p, 3);
		while(parser_LT(p));
        if( !parser_memberExpression(p, TOKEN_NEW) ) return 0;
		while(parser_LT(p));
		if( !parser_arguments(p) ) return 0;
		while(1) {
			while(parser_LT(p));
			if( !parser_memberExpressionSuffix(p) ) break;
		}
		return 1;
	}
	return 0;
}

int parser_memberExpressionSuffix(parser_t * p)
{
	if( parser_indexSuffix(p) ) return 1;
	if( parser_propertyReferenceSuffix(p) ) return 1;
	return 0;
}

int parser_callExpression(parser_t * p)
{
    uint8_t token;
    uint8_t flag = p->flag;
    uint8_t num_of_args;
    token = parser_memberExpression(p, TOKEN_VAR);
    if( !token ) return 0;
	while(parser_LT(p));
    if( !parser_token(p, "(", 1) ) return 0;

    num_of_args = p->num_of_args;
    p->num_of_args = 0;
    parser_flag_set_assign(p);
    if( !parser_arguments(p) ) return 0;
    p->flag = flag;
    if(token == TOKEN_memberExpressionSuffix)
        parser_push_opcode_1(p, CallProp, p->num_of_args);
    else
        parser_push_opcode_1(p, Call, p->num_of_args);
    p->num_of_args = num_of_args;
    if(!parser_flag_is_assign(p))
        parser_push_opcode(p, Sdn);
	while(1) {
		while(parser_LT(p));
		if( !parser_memberExpressionSuffix(p) ) break;
	}
	return 1;
}

int parser_callExpressionSuffix(parser_t * p)
{
	if( parser_arguments(p) ) return 1;
	if( parser_indexSuffix(p) ) return 1;
	if( parser_propertyReferenceSuffix(p) ) return 1;
	return 0;
}

int parser_arguments(parser_t * p)
{
	if( !parser_token(p, "(", 1) ) return 0;
	parser_eat_token(p, 1);
	while(parser_LT(p));
	if( parser_assignmentExpression(p) ) {
        p->num_of_args++;
		while(1) {
			while(parser_LT(p));
            if( !parser_token(p, ",", 1) ) break;
			parser_eat_token(p, 1);
			while(parser_LT(p));
            if( !parser_assignmentExpression(p) ) break;
            p->num_of_args++;
		}
	}	
	while(parser_LT(p));
	if( !parser_token(p, ")", 1) ) return 0;
	parser_eat_token(p, 1);
	return 1;
}

int parser_indexSuffix(parser_t * p)
{
	if( !parser_token(p, "[", 1) ) return 0;
	parser_eat_token(p, 1);
	while(parser_LT(p));
	if( !parser_expression(p) ) return 0;
	while(parser_LT(p));
	if( !parser_token(p, "]", 1) ) return 0;
	parser_eat_token(p, 1);
    parser_push_opcode(p, LoadArray);
	return 1;
}

int parser_propertyReferenceSuffix(parser_t * p)
{
    uint32_t hash;
    if( !parser_token(p, ".", 1) ) return 0;
	parser_eat_token(p, 1);
	while(parser_LT(p));
    if( !parser_Identifier(p) ) return 0;
    hash = parser_hash(p->id_name);
    parser_push_opcode(p, LoadProp);
    parser_push_opcode(p, hash);
    parser_push_opcode(p, hash >> 8);
    parser_push_opcode(p, hash >> 16);
    parser_push_opcode(p, hash >> 24);
	return 1;
}

int parser_assignmentOperator(parser_t * p)
{
    if( parser_token(p, "=", 1) ) {
		parser_eat_token(p, 1);
        p->op = Store;
		return 1;
	} else if( parser_token(p, "*=", 2) ) {
		parser_eat_token(p, 2);
        p->op = StoreMul;
		return 1;
	} else if( parser_token(p, "/=", 2) ) {
		parser_eat_token(p, 2);
        p->op = StoreDiv;
		return 1;
	} else if( parser_token(p, "%=", 2) ) {
		parser_eat_token(p, 2);
        p->op = StoreMod;
		return 1;
	} else if( parser_token(p, "+=", 2) ) {
		parser_eat_token(p, 2);
        p->op = StoreAdd;
		return 1;
	} else if( parser_token(p, "-=", 2) ) {
		parser_eat_token(p, 2);
        p->op = StoreSub;
		return 1;
	} else if( parser_token(p, "<<=", 3) ) {
		parser_eat_token(p, 3);
        p->op = StoreLShift;
		return 1;
	} else if( parser_token(p, ">>=", 3) ) {
		parser_eat_token(p, 3);
        p->op = StoreRShift;
		return 1;
	} else if( parser_token(p, ">>>=", 4) ) {
		parser_eat_token(p, 4);
		return 1;
	} else if( parser_token(p, "&=", 2) ) {
		parser_eat_token(p, 2);
        p->op = StoreAnd;
		return 1;
	} else if( parser_token(p, "^=", 2) ) {
		parser_eat_token(p, 2);
        p->op = StoreXor;
		return 1;
	} else if( parser_token(p, "|=", 2) ) {
		parser_eat_token(p, 2);
        p->op = StoreOr;
		return 1;
	} 
	return 0;
}

int parser_conditionalExpression(parser_t * p)
{
    if( !parser_logicalORExpression(p) ) return 0;
	while(parser_LT(p));
	if( parser_token(p, "?", 1) ) {
		parser_eat_token(p, 1);
		while(parser_LT(p));
		if( !parser_assignmentExpression(p) ) return 1;
		while(parser_LT(p));
		if( !parser_token(p, ":", 1) ) return 1;
		while(parser_LT(p));
		if( !parser_assignmentExpression(p) ) return 1;
	}
	return 1;
}

int parser_conditionalExpressionNoIn(parser_t * p)
{
	return 0;
}

int parser_logicalORExpression(parser_t * p)
{
    uint8_t code[1];
    int status = parser_logicalANDExpression(p);
	while(parser_LT(p));
	if( parser_token(p, "||", 2) ) {
        do {
            parser_eat_token(p, 2);
            while(parser_LT(p));
            if( !parser_logicalANDExpression(p) ) return 1;
            while(parser_LT(p));
            if( !parser_token(p, "||", 2) ) return 1;
            parser_push_opcode(p, Or);
        }while(1);
	}
    return status;
}

int parser_logicalORExpressionNoIn(parser_t * p)
{
	return 0;
}

int parser_logicalANDExpression(parser_t * p)
{    
    int status = parser_bitwiseORExpression(p);
    while(parser_LT(p));
    if( parser_token(p, "&&", 2) ) {
        do {
            parser_eat_token(p, 2);
            while(parser_LT(p));
            if( !parser_bitwiseORExpression(p) ) return 1;
            while(parser_LT(p));
            if( !parser_token(p, "&&", 2) ) return 1;
            parser_push_opcode(p, And);
        }while(1);
    }
    return status;
}

int parser_logicalANDExpressionNoIn(parser_t * p)
{
	return 0;
}

int parser_bitwiseORExpression(parser_t * p)
{
    int status = parser_bitwiseXORExpression(p);
    while(parser_LT(p));
    if( parser_token(p, "|", 1) ) {
        do {
            parser_eat_token(p, 1);
            while(parser_LT(p));

            if( !parser_bitwiseXORExpression(p) ) return 1;
            parser_push_opcode(p, Or);
            while(parser_LT(p));
            if( !parser_token(p, "|", 1) ) return 1;

        }while(1);
    }
    return status;
}

int parser_bitwiseORExpressionNoIn(parser_t * p)
{
	return 0;
}

int parser_bitwiseXORExpression(parser_t * p)
{
    uint8_t code[1];
    int status = parser_bitwiseANDExpression(p);
    while(parser_LT(p));
    if( parser_token(p, "^", 1) ) {
        do {
            parser_eat_token(p, 1);
            while(parser_LT(p));
            if( !parser_bitwiseANDExpression(p) ) return 1;
            while(parser_LT(p));
            if( !parser_token(p, "^", 1) ) return 1;
            code[0] = Xor;
            parser_append_opcode(p, code, 1);
        }while(1);
    }
    return status;
}

int parser_bitwiseXORExpressionNoIn(parser_t * p)
{
	return 0;
}

int parser_bitwiseANDExpression(parser_t * p)
{
    int status = parser_equalityExpression(p);
    while(parser_LT(p));
    if( parser_token(p, "&", 1) ) {
        do {
            parser_eat_token(p, 1);
            while(parser_LT(p));

            if( !parser_equalityExpression(p) ) return 1;
            parser_push_opcode(p, And);
            while(parser_LT(p));
            if( !parser_token(p, "&", 1) ) return 1;
        }while(1);
    }
    return status;
}

int parser_bitwiseANDExpressionNoIn(parser_t * p)
{
	return 0;
}

int parser_equalityExpression(parser_t * p)
{
    char sign;
    int len = 0;
    int status = parser_relationalExpression(p);
    while(parser_LT(p));

    if( parser_token(p, "==", 2) ) {
        len = 2;
        sign = TOKEN_EQ;
    } else if( parser_token(p, "!=", 2) ) {
        len = 2;
        sign = TOKEN_NOTEQ;
    } else if( parser_token(p, "===", 3) ) {
        len = 3;
        sign = TOKEN_TYPE_EQ;
    } else if( parser_token(p, "!==", 3) ) {
        len = 3;
        sign = TOKEN_TYPE_NOTEQ;
    } else {
        sign = TOKEN_INVALID;
    }

    if( sign ) {
        do {
            parser_eat_token(p, len);
            while(parser_LT(p));
            if( !parser_relationalExpression(p) ) return 1;

            switch (sign) {
            case TOKEN_EQ:          parser_push_opcode(p, Eq);break;
            case TOKEN_NOTEQ:       parser_push_opcode(p, NotEq);break;
            case TOKEN_TYPE_EQ:     parser_push_opcode(p, Eq);break;
            case TOKEN_TYPE_NOTEQ:  parser_push_opcode(p, Eq);break;
            default:
                break;
            }
            while(parser_LT(p));
            if( parser_token(p, "==", 2) ) {
                len = 2;
                sign = TOKEN_EQ;
            } else if( parser_token(p, "!=", 2) ) {
                len = 2;
                sign = TOKEN_NOTEQ;
            } else if( parser_token(p, "===", 3) ) {
                len = 3;
                sign = TOKEN_TYPE_EQ;
            } else if( parser_token(p, "!==", 3) ) {
                len = 3;
                sign = TOKEN_TYPE_NOTEQ;
            } else {
                sign = TOKEN_INVALID;
                return 1;
            }
        }while(1);
    }

    return status;
}

int parser_equalityExpressionNoIn(parser_t * p)
{
	return 0;
}

int parser_relationalExpression(parser_t * p)
{
    char sign;
	int len = 0;
    int status = parser_shiftExpression(p);
	while(parser_LT(p));
	
    if( parser_token(p, "<=", 2) ) {
        len = 2;
        sign = TOKEN_LTEQ;
    } else if( parser_token(p, ">=", 2) ) {
        len = 2;
        sign = TOKEN_GTEQ;
    } else if( parser_token(p, "<", 1) ) {
        len = 1;
        sign = TOKEN_LT;
    } else if( parser_token(p, ">", 1) ) {
        len = 1;
        sign = TOKEN_GT;
    } else {
        sign = TOKEN_INVALID;
    }

    if( sign ) {
        do {
            parser_eat_token(p, len);
            while(parser_LT(p));
            if( !parser_shiftExpression(p) ) return 1;
            switch (sign) {
            case TOKEN_LT:      parser_push_opcode(p, Lt);break;
            case TOKEN_GT:      parser_push_opcode(p, Gt);break;
            case TOKEN_LTEQ:    parser_push_opcode(p, LtEq);break;
            case TOKEN_GTEQ:    parser_push_opcode(p, GtEq);break;
            default:
                break;
            }
            while(parser_LT(p));
            if( parser_token(p, "<=", 2) ) {
                len = 2;
                sign = TOKEN_LTEQ;
            } else if( parser_token(p, ">=", 2) ) {
                len = 2;
                sign = TOKEN_GTEQ;
            } else if( parser_token(p, "<", 1) ) {
                len = 1;
                sign = TOKEN_LT;
            } else if( parser_token(p, ">", 1) ) {
                len = 1;
                sign = TOKEN_GT;
            } else {
                sign = TOKEN_INVALID;
                return 1;
            }
        }while(1);
    }

    return status;
}

int parser_shiftExpression(parser_t * p)
{
    char flag = 1;
    int len = 0;
    int status = parser_additiveExpression(p);
    while(parser_LT(p));

    if( parser_token(p, ">>>", 3) ) {
        len = 3;
    } else if( parser_token(p, "<<", 2) ) {
        len = 2;
    } else if( parser_token(p, ">>", 2) ) {
        len = 2;
    } else {
        flag = 0;
    }

    if( flag ) {
        do {
            parser_eat_token(p, len);
            while(parser_LT(p));
            if( !parser_additiveExpression(p) ) return 1;
            while(parser_LT(p));
            if( parser_token(p, ">>>", 3) ) {
                    len = 3;
                } else if( parser_token(p, "<<", 2) ) {
                len = 2;
            } else if( parser_token(p, ">>", 2) ) {
                len = 2;
            } else {
                flag = 0;
                return 1;
            }
        }while(1);
    }

    return status;
}

int parser_additiveExpression(parser_t * p)
{
    char sign;
    int len = 0;
    int status = parser_multiplicativeExpression(p);
    while(parser_LT(p));

    if( parser_token(p, "+", 1) ) {
        len = 1;
        sign = '+';
    } else if( parser_token(p, "-", 1) ) {
        len = 1;
        sign = '-';
    } else {
        sign = 0;
    }

    if( sign ) {
        do {
            parser_eat_token(p, len);
            while(parser_LT(p));
            if( !parser_multiplicativeExpression(p) ) return 1;
            if(sign == '+')
                parser_push_opcode(p, Add);
            else if(sign == '-')
                parser_push_opcode(p, Sub);
            while(parser_LT(p));
            if( parser_token(p, "+", 1) ) {
                len = 1;
                sign = '+';
            } else if( parser_token(p, "-", 1) ) {
                len = 1;
                sign = '-';
            } else {
                sign = 0;
                return 1;
            }
        }while(1);
    }

    return status;
}

int parser_multiplicativeExpression(parser_t * p)
{
    char sign;
    int len = 0;
    int status = parser_unaryExpression(p);
    while(parser_LT(p));

    if( parser_token(p, "*", 1) ) {
        len = 1;
        sign = '*';
    } else if( parser_token(p, "/", 1) ) {
        len = 1;
        sign = '/';
    } else if( parser_token(p, "%", 1) ) {
        len = 1;
        sign = '%';
    } else {
        sign = 0;
    }

    if( sign ) {
        do {
            parser_eat_token(p, len);
            while(parser_LT(p));

            if( !parser_unaryExpression(p) ) return 1;
            while(parser_LT(p));
            if(sign == '*')
                parser_push_opcode(p, Mul);
            else if(sign == '/')
                parser_push_opcode(p, Div);
            else if(sign == '%')
                parser_push_opcode(p, Mod);
            if( parser_token(p, "*", 1) ) {
                len = 1;
                sign = '*';
            } else if( parser_token(p, "/", 1) ) {
                len = 1;
                sign = '/';
            } else if( parser_token(p, "%", 1) ) {
                len = 1;
                sign = '%';
            } else {
                sign = 0;
                return 1;
            }
        }while(1);
    }

    return status;
}

int parser_unaryExpression(parser_t * p)
{
    if( parser_postfixExpression(p) ) return 1;
	if( parser_token(p, "delete", 6) ) {
		parser_eat_token(p, 6);
		return 1;
	} else if( parser_token(p, "void", 4) ) {
		parser_eat_token(p, 4);
		return 1;
	} else if( parser_token(p, "typeof", 6) ) {
		parser_eat_token(p, 6);
        p->token = TOKEN_TYPEOF;
		return 1;
	} else if( parser_token(p, "++", 2) ) {
        parser_eat_token(p, 2);
        p->token = TOKEN_INC;
		return 1;
	} else if( parser_token(p, "--", 2) ) {
		parser_eat_token(p, 2);
        p->token = TOKEN_DEC;
		return 1;
	} else if( parser_token(p, "+", 1) ) {
		parser_eat_token(p, 1);
        p->token = TOKEN_ADD;
		return 1;
	} else if( parser_token(p, "-", 1) ) {
		parser_eat_token(p, 1);
        p->token = TOKEN_NEG;
		return 1;
	} else if( parser_token(p, "~", 1) ) {
		parser_eat_token(p, 1);
		return 1;
	} else if( parser_token(p, "!", 1) ) {
		parser_eat_token(p, 1);
        p->token = TOKEN_NOT;
		return 1;
	} 
	return 0;
}

int parser_postfixExpression(parser_t * p)
{
	if( parser_leftHandSideExpression(p) ) {
		if( parser_token(p, "++", 2) ) {
			parser_eat_token(p, 2);
            parser_push_opcode(p, Inc);
		} else if( parser_token(p, "--", 2) ) {
			parser_eat_token(p, 2);
            parser_push_opcode(p, Dec);
		} 
		return 1;
	}
	return 0;
}

int parser_primaryExpression(parser_t * p, uint8_t type)
{
    uint8_t scope;
    if( parser_token(p, "this", 4) ) {
		parser_eat_token(p, 4);
		return 1;
    } else if( parser_literal(p) ) {
        return 1;
    } else if( parser_arrayLiteral(p) ) {
		return 1;
	} else if( parser_objectLiteral(p) ) {
		return 1;
    } else if( parser_Identifier(p) ) {
        if(type == TOKEN_VAR)
            scope = parser_var_get(p, p->id_name, &p->id1, &p->id2);
            if( scope == 0 ) {
                if( parser_func_get(p, p->id_name, &p->id1) )
                    parser_push_opcode_2(p, LoadFunc, p->id1, p->id1 >> 8);
                else if( parser_native_get(p, p->id_name, &p->id1) )
                    parser_push_opcode_2(p, LoadNative, p->id1, p->id1 >> 8);
                else
                    parser_push_opcode(p, LoadUndef);
            } else {
                if(p->token == TOKEN_INC)
                    parser_push_opcode(p, Inc);
                else if(p->token == TOKEN_DEC)
                    parser_push_opcode_2(p, Dec, p->id1, p->id2);

                if(scope == 2)
                {
                    parser_push_opcode_2(p, LoadLocalVar, p->id1, p->id2);
                }
                else
                {
                    parser_push_opcode_2(p, LoadVar, p->id1, p->id1 >> 8);
                }
                if(p->token == TOKEN_NOT)
                    parser_push_opcode(p, Not);
                else if(p->token == TOKEN_NEG)
                    parser_push_opcode(p, Neg);
            }
        p->token = 0;
        return 1;
    } else if( parser_token(p, "(", 1) ) {
		parser_eat_token(p, 1);
		while(parser_LT(p));
		if( !parser_expression(p) ) return 0;
		while(parser_LT(p));
		if( !parser_token(p, ")", 1) ) return 0;
		parser_eat_token(p, 1);
		return 1;
	} 
	return 0;
}

int parser_arrayLiteral(parser_t * p)
{
	if( !parser_token(p, "[", 1) ) return 0;
	parser_eat_token(p, 1);
	while(parser_LT(p));
	parser_assignmentExpression(p);
	while(parser_LT(p));
	while(1) {
		while(parser_LT(p));
		if( !parser_token(p, ",", 1) ) break;
		parser_eat_token(p, 1);
		while(parser_LT(p));
		parser_assignmentExpression(p);
	}
	while(parser_LT(p));
	if( !parser_token(p, "]", 1) ) return 0;
	parser_eat_token(p, 1);
	return 1;
}

int parser_objectLiteral(parser_t * p)
{
	if( !parser_token(p, "{", 1) ) return 0;
	parser_eat_token(p, 1);
	while(parser_LT(p));
	if( !parser_propertyNameAndValue(p) ) return 0;
	while(parser_LT(p));
	while(1) {
		while(parser_LT(p));
		if( !parser_token(p, ",", 1) ) break;
		parser_eat_token(p, 1);
		while(parser_LT(p));
		if( !parser_propertyNameAndValue(p) ) break;
	}
	while(parser_LT(p));
	if( !parser_token(p, "}", 1) ) return 0;
	parser_eat_token(p, 1);
	return 1;
}

int parser_propertyNameAndValue(parser_t * p)
{
	if( !parser_propertyName(p) ) return 0;
	while(parser_LT(p));
	if( !parser_token(p, ":", 1) ) return 0;
	parser_eat_token(p, 1);
	while(parser_LT(p));
	if( !parser_assignmentExpression(p) ) return 0;
	return 1;
}

int parser_propertyName(parser_t * p)
{
    if( parser_Identifier(p) )
		return 1;
	else if( parser_StringLiteral(p) )
		return 1;
	else if( parser_NumericLiteral(p) )
		return 1;
	return 0;
}

int parser_literal(parser_t * p)
{
    uint8_t code[5];
    if( parser_token(p, "null", 4) ) {
		parser_eat_token(p, 4);
        code[0] = LoadNull;
        parser_append_opcode(p, code, 1);
		return 1;
	}
	else if( parser_token(p, "true", 4) ) {
		parser_eat_token(p, 4);
        code[0] = LoadTrue;
        parser_append_opcode(p, code, 1);
		return 1;
	}
	else if( parser_token(p, "false", 5) ) {
		parser_eat_token(p, 5);
        code[0] = LoadFalse;
        parser_append_opcode(p, code, 1);
		return 1;
	}
	else if( parser_StringLiteral(p) )
    {
        code[0] = LoadStr;
        code[1] = p->id1;
        code[2] = p->id1 >> 8;
        parser_append_opcode(p, code, 3);
        return 1;
    }
	else if( parser_NumericLiteral(p) )
    {
        parser_push_opcode(p, LoadNum);
        parser_push_opcode(p, p->num.ival >> 0);
        parser_push_opcode(p, p->num.ival >> 8);
        parser_push_opcode(p, p->num.ival >> 16);
        parser_push_opcode(p, p->num.ival >> 24);
        return 1;
    }
	return 0;
}

int parser_StringLiteral(parser_t * p)
{
	char ch;
    int len = 0;
    char str[100];

	if( parser_token(p, "\"", 1) ) {
		parser_eat_token(p, 1);
		do {
            ch = parser_next_ch(p, p->index++);
            str[len++] = ch;
		} while( ch != '\"' );
        str[len - 1] = 0;
        parser_str_put(p, str, &p->id1);
		return 1;
	} else if( parser_token(p, "'", 1) ) {
		parser_eat_token(p, 1);
		do {
            ch = parser_next_ch(p, p->index++);
            str[len++] = ch;
		} while( ch != '\'' );
        str[len - 1] = 0;
        parser_str_put(p, str, &p->id1);
		return 1;
	}
	return 0;
}

int parser_NumericLiteral(parser_t * p)
{
	char ch;
    char str[100];
    int len = 0;
	int index = p->index;
	if( isdigit( parser_next_ch(p, index) ) ) {
		do {
			index =  p->index;
			ch = parser_next_ch(p, index);
			if( !isdigit(ch) ) break;
            str[len++] = ch;
			p->index++;
		} while(isdigit(ch));
		
		if( parser_token(p, ".", 1) ) {
            str[len++] = '.';
            parser_eat_token(p, 1);
			do {
				index =  p->index;
                ch = parser_next_ch(p, index);
                if( !isdigit(ch) ) break;
                str[len++] = ch;
				p->index++;
			} while(isdigit(ch));
		}
        str[len++] = 0;

        p->num.fval = atof(str);
		return 1;
	}
	return 0;
}

int parser_Identifier(parser_t * p)
{
	char ch;
	int index;
	int len = 0;
    char * name = p->id_name;
	
    if(isdigit(parser_next_ch(p, p->index)))
        return 0;

	while(1) {
		index = p->index;
        ch = parser_next_ch(p, index);

		if( isalnum(ch) || '_' == ch || '$' == ch ) {
            p->index++;
            name[len] = ch;
            len++;
        } else {
            if(len)
            {
                name[len] = 0;
                return 1;
            }
            else
                return 0;
        }
	}
}

int parser_Comment(parser_t * p)
{
	if( parser_token(p, "/*", 2) ) {
		parser_eat_token(p, 2); 
		
		do {
			if( parser_token(p, "*/", 2) ) {
				parser_eat_token(p, 2); 
				return 1;
			}
            parser_next_ch(p, p->index++);
		} while(1);
	} else
		return 0;
}

int parser_LineComment(parser_t * p)
{
    if( parser_token(p, "//", 2) ) {
        parser_eat_token(p, 2);
        while(parser_LT(p));
		do {
            if( parser_token(p, "\n", 1) ) {
                parser_eat_token(p, 1);
                return 1;
            }
            parser_next_ch(p, p->index++);
		} while(1);
	} else
		return 0;
}
 
