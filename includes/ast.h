

typedef enum {
  BNOP_SUM,
  BNOP_SUB,
  BNOP_MULT,
  BNOP_DIV,
  BNOP_MOD,

  BNOP_SHL,
  BNOP_SHR,
  BNOP_BW_OR,
  BNOP_BW_AND,
  BNOP_BW_XOR,
  
  BNOP_OR,
  BNOP_AND,

  BNOP_EQ,
  BNOP_NEQ,
  BNOP_GT,
  BNOP_GEQT,
  BNOP_LT,
  BNOP_LEQT,
} BINOPTYPE ;

typedef struct ASTNode {
  long type;
  union {
    // int
    long ival;

    // float/double
    struct {
      double fval;
      char *frepr;
    };
    
    // str
    char *sval;

    // binary op
    struct {
      BINOPTYPE binop_type;
      struct ASTNode *left;
      struct ASTNode *right;
    };





  };
} ASTNode ;

