#ifndef PARSER_H
#define PARSER_H

// Possible LVAL types
enum { LVAL_NUM, LVAL_ERR, LVAL_SYM, LVAL_SEXPR, LVAL_QEXPR };

// Wrapper for values for error handling
typedef struct lval {
  int type;
  long num;
  char* err;
  char* sym;

  int count;
  struct lval** cell;
} lval;

// Constructors and destructors for type wrapper
lval* lval_num (long);
lval* lval_err (char*);
lval* lval_sym (char*);
lval* lval_sexpr (void);
lval* lval_qexpr (void);
void lval_del (lval*);

// Parsing logic
lval* lval_read_num (mpc_ast_t*);
lval* lval_read (mpc_ast_t*);
lval* lval_add (lval*, lval*);

// Printing
void lval_expr_print (lval*, char, char);
void lval_print (lval*);
void lval_println (lval*);

// Evaluate expressions (uses builtin for evaluation)
lval* lval_eval_sexpr (lval*);
lval* lval_eval (lval*);
// Helpers
lval* lval_pop (lval*, int);
lval* lval_take (lval*, int);

// Q expressions
lval* lval_join (lval*, lval*);

#endif