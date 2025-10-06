#include "common.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>


int match(char *t);
void consume(char *t);

int expr();
int term();
int factor();

StringView cur_sv;

int main(){
  // char *fmt = "(2 + 2) * 3";
  // cur_sv.start = fmt;
  // cur_sv.size = 0;
  // move_sv(&cur_sv);
  // expr();

  char line[BUFSIZ];

  while (1) {
    printf("calc > ");
    fflush(stdout);

    if (readline(line, BUFSIZ)) {
      cur_sv.start = line;
      cur_sv.size = 0;
      move_sv(&cur_sv);
      printf("\t = %d\n", expr());
    } else {
      break;
    }
  }

  return 0;
}

int match(char *t) { return match_sv(&cur_sv, t); }
void consume(char *t){
  if (!consume_sv(&cur_sv, t)){
    fprintf(stderr, "syntax error\n");
    exit(1);
  }
}

int expr() {
  int ret = term();
  while (1) {
    switch (*cur_sv.start) {
    case '+': {
      consume("+");
      ret += term();
    } break;

    case '-': {
      consume("-");
      ret -= term();
    } break;

    default:
      return ret;
    }
  }
}

int term() {
  int ret = factor();
  while (1) {
    switch (*cur_sv.start) {
    case '*': {
      consume("*");
      ret *= factor();
    } break;

    case '/': {
      consume("/");
      ret /= factor();
    } break;

    default:
      return ret;
    }
  }

}

int factor() {
  int ret;
  if (*cur_sv.start == '(') {
    consume("(");
    ret = expr();
    consume(")");
    return ret;
  } else {
    char *c = cur_sv.start;
    int i = 0;

    for (; i < cur_sv.size; i++) {
      if (!isdigit(c[i])) {
        fprintf(stderr, "syntax error\n");
        exit(1);
      }
    }

    ret = atoi(cur_sv.start);
    move_sv(&cur_sv);
    return ret;
  }
}

