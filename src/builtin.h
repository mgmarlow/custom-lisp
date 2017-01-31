#ifndef BUILTIN_H
#define BUILTIN_H

/*
 * Library for evaluating expressions provided from LVALs
 */

lval* builtin_op (lval*, char*);
lval* builtin_head (lval*);
lval* builtin_tail (lval*);

#endif