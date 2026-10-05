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

#include <atexit.h>
#include <debug.h>

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

/* fclose() isn't equals void (*func)(void*),
   needed wrapper. */
static void AtExitFClose(void* file) {
  if (file != NULL) {
    fclose((FILE*)file);
  }
}

/* Check validate of HEADER */
static int BadAtExitHeader(AtExitHeader* header) {
  if (header == NULL) {
    return 1;
  }

  if (header->array == NULL ||
      header->used < 0      ||
      header->capacity <= 0 ||
      header->used > header->capacity) {
    return 1;
  }

  return 0;
}

AtExitHeader* InitAtExit(int size) {
  AtExitHeader* ret = NULL;
  void* slice = NULL;
  int cur_node = 0;

  /* No size / Big size error */
  if (size < 0 || size > 175) {
    return ret;
  }

  /* Default value */
  if (size == 0) {
    size = DEFAULT_ATEXIT_SIZE;
  }

  slice = (AtExitHeader*)MemAlloc(
          sizeof(AtExitHeader) + (sizeof(AtExitNode) * (size_t)size));
  if (slice == NULL) {
    return NULL;
  }

  ret = slice;
  ret->capacity = size;
  ret->used = 0;

  /* slice = header + nodes... */
  ret->array = slice + sizeof(AtExitHeader);
  
  for(; cur_node < size; cur_node++) {
    ret->array[cur_node].function = NULL;
    ret->array[cur_node].arg      = NULL;
  }
  
  return ret;
}

int SetAtExit(AtExitHeader* head, V_VP_F func, void* arg) {
  if (BadAtExitHeader(head) || func == NULL || arg == NULL) {
    return 1;
  }

  /* Return 1 if haven't places for function */
  if (head->used >= head->capacity) {
    return 1;
  }

  head->array[head->used].function = func;
  head->array[head->used].arg      =  arg;
  head->used++;
  
  return 0;
}

void DoAtExit(AtExitHeader* head) {
  int cur_func = 0;
  
  if (BadAtExitHeader(head)) {
    return;
  }

  /* Using LIFO -> Last In First Out. */
  for(cur_func = head->used; cur_func >= 0; cur_func--) {
    if (head->array[cur_func].function == NULL) {
      continue;
    }

    head->array[cur_func].function(
                            head->array[cur_func].arg);

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

  if (BadAtExitHeader(head) || bytes == 0) {
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
  if (BadAtExitHeader(head) || filename == NULL || modes == NULL) {
    return NULL;
  }

  ret = fopen(filename, modes);
  if (ret == NULL) {
    return NULL;
  }

  SetAtExit(head, AtExitFClose, ret);

  return ret;
}

void AtExitClean(AtExitHeader *head) {
  int cur_node = 0;
  
  if (BadAtExitHeader(head)) {
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
  if (BadAtExitHeader(head)) {
    return;
  }

  DoAtExit(head);

  MemFree(head);
}
