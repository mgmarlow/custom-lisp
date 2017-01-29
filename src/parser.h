#ifndef PARSER_H_
#define PARSER_H_

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
lval* lval_num(long);
lval* lval_err(char*);
lval* lval_sym(char*);
lval* lval_sexpr(void);
void lval_del(lval*);

// Parsing logic
lval* lval_read_num(mpc_ast_t*);
lval* lval_read(mpc_ast_t*);
lval* lval_add(lval*, lval*);

// Printing
void lval_expr_print(lval*, char, char);
void lval_print(lval*);
void lval_println(lval*);

// Evaluate expressions
lval eval(mpc_ast_t*);
lval eval_op(lval, char*, lval);

#endif