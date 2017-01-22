#include <stdio.h>
#include <stdlib.h>

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
  puts("CLisp v0.1");
  puts("Press Ctrl+c to Exit");

  while (1) {
    char* input = readline("clisp> "); 
    add_history(input);

    printf("%s", input);
    free(input);
  }

  return 0;
}
