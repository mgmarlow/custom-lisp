#ifndef BUILTIN_H
#define BUILTIN_H

#define LASSERT(args, cond, err) \
  if (!(cond)) { lval_del(args); return lval_err(err); }

/*
 * Library for evaluating expressions provided from LVALs
 */

lval* builtin (lval*, char*);
lval* builtin_op (lval*, char*);

// Q-Expressions
// Grab first element
lval* builtin_head (lval*);
// Grab every element but first
lval* builtin_tail (lval*);
// Convert Sexpr to Qexpr
lval* builtin_list (lval*);
// Convert Qexpr to Sexpr
lval* builtin_eval (lval*);
// Combine multiple qexpr into a single qexpr
lval* builtin_join (lval*);

#endif