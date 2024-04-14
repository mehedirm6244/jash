#include "../../include/builtins/cd.h"

int jash_cd(char **args) {
  /*
   * cd to ${HOME} if no argument is passed
   */
  if (args[1] == NULL) {
    args[1] = strdup(getenv("HOME"));
  }

  if (chdir(args[1]) != 0) {
    perror("JASH");
  } else {
    /*
     * Overwrite PWD shell environment to the current
     * working directory
     */
    char pwd[PATH_MAX];
    getcwd(pwd, sizeof(pwd));
    setenv("PWD", pwd, 1);
  }

  return 1;
}