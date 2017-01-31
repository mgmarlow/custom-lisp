#include "../lib/mpc.h"
#include "lval.h"
#include "builtin.h"

// Takes in list of lval arguments and returns expressions
lval* builtin_op (lval* a, char* op) {
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

lval* builtin_head (lval* a) {
  if (a->count != 1) {
    lval_del(a);
    return lval_err("Function 'head' passed too many arguments.");
  }

  if (a->cell[0]->type != LVAL_QEXPR) {
    lval_del(a);
    return lval_err("Function 'head' passed incorrect types.");
  }

  if (a->cell[0]->count == 0) {
    lval_del(a);
    return lval_err("Function 'head' passed {}.");
  }

  // If all error conditions pass, grab first element
  lval* v = lval_take(a, 0);

  // Clean up rest of qexpr
  while (v->count > 1) {
    lval_del(lval_pop(v, 1));
  }
  return v;
}

lval* builtin_tail (lval* a) {
  if (a->count != 1) {
    lval_del(a);
    return lval_err("Function 'tail' passed too many arguments.");
  }

  if (a->cell[0]->type != LVAL_QEXPR) {
    lval_del(a);
    return lval_err("Function 'tail' passed incorrect types.");
  }

  if (a->cell[0]->count == 0) {
    lval_del(a);
    return lval_err("Function 'tail' passed {}.");
  }

  lval* v = lval_take(a, 0);
  lval_del(lval_pop(v, 0));
  return v;
}