#ifndef BUILTIN_H
#define BUILTIN_H

#define LASSERT(args, cond, err) \
  if (!(cond)) { lval_del(args); return lval_err(err); }

/*
 * Library for evaluating expressions provided from LVALs
 */

lval* builtin_op (lval*, char*);
lval* builtin_head (lval*);
lval* builtin_tail (lval*);

#endif