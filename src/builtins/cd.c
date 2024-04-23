/*
 * This file is a part of jash
 *
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
#include <linux/limits.h>
#include <unistd.h>

#include <jash/builtins/cd.h>

int jash_cd(char **args) {
  /* cd to $HOME if no argument is passed */
  if (args[1] == NULL) {
    args[1] = strdup(getenv("HOME"));
  }

  /* Replace ~ with $HOME (/home/user) */
  if (args[1][0] == '~') {
    char *home = strdup(getenv("HOME"));
    size_t homeLen = strlen(home);
    memmove(args[1] + homeLen, args[1] + 1, strlen(args[1]));
    memcpy(args[1], home, homeLen);
  }

  if (chdir(args[1]) != 0) {
    perror("JASH");
  } else {
    /* Overwrite PWD shell environment to the current working directory */
    char pwd[PATH_MAX];
    getcwd(pwd, sizeof(pwd));
    setenv("PWD", pwd, 1);
  }

  return 1;
}
