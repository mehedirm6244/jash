#include "../../include/builtins/help.h"

int jash_help(char **args) {
  printf("Builtin commands:\n");
  printf("  cd\tChange directory\n");
  printf("  exit\tExit jash\n");
  printf("  help\tShow this message\n");
  printf("For other commands, use `man command`\n");

  return 1;
}