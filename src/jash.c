#include "../include/jash.h"

void jash_version() {
  printf("JASH version %d.%d.%d\n",
    JASH_VERSION_MAJOR, JASH_VERSION_MINOR, JASH_VERSION_PATCH);
}