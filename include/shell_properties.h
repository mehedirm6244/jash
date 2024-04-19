#ifndef SHELL_PROPERTIES
#define SHELL_PROPERTIES

#define MAX_CONFIG_LENGTH 64

struct ShellProperties {
  /*
   * Apparently all of the variables inside this
   * struct are of char type. Need a bit help over here
   */
  bool showWelcome;
  bool showPWD;
  bool showClock; 
  char *promptCharacter;
  char *cmdNotFound;
};

struct ShellProperties shellProperties;

void jash_init_shellProperties();

#endif
