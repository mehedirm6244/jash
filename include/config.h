#ifndef SHELL_CONFIG
#define SHELL_CONFIG

struct ShellConfig {
  /*
   * Apparently all of the variables inside this
   * struct are of char type. Need a bit help over here
   */
  bool showWelcome;
  bool showPWD;
  char *promptCharacter;
};

struct ShellConfig shellConfig;

void jash_init_shellProperties();

#endif
