#include "../lib/mpc.h"
#include "parser.h"

// Possible lval types
enum { LVAL_NUM, LVAL_ERR, LVAL_SYM, LVAL_SEXPR };

// Constructors for lvals 
lval* lval_num (long x) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_NUM;
  v->num = x;
  return v;
}

lval* lval_err (char* m) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_ERR;
  // Account for null-terminating character
  v->err = malloc(strlen(m) + 1);
  strcpy(v->err, m);
  return v;
}

lval* lval_sym (char* s) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_SYM;
  v->sym = malloc(strlen(s) + 1);
  strcpy(v->sym, s);
  return v;
}

lval* lval_sexpr (void) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_SEXPR;
  v->count = 0;
  v->cell = NULL;
  return v;
}

void lval_del (lval* v) {
  switch (v->type) {
    case LVAL_NUM:
      break;

    case LVAL_ERR:
      free(v->err);
      break;

    case LVAL_SYM:
      free(v->sym);
      break;

    case LVAL_SEXPR:
      for (int i = 0; i < v->count; i++) {
        lval_del(v->cell[i]);
      }
      free(v->cell);
      break;
  }

  free(v);
}

lval* lval_read_num (mpc_ast_t* t) {
  errno = 0;
  long x = strtol(t->contents, NULL, 10);
  return errno != ERANGE ? lval_num(x) : lval_err("invaild number");
}

lval* lval_read (mpc_ast_t* t) {
  if (strstr(t->tag, "number"))
    return lval_read_num(t);
  if (strstr(t->tag, "symbol"))
    return lval_sym(t->contents);
  
  lval* x = NULL;
  if (strcmp(t->tag, ">") == 0 || strstr(t->tag, "sexpr"))
    x = lval_sexpr();

  for (int i = 0; i < t->children_num; i++) {
    if (strcmp(t->children[i]->contents, "(") == 0) { continue; }
    if (strcmp(t->children[i]->contents, ")") == 0) { continue; }
    if (strcmp(t->children[i]->contents, "{") == 0) { continue; }
    if (strcmp(t->children[i]->contents, "}") == 0) { continue; }
    if (strcmp(t->children[i]->tag, "regex") == 0) { continue; }
    x = lval_add(x, lval_read(t->children[i]));
  }

  return x;
}

lval* lval_add(lval* v, lval*x) {
  v->count++;
  v->cell = realloc(v->cell, sizeof(lval*) * v->count);
  v->cell[v->count - 1] = x;
  return v;
}

void lval_expr_print (lval* v, char open, char close) {
  putchar(open);
  for (int i = 0; i < v->count; i++) {
    lval_print(v->cell[i]);

    // Trailing space (except last element)
    if (i != (v->count - 1))
      putchar(' ');
  }
  putchar(close);
}

void lval_print (lval* v) {
  switch (v->type) {
    case LVAL_NUM:
      printf("%li", v->num);
      break;
    case LVAL_ERR:
      printf("Error: %s", v->err);
      break;
    case LVAL_SYM:
      printf("%s", v->sym);
      break;
    case LVAL_SEXPR:
      lval_expr_print(v, '(', ')');
      break;
  }
}

void lval_println (lval* v) {
  lval_print(v);
  putchar('\n');
}

// Evaluate Lispy Operations
// lval eval (mpc_ast_t* t) {
//   if (strstr(t->tag, "number")) {
//     // Check for error in conversion (provided by strtol)
//     errno = 0;
//     long x = strtol(t->contents, NULL, 10);
//     return errno != ERANGE ? lval_num(x) : lval_err(LERR_BAD_NUM);
//   }

//   // Operator is always a second child
//   char* op = t->children[1]->contents;
//   lval x = eval(t->children[2]);

//   int i = 3;
//   while (strstr(t->children[i]->tag, "expr")) {
//     x = eval_op(x, op, eval(t->children[i]));
//     i++;
//   }

//   return x;
// }

// lval eval_op (lval x, char* op, lval y) {
//   if (x.type == LVAL_ERR) { return x; }
//   if (y.type == LVAL_ERR) { return y; }

//   if (strcmp(op, "+") == 0) { return lval_num(x.num + y.num); }
//   if (strcmp(op, "-") == 0) { return lval_num(x.num - y.num); }
//   if (strcmp(op, "*") == 0) { return lval_num(x.num * y.num); }
//   if (strcmp(op, "/") == 0) {
//     return y.num == 0 ? lval_err(LERR_DIV_ZERO) : lval_num(x.num / y.num);
//   }

//   return lval_err(LERR_BAD_OP);
// }