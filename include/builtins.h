#ifndef JASH_BUILTINS_H
#define JASH_BUILTINS_H

#include "../src/builtins/help.c"
#include "../src/builtins/cd.c"
#include "../src/builtins/exit.c"

char *jash_builtins[] = {
  "cd",
  "help",
  "exit"
};

int (*jash_builtinFunc[]) (char **) = {
  &jash_cd,
  &jash_help,
  &jash_exit
};

#endif