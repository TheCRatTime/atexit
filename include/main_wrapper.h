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
 
#ifndef ATEXIT_DIS_ERROR
# if !defined(MAIN_VOID) && \
     !defined(MAIN_ARGV) && \
     !defined(MAIN_ENVP)
#  error "You don't define: MAIN_VOID, MAIN_ARGV or MAIN_ENVP."
# endif
#endif

/* int Main(void) */
#ifdef MAIN_VOID
#include <stdio.h>
#include "atexit.h"

extern int Main(AtExitHeader*);

int main(void) {
  AtExitHeader* header = InitAtExit(ATEXIT_DEFVAL);
  int ret = 0;
  
  if (header == NULL) {
    printf("Can't allocate memory for main atexit.\n");
    return 1;
  }

  ret = Main(header);
  AtExitFree(header);
  
  return ret;
}

#endif

#ifdef MAIN_ARGV
#include <stdio.h>
#include "atexit.h"

extern int Main(AtExitHeader*, int, char**);

int main(int argc, char** argv) {
  AtExitHeader* header = InitAtExit(ATEXIT_DEFVAL);
  int ret = 0;
  
  if (header == NULL) {
    printf("%s: can't allocate memory for main atexit.\n", argv[0]);
    return 1;
  }

  ret = Main(header, argc, argv);
  AtExitFree(header);
  
  return ret;
}

#endif

#ifdef MAIN_ENVP
#include <stdio.h>
#include "atexit.h"

extern int Main(AtExitHeader*, int, char**, char**);

int main(int argc, char** argv, char** envp) {
  AtExitHeader* header = InitAtExit(ATEXIT_DEFVAL);
  int ret = 0;
  
  if (header == NULL) {
    printf("%s: can't allocate memory for main atexit.\n", argv[0]);
    return 1;
  }

  ret = Main(header, argc, argv, envp);
  AtExitFree(header);
  
  return ret;
}

#endif

#ifndef NO_UNDEFS
# undef MAIN_VOID
# undef MAIN_ARGV
# undef MAIN_ENVP
#endif
