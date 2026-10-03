/**********************************************
 * Example N1: small usage.
 * Usage of AtExitInit, DoAtExit, AtExitMalloc.
 * Writing wrapper for function.
 * Use AtExit in main.
 */

/* Disable debug */
#ifdef ATEXIT_DEBUG
#undef ATEXIT_DEBUG
#endif

/* Using keyword: defer */
#define USE_DEFER

#include <atexit.h>

#include <stdio.h>
#include <stdlib.h>

/**********************************************
 * Test function.
 * Function must take:
 * AtExitHeader* -> for functions, that will be
 * executed at exit.
 * Other         -> functions arguments
 */
static void TestFunc(AtExitHeader* atexit_h, int num1, int num2) {
  /* Using special function AtExitMalloc for malloc() */
  int* allocated1 = (int*)AtExitMalloc(atexit_h, sizeof(int));
  int* allocated2 = (int*)AtExitMalloc(atexit_h, sizeof(int));

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
  
  *allocated1 = num1;
  *allocated2 = num2;

  printf("%d + %d = %d\n",
         *allocated1, *allocated2,
         *allocated1 + *allocated2);
  /* Variables will be automatically freed. */
  return;
}

/**************************************
 * GENERATING wrapper by including file
 *
 * Need no define four macro:
 */

/* 1st: name of function */
#define GEN_NAME TestFunc

/* 2nd: name of wrapper*/
#define GEN_OUT  TestFuncWrapper

/* 3rd: type */
#define GEN_TYPE void

/* 4th: arguments.
   to add argument write: GENARG(type, name) */
#define GEN_ARGS GENARG(int, n1) GENARG(int, n2) /* ... */

/* Generate our wrapper. */
#include <gen_wrapper.h>

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
    AtExitFree(main_atexit);
    /* or create wrapper.    */
    perror("InitAtExit");
    return 1;
  }

  /*****************************************
   * Just calling functions, without free(),
   * fclose() or your function
   */
  TestFuncWrapper(test_atexit, 6, 7);

  /* Before free calls DoAtExit() */
  AtExitFree(main_atexit);
  return 0;
}

/* *** Example end :( *** */
