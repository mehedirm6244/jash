#include <stdio.h>

#include <jash/builtins/exit.h>

int jash_exit(char **args) {
  printf("JASH: exiting...");
  return 0;
}