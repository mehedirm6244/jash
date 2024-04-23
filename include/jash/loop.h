#ifndef JASH_LOOP
#define JASH_LOOP

#include "jstring.h"

#define JASH_READLINE_BUFSIZE 256
#define JASH_SPLITLINE_BUFSIZE 64
#define JASH_SPLITLINE_DELIM " \t\r\n\a"

int jash_execute(char **args);

void jash_loop();

JString *jash_prompt(time_t);

void add_history(const char *);

JString *read_line(const char *);

char **jash_splitLine(char *arg);

#endif
