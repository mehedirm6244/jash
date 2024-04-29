#ifndef JASH_UTILS
#define JASH_UTILS

#define JASH_SPLITSTR_BUFSIZE 64

char **jash_split_cstring(char *arg, char *delim, int *count);

char *jash_substr(const char *str, int start, int length);

void jash_set_console_title(const char *title);

#endif // JASH_UTILS
