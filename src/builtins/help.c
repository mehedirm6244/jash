#include "../../include/builtins/help.h"

int jash_help(char **args) {
  printf("Welcome to JASH (Just Another SHell)!\n");
  printf("To submit an issue, visit https://github.com/mehedirm6244/jash/issues\n\n");
  printf("Supported builtin commands:\n");
  printf("  cd\tChange directory\n");
  printf("  exit\tExit jash\n");
  printf("  help\tShow this message\n");
  printf("For other commands, use `man command`\n");

  return 1;
}