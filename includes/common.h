#ifndef COMMON_H
#define COMMON_H

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct{
  char *start;
  size_t size;
} StringView ;

int move_sv(StringView *sv){
  char *p = sv->start + sv->size;
  int n = 0;

  while (isspace(*p))
    p++;

  if (*p == '\0')
    return 0;

  switch (*p) {
    case '[':
    case ']':
    case '(':
    case ')':
    case ';':
    case ',':
    case '+':
    case '-':
    case '*':
    case '/': { 
      sv->start = p;
      sv->size = 1;
    } return 1;

    case '\'':
    case '\"': {
      char cmp = *p;
      n++;
      while(p[n] && p[n] != cmp)
        n++;
      n++;
      sv->start = p;
      sv->size = n;
    } return 1;

    default: break;
  }
  
  while (isalnum(p[n])) 
    n++;

  sv->start = p;
  sv->size = n;
  return 1;
}

int match_sv(StringView *sv, char *t) {
  return strncmp(sv->start, t, sv->size) == 0 ? 1 : 0;
}

int consume_sv(StringView *sv, char *t){
  if (match_sv(sv, t)){
    move_sv(sv);
    return 1;
  } 
  return 0;
}

int readline(char *buffer, int bufmax) {
  int c;
  while ((c = getchar()) != EOF && bufmax > 0) {
    *buffer++ = c;
    bufmax--;
    if (c == '\n') {
      *(--buffer) = '\0';
      return 1;
    }
  }
  return 0;
}


#endif //COMMON_H
