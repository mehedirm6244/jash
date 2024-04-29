#ifndef JASH_CONFIG
#define JASH_CONFIG

#include <stdbool.h>

struct Shell_config {
  bool show_welcome;
  bool show_pwd;
  char *prompt_char;
};

extern struct Shell_config shell_config;

void jash_init_config();

#endif //JASH_CONFIG
