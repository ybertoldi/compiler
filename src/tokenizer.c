#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../includes/tokenizer.h"

#define KWORD(kw, token)\
  {sizeof(kw) -1, (uintptr_t)kw, token}

#define KWORD_COUNT (sizeof(keywords) / (sizeof(uintptr_t) * 3))

static uintptr_t keywords[][3] = {
    KWORD("if",     TKTYPE_KW_IF),
    KWORD("else",   TKTYPE_KW_ELSE),
    KWORD("switch", TKTYPE_KW_SWITCH),
    KWORD("case",   TKTYPE_KW_CASE),

    KWORD("for",      TKTYPE_KW_FOR),
    KWORD("while",    TKTYPE_KW_WHILE),
    KWORD("break",    TKTYPE_KW_BREAK),
    KWORD("continue", TKTYPE_KW_CONTINUE),

    KWORD("char",   TKTYPE_KW_CHAR),
    KWORD("int",    TKTYPE_KW_INT),
    KWORD("long",   TKTYPE_KW_LONG),
    KWORD("float",  TKTYPE_KW_FLOAT),
    KWORD("double", TKTYPE_KW_DOUBLE),
    KWORD("bool",   TKTYPE_KW_BOOL),
    KWORD("void",   TKTYPE_KW_VOID),

    KWORD("false",  TKTYPE_KW_FALSE),
    KWORD("true",   TKTYPE_KW_TRUE),
};

static TokenType match_var(const char *val){
  size_t val_len = strlen(val);

  for (int i = 0; i < KWORD_COUNT; i++){
    size_t kwlen = keywords[i][0];
    char *kw = (char *) keywords[i][1];
    TokenType kw_token = keywords[i][2];

    if (val_len == kwlen && strcmp(val, kw) == 0) 
      return kw_token;
  }
  return TKTYPE_NAME;
}

/*-----------------------DEFINICAO DOS ESTADOS-----------------------*/
static DfaNode q0 = BUILD_DFA_NODE(TKTYPE_INIT);
static DfaNode q1 = BUILD_DFA_NODE_FINAL(TKTYPE_WTSP);

static DfaNode q2 = BUILD_DFA_NODE_FINAL(TKTYPE_OPPAREN);
static DfaNode q3 = BUILD_DFA_NODE_FINAL(TKTYPE_CLPAREN);
static DfaNode q4 = BUILD_DFA_NODE_FINAL(TKTYPE_OPBRACKET);
static DfaNode q5 = BUILD_DFA_NODE_FINAL(TKTYPE_CLBRACKET);
static DfaNode q6 = BUILD_DFA_NODE_FINAL(TKTYPE_OPSQRBRACK);
static DfaNode q7 = BUILD_DFA_NODE_FINAL(TKTYPE_CLSQRBRACK);
static DfaNode q8 = BUILD_DFA_NODE_FINAL(TKTYPE_COMMA);
static DfaNode q9 = BUILD_DFA_NODE_FINAL(TKTYPE_SMCOLON);

static DfaNode q10 = BUILD_DFA_NODE_FINAL(TKTYPE_SUM);
static DfaNode q11 = BUILD_DFA_NODE_FINAL(TKTYPE_SUB);
static DfaNode q12 = BUILD_DFA_NODE_FINAL(TKTYPE_DIV);
static DfaNode q13 = BUILD_DFA_NODE_FINAL(TKTYPE_MULT);
static DfaNode q14 = BUILD_DFA_NODE_FINAL(TKTYPE_MOD);
static DfaNode q15 = BUILD_DFA_NODE_FINAL(TKTYPE_BITWISE_AND);
static DfaNode q16 = BUILD_DFA_NODE_FINAL(TKTYPE_BITWISE_OR);
static DfaNode q17 = BUILD_DFA_NODE_FINAL(TKTYPE_BITWISE_SHFTL);
static DfaNode q18 = BUILD_DFA_NODE_FINAL(TKTYPE_BITWISE_SHFTR);

static DfaNode q19 = BUILD_DFA_NODE_FINAL(TKTYPE_EQUALS);
static DfaNode q20 = BUILD_DFA_NODE_FINAL(TKTYPE_GT);
static DfaNode q21 = BUILD_DFA_NODE_FINAL(TKTYPE_GEQT);
static DfaNode q22 = BUILD_DFA_NODE_FINAL(TKTYPE_LT);
static DfaNode q23 = BUILD_DFA_NODE_FINAL(TKTYPE_LEQT);
static DfaNode q24 = BUILD_DFA_NODE_FINAL(TKTYPE_LOGIC_OR);
static DfaNode q25 = BUILD_DFA_NODE_FINAL(TKTYPE_LOGIC_AND);

static DfaNode q26 = BUILD_DFA_NODE_FINAL(TKTYPE_NAME);
static DfaNode q27 = BUILD_DFA_NODE_FINAL(TKTYPE_ATTRIBUTION);

static DfaNode q28 =
    BUILD_DFA_NODE_FINAL(TKTYPE_INT); // 0 (pode comecar um octal o hexa)
static DfaNode q29 = BUILD_DFA_NODE_FINAL(TKTYPE_INT);
static DfaNode q30 = BUILD_DFA_NODE_FINAL(TKTYPE_FLOAT);
static DfaNode q31 = BUILD_DFA_NODE_FINAL(TKTYPE_HEXINT);
static DfaNode q32 = BUILD_DFA_NODE_FINAL(TKTYPE_OCTALINT);

/* STRLIT. 
 * TODO: ajeitar essa gambiarra. So de adicionar uma coisa nova ja quebrou */
static DfaNode q90 = BUILD_DFA_NODE_FINAL(TKTYPE_STRLIT);

static bool tokenizer_initialized = 0;

// utilitario para inicializar os estados
static void dfa_add_delta_orelse(DfaNode *src, char c, DfaNode *target, DfaNode *other) {
  char otherexpr[16];
  dfa_add_delta(src, c, target);
  sprintf(otherexpr, "_[A-Za-z0-9]^%c", c);
  dfa_add_delta_expr(src, otherexpr, other);
}

static void init_dfa(void){
  /*-----WTPS------*/
  dfa_add_delta_expr(&q0, " \t\n", &q1);
  dfa_add_delta_expr(&q1, " \t\n", &q1);

  /*-----SEPARADORES/DEMARCADORES------*/
  dfa_add_delta(&q0, '(', &q2);
  dfa_add_delta(&q0, ')', &q3);
  dfa_add_delta(&q0, '{', &q4);
  dfa_add_delta(&q0, '}', &q5);
  dfa_add_delta(&q0, '[', &q6);
  dfa_add_delta(&q0, ']', &q7);
  dfa_add_delta(&q0, ',', &q8);
  dfa_add_delta(&q0, ';', &q9);

  /*-----OPERADORES ARITIMETICOS------*/
  dfa_add_delta(&q0, '+', &q10);
  dfa_add_delta(&q0, '-', &q11);
  dfa_add_delta(&q0, '/', &q12);
  dfa_add_delta(&q0, '*', &q13);
  dfa_add_delta(&q0, '%', &q14);

  // bitwise
  dfa_add_delta(&q0, '&', &q15);
  dfa_add_delta(&q0, '|', &q16);
  dfa_add_delta(&q22, '<', &q17);
  dfa_add_delta(&q20, '>', &q18);

  /*-----OPERADORES LOGICOS------*/
  dfa_add_delta(&q27, '=', &q19);

  dfa_add_delta(&q0, '>', &q20);
  dfa_add_delta(&q20, '=', &q21);

  dfa_add_delta(&q0, '<', &q22);
  dfa_add_delta(&q22, '=', &q23);

  dfa_add_delta(&q16, '|', &q24);
  dfa_add_delta(&q15, '&', &q25);

  /*-----VARIAVEIS------*/
  dfa_add_delta_expr(&q0, "_[A-Za-z]", &q26);
  dfa_add_delta_expr(&q26, "_[A-Za-z]", &q26);

  dfa_add_delta(&q0, '=', &q27); // atribuicao
  /*-----NUMEROS------*/
  // int comum
  dfa_add_delta_expr(&q0, "[0-9]^0", &q29);
  dfa_add_delta_expr(&q29, "[0-9]", &q29);

  // float
  dfa_add_delta(&q29, '.', &q30);
  dfa_add_delta_expr(&q30, "[0-9]", &q30);

  // ou int em hexa/octal. ou float so com casas decimais. ou simplesmente o
  // numero 0
  dfa_add_delta(&q0, '0', &q28);
  dfa_add_delta_expr(&q28, "[0-9]", &q32);
  dfa_add_delta_expr(&q32, "[0-9]", &q32);

  dfa_add_delta(&q28, 'x', &q31);
  dfa_add_delta_expr(&q31, "ABCDEFabcdef[0-9]", &q31);

  dfa_add_delta(&q28, '.', &q30);

  /*-----STRING LITERAL - gambiarra ------*/ 
  dfa_add_delta(&q0, '"', &q90);
}

static TokenList* tklalloc(char *lexeme, TokenType type){
  TokenList *new = malloc(sizeof(TokenList));
  new->lexeme = lexeme;
  new->type = type;
  new->next = NULL;
  return new;
}

static void tklappend(TokenList *head, TokenList *val){
  TokenList *ant;
  while (head) {
    ant = head;
    head = head->next;
  }
  ant->next = val;
}

TokenList *tokenize(char *eval){
  if (!tokenizer_initialized){
    init_dfa();
    tokenizer_initialized = 1;
  }

  int i = 0;
  TokenList *curlist = NULL, *head = NULL;
  DfaNode *curnode = &q0;

  while (eval[i]) {
    int tknstart = i, n = 0;
    while (dfa_node_move(&curnode, eval[i])) {
      i++;
      n++;
    }

    /*---------TODO: remover isto---------*/
    if (curnode->type == TKTYPE_STRLIT) {
      while (eval[i] != '"') {
        i++;
        n++;
        if (eval[i] == '\\') {
          i += 2;
          n += 2;
        }
      }
      i++;
      n++;
    }
    /*------------------------------------*/
    long type = curnode->type;
    char *copy_str = strndup(&eval[tknstart], n);
    if (type == TKTYPE_NAME)
      type = match_var(copy_str);

    if (!curnode->is_final) {
      fprintf(stderr, "ERRO: caracter nao tratado '%c' em %d\n", eval[i], i);
      exit(1);
    }

    if (curnode->type != TKTYPE_WTSP) {
      if (curlist) {
        tklappend(curlist, tklalloc(copy_str, type));
      } else {
        curlist = tklalloc(copy_str, type);
        head = curlist;
      }
    }
    curnode = &q0;
  }
  return head;
}

char *type2str(TokenType t) {
  switch (t) {
  case TKTYPE_INIT:          return "$";
  case TKTYPE_WTSP:          return "WTSP";
  case TKTYPE_OPPAREN:       return "OPPAREN";
  case TKTYPE_CLPAREN:       return "CLPAREN";
  case TKTYPE_OPBRACKET:     return "OPBRACKET";
  case TKTYPE_CLBRACKET:     return "CLBRACKET";
  case TKTYPE_OPSQRBRACK:    return "OPSQRBRACK";
  case TKTYPE_CLSQRBRACK:    return "CLSQRBRACK";
  case TKTYPE_COMMA:         return "COMMA";
  case TKTYPE_SMCOLON:       return "SMCOLON";
  case TKTYPE_SUM:           return "SUM";
  case TKTYPE_SUB:           return "SUB";
  case TKTYPE_DIV:           return "DIV";
  case TKTYPE_MULT:          return "MULT";
  case TKTYPE_MOD:           return "MOD";
  case TKTYPE_BITWISE_AND:   return "BITWISE_AND";
  case TKTYPE_BITWISE_OR:    return "BITWISE_OR";
  case TKTYPE_BITWISE_SHFTL: return "BITWISE_SHFTL";
  case TKTYPE_BITWISE_SHFTR: return "BITWISE_SHFTR";
  case TKTYPE_EQUALS:        return "EQUALS";
  case TKTYPE_GT:            return "GT";
  case TKTYPE_GEQT:          return "GEQT";
  case TKTYPE_LT:            return "LT";
  case TKTYPE_LEQT:          return "LEQT";
  case TKTYPE_LOGIC_OR:      return "LOGIC_OR";
  case TKTYPE_LOGIC_AND:     return "LOGIC_AND";
  case TKTYPE_NEGATION:      return "NEGATION";
  case TKTYPE_NAME:          return "NAME";
  case TKTYPE_ATTRIBUTION:   return "ATTRIBUTION";
  case TKTYPE_INT:           return "INT";
  case TKTYPE_FLOAT:         return "FLOAT";
  case TKTYPE_HEXINT:        return "HEXINT";
  case TKTYPE_OCTALINT:      return "OCTALINT";
  case TKTYPE_KW_IF:         return "IF";
  case TKTYPE_KW_ELSE:       return "ELSE";
  case TKTYPE_KW_FOR:        return "FOR";
  case TKTYPE_KW_WHILE:      return "WHILE";
  case TKTYPE_KW_SWITCH:     return "SWITCH";
  case TKTYPE_KW_CASE:       return "CASE";
  case TKTYPE_KW_FALSE:      return "KW_FALSE";
  case TKTYPE_KW_TRUE:       return "KW_TRUE";
  case TKTYPE_KW_INT:        return "KW_INT";
  case TKTYPE_KW_CHAR:       return "KW_CHAR";
  case TKTYPE_KW_BOOL:       return "KW_BOOL";
  case TKTYPE_KW_LONG:       return "KW_LONG";
  case TKTYPE_KW_DOUBLE:     return "KW_DOUBLE";
  case TKTYPE_KW_FLOAT:      return "KW_FLOAT";
  case TKTYPE_STRLIT:        return "STRLIT";
  case TKTYPE_KW_BREAK:      return "BREAK";
  case TKTYPE_KW_CONTINUE:   return "CONTINUE";
  case TKTYPE_KW_VOID:       return "VOID";
  default:
    fprintf(stderr, "ERRO: token nao especificado");
    exit(1);
  }
}



