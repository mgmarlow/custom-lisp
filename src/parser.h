#ifndef PARSER_H_
#define PARSER_H_

// Wrapper for values for error handling
typedef struct {
  int type;
  long num;
  int err;
} lval;

// Constructors and printing for type wrapper
lval lval_num (long);
lval lval_err (int);
void lval_print (lval);
void lval_println (lval);

// Evaluate expressions
lval eval (mpc_ast_t*);
lval eval_op(lval, char*, lval);

#endif