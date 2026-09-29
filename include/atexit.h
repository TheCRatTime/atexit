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

#ifndef ATEXIT_ATEXIT_H_
#define ATEXIT_ATEXIT_H_

#include <stdio.h>

#define DEFAULT_ATEXIT_SIZE 50

/* AtExit node */
typedef struct AtExitNode {
  void*               arg; /* Argument, that will be used */
  void (*function)(void*); /* Pointer to function */
} AtExitNode;

/* Header of AtExit nodes */
typedef struct AtExitHeader {
  int         capacity; /* Size of nodes */
  int             left; /* Used nodes    */
  AtExitNode*    array; /* Nodes         */
} AtExitHeader;

/* Init AtExitHeader
   Return value is pointer to AtExitHeader*
   or NULL if OOM */
AtExitHeader* InitAtExit(int size);

/* Set function, that will be called later.
   Return value is 0 if all good and 1 if
   haven't left nodes */
int SetAtExit(AtExitHeader* head, void (*to_set)(void*), void* arg);

/* Run and reset all AtExit functions */
void DoAtExit(AtExitHeader* head);

/* Malloc data and set AtExit free function
   Return pointer, that gave by malloc */
void* AtExitMalloc(AtExitHeader* head, size_t bytes);

/* Open file and set AtExit fclose function
   Return pointer to FILE*, that gave by fopen */
FILE* AtExitFopen(AtExitHeader* head, const char* filename,
                  const char* modes);

/* Clean all AtExit list.
   Warning: if you have set free, don't call this
   function, you can get memory leak. */
void AtExitClean(AtExitHeader* head);

/* Stolen from Go */
#define defer(hd, func, arg) SetAtExit(hd, func, arg)

#endif /* ATEXIT_ATEXIT_H_ */
