#ifndef JASH_LOOP
#define JASH_LOOP

#include "jstring.h"

int jash_execute(char **args);

void jash_loop();

JString *jash_prompt(time_t);

void add_history(const char *);

JString *read_line(const char *);

void replace_variable(char *arg);

#endif // JASH_LOOP
