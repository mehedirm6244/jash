/*
 * Feel free to do whatever with this program
 * Repository: github.com/mehedirm6244/jash
 * License: GNU GPL-3
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include <linux/limits.h>

#include "jash.c"
#include "builtins.c"
#include "shell_properties.c"
#include "loop.c"

int main(int argc, char *argv[]) {
  /*
   * Check if any flag is passed
   * example: `jash --version` will not run jash_loop
   * but just print the running version of JASH and then
   * exit
   */
  int opt;
  for (opt = 1; opt < argc; opt++) {
    if (strcmp(argv[opt], "--help") == 0 || strcmp(argv[opt], "-h") == 0) {
      jash_help(NULL);
      return 0;
    } else if (strcmp(argv[opt], "--version") == 0 ||
      strcmp(argv[opt], "-v") == 0) {
      printf("JASH version %d.%d.%d\n",
        JASH_VERSION_MAJOR, JASH_VERSION_MINOR, JASH_VERSION_PATCH);
      return 0;
    } else {
      printf("Invalid format! Type `jash --help` for assistance");
    }
  }

  /*
   * Initialize shell properties
   */
  jash_init_shellProperties();

  /*
   * Show a welcome message when running JASH for the first time
   * unless disabled in the config
   */
  if (shellProperties.showWelcome == "1") {
    printf("JASH %d.%d.%d\n", JASH_VERSION_MAJOR, JASH_VERSION_MINOR, JASH_VERSION_PATCH);
    printf("Welcome to JASH (Just Another SHell)!\n");
    printf("To submit an issue, visit https://github.com/mehedirm6244/jash/issues\n");
    printf("Type `help` for assistance\n\n");
  }

  /*
   * JASH starts from here
   */
  jash_loop();

  return EXIT_SUCCESS;
}