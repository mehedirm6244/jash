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
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include <jash/builtins.h>
#include <jash/config.h>
#include <jash/loop.h>
#include <jash/readline.h>
#include <jash/trace.h>
#include <jash/utils.h>

void signal_handler() {}

void jash_loop() {
  int status;
  JString *buffer;
  char **args;
  time_t time_elapsed;
  init_termios();
  signal(SIGINT, signal_handler);

  /* Set console window title */
  jash_set_console_title("jash");

  do {
    int cmd_count;
    char **cmd;
    status = 1;
    time_elapsed = 0;

    /* Read input */
    buffer = jash_prompt(time_elapsed);
    char *c_string = jstring_get_cstring_copy(buffer);

    /*
     * Check if the buffer contains multiple commands
     * and split them to execute individually
     */
    cmd = jash_split_cstring(c_string, ";", &cmd_count);

    time_elapsed = time(NULL);
    for (int i = 0; i < cmd_count; i++) {
      char **args = jash_split_cstring(cmd[i], " \t\r\n\a", NULL);

      /* Avoid SEGFAULT by ensuring that input is not empty */
      if (args[0] != NULL) {
        status = jash_execute(args);
      }

      free(args);
    }
    time_elapsed = time(NULL) - time_elapsed;


    jstring_free(buffer);
    free(c_string);
    free(cmd);

    printf("\n");
  } while (status);
}

JString *jash_prompt(time_t time_elapsed) {
  if (shell_config.show_pwd) {
    char *pwd = strdup(getenv("PWD")),
        *home = strdup(getenv("HOME"));

    /* Print "~" instead of "/home/user" */
    if (strstr(pwd, home) != NULL) {
      int home_len = strlen(home), pwd_len = strlen(pwd);
      asprintf(&pwd, "~%s", jash_substr(pwd, home_len, pwd_len - home_len));
    }
      
    printf("\x1b[36m%s\x1b[0m • \x1b[32mTook %lds\x1b[0m\n", pwd, time_elapsed);

    free(pwd);
    free(home);
  }

  /*
   * prompt[1] or ps1 for single line inputs
   * prompt[2] or ps2 for multi line inputs
   */
  int prompt_idx = 0;
  char *prompt[2];
  asprintf(&prompt[0], "\x1b[35m%s\x1b[0m ", shell_config.prompt_char);
  asprintf(&prompt[1], "… \x1b[33m%s\x1b[0m ", shell_config.prompt_char);

  /* A very dirty way to handle multiline inputs */
  JString *buffer = jstring_create();
  JString *raw_buffer = jstring_create();
  bool read_input = true;

  do {
    JString *line = read_line(prompt[prompt_idx]);
    if (!jstring_is_empty(line)) {
      if (!jstring_is_empty(raw_buffer)) {
        jstring_push(raw_buffer, '\n');
      }
      jstring_append(raw_buffer, line);
    }

    if (jstring_last(line) == '\\') {
      jstring_remove(line, jstring_last_idx(line));
      prompt_idx = 1;
    } else {
      read_input = false;
    }

    jstring_append(buffer, line);
    jstring_free(line);
  } while (read_input);

  jstring_free(raw_buffer);
  free(prompt[0]);
  free(prompt[1]);
  return buffer;
}

int jash_execute(char **args) {
  /* Check if input is a builtin. If so, it can be called directly */
  for (int i = 0; i < N_BUILTINS; i++) {
    if (strcmp(args[0], jash_builtins[i]) == 0) {
      return (*jash_builtin_func[i])(args);
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
