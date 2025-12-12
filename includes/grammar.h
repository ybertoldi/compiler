#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "tokenizer.h"


typedef struct {
  long target;
  long elements[256];
  long num_elems;
} Production;

#define START_SYMBOL VAR_STMTS
typedef enum {	VAR_STMTS = TKTYPE_NUM_TOKENS,
	VAR_STMT,
	VAR_DEC,
	VAR_EXPR,
	VAR_LOGIC_OR,
	VAR_EXPR1,
	VAR_LOGIC_AND,
	VAR_EXPR2,
	VAR_CMP_NEQ,
	VAR_CMP_EQ,
	VAR_EXPR3,
	VAR_CMP_LT,
	VAR_CMP_LEQT,
	VAR_CMP_GT,
	VAR_CMP_GEQT,
	VAR_EXPR4,
	VAR_MULT,
	VAR_DIV,
	VAR_EXPR5,
	VAR_SUM,
	VAR_SUB,
	VAR_EXPR6,
	VAR_CONST,
	VAR_TIPO,
	VAR_NUM_VARS} VARTYPE;



char *var2str(VARTYPE t);

extern long goto_table[70][VAR_NUM_VARS];
extern Production rdc_table[70];
#endif // GRAMMAR_H