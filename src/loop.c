#include "../include/loop.h"

void jash_loop() {
  int status;
  char *line;
  char **args;

  do {
    /* Draw the prompt */
    jash_prompt();

    /* Read line from input */
    line = jash_readLine();

    /* Split the line */
    args = jash_splitLine(line);

    /* Execute command */
    status = jash_execute(args);

    free(line);
    free(args);

    printf("\n");
  } while (status);
}

void jash_prompt() {
  /* Print the present working directory if not forbidden */
  if (shellProperties.showPWD == 1) {
    printf("\x1b[36m");
    char *pwd = strdup(getenv("PWD")),
         *home = strdup(getenv("HOME"));

    /* Print "~" instead of "/home/user" for aesthetics */
    if (strstr(pwd, home) != NULL) {
      int i;
      printf("~");
      for (i = strlen(home); i < strlen(pwd); i++) {
        printf("%c", pwd[i]);
      }
    } else {
      printf("%s", pwd);
    }

    printf("\x1b[0m");
  }

  /* Print a seperator if necessary */
  if (shellProperties.showPWD == 1 &&
    shellProperties.showClock == 1) {
    printf(" • ");
  }

  /* Print current time if not forbidden */
  if (shellProperties.showClock == 1) {
    struct tm * currentTime;
    time_t currentRawTime;
    time(&currentRawTime);
    currentTime = localtime(&currentRawTime);

    printf("\x1b[32m%d:%d:%d\x1b[0m",
      currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec);
  }

  /* Print a newline if necessary */
  if (shellProperties.showPWD == 1 ||
    shellProperties.showClock == 1) {
    printf("\n");
  }

  printf("\x1b[35m%s\x1b[0m ", shellProperties.promptCharacter);
}

char* jash_readLine() {
  int bufSize = JASH_READLINE_BUFSIZE;
  int index = 0;
  char ch;
  char *buffer = malloc(sizeof(char) * bufSize);

  if (!buffer) {
    perror("JASH");
    exit(EXIT_FAILURE);
  }

  while (1) {
    ch = getchar();

    if (ch == EOF) {
      exit(EXIT_SUCCESS);
    } else if (ch == '\n') {
      buffer[index] = '\0';
      return buffer;
    } else {
      buffer[index] = ch;
    }
    index++;

    if (index >= bufSize) {
      bufSize += JASH_READLINE_BUFSIZE;
      buffer = realloc(buffer, bufSize);

      if (!buffer) {
        perror("JASH");
        exit(EXIT_FAILURE);
      }
    }
  }
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
   * Exit if the command is empty
   */
  if (args[0] == NULL) {
    return 1;
  }

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
      perror("JASH");
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