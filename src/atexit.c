/*
 * Copyright (C) 2026 TheCRatTime
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Without compile_commands.json or
 * needed not <path>, using quotes.
 * If set path, use it.
 */

#define ATEXIT_DEBUG

#if !defined(INC_USE_QUOTES)
# include <atexit.h>
#else
# if defined(ATEXIT_PATH_TO_INC)
# include ATEXIT_PATH_TO_INC
# else
# include "../include/atexit.h"
# endif
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

/* fclose() isn't equals void (*func)(void*),
   needed wrapper. */
static void AtExitFClose(void* file) {
  FILE* as_file = (FILE*)file;
  fclose(as_file);
}

AtExitHeader* InitAtExit(int size) {
  AtExitHeader* ret = NULL;
  int cur_node = 0;

  if (size < 0) {
    return ret;
  }

  /* Default value */
  if (size == 0) {
    size = DEFAULT_ATEXIT_SIZE;
  }

  ret = (AtExitHeader*)MemAlloc(sizeof(AtExitHeader), NULL);
  if (ret == NULL) {
    return NULL;
  }

  ret->capacity = size;
  ret->used     = 0;
  ret->array = (AtExitNode*)MemAlloc(sizeof(AtExitNode) * (size_t)size, NULL);
  if (ret->array == NULL) {
    MemFree(ret, 1, (void*)sizeof(AtExitHeader));
    return NULL;
  }

  for(; cur_node < size; cur_node++) {
    ret->array[cur_node].function = NULL;
    ret->array[cur_node].arg      = NULL;
  }
  
  return ret;
}

int SetAtExit(AtExitHeader* head, void (*to_set)(void*), void* arg) {
  if (head == NULL || to_set == NULL || arg == NULL) {
    return 1;
  }

  /* Return 1 if haven't places for function */
  if (head->used >= head->capacity) {
    return 1;
  }

  head->array[head->used].function = to_set;
  head->array[head->used].arg      =    arg;
  head->used++;
  
  return 0;
}

void DoAtExit(AtExitHeader* head) {
  int cur_func = 0;
  if (head == NULL) {
    return;
  }

  /* Using LIFO -> Last In First Out. */
  for(cur_func = head->used; cur_func >= 0; cur_func--) {
    void* argument = head->array[cur_func].arg;
    if (head->array[cur_func].function == NULL) {
      continue;
    }

    head->array[cur_func].function(argument);

    head->array[cur_func].function = NULL;
    head->array[cur_func].arg      = NULL;
  }

  /* Need to reset value of used. */  
  head->used = 0;
}

/* AtExitMalloc and AtExitFopen just wrappers
   for malloc()/fopen() and SetAtExit */

void* AtExitMalloc(AtExitHeader* head, size_t bytes) {
  void* ret = NULL;

  if (head == NULL || bytes <= 0) {
    return NULL;
  }

  ret = malloc(bytes);
  if (ret == NULL) {
    return NULL;
  }

  SetAtExit(head, free, ret);
  
  return ret;
}

FILE* AtExitFopen(AtExitHeader* head, const char* filename,
                  const char* modes) {
  FILE* ret = NULL;
  if (head == NULL || filename == NULL || modes == NULL) {
    return NULL;
  }

  ret = fopen(filename, modes);
  if (!ret) {
    return NULL;
  }

  /* Using internal function. */
  SetAtExit(head, AtExitFClose, ret);

  return ret;
}

void AtExitClean(AtExitHeader *head) {
  int cur_node = 0;
  if (head == NULL) {
    return;
  }

  for(; cur_node < head->capacity; cur_node++) {
    head->array[cur_node].function = NULL;
    head->array[cur_node].arg      = NULL;
  }

  head->used = 0;
  
  return;
}

void AtExitFree(AtExitHeader *head) {
  size_t size1 = 0;
  size_t size2 = 0;
  if (head == NULL) {
    return;
  }

  DoAtExit(head);

  size1 = sizeof(AtExitNode) * (size_t)head->capacity;
  size2 = sizeof(AtExitHeader);
  
  MemFree(head->array, 1, &size1);
  MemFree(head, 1, &size2);
}
