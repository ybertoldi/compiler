#include "../includes/grammar.h"
#include <assert.h>
long goto_table[58][VAR_NUM_VARS] = {
	[1] = { [VAR_STMTS] = 2
, [VAR_STMT] = 3
, [VAR_DEC] = 4
, [VAR_TIPO] = 5
, [TKTYPE_T_INT] = 6
, [TKTYPE_T_CHAR] = 7
, [TKTYPE_T_BOOL] = 8
, [TKTYPE_T_LONG] = 9
, [TKTYPE_T_FLOAT] = 10
, [TKTYPE_T_DOUBLE] = 11
 },
	[2] = { [VAR_STMT] = 12
, [VAR_DEC] = 4
, [VAR_TIPO] = 5
, [TKTYPE_T_INT] = 6
, [TKTYPE_T_CHAR] = 7
, [TKTYPE_T_BOOL] = 8
, [TKTYPE_T_LONG] = 9
, [TKTYPE_T_FLOAT] = 10
, [TKTYPE_T_DOUBLE] = 11
 },
	[3] = {  },
	[4] = {  },
	[5] = { [TKTYPE_VAR] = 13
 },
	[6] = {  },
	[7] = {  },
	[8] = {  },
	[9] = {  },
	[10] = {  },
	[11] = {  },
	[12] = {  },
	[13] = { [TKTYPE_ATTRIBUTION] = 14
 },
	[14] = { [TKTYPE_VAR] = 15
, [VAR_EXPR] = 16
, [VAR_EXPR1] = 17
, [VAR_EXPR2] = 18
, [VAR_EXPR3] = 19
, [VAR_EXPR4] = 20
, [VAR_EXPR5] = 21
, [VAR_EXPR6] = 22
, [VAR_CONST] = 23
, [TKTYPE_OPPAREN] = 24
, [TKTYPE_INT] = 25
, [TKTYPE_FLOAT] = 26
, [TKTYPE_HEXINT] = 27
, [TKTYPE_OCTALINT] = 28
, [TKTYPE_FALSE] = 29
, [TKTYPE_TRUE] = 30
 },
	[15] = {  },
	[16] = { [TKTYPE_SMCOLON] = 31
, [VAR_COND1] = 32
, [TKTYPE_LOGIC_OR] = 33
 },
	[17] = { [VAR_COND2] = 34
, [TKTYPE_LOGIC_AND] = 35
 },
	[18] = { [VAR_REL1] = 36
, [TKTYPE_EQUALS] = 37
, [TKTYPE_NEQUALS] = 38
 },
	[19] = { [VAR_REL2] = 39
, [TKTYPE_LT] = 40
, [TKTYPE_LEQT] = 41
, [TKTYPE_GT] = 42
, [TKTYPE_GEQT] = 43
 },
	[20] = { [VAR_OP1] = 44
, [TKTYPE_SUM] = 45
, [TKTYPE_SUB] = 46
 },
	[21] = { [VAR_OP2] = 47
, [TKTYPE_MULT] = 48
, [TKTYPE_DIV] = 49
 },
	[22] = {  },
	[23] = {  },
	[24] = { [TKTYPE_VAR] = 15
, [VAR_EXPR] = 50
, [VAR_EXPR1] = 17
, [VAR_EXPR2] = 18
, [VAR_EXPR3] = 19
, [VAR_EXPR4] = 20
, [VAR_EXPR5] = 21
, [VAR_EXPR6] = 22
, [VAR_CONST] = 23
, [TKTYPE_OPPAREN] = 24
, [TKTYPE_INT] = 25
, [TKTYPE_FLOAT] = 26
, [TKTYPE_HEXINT] = 27
, [TKTYPE_OCTALINT] = 28
, [TKTYPE_FALSE] = 29
, [TKTYPE_TRUE] = 30
 },
	[25] = {  },
	[26] = {  },
	[27] = {  },
	[28] = {  },
	[29] = {  },
	[30] = {  },
	[31] = {  },
	[32] = { [TKTYPE_VAR] = 15
, [VAR_EXPR1] = 51
, [VAR_EXPR2] = 18
, [VAR_EXPR3] = 19
, [VAR_EXPR4] = 20
, [VAR_EXPR5] = 21
, [VAR_EXPR6] = 22
, [VAR_CONST] = 23
, [TKTYPE_OPPAREN] = 24
, [TKTYPE_INT] = 25
, [TKTYPE_FLOAT] = 26
, [TKTYPE_HEXINT] = 27
, [TKTYPE_OCTALINT] = 28
, [TKTYPE_FALSE] = 29
, [TKTYPE_TRUE] = 30
 },
	[33] = {  },
	[34] = { [TKTYPE_VAR] = 15
, [VAR_EXPR2] = 52
, [VAR_EXPR3] = 19
, [VAR_EXPR4] = 20
, [VAR_EXPR5] = 21
, [VAR_EXPR6] = 22
, [VAR_CONST] = 23
, [TKTYPE_OPPAREN] = 24
, [TKTYPE_INT] = 25
, [TKTYPE_FLOAT] = 26
, [TKTYPE_HEXINT] = 27
, [TKTYPE_OCTALINT] = 28
, [TKTYPE_FALSE] = 29
, [TKTYPE_TRUE] = 30
 },
	[35] = {  },
	[36] = { [TKTYPE_VAR] = 15
, [VAR_EXPR3] = 53
, [VAR_EXPR4] = 20
, [VAR_EXPR5] = 21
, [VAR_EXPR6] = 22
, [VAR_CONST] = 23
, [TKTYPE_OPPAREN] = 24
, [TKTYPE_INT] = 25
, [TKTYPE_FLOAT] = 26
, [TKTYPE_HEXINT] = 27
, [TKTYPE_OCTALINT] = 28
, [TKTYPE_FALSE] = 29
, [TKTYPE_TRUE] = 30
 },
	[37] = {  },
	[38] = {  },
	[39] = { [TKTYPE_VAR] = 15
, [VAR_EXPR4] = 54
, [VAR_EXPR5] = 21
, [VAR_EXPR6] = 22
, [VAR_CONST] = 23
, [TKTYPE_OPPAREN] = 24
, [TKTYPE_INT] = 25
, [TKTYPE_FLOAT] = 26
, [TKTYPE_HEXINT] = 27
, [TKTYPE_OCTALINT] = 28
, [TKTYPE_FALSE] = 29
, [TKTYPE_TRUE] = 30
 },
	[40] = {  },
	[41] = {  },
	[42] = {  },
	[43] = {  },
	[44] = { [TKTYPE_VAR] = 15
, [VAR_EXPR5] = 55
, [VAR_EXPR6] = 22
, [VAR_CONST] = 23
, [TKTYPE_OPPAREN] = 24
, [TKTYPE_INT] = 25
, [TKTYPE_FLOAT] = 26
, [TKTYPE_HEXINT] = 27
, [TKTYPE_OCTALINT] = 28
, [TKTYPE_FALSE] = 29
, [TKTYPE_TRUE] = 30
 },
	[45] = {  },
	[46] = {  },
	[47] = { [TKTYPE_VAR] = 15
, [VAR_EXPR6] = 56
, [VAR_CONST] = 23
, [TKTYPE_OPPAREN] = 24
, [TKTYPE_INT] = 25
, [TKTYPE_FLOAT] = 26
, [TKTYPE_HEXINT] = 27
, [TKTYPE_OCTALINT] = 28
, [TKTYPE_FALSE] = 29
, [TKTYPE_TRUE] = 30
 },
	[48] = {  },
	[49] = {  },
	[50] = { [VAR_COND1] = 32
, [TKTYPE_CLPAREN] = 57
, [TKTYPE_LOGIC_OR] = 33
 },
	[51] = { [VAR_COND2] = 34
, [TKTYPE_LOGIC_AND] = 35
 },
	[52] = { [VAR_REL1] = 36
, [TKTYPE_EQUALS] = 37
, [TKTYPE_NEQUALS] = 38
 },
	[53] = { [VAR_REL2] = 39
, [TKTYPE_LT] = 40
, [TKTYPE_LEQT] = 41
, [TKTYPE_GT] = 42
, [TKTYPE_GEQT] = 43
 },
	[54] = { [VAR_OP1] = 44
, [TKTYPE_SUM] = 45
, [TKTYPE_SUB] = 46
 },
	[55] = { [VAR_OP2] = 47
, [TKTYPE_MULT] = 48
, [TKTYPE_DIV] = 49
 },
	[56] = {  },
	[57] = {  },
};

#define PROD(tgt, ...) (Production) \
                {.target = tgt, .elements = {__VA_ARGS__}, .num_elems = (sizeof((long[]){__VA_ARGS__})/sizeof(long))}
Production reduce[58] = {
	[3] = PROD(VAR_STMTS,   VAR_STMT),
	[4] = PROD(VAR_STMT,   VAR_DEC),
	[6] = PROD(VAR_TIPO,   TKTYPE_T_INT),
	[7] = PROD(VAR_TIPO,   TKTYPE_T_CHAR),
	[8] = PROD(VAR_TIPO,   TKTYPE_T_BOOL),
	[9] = PROD(VAR_TIPO,   TKTYPE_T_LONG),
	[10] = PROD(VAR_TIPO,   TKTYPE_T_FLOAT),
	[11] = PROD(VAR_TIPO,   TKTYPE_T_DOUBLE),
	[12] = PROD(VAR_STMTS,   VAR_STMTS, VAR_STMT),
	[15] = PROD(VAR_EXPR6,   TKTYPE_VAR),
	[17] = PROD(VAR_EXPR,   VAR_EXPR1),
	[18] = PROD(VAR_EXPR1,   VAR_EXPR2),
	[19] = PROD(VAR_EXPR2,   VAR_EXPR3),
	[20] = PROD(VAR_EXPR3,   VAR_EXPR4),
	[21] = PROD(VAR_EXPR4,   VAR_EXPR5),
	[22] = PROD(VAR_EXPR5,   VAR_EXPR6),
	[23] = PROD(VAR_EXPR6,   VAR_CONST),
	[25] = PROD(VAR_CONST,   TKTYPE_INT),
	[26] = PROD(VAR_CONST,   TKTYPE_FLOAT),
	[27] = PROD(VAR_CONST,   TKTYPE_HEXINT),
	[28] = PROD(VAR_CONST,   TKTYPE_OCTALINT),
	[29] = PROD(VAR_CONST,   TKTYPE_FALSE),
	[30] = PROD(VAR_CONST,   TKTYPE_TRUE),
	[31] = PROD(VAR_DEC,   VAR_TIPO, TKTYPE_VAR, TKTYPE_ATTRIBUTION, VAR_EXPR, TKTYPE_SMCOLON),
	[33] = PROD(VAR_COND1,   TKTYPE_LOGIC_OR),
	[35] = PROD(VAR_COND2,   TKTYPE_LOGIC_AND),
	[37] = PROD(VAR_REL1,   TKTYPE_EQUALS),
	[38] = PROD(VAR_REL1,   TKTYPE_NEQUALS),
	[40] = PROD(VAR_REL2,   TKTYPE_LT),
	[41] = PROD(VAR_REL2,   TKTYPE_LEQT),
	[42] = PROD(VAR_REL2,   TKTYPE_GT),
	[43] = PROD(VAR_REL2,   TKTYPE_GEQT),
	[45] = PROD(VAR_OP1,   TKTYPE_SUM),
	[46] = PROD(VAR_OP1,   TKTYPE_SUB),
	[48] = PROD(VAR_OP2,   TKTYPE_MULT),
	[49] = PROD(VAR_OP2,   TKTYPE_DIV),
	[51] = PROD(VAR_EXPR,   VAR_EXPR, VAR_COND1, VAR_EXPR1),
	[52] = PROD(VAR_EXPR1,   VAR_EXPR1, VAR_COND2, VAR_EXPR2),
	[53] = PROD(VAR_EXPR2,   VAR_EXPR2, VAR_REL1, VAR_EXPR3),
	[54] = PROD(VAR_EXPR3,   VAR_EXPR3, VAR_REL2, VAR_EXPR4),
	[55] = PROD(VAR_EXPR4,   VAR_EXPR4, VAR_OP1, VAR_EXPR5),
	[56] = PROD(VAR_EXPR5,   VAR_EXPR5, VAR_OP2, VAR_EXPR6),
	[57] = PROD(VAR_EXPR6,   TKTYPE_OPPAREN, VAR_EXPR, TKTYPE_CLPAREN),
};
char *var2str(VARTYPE t){
  switch(t){
    case VAR_STMTS: return "VAR_STMTS";
    case VAR_STMT: return "VAR_STMT";
    case VAR_DEC: return "VAR_DEC";
    case VAR_EXPR: return "VAR_EXPR";
    case VAR_EXPR1: return "VAR_EXPR1";
    case VAR_EXPR2: return "VAR_EXPR2";
    case VAR_EXPR3: return "VAR_EXPR3";
    case VAR_EXPR4: return "VAR_EXPR4";
    case VAR_EXPR5: return "VAR_EXPR5";
    case VAR_EXPR6: return "VAR_EXPR6";
    case VAR_COND1: return "VAR_COND1";
    case VAR_COND2: return "VAR_COND2";
    case VAR_REL1: return "VAR_REL1";
    case VAR_REL2: return "VAR_REL2";
    case VAR_OP1: return "VAR_OP1";
    case VAR_OP2: return "VAR_OP2";
    case VAR_CONST: return "VAR_CONST";
    case VAR_TIPO: return "VAR_TIPO";
    default: assert(0 && "type2str: invalid value");
  }
}
