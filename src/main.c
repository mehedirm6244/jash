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
#include <string.h>

#include <jash/jash.h>
#include <jash/builtins/help.h>
#include <jash/config.h>
#include <jash/loop.h>

int main(int argc, char *argv[]) {
  /*
   * Check if any flag is passed
   * Example: `jash --version` will not call jash_loop() i.e. the shell
   * itself but just print the running version of JASH and then exit
   */
  int opt;
  for (opt = 1; opt < argc; opt++) {
    if (strcmp(argv[opt], "--help") == 0 || strcmp(argv[opt], "-h") == 0) {
      jash_help(NULL);
    } else if (strcmp(argv[opt], "--version") == 0 ||
      strcmp(argv[opt], "-v") == 0) {
      jash_version();
    } else {
      printf("Invalid command!\n");
      printf("Recognised formats are:\n");
      printf("  -h | --help\n");
      printf("  -v | --version\tShow current JASH version\n");
      exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
  }

  /* Initialize shell properties */
  jash_initConfig();

  /*
   * Show a welcome message when running JASH for the first time
   * unless disabled in the config
   */
  if (shellConfig.showWelcome == 1) {
    jash_welcome();
  }

  /* JASH starts from here */
  jash_loop();

  return EXIT_SUCCESS;
}
