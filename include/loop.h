#ifndef JASH_LOOP
#define JASH_LOOP

#define JASH_READLINE_BUFSIZE 1024
#define JASH_SPLITLINE_BUFSIZE 64
#define JASH_SPLITLINE_DELIM " \t\r\n\a"

int jash_execute(char **args);
void jash_loop();
char* jash_readLine();
char** jash_splitLine(char *arg);

#endif