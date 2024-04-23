//
// Created by mdgaziur001 on 4/23/24.
//

#include <ctype.h>
#include <linux/limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include <jash/jstring.h>
#include <jash/readline.h>
#include <jash/trace.h>

struct termios old;

void init_termios() {
  tcgetattr(STDIN_FILENO, &old);
  atexit(reset_termios);
}

void reset_termios() {
  tcsetattr(STDIN_FILENO, TCSANOW, &old);
}

static void jputch(char ch) {
  write(STDOUT_FILENO, &ch, 1);
}

static void jputs(const char *string) {
  for (size_t i = 0; i < strlen(string); i++) {
    jputch(string[i]);
  }
}

static void jprintf(const char *fmt, ...) {
  va_list argp;
  va_start(argp, fmt);

  char *res;
  if (vasprintf(&res, fmt, argp) != -1) {
    jputs(res);
    free(res);
  }

  va_end(argp);
}

static void add_history(const char *command) {
  char history_path[PATH_MAX];
  sprintf(history_path, "%s/.jash_history", getenv("HOME"));

  FILE *history_file = fopen(history_path, "a+");

  fwrite("\n", sizeof(char), 1, history_file);
  fwrite(command, sizeof(char), strlen(command), history_file);
  fclose(history_file);
}

static JString *read_history() {
  char history_path[PATH_MAX];
  sprintf(history_path, "%s/.jash_history", getenv("HOME"));

  size_t fsize;
  FILE *history_file = fopen(history_path, "r");
  if (!history_file) {
    return jstring_create();
  }
  fseek(history_file, 0, SEEK_END);
  fsize = ftell(history_file);
  rewind(history_file);

  char *history_content = malloc(sizeof(char) * fsize + 1);
  if (!history_content) {
    fprintf(stderr, "readline[Fatal]: failed to allocate buffer for history");
    print_trace();
    exit(1);
  }

  fread(history_content, sizeof(char), fsize, history_file);
  history_content[fsize] = '\0';

  JString *history = jstring_create_from_buf(history_content);

  fclose(history_file);
  free(history_content);

  return history;
}

void clear_input_from_terminal(size_t input_start_x, size_t input_start_y, JString *input) {
  jprintf("\x1b[%zu;%zuH", input_start_x, input_start_y);
  for (size_t i = 0; i < jstring_len(input); i++) {
    jputch(' ');
  }
  jprintf("\x1b[%zu;%zuH", input_start_x, input_start_y);
}

void clear_input(size_t input_start_x, size_t input_start_y, JString *input) {
  clear_input_from_terminal(input_start_x, input_start_y, input);
  jstring_clear(input);
}

void set_cursor_pos(size_t y, size_t x) {
  jprintf("\x1b[%zu;%zuH", y, x);
}

void get_cursor_pos(size_t *y, size_t *x) {
  printf("\x1b[6n");
  scanf("\x1b[%zu;%zuR", y, x);
}

JString *read_line(const char *prompt) {
  struct termios new;
  tcgetattr(STDIN_FILENO, &new);
  new.c_lflag &= ~(ECHO | ICANON | ECHOE | IEXTEN | IGNBRK);
  tcsetattr(STDIN_FILENO, TCSANOW, &new);

  JString *history_file = read_history();
  size_t previous_commands_count;
  JString **previous_commands = jstring_split(history_file, '\n', &previous_commands_count);

  size_t current_command_from_history = previous_commands_count;

  printf("%s", prompt);

  size_t input_start_y, input_start_x;
  get_cursor_pos(&input_start_y, &input_start_x);

  size_t max_pos = 1, cur_pos = 1;

  JString *input = jstring_create();

  while (1) {
    char ch = (char) getchar();

    if (ch == '\x1b') {
      while (!isalnum(ch)) {
        ch = (char) getchar();
      }
      switch (ch) {
        case 'A':
          if (current_command_from_history > 0) {
            clear_input(input_start_y, input_start_x, input);

            JString *command_to_display = previous_commands[current_command_from_history - 1];
            max_pos = cur_pos = jstring_len(command_to_display) + 1;
            jputs(jstring_get_string(command_to_display));
            jstring_free(input);
            input = jstring_clone(command_to_display);
            current_command_from_history -= 1;
          }
          break;
        case 'B':
          if (current_command_from_history < previous_commands_count - 1) {
            current_command_from_history += 1;
            clear_input(input_start_y, input_start_x, input);

            JString *command_to_display = previous_commands[current_command_from_history - 1];
            max_pos = cur_pos = jstring_len(command_to_display) + 1;
            jputs(jstring_get_string(command_to_display));
            jstring_free(input);
            input = jstring_clone(command_to_display);
          } else {
            max_pos = cur_pos = 1;
            current_command_from_history = previous_commands_count - 1;
            clear_input(input_start_y, input_start_x, input);
            jputs(jstring_get_string(input));
          }
          break;
        case 'C':
          if (cur_pos != max_pos) {
            jputs("\x1b[1C");
            cur_pos += 1;
          }
          break;
        case 'D':
          if (cur_pos > 1) {
            jputs("\x1b[1D");
            cur_pos -= 1;
          }
          break;
        case '3':
          if (getchar() == '~') {
            if (cur_pos <= jstring_len(input) && !jstring_is_empty(input)) {
              size_t cur_y, cur_x;
              get_cursor_pos(&cur_y, &cur_x);
              clear_input_from_terminal(input_start_y, input_start_x, input);

              jstring_remove(input, cur_pos - 1);

              jprintf("%s", jstring_get_string(input));

              set_cursor_pos(cur_y, cur_x);

              max_pos -= 1;
            }
          }
          break;
        default:
          jputch('\x1b');
          jputch(ch);
      }
    } else if (ch == new.c_cc[VERASE]) {
      if (cur_pos > 1) {
        size_t cur_y, cur_x;
        get_cursor_pos(&cur_y, &cur_x);
        clear_input_from_terminal(input_start_y, input_start_x, input);
        jstring_remove(input, cur_pos - 2);
        jprintf("%s", jstring_get_string(input));

        set_cursor_pos(cur_y, cur_x - 1);

        cur_pos -= 1;
        max_pos -= 1;
      }
    } else if (ch == new.c_cc[VEOF]) {
      jstring_free(input);
      input = jstring_create_from_buf("exit\n");
      jputs(jstring_get_string(input));
      break;
    } else if (ch == new.c_cc[VINTR]) {
      jprintf("^C\n");
      jstring_free(input);
      input = jstring_create();
      break;
    } else {
      if (ch == '\n') {
        jputch(ch);
        break;
      }

      if (cur_pos != max_pos) {
        jstring_insert(input, cur_pos - 1, ch);

        size_t cur_y, cur_x;
        get_cursor_pos(&cur_y, &cur_x);

        clear_input_from_terminal(input_start_y, input_start_x, input);

        jprintf("%s", jstring_get_string(input));

        set_cursor_pos(cur_y, cur_x + 1);
      } else {
        jstring_push(input, ch);
        jputch(ch);
      }
      cur_pos += 1;
      max_pos += 1;
    }
  }

  if (!jstring_is_empty(input)) {
    add_history(jstring_get_string(input));
  }

  jstring_free(history_file);
  for (size_t i = 0; i < previous_commands_count; i++) {
    jstring_free(previous_commands[i]);
  }
  free(previous_commands);

  reset_termios();
  return input;
}
