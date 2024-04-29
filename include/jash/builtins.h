#ifndef JASH_BUILTINS
#define JASH_BUILTINS

#define N_BUILTINS 3

extern char *jash_builtins[];
extern int (*jash_builtin_func[]) (char **);

#endif // JASH_BUILTINS