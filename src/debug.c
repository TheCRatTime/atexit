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

#include <debug.h>

#include <stddef.h>
#include <stdlib.h>

#ifdef ATEXIT_DEBUG
# include <stdio.h>

/* Google style: bad: created global non const variable */
static int allocated_blocks = 0;
#endif

/* Malloc wrapper: */
void* MemAlloc(size_t bytes) {
  void* ret = malloc(bytes);
#ifdef ATEXIT_DEBUG
  allocated_blocks++;
#endif
  return ret;
}

void MemFree(void* ptr) {
  free(ptr);
#ifdef ATEXIT_DEBUG
  allocated_blocks--;
#endif
  return;
}

#ifdef ATEXIT_DEBUG
void DropAllocated(void) {
  printf("Allocated blocks: %d\n", allocated_blocks);
  return;
}

int GetAllocated(void) {
  return allocated_blocks;
}
#endif
