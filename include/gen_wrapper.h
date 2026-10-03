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

#include "atexit.h"

/* Display errors if haven't defined macro.
   Check for each macro. */
#ifndef ATEXIT_DIS_ERROR
# if !defined(GEN_NAME)
#  error Don't defined GEN_NAME.
# endif

# if !defined(GEN_OUT)
#  error Don't defined GEN_OUT.
# endif

# if !defined(GEN_TYPE)
#  error Don't defined GEN_TYPE.
# endif

# if !defined(GEN_ARGS)
#  error Don't defined GEN_ARGS.
# endif
#endif

/* Check for type 'void' */
#define TYPEvoid 1
#define GLUE(a, b) a##b
#define IS_VOID(t) GLUE(TYPE, t)

/* Generate argument macro */
#define GENARG(t, n) , t n

/* Can define static functions*/
#ifdef GEN_STATIC
static
#endif
GEN_TYPE GEN_OUT(AtExitHeader* atexit_h GEN_ARGS);

GEN_TYPE GEN_OUT(AtExitHeader* atexit_h GEN_ARGS) {
  /* If type is void, no need declare 'ret' */
#if IS_VOID(GEN_TYPE)
#else
  GEN_TYPE ret;
  ret = 
#endif

/* Redefine GENARG: need only variable names */
#undef GENARG
#define GENARG(t, v) , v

  GEN_NAME(atexit_h GEN_ARGS);
  DoAtExit(atexit_h);

  /* Check for void again */
#if IS_VOID(GEN_TYPE)
  return;
#else
  return ret;
#endif
}

/* Undefine all of created macro */
#undef GENARG

#undef IS_VOID
#undef GLUE
#undef TYPEvoid

#undef GEN_NAME
#undef GEN_OUT
#undef GEN_TYPE
#undef GEN_ARGS
