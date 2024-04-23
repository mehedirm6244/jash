//
// Created by mdgaziur001 on 4/23/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <jash/trace.h>

void print_trace() {
  void *array[10];
  char **strings;
  int size;

  size = backtrace(array, 10);
  strings = backtrace_symbols(array, size);
  if (strings != NULL) {
    fprintf(stderr, "backtrace: \n");
    for (int i = 0; i < size; i++) {
      fprintf(stderr, "%s\n", strings[i]);
    }
  }

  free(strings);
}
