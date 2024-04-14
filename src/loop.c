#include "../include/loop.h"

void jash_loop() {
  int status;
  char *line;
  char **args;

  /* For displaying current time */
  struct tm * currentTime;
  time_t currentRawTime;

  do {
    /* Get current time */
    time(&currentRawTime);
    currentTime = localtime(&currentRawTime);

    /* Draw the prompt */
    char *pwd = strdup(getenv("PWD"));
    printf("\x1b[36m%s\x1b[0m", pwd);
    printf(" • \x1b[32m%d:%d:%d\x1b[0m\n",
      currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec);
    printf("%s ", shellProperties.promptCharacter);

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