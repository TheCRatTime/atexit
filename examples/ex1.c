/**********************************************
 * Example N1: small usage.
 * Usage of AtExitInit, DoAtExit, AtExitMalloc.
 * Writing wrapper for function.
 * Use AtExit in main.
 */

#if !defined(INC_USE_QUOTES)
#include <atexit.h>
#else
# if defined(ATEXIT_PATH_TO_INC)
# include ATEXIT_PATH_TO_INC
# else
# include "../include/atexit.h"
# endif
#endif

#include <stdio.h>
#include <stdlib.h>

/* *** Prototypes *** */
void TestFunc(AtExitHeader* atexit_h, int num1, int num2);

/* Using wrapper for cleaned code */
void TestFuncWrapper(AtExitHeader* atexit_do, int n1, int n2);

/**********************************************
 * Test function.
 * Function must take:
 * AtExitHeader* -> for functions, that will be
 * executed at exit.
 * Other         -> functions arguments
 */
void TestFunc(AtExitHeader* atexit_h, int num1, int num2) {
  /* Using special function AtExitMalloc for malloc() */
  int* allocated1 = (int*)AtExitMalloc(atexit_h, sizeof(int));
  int* allocated2 = (int*)AtExitMalloc(atexit_h, sizeof(int));

  *allocated1 = num1;
  *allocated2 = num2;

  if (allocated1 == NULL) {
    /**************************************************
     * No need call like CallAtExit or another function
     * because using wrapper
     */
    return;
  }
  
  if (allocated2 == NULL) {
    /*****************************************
     * No need check like this:
     *   if (allocated1 != NULL) {
     *     defer(...)
     *   }
     * because AtExitMalloc automatically sets
     * auto-free function.
     */
    return;
  }

  printf("%d + %d = %d\n",
         *allocated1, *allocated2,
         *allocated1 + *allocated2);
  /* Variables will be automatically freed. */
  return;
}

/*************************************
 * Small wrapper.
 * Arguments: the same of the function
 * 
 * Need to write:
 * 1. Call to function
 * 2. DoAtExit() call
 * (3. Return value)
 */
void TestFuncWrapper(AtExitHeader* atexit_do, int n1, int n2) {
  TestFunc(atexit_do, n1, n2);
  DoAtExit(atexit_do);
  return;
}

/* *** Main function *** */
int main(void) {
  const int MainAtExitSize = 10;
  AtExitHeader* test_atexit = NULL;
  /* You can create AtExit in main... */
  AtExitHeader* main_atexit = InitAtExit(MainAtExitSize);
  if (main_atexit == NULL) {
    perror("InitAtExit");
    return 1;
  }

  /* Using 'defer' from Go */
  defer(main_atexit, free, test_atexit);

  test_atexit = InitAtExit(0);
  if (test_atexit == NULL) {
    /* ...but you need call: */
    DoAtExit(main_atexit);
    /* or create wrapper.    */
    perror("InitAtExit");
    return 1;
  }

  /*****************************************
   * Just calling functions, without free(),
   * fclose() or your function
   */
  TestFuncWrapper(test_atexit, 6, 7);

  DoAtExit(main_atexit);
  free(main_atexit);
  return 0;
}

/* *** Example end :( *** */
