#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <jash/jstring.h>
#include <jash/trace.h>

#define GROW_BUFFER() \
  if (j_string->size + 1 >= j_string->cap) { \
    j_string->cap *= 2; \
    jstring_realloc(j_string, j_string->cap); \
  }

#define BOUNDS_CHECK(idx) \
  if (idx >= j_string->size) { \
    fprintf(stderr, "JString[Fatal]: out of bound access. \
      Index is %zu and size is %zu\n", idx, j_string->size); \
    print_trace(); \
    exit(EXIT_FAILURE); \
  }

static void *jstring_alloc(size_t size) {
  void *block = malloc(size);
  if (block == NULL) {
    fprintf(stderr, "JString[Fatal]: \
      failed to allocate block with size %zu\n", size);
    print_trace();
    exit(EXIT_FAILURE);
  }

  return block;
}

static void jstring_realloc(JString *j_string, size_t new_size) {
  void *block = realloc(j_string->buf, new_size);
  if (block == NULL) {
    fprintf(stderr, "JString[Fatal]: \
      failed to reallocate block with new_size %zu\n", new_size);
    print_trace();
    exit(EXIT_FAILURE);
  }

  j_string->buf = block;
}

JString *jstring_create() {
  JString *jstring = (JString *) jstring_alloc(sizeof(JString));

  jstring->buf = (char *) jstring_alloc(sizeof(char));
  jstring->buf[0] = '\0';
  jstring->size = 0;
  jstring->cap = 1;

  return jstring;
}

JString *jstring_clone(JString *src) {
  JString *jstring = (JString *) jstring_alloc(sizeof(JString));

  jstring->buf = (char *) jstring_alloc(sizeof(char) * src->size + 1);
  memcpy(jstring->buf, src->buf, src->size + 1);
  jstring->size = src->size;
  jstring->cap = src->size + 1;

  return jstring;
}

JString *jstring_create_from_buf(const char *src_buf) {
  JString *jstring = (JString *) jstring_alloc(sizeof(JString));

  size_t src_size = strlen(src_buf);
  size_t src_alloc_size = src_size * sizeof(char) + 1;
  size_t buf_alloc_size = src_alloc_size + !(src_alloc_size & 1);

  jstring->buf = (char *) jstring_alloc(buf_alloc_size);
  jstring->size = src_size;
  jstring->cap = buf_alloc_size;

  strcpy(jstring->buf, src_buf);

  return jstring;
}

void jstring_free(JString *j_string) {
  free(j_string->buf);
  free(j_string);
}

const char *jstring_get_string(JString *j_string) {
  return j_string->buf;
}

char *jstring_get_cstring_copy(JString *j_string) {
  char *buf = jstring_alloc(j_string->size + 1);

  strcpy(buf, j_string->buf);

  return buf;
}

size_t jstring_len(JString *j_string) {
  return j_string->size;
}

bool jstring_is_empty(JString *j_string) {
  return j_string->size == 0;
}

char jstring_index(JString *j_string, size_t idx) {
  BOUNDS_CHECK(idx)

  return j_string->buf[idx];
}

char jstring_last(JString *j_string) {
  if (j_string->size == 0) {
    return '\0';
  }

  return j_string->buf[j_string->size - 1];
}

size_t jstring_last_idx(JString *j_string) {
  if (j_string->size == 0) {
    return 0;
  }

  return j_string->size - 1;
}

void jstring_set(JString *j_string, size_t idx, char ch) {
  BOUNDS_CHECK(idx)

  j_string->buf[idx] = ch;
}

void jstring_push(JString *j_string, char ch) {
  GROW_BUFFER()

  j_string->buf[j_string->size] = ch;
  j_string->buf[j_string->size + 1] = '\0';
  j_string->size++;
}

void jstring_append(JString *lhs, JString *rhs) {
  for (size_t idx = 0; idx < jstring_len(rhs); idx++) {
    jstring_push(lhs, jstring_index(rhs, idx));
  }
}

void jstring_insert(JString *j_string, size_t idx, char ch) {
  BOUNDS_CHECK(idx)
  GROW_BUFFER()

  for (size_t i = j_string->size + 1; i > idx; i--) {
    j_string->buf[i] = j_string->buf[i - 1];
  }

  j_string->buf[idx] = ch;
  j_string->size += 1;
}

char jstring_pop(JString *j_string) {
  return jstring_remove(j_string, jstring_last_idx(j_string));
}

char jstring_remove(JString *j_string, size_t idx) {
  BOUNDS_CHECK(idx)

  if (j_string->size == 0) {
    return '\0';
  }

  char removed = jstring_index(j_string, idx);

  for (size_t i = idx; i < j_string->size; i++) {
    j_string->buf[i] = j_string->buf[i + 1];
  }

  j_string->size -= 1;
  return removed;
}

void jstring_clear(JString *j_string) {
  while (!jstring_is_empty(j_string)) {
    jstring_pop(j_string);
  }
}

JString **jstring_split(JString *j_string, char delim, size_t *count) {
  JString **splitted = NULL;
  JString *tmp = jstring_create();
  size_t n_splitted = 0;

  for (int i = 0; i < jstring_len(j_string); i++) {
    if (jstring_index(j_string, i) == delim) {
      int j;
      for (j = i; j < jstring_len(j_string); j++) {
        if (jstring_index(j_string, j) != ' ') {
          break;
        }
      }
      i = j;

      n_splitted += 1;
      JString **reallocd = realloc(splitted, sizeof (JString *) * n_splitted);
      if (!reallocd) {
        fprintf(stderr, "JString[Fatal]: failed to reallocate buffer");
        print_trace();
        exit(1);
      }
      splitted = reallocd;

      splitted[n_splitted - 1] = tmp;
      tmp = jstring_create();
    } else {
      jstring_push(tmp, jstring_index(j_string, i));
    }
  }
  if (!jstring_is_empty(tmp)) {
    n_splitted += 1;
    JString **reallocd = realloc(splitted, sizeof (JString *) * n_splitted);
    if (!reallocd) {
      fprintf(stderr, "JString[Fatal]: failed to reallocate buffer");
      print_trace();
      exit(EXIT_FAILURE);
    }
    splitted = reallocd;

    splitted[n_splitted - 1] = tmp;
  } else {
    jstring_free(tmp);
  }

  *count = n_splitted;
  return splitted;
}
