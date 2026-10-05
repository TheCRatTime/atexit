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

#ifndef ATEXIT_DEBUG_H_
#define ATEXIT_DEBUG_H_

#include <stddef.h>

/* For trace memory leak */
void* MemAlloc(size_t);

/* For trace memory leak */
void MemFree(void*);

#ifdef ATEXIT_DEBUG
/* Print all malloc' blocks, that not freed. */
void DropAllocated(void);

/* Get number of not freed blocks */
int GetAllocated(void);
#endif

#endif /* ATEXIT_DEBUG_H_ */
