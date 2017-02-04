#ifndef PARSER_H
#define PARSER_H

struct lval;
struct lenv;

typedef struct lval lval;
typedef struct lenv lenv;

// Possible LVAL types
enum { LVAL_NUM, LVAL_ERR, LVAL_SYM, LVAL_SEXPR, LVAL_QEXPR, LVAL_FUN };

typedef lval*(*lbuiltin)(lenv*, lval*);

struct lval {
  int type;

  long num;
  char* err;
  char* sym;
  lbuiltin fun;

  int count;
  struct lval** cell;
};

struct lenv {
  int count;
  char** syms;
  lval** vals;
};

// Constructors and destructors for type wrapper
lval* lval_num (long);
lval* lval_err (char*);
lval* lval_sym (char*);
lval* lval_sexpr (void);
lval* lval_qexpr (void);
lval* lval_fun (lbuiltin);
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
lval* lval_eval_sexpr (lenv*, lval*);
lval* lval_eval (lenv*, lval*);
// Helpers
lval* lval_pop (lval*, int);
lval* lval_take (lval*, int);
lval* lval_copy (lval*);

// Q expressions
lval* lval_join (lval*, lval*);

/*
 * ENVIRONMENT
 */

lenv* lenv_new (void);
void lenv_del (lenv*);

lval* lenv_get (lenv*, lval*);
void lenv_put (lenv*, lval*, lval*);

void lenv_add_builtin (lenv*, char*, lbuiltin);

#endif