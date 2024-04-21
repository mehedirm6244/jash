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

#include "../include/loop.h"

void jash_loop() {
  int status;
  char *buffer;
  char **args;
  time_t timeElapsed = 0;

  do {
    /* Read line from input */
    buffer = jash_prompt(timeElapsed);

    /* Save input to history */
    add_history(buffer);

    /*
     * TODO: check for multi-commands in a single line
     * through looking for ';'
     * Corner case: `echo "hqhqhq; hqhqhq"
     *
     * How to implement:
     * Use strtok, and something equivalent to string commands[]
     * then for each cmd in commands:
     *   splitline(cmd)
     *   execute(cmd)
     */

    /*
     * Split input to tokens
     * args[0] is the command being called
     * args[i] (i>0) is an argument of args[0]
     */
    args = jash_splitLine(buffer);

    timeElapsed = time(NULL);

    /* Execute the command if it's not empty to prevent segfault */
    if (args[0] != NULL) {
      status = jash_execute(args);
    } else {
      status = 1;
    }

    timeElapsed = time(NULL) - timeElapsed;

    free(buffer);
    free(args);

    printf("\n");
  } while (status);
}

char* jash_prompt(time_t timeElapsed) {
  /* Print the present working directory if not forbidden */
  if (shellConfig.showPWD == 1) {
    char *pwd = strdup(getenv("PWD")),
         *home = strdup(getenv("HOME"));

    printf("\x1b[36m");

    /* Print "~" instead of "/home/user" for aesthetics */
    if (strstr(pwd, home) != NULL) {
      int i;
      printf("~");
      for (i = strlen(home); i < strlen(pwd); i++) {
        printf("%c", pwd[i]);
      }
    } else {
      /* Example: "/usr/share" which is not under $HOME */
      printf("%s", pwd);
    }

    printf("\x1b[0m • \x1b[32mTook %lds\x1b[0m\n", timeElapsed);
  }
  
  /*
   * prompt[1] or ps1 is used for single line inputs
   * prompt[2] or ps2 is used for multi line inputs
   */
  char *prompt[2];
  asprintf(&prompt[0], "\x1b[35m%s\x1b[0m ", shellConfig.promptCharacter);
  asprintf(&prompt[1], "… \x1b[33m%s\x1b[0m ", shellConfig.promptCharacter);
  int promptIdx = 0;

  int bufSize = JASH_READLINE_BUFSIZE, index = 0;
  char *buffer = malloc(bufSize * sizeof(char*));

  if (!buffer) {
    perror("JASH");
    exit(EXIT_FAILURE);
  }
  /*
   * Prevent weird but obvious behavior for using strcat(); Read more: 
   * https://stackoverflow.com/questions/18838933/why-do-i-first-have-to-strcpy-before-strcat
   */
  buffer[0] = '\0';

  /*
   * This is a very dirty way to handle multiline inputs
   * Anyway, who cares
   */
  bool readInput = true;
  do {
    char *line;
    int lineLength;

    line = readline(prompt[promptIdx]);
    lineLength = strlen(line);
    if (line[lineLength - 1] == '\\') {
      line[lineLength - 1] = '\0';
      promptIdx = 1;
    } else {
      readInput = false;
      promptIdx = 0;
    }

    index += lineLength;
    if (index >= bufSize) {
      bufSize += JASH_READLINE_BUFSIZE;
      buffer = realloc(buffer, bufSize * sizeof(char*));

      if (!buffer) {
        perror("JASH");
        exit(EXIT_FAILURE);
      }
    }

    strcat(buffer, line);
  } while (readInput);
  
  return buffer;
}

char** jash_splitLine(char *arg) {
  int bufSize = JASH_SPLITLINE_BUFSIZE;
  int index = 0;
  char **tokens = malloc(bufSize * sizeof(char*));
  char *token;

  if (!tokens) {
    perror("JASH");
    exit(EXIT_FAILURE);
  }

  token = strtok(arg, JASH_SPLITLINE_DELIM);
  while (token != NULL) {
    tokens[index] = token;
    index++;

    if (index >= bufSize) {
      bufSize += JASH_SPLITLINE_BUFSIZE;
      tokens = realloc(tokens, bufSize * sizeof(char*));
      if (!tokens) {
        perror("JASH");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, JASH_SPLITLINE_DELIM);
  }
  tokens[index] = NULL;
  return tokens;
}

int jash_execute(char **args) {
  /*
   * Check if the command is a builtin of JASH
   * If that's the case, then there's no need to
   * spawn a fork and do stuffs
   * We can call that command directly as a function
   * then
   */
  int i, numberOfBuiltins = (sizeof(jash_builtins) / sizeof(char *));
  for (i = 0; i < numberOfBuiltins; i++) {
    if (strcmp(args[0], jash_builtins[i]) == 0) {
      return (*jash_builtinFunc[i])(args);
    }
  }

  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    if (execvp(args[0], args) == -1) {
      printf("JASH: Failed to execute: %s\n", args[0]);
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    perror("JASH");
  } else {
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}
