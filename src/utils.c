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

#include <jash/utils.h>

char **jash_split_cstring(char *arg, char *delim, int *count) {
  int buff_size = JASH_SPLITSTR_BUFSIZE, index = 0;
  char **tokens = malloc(buff_size * sizeof(char *));
  char *token;

  if (!tokens) {
    perror("JASH");
    exit(EXIT_FAILURE);
  }

  token = strtok(arg, delim);
  while (token != NULL) {
    tokens[index] = token;
    index++;

    if (index >= buff_size) {
      buff_size += JASH_SPLITSTR_BUFSIZE;
      tokens = realloc(tokens, buff_size * sizeof(char *));
      if (!tokens) {
        perror("JASH");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, delim);
  }
  tokens[index] = NULL;

  if (count != NULL) {
    *count = index;
  }
  return tokens;
}

char *jash_substr(const char *str, int start, int length) {
  if (str == NULL || start < 0 || length < 0) {
    return NULL;
  }

  size_t str_length = strlen(str);
  length = (length < str_length - start) ? length : (str_length - start);

  char *substr = (char*)malloc((length + 1) * sizeof(char));
  if (substr == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    exit(EXIT_FAILURE);
  }

  strncpy(substr, str + start, length);
  substr[length] = '\0';

  return substr;
}

void jash_set_console_title(const char *title) {
  printf("\033]0;%s\007", title);
}
