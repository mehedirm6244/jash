#ifndef JASH_BUILTINS_H
#define JASH_BUILTINS_H

#define N_BUILTINS 3

extern char *jash_builtins[];
extern int (*jash_builtinFunc[]) (char **);

#endif