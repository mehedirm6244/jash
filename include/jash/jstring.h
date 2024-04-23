#ifndef JASH_JSTRING_H
#define JASH_JSTRING_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct {
  char *buf;
  size_t size, cap;
} JString;

JString *jstring_create();

JString *jstring_clone(JString *);

JString *jstring_create_from_buf(const char *);

void jstring_free(JString *);

const char *jstring_get_string(JString *);

char *jstring_get_cstring_copy(JString *j_string);

char jstring_index(JString *, size_t);

char jstring_last(JString *);

size_t jstring_last_idx(JString *j_string);

size_t jstring_len(JString *);

bool jstring_is_empty(JString *);

void jstring_set(JString *, size_t, char);

void jstring_append(JString *, JString *);

void jstring_push(JString *, char);

void jstring_insert(JString *, size_t, char);

char jstring_pop(JString *);

char jstring_remove(JString *, size_t);

void jstring_clear(JString *);

JString **jstring_split(JString *, char, size_t *);

#endif //JASH_JSTRING_H
