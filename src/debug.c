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

#define ATEXIT_DEBUG
#include <debug.h>

#include <stddef.h>
#include <stdlib.h>

#ifdef ATEXIT_DEBUG
#include <stdio.h>

/* Google style: bad: created global non const variable */
static size_t allocated = 0;
static size_t freed     = 0;
#endif

/* Malloc wrapper: */
void* MemAlloc(size_t bytes, char* key) {
  void* ret = NULL;
  ret = malloc(bytes);
#ifdef ATEXIT_DEBUG
  if (ret) {
    allocated += bytes;
    if (key != NULL) {
      SetTrace(key, bytes);
    }
  }
#else
  (void)key;
#endif
  return ret;
}

void MemFree(void* ptr, int is_bytes, void* bytes_or_key) {
  free(ptr);
#ifdef ATEXIT_DEBUG
  if (!is_bytes) {
    if (FreeMapFind((char*)bytes_or_key)) {
      MapNode* data = FreeMapGet((char*)bytes_or_key);
      freed += *(size_t*)data->val;
    }
  } else {
    freed += *(size_t*)bytes_or_key;
  }
#else
  (void)bytes_or_key;
  (void)is_bytes;
#endif
  return;
}

#ifdef ATEXIT_DEBUG
void DropAllocated(void) {
  printf("Allocated: %lu\n", allocated);
  printf("Freed:     %lu\n", freed);
  return;
}

void WriteAllocated(DropAllocRet *stats) {
  stats->allocated = allocated;
  stats->freed     = freed;
}
#endif
