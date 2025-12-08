#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../includes/tokenizer.h"

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

static DfaNode q26 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);
static DfaNode q27 = BUILD_DFA_NODE_FINAL(TKTYPE_ATTRIBUTION);

static DfaNode q28 =
    BUILD_DFA_NODE_FINAL(TKTYPE_INT); // 0 (pode comecar um octal o hexa)
static DfaNode q29 = BUILD_DFA_NODE_FINAL(TKTYPE_INT);
static DfaNode q30 = BUILD_DFA_NODE_FINAL(TKTYPE_FLOAT);
static DfaNode q31 = BUILD_DFA_NODE_FINAL(TKTYPE_HEXINT);
static DfaNode q32 = BUILD_DFA_NODE_FINAL(TKTYPE_OCTALINT);

static DfaNode q33 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR); // i
static DfaNode q34 = BUILD_DFA_NODE_FINAL(TKTYPE_IF);  // if

static DfaNode q35 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);  // e
static DfaNode q36 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);  // el
static DfaNode q37 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);  // els
static DfaNode q38 = BUILD_DFA_NODE_FINAL(TKTYPE_ELSE); // else

static DfaNode q39 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR); // f
static DfaNode q40 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR); // fo
static DfaNode q41 = BUILD_DFA_NODE_FINAL(TKTYPE_FOR); // for

static DfaNode q42 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);   // w
static DfaNode q43 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);   // wh
static DfaNode q44 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);   // whi
static DfaNode q45 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);   // whil
static DfaNode q46 = BUILD_DFA_NODE_FINAL(TKTYPE_WHILE); // while

static DfaNode q47 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);    // s
static DfaNode q48 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);    // sw
static DfaNode q49 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);    // swi
static DfaNode q50 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);    // swit
static DfaNode q51 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);    // switc
static DfaNode q52 = BUILD_DFA_NODE_FINAL(TKTYPE_SWITCH); // switch

static DfaNode q53 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);  // c
static DfaNode q54 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);  // ca
static DfaNode q55 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);  // cas
static DfaNode q56 = BUILD_DFA_NODE_FINAL(TKTYPE_CASE); // case

static DfaNode q57 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);   // fa (f do for)
static DfaNode q58 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);   // fal
static DfaNode q59 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);   // fals
static DfaNode q60 = BUILD_DFA_NODE_FINAL(TKTYPE_FALSE); // false

static DfaNode q61 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);  // t
static DfaNode q62 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);  // tr
static DfaNode q63 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);  // tru
static DfaNode q64 = BUILD_DFA_NODE_FINAL(TKTYPE_TRUE); // true

// TODO: mudar os indices
static DfaNode q66 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);   // in (i do if)
static DfaNode q67 = BUILD_DFA_NODE_FINAL(TKTYPE_T_INT); // int

static DfaNode q68 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);    // c //TODO: tirar esse estado (c do case)
static DfaNode q69 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);    // ch
static DfaNode q70 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);    // cha
static DfaNode q71 = BUILD_DFA_NODE_FINAL(TKTYPE_T_CHAR); // char

static DfaNode q72 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);    // b
static DfaNode q73 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);    // bo
static DfaNode q74 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);    // boo
static DfaNode q75 = BUILD_DFA_NODE_FINAL(TKTYPE_T_BOOL); // bool

static DfaNode q76 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);    // l
static DfaNode q77 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);    // lo
static DfaNode q78 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);    // lon
static DfaNode q79 = BUILD_DFA_NODE_FINAL(TKTYPE_T_LONG); // long

static DfaNode q80 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);      // d
static DfaNode q81 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);      // do
static DfaNode q82 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);      // dou
static DfaNode q83 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);      // doub
static DfaNode q84 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);      // doubl
static DfaNode q85 = BUILD_DFA_NODE_FINAL(TKTYPE_T_DOUBLE); // double

static DfaNode q86 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);     // fl (f do for)
static DfaNode q87 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);     // flo
static DfaNode q88 = BUILD_DFA_NODE_FINAL(TKTYPE_VAR);     // floa
static DfaNode q89 = BUILD_DFA_NODE_FINAL(TKTYPE_T_FLOAT); // float

static int initialized = 0;

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
  dfa_add_delta_expr(&q0, "_[A-Za-z]^iefwsctlbd", &q26);
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

  /*----------------------PALAVRAS RESERVADAS-----------------------*/
  // if
  dfa_add_delta(&q0, 'i', &q33);
  dfa_add_delta(&q33, 'f', &q34);
  dfa_add_delta_expr(&q33, "_[A-Za-z0-9]^n", &q26);
  dfa_add_delta_expr(&q34, "_[A-Za-z0-9]", &q26);

  // else
  dfa_add_delta(&q0, 'e', &q35);
  dfa_add_delta_orelse(&q35, 'l', &q36, &q26);
  dfa_add_delta_orelse(&q36, 's', &q37, &q26);
  dfa_add_delta_orelse(&q37, 'e', &q38, &q26);
  dfa_add_delta_expr(&q38, "_[A-Za-z0-9]", &q26);

  // for
  dfa_add_delta(&q0, 'f', &q39);
  dfa_add_delta(&q39, 'o', &q40);
  dfa_add_delta_expr(&q39, "_[A-Za-z0-9]^oal", &q26); // for, false e float

  dfa_add_delta_orelse(&q40, 'r', &q41, &q26);
  dfa_add_delta_expr(&q41, "_[A-Za-z0-9]", &q26);

  // while
  dfa_add_delta(&q0, 'w', &q42);
  dfa_add_delta_orelse(&q42, 'h', &q43, &q26);
  dfa_add_delta_orelse(&q43, 'i', &q44, &q26);
  dfa_add_delta_orelse(&q44, 'l', &q45, &q26);
  dfa_add_delta_orelse(&q45, 'e', &q46, &q26);
  dfa_add_delta_expr(&q46, "_[A-Za-z0-9]", &q26);

  // switch
  dfa_add_delta(&q0, 's', &q47);
  dfa_add_delta_orelse(&q47, 'w', &q48, &q26);
  dfa_add_delta_orelse(&q48, 'i', &q49, &q26);
  dfa_add_delta_orelse(&q49, 't', &q50, &q26);
  dfa_add_delta_orelse(&q50, 'c', &q51, &q26);
  dfa_add_delta_orelse(&q51, 'h', &q52, &q26);
  dfa_add_delta_expr(&q52, "_[A-Za-z0-9]", &q26);

  // case
  dfa_add_delta(&q0, 'c', &q53);
  dfa_add_delta_orelse(&q53, 'a', &q54, &q26);
  dfa_add_delta_orelse(&q54, 's', &q55, &q26);
  dfa_add_delta_orelse(&q55, 'e', &q56, &q26);
  dfa_add_delta_expr(&q56, "_[A-Za-z0-9]", &q26);

  // false
  dfa_add_delta(&q39, 'a', &q57);
  dfa_add_delta_orelse(&q57, 'l', &q58, &q26);
  dfa_add_delta_orelse(&q58, 's', &q59, &q26);
  dfa_add_delta_orelse(&q59, 'e', &q60, &q26);
  dfa_add_delta_expr(&q60, "_[A-Za-z0-9]", &q26);

  // true
  dfa_add_delta(&q0, 't', &q61);
  dfa_add_delta_orelse(&q61, 'r', &q62, &q26);
  dfa_add_delta_orelse(&q62, 'u', &q63, &q26);
  dfa_add_delta_orelse(&q63, 'e', &q64, &q26);
  dfa_add_delta_expr(&q64, "_[A-Za-z0-9]", &q26);

  // int
  dfa_add_delta(&q33, 'n', &q66); //i do if
  dfa_add_delta_orelse(&q66, 't', &q67, &q26);
  dfa_add_delta_expr(&q67, "_[A-Za-z0-9]", &q26);

  // char
  dfa_add_delta(&q0, 'c', &q68);
  dfa_add_delta_orelse(&q68, 'h', &q69, &q26);
  dfa_add_delta_orelse(&q69, 'a', &q70, &q26);
  dfa_add_delta_orelse(&q70, 'r', &q71, &q26);
  dfa_add_delta_expr(&q71, "_[A-Za-z0-9]", &q26);

  // bool
  dfa_add_delta(&q0, 'b', &q72);
  dfa_add_delta_orelse(&q72, 'o', &q73, &q26);
  dfa_add_delta_orelse(&q73, 'o', &q74, &q26);
  dfa_add_delta_orelse(&q74, 'l', &q75, &q26);
  dfa_add_delta_expr(&q75, "_[A-Za-z0-9]", &q26);

  // long
  dfa_add_delta(&q0, 'l', &q76);
  dfa_add_delta_orelse(&q76, 'o', &q77, &q26);
  dfa_add_delta_orelse(&q77, 'n', &q78, &q26);
  dfa_add_delta_orelse(&q78, 'g', &q79, &q26);
  dfa_add_delta_expr(&q79, "_[A-Za-z0-9]", &q26);

  // double
  dfa_add_delta(&q0, 'd', &q80);
  dfa_add_delta_orelse(&q80, 'o', &q81, &q26);
  dfa_add_delta_orelse(&q81, 'u', &q82, &q26);
  dfa_add_delta_orelse(&q82, 'b', &q83, &q26);
  dfa_add_delta_orelse(&q83, 'l', &q84, &q26);
  dfa_add_delta_orelse(&q84, 'e', &q85, &q26);
  dfa_add_delta_expr(&q85, "_[A-Za-z0-9]", &q26);

  // float
  dfa_add_delta(&q39, 'l', &q86);
  dfa_add_delta_orelse(&q86, 'o', &q87, &q26);
  dfa_add_delta_orelse(&q87, 'a', &q88, &q26);
  dfa_add_delta_orelse(&q88, 't', &q89, &q26);
  dfa_add_delta_expr(&q89, "_[A-Za-z0-9]", &q26);
}


static TokenList* tklalloc(char *lexeme, TKTYPE type){
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
  if (!initialized){
    init_dfa();
    initialized = 1;
  }

  int i = 0;
  TokenList *curlist = NULL, *head;
  DfaNode *curnode = &q0;

  while (eval[i]) {
    int tknstart = i, n = 0;
    while (dfa_node_move(&curnode, eval[i])) {
      i++;
      n++;
    }
    
    if (!curnode->is_final){
      fprintf(stderr, "ERRO: caracter nao tratado '%c' em %d\n", eval[i], i);
      exit(1);
    }

    if (curnode->type != TKTYPE_WTSP) {
      if (curlist) {
        tklappend(curlist,
                  tklalloc(strndup(&eval[tknstart], n), curnode->type));
      } else {
        curlist = tklalloc(strndup(&eval[tknstart], n), curnode->type);
        head = curlist;
      }
    }
    curnode = &q0;
  }
  return head;
}

char *type2str(TKTYPE t) {
  switch (t) {
  case TKTYPE_INIT: return "INIT";
  case TKTYPE_WTSP: return "WTSP";
  case TKTYPE_OPPAREN: return "OPPAREN";
  case TKTYPE_CLPAREN: return "CLPAREN";
  case TKTYPE_OPBRACKET: return "OPBRACKET";
  case TKTYPE_CLBRACKET: return "CLBRACKET";
  case TKTYPE_OPSQRBRACK: return "OPSQRBRACK";
  case TKTYPE_CLSQRBRACK: return "CLSQRBRACK";
  case TKTYPE_COMMA: return "COMMA";
  case TKTYPE_SMCOLON: return "SMCOLON";
  case TKTYPE_SUM: return "SUM";
  case TKTYPE_SUB: return "SUB";
  case TKTYPE_DIV: return "DIV";
  case TKTYPE_MULT: return "MULT";
  case TKTYPE_MOD: return "MOD";
  case TKTYPE_BITWISE_AND: return "BITWISE_AND";
  case TKTYPE_BITWISE_OR: return "BITWISE_OR";
  case TKTYPE_BITWISE_SHFTL: return "BITWISE_SHFTL";
  case TKTYPE_BITWISE_SHFTR: return "BITWISE_SHFTR";
  case TKTYPE_EQUALS: return "EQUALS";
  case TKTYPE_GT: return "GT";
  case TKTYPE_GEQT: return "GEQT";
  case TKTYPE_LT: return "LT";
  case TKTYPE_LEQT: return "LEQT";
  case TKTYPE_LOGIC_OR: return "LOGIC_OR";
  case TKTYPE_LOGIC_AND: return "LOGIC_AND";
  case TKTYPE_NEGATION: return "NEGATION";
  case TKTYPE_VAR: return "VAR";
  case TKTYPE_ATTRIBUTION: return "ATTRIBUTION";
  case TKTYPE_INT: return "INT";
  case TKTYPE_FLOAT: return "FLOAT";
  case TKTYPE_HEXINT: return "HEXINT";
  case TKTYPE_OCTALINT: return "OCTALINT";
  case TKTYPE_IF: return "IF";
  case TKTYPE_ELSE: return "ELSE";
  case TKTYPE_FOR: return "FOR";
  case TKTYPE_WHILE: return "WHILE";
  case TKTYPE_SWITCH: return "SWITCH";
  case TKTYPE_CASE: return "CASE";
  case TKTYPE_FALSE: return "FALSE";
  case TKTYPE_TRUE: return "TRUE";
  case TKTYPE_T_INT: return "T_INT";
  case TKTYPE_T_CHAR: return "T_CHAR";
  case TKTYPE_T_BOOL: return "T_BOOL";
  case TKTYPE_T_LONG: return "T_LONG";
  case TKTYPE_T_DOUBLE: return "T_DOUBLE";
  case TKTYPE_T_FLOAT: return "T_FLOAT";
  //TODO: adicionar string literal
  case TKTYPE_SINGLE_QUOTE: return "SINGLE_QUOTE";
  case TKTYPE_DOUBLE_QUOTE: return "DOUBLE_QUOTE"; //TODO: adicionar caracteres de escape 
  default:
    fprintf(stderr, "ERRO: token nao especificado");
    exit(1);
  }
}



