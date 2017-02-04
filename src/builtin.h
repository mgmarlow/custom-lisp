#ifndef BUILTIN_H
#define BUILTIN_H

#define LASSERT(args, cond, fmt, ...) \
  if (!(cond)) { lval* err = lval_err(fmt, ##__VA_ARGS__); lval_del(args); return err; }

#define LASSERT_TYPE(func, args, index, expect) \
  LASSERT(args, args->cell[index]->type == expect, \
    "Function '%s' passed incorrect type for argument %i. Got %s, Expected %s.", \
    func, index, ltype_name(args->cell[index]->type), ltype_name(expect))

#define LASSERT_NUM(func, args, num) \
  LASSERT(args, args->count == num, \
    "Function '%s' passed incorrect number of arguments. Got %i, Expected %i.", \
    func, args->count, num)

#define LASSERT_NOT_EMPTY(func, args, index) \
  LASSERT(args, args->cell[index]->count != 0, \
    "Function '%s' passed {} for argument %i.", func, index);

/*
 * Library for evaluating expressions provided from LVALs
 */

void lenv_add_builtins (lenv*);
lval* builtin (lenv*, lval*, char*);
lval* builtin_op (lenv*, lval*, char*);

lval* builtin_add (lenv*, lval*);
lval* builtin_sub (lenv*, lval*);
lval* builtin_mul (lenv*, lval*);
lval* builtin_div (lenv*, lval*);

// Q-Expressions
// Grab first element
lval* builtin_head (lenv*, lval*);
// Grab every element but first
lval* builtin_tail (lenv*, lval*);
// Convert Sexpr to Qexpr
lval* builtin_list (lenv*, lval*);
// Convert Qexpr to Sexpr
lval* builtin_eval (lenv*, lval*);
// Combine multiple qexpr into a single qexpr
lval* builtin_join (lenv*, lval*);
// Define variables
lval* builtin_def (lenv*, lval*);

#endif