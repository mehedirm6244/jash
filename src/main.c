/*
 * jash is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * jash is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with jash. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include <linux/limits.h>
#include <readline/readline.h> 
#include <readline/history.h> 

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
      printf("Invalid format!\n");
      printf("Available formats are:\n");
      printf("  -h | --help\n");
      printf("  -v | --version\tShow current JASH version\n");
      return 0;
    }
  }

  /* Initialize shell properties */
  jash_init_shellProperties();

  /*
   * Show a welcome message when running JASH for the first time
   * unless disabled in the config
   */
  if (shellProperties.showWelcome == 1) {
    printf("JASH %d.%d.%d\n", JASH_VERSION_MAJOR, JASH_VERSION_MINOR, JASH_VERSION_PATCH);
    printf("Welcome to JASH (Just Another SHell)!\n");
    printf("To submit an issue, visit https://github.com/mehedirm6244/jash/issues\n");
    printf("Type `help` for assistance\n\n");
  }

  /* JASH starts from here */
  jash_loop();

  return EXIT_SUCCESS;
}
