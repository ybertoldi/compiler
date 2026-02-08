#define TKL_D
#include "../includes/dfa_node.h"

#ifndef TOKENIZER_H
#define TOKENIZER_H 

typedef  enum {
  TKTYPE_INIT = 0,
  TKTYPE_WTSP,

  //SEPARADORES
  TKTYPE_OPPAREN,
  TKTYPE_CLPAREN,
  TKTYPE_OPBRACKET,
  TKTYPE_CLBRACKET,
  TKTYPE_OPSQRBRACK,
  TKTYPE_CLSQRBRACK,
  TKTYPE_COMMA,
  TKTYPE_SMCOLON,
  TKTYPE_SINGLE_QUOTE,
  TKTYPE_DOUBLE_QUOTE,

  //OPERADORES ARITIMETICOS
  TKTYPE_SUM,
  TKTYPE_SUB,
  TKTYPE_DIV,
  TKTYPE_MULT, // mesmo token para derefenciar um ponteiro
  TKTYPE_MOD,
  TKTYPE_BITWISE_AND, // mesmo token para pegar o endereco de memoria de uma variavel (&)
  TKTYPE_BITWISE_OR,
  TKTYPE_BITWISE_SHFTL,
  TKTYPE_BITWISE_SHFTR,

  //OPERADORES LOGICOS
  TKTYPE_EQUALS,
  TKTYPE_NEQUALS,
  TKTYPE_GT,
  TKTYPE_GEQT,
  TKTYPE_LT,
  TKTYPE_LEQT,
  TKTYPE_LOGIC_OR,
  TKTYPE_LOGIC_AND,
  TKTYPE_NEGATION, //negacao em bitwise e negacao logica sao iguais
                   
  //VARIAVEIS E NUMS
  TKTYPE_NAME,
  TKTYPE_ATTRIBUTION,
  TKTYPE_INT,
  TKTYPE_FLOAT,
  TKTYPE_HEXINT,
  TKTYPE_OCTALINT,

  //PALAVRAS RESERVADAS
  TKTYPE_KW_IF,
  TKTYPE_KW_ELSE,

  TKTYPE_KW_FOR,
  TKTYPE_KW_WHILE,
  TKTYPE_KW_BREAK,
  TKTYPE_KW_CONTINUE,

  TKTYPE_KW_SWITCH,
  TKTYPE_KW_CASE,
  TKTYPE_KW_FALSE,
  TKTYPE_KW_TRUE,

  //PALAVRAS RESERVADAS, TIPOS
  TKTYPE_KW_INT,
  TKTYPE_KW_CHAR,
  TKTYPE_KW_BOOL,
  TKTYPE_KW_LONG,
  TKTYPE_KW_DOUBLE,
  TKTYPE_KW_FLOAT,
  TKTYPE_KW_VOID,

  // STRING e CHAR
  TKTYPE_STRLIT,

  // sentinela para contar o numero de tokens
  TKTYPE_NUM_TOKENS
}TokenType;


// STRUCTS
typedef struct tk{
  char *lexeme;
  TokenType type;
  struct tk *next;
} TokenList;


// FUNCOES
TokenList *tokenize(char *eval);
char *type2str(TokenType t);

// macros para printar
#ifdef TOKENIZER_H_DEBUG
  #define TKL_FMT "(%s, %s)"
  #define TKL_ARGS(tkl) type2str(tkl->type), tkl->lexeme
#endif // TOKENIZER_H_DEBUG

#endif //TOKENIZER_H
