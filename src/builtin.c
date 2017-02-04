#include "../lib/mpc.h"
#include "lval.h"
#include "builtin.h"

void lenv_add_builtins (lenv* e) {
  lenv_add_builtin(e, "list", builtin_list);
  lenv_add_builtin(e, "head", builtin_head);
  lenv_add_builtin(e, "tail", builtin_tail);
  lenv_add_builtin(e, "eval", builtin_eval);
  lenv_add_builtin(e, "join", builtin_join);
  lenv_add_builtin(e, "def", builtin_def);

  lenv_add_builtin(e, "+", builtin_add);
  lenv_add_builtin(e, "-", builtin_sub);
  lenv_add_builtin(e, "*", builtin_mul);
  lenv_add_builtin(e, "/", builtin_div);
}

// Function lookups
lval* builtin (lenv* e, lval* a, char* func) {
  if (strcmp("list", func) == 0) { return builtin_list(e, a); }
  if (strcmp("head", func) == 0) { return builtin_head(e, a); }
  if (strcmp("tail", func) == 0) { return builtin_tail(e, a); }
  if (strcmp("join", func) == 0) { return builtin_join(e, a); }
  if (strcmp("eval", func) == 0) { return builtin_eval(e, a); }
  if (strstr("+-/*", func)) { return builtin_op(e, a, func); }

  lval_del(a);
  return lval_err("Unknown Function.");
}

lval* builtin_def (lenv* e, lval* a) {
  LASSERT(a, a->cell[0]->type == LVAL_QEXPR, 
    "Function 'def' passed incorrect type!");

  lval* syms = a->cell[0];

  for (int i = 0; i < syms->count; i++) {
    LASSERT(a, syms->cell[i]->type == LVAL_SYM,
      "Function 'def' cannot define non-symbol");
  }

  LASSERT(a, syms->count == (a->count - 1),
    "Function 'def' cannot define incorrect number of values to symbols");

  for (int i = 0; i < syms->count; i++) {
    lenv_put(e, syms->cell[i], a->cell[i + 1]);
  }

  lval_del(a);
  return lval_sexpr();
}

// Takes in list of lval arguments and returns expressions
lval* builtin_op (lenv* e, lval* a, char* op) {
  for (int i = 0; i < a->count; i++) {
    if (a->cell[i]->type != LVAL_NUM) {
      lval_del(a);
      return lval_err("Cannot operate on non-number!");
    }
  }

  lval* x = lval_pop(a, 0);

  if ((strcmp(op, "-") == 0) && a->count == 0) {
    x->num = -x->num;
  }

  while (a->count > 0) {
    lval* y = lval_pop(a, 0);

    if (strcmp(op, "+") == 0) { x->num += y->num; }
    if (strcmp(op, "-") == 0) { x->num -= y->num; }
    if (strcmp(op, "*") == 0) { x->num *= y->num; }
    if (strcmp(op, "/") == 0) {
      if (y->num == 0) {
        lval_del(x);
        lval_del(y);
        x = lval_err("Division By Zero.");
        break;
      }
      x->num /= y->num;
    }

    lval_del(y);
  }

  lval_del(a);
  return x;
}

lval* builtin_add (lenv* e, lval* a) {
  return builtin_op(e, a, "+");
}

lval* builtin_sub (lenv* e, lval* a) {
  return builtin_op(e, a, "-");
}

lval* builtin_mul (lenv* e, lval* a) {
  return builtin_op(e, a, "*");
}

lval* builtin_div (lenv* e, lval* a) {
  return builtin_op(e, a, "/");
}

lval* builtin_head (lenv* e, lval* a) {
  LASSERT(a, a->count == 1,
    "Function 'head' passed too many arguments.");
  LASSERT(a, a->cell[0]->type == LVAL_QEXPR,
    "Function 'head' passed incorrect type!");
  LASSERT(a, a->cell[0]->type != 0,
    "Function 'head' passed {}.");

  // If all error conditions pass, grab first element
  lval* v = lval_take(a, 0);

  // Clean up rest of qexpr
  while (v->count > 1) {
    lval_del(lval_pop(v, 1));
  }
  return v;
}

lval* builtin_tail (lenv* e, lval* a) {
  LASSERT(a, a->count == 1,
    "Function 'tail' passed too many arguments.");
  LASSERT(a, a->cell[0]->type == LVAL_QEXPR,
    "Function 'tail' passed incorrect type!");
  LASSERT(a, a->cell[0]->count != 0,
    "Function 'tail' passed {}.");

  lval* v = lval_take(a, 0);
  lval_del(lval_pop(v, 0));
  return v;
}

lval* builtin_list (lenv* e, lval* a) {
  a->type = LVAL_QEXPR;
  return a;
}

lval* builtin_eval (lenv* e, lval* a) {
  LASSERT(a, a->count == 1,
    "Function 'eval' passed too many arguments.");
  LASSERT(a, a->cell[0]->type == LVAL_QEXPR,
    "Function 'eval' passed incorrect type.");

  lval* x = lval_take(a, 0);
  x->type = LVAL_SEXPR;
  return lval_eval(e, x);
}

lval* builtin_join (lenv* e, lval* a) {
  for (int i = 0; i < a->count; i++) {
    LASSERT(a, a->cell[i]->type == LVAL_QEXPR,
      "Function 'join' passed incorrect type.");
  }

  lval* x = lval_pop(a, 0);
  while (a->count) {
    x = lval_join(x, lval_pop(a, 0));
  }

  lval_del(a);
  return x;
}