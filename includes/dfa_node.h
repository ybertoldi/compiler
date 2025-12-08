#include <stdbool.h>
#include <stdint.h>

#ifndef DFAUTOMATA_H
#define DFAUTOMATA_H

#define MAX_MAP_SIZE 256

#define BUILD_DFA_NODE(n_type)                                                 \
  {.type = n_type, .map = {{0}}, .is_final = false}

#define BUILD_DFA_NODE_FINAL(n_type)                                           \
  {.type = n_type, .map = {{0}}, .is_final = true}

typedef struct dfa_node_map {
  char key[MAX_MAP_SIZE];
  struct dfa_node *val[MAX_MAP_SIZE];
  int size;
} DfaMap;

typedef struct dfa_node {
  int type;
  struct dfa_node_map map;
  bool is_final;
} DfaNode;


DfaMap init_dfa_map(void);
DfaNode *dfa_map_get(DfaMap *map, char key);
DfaNode *dfalloc(char *name, bool isfinal);

void dfa_add_delta(DfaNode *node, char key, DfaNode *value);
void dfa_add_delta_expr(DfaNode *s, char *expr, DfaNode *t);

bool dfa_valida_cadeia(DfaNode *estado_inicial, char *cadeia);
//void print_dfa_node(DfaNode *node);
bool dfa_node_move(DfaNode **node, char input);
#endif
