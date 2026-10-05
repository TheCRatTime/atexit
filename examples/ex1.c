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

#include <atexit.h>

#include <stdio.h>
#include <stdlib.h>

/**************************************
 * GENERATING wrapper by #including file
 *
 * Need no define four macro:
 */

/* If you want static, define: */
#define GEN_STATIC

/* 1st: name of function */
#define GEN_NAME TestFunc

/* 2nd: name of wrapper*/
#define GEN_OUT  TestFuncWrapper

/* 3rd: type */
#define GEN_TYPE void

/* 4th: arguments.
   to add argument write: GENARG(type, name) */
#define GEN_ARGS GENARG(int, n1) GENARG(int, n2) /* ... */

/* The wrapper generation next */

/**********************************************
 * Test function.
 * 
 * Function must take:
 * - AtExitHeader* -> for functions, that will be
 * executed at exit. Needed for wrapper, see
 * upper.
 * - Other -> functions arguments or none.
 */
static void TestFunc(AtExitHeader* atexit_h, int num1, int num2) {
  /* Using special function AtExitMalloc for malloc() */
  int* allocated1 = (int*)AtExitMalloc(atexit_h, sizeof(int));
  int* allocated2 = (int*)AtExitMalloc(atexit_h, sizeof(int));

  if (allocated1 == NULL) {
    /*******************************************
     * No need call DoAtExit or another function
     * because using wrapper.
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

/* *** WRAPPER GENERATION *** */

/* Generate our wrapper. */
#include <gen_wrapper.h>

/* This is will generate wrapper by macro. No need write #undef.
   NOTE: GEN_STATIC don't will be undefined */

/* *** Main function *** */

/* Generating wrapper. Without gen_wrapper.h: */
#define MAIN_VOID /* void type */
#include <main_wrapper.h>

int Main(AtExitHeader* main_atexit) {
  /* You CAN't do AtExitFree(main_atexit)
     because using wrapper. */
     
  AtExitHeader* test_atexit = NULL;

  test_atexit = InitAtExit(0);
  if (test_atexit == NULL) {
    perror("InitAtExit");
    /* Just return */
    return 1;
  }

  defer(main_atexit, free, test_atexit);

  /* Just calling functions, without free(),
     fclose() or your function */
  
  TestFuncWrapper(test_atexit, 6, 7);

  return 0;
}

/* *** Example end :( *** */
