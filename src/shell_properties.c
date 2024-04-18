#include "../include/shell_properties.h"

void jash_init_shellProperties() {
  /*
   * These are the default properties for JASH shell
   * These will be overwritten by `.jashrc` if it exists
   */
  shellProperties.showWelcome = 1;
  shellProperties.showPWD = 1;
  shellProperties.showClock = 1;
  shellProperties.promptCharacter = "❯";

  // TO BE IMPLEMENTED
}