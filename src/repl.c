#include <stdio.h>
#include <stdlib.h>

#include "../lib/mpc.h"
#include "lval.h"

#ifdef _WIN32
#include <string.h>

static char buffer[2048];

char* readline (char* prompt) {
  fputs(prompt, stdout);
  fgets(buffer, 2048, stdin);
  char* cpy = malloc(strlen(buffer) + 1);
  strcpy(cpy, buffer);
  cpy[strlen(cpy - 1)] = '\0';
  return cpy;
}

void add_history (char* unused) {}

// Linux and Mac dependencies
#else
#include <editline/readline.h>
#include <editline/history.h>
#endif

int main (int argc, char** argv) {
  mpc_parser_t* Number   = mpc_new("number");
  mpc_parser_t* Symbol   = mpc_new("symbol");
  mpc_parser_t* Sexpr    = mpc_new("sexpr");
  mpc_parser_t* Qexpr    = mpc_new("qexpr");
  mpc_parser_t* Expr     = mpc_new("expr");
  mpc_parser_t* Lispy    = mpc_new("lispy");

  mpca_lang(MPCA_LANG_DEFAULT,
    "                                                      \
      number   : /-?[0-9]+/ ;                              \
      symbol   : \"list\" | \"head\" | \"tail\" | \"join\" \
               | \"eval\" | '+' | '-' | '*' | '/' ;        \
      sexpr    : '(' <expr>* ')' ;                         \
      qexpr    : '{' <expr>* '}' ;                         \
      expr     : <number> | <symbol> | <sexpr> | <qexpr> ; \
      lispy    : /^/ <expr>* /$/ ;                         \
    ",
    Number, Symbol, Sexpr, Qexpr, Expr, Lispy
  );

  puts("CLisp v0.1");
  puts("Press Ctrl+c to Exit");

  while (1) {
    char* input = readline("clisp> "); 
    add_history(input);

    // Parse user input
    mpc_result_t r;
    if (mpc_parse("<stdin>", input, Lispy, &r)) {
      lval* x = lval_eval(lval_read(r.output));
      lval_println(x);
      lval_del(x);
    } else {
      // Print Error
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }

    free(input);
  }

  // Undefine & delete parsers
  mpc_cleanup(6, Number, Symbol, Sexpr, Qexpr, Expr, Lispy);
  return 0;
}