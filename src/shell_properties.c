#include "../include/shell_properties.h"

void jash_init_shellProperties() {
  /*
   * These are the default properties for JASH shell
   * These will be overwritten by `.jashrc` if it exists
   */
  shellProperties.showWelcome = "1";
  shellProperties.promptCharacter = "\x1b[35m❯\x1b[0m";

  // TO BE IMPLEMENTED
}