#ifndef SHELL_CONFIG
#define SHELL_CONFIG

#include <stdbool.h>

struct ShellConfig {
  /*
   * Apparently all of the variables inside this
   * struct are of char type. Need a bit help over here
   */
  bool showWelcome;
  bool showPWD;
  char *promptCharacter;
};

extern struct ShellConfig shellConfig;

void jash_initConfig();

#endif
