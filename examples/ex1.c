/**********************************************
 * Example N1: basic usage.
 * Demonstrates AtExitInit, DoAtExit, AtExitMalloc,
 * and generating a wrapper for a custom function.
 */

/* Required source flag.
   Define this in exactly one source file. */
#define ATEXIT_SOURCE
#include <atexit.h>

#include <stdio.h>
#include <stdlib.h>

/**************************************
 * GENERATING a wrapper via file inclusion.
 *
 * Four macros must be defined beforehand:
 */

/* Optional: make the wrapper static */
#define GEN_STATIC

/* 1st: name of the target function */
#define GEN_NAME TestFunc

/* 2nd: name of the generated wrapper */
#define GEN_OUT  TestFuncWrapper

/* 3rd: return type */
#define GEN_TYPE void

/* 4th: arguments.
   To add an argument, use: GENARG(type, name) */
#define GEN_ARGS GENARG(int, n1) GENARG(int, n2) /* ... */

/**********************************************
 * Test function.
 *
 * The function must accept:
 * - AtExitHeader* -> tracks cleanups executed at exit.
 *   Required by the wrapper generator (see below).
 * - Custom arguments -> function-specific arguments or none.
 */
static void TestFunc(AtExitHeader* atexit_h, int num1, int num2) {
  /* Use the specialized AtExitMalloc for managed allocations */
  int* allocated1 = (int*)AtExitMalloc(atexit_h, sizeof(int));
  int* allocated2 = (int*)AtExitMalloc(atexit_h, sizeof(int));

  if (allocated1 == NULL) {
    /*******************************************
     * No need to call DoAtExit manually here
     * because the wrapper handles it automatically.
     */
    return;
  }

  if (allocated2 == NULL) {
    /*****************************************
     * No need for nested checks like:
     *   if (allocated1 != NULL) { free(allocated1); }
     * because AtExitMalloc automatically registers
     * an auto-free callback.
     */
    return;
  }

  *allocated1 = num1;
  *allocated2 = num2;

  printf("%d + %d = %d\n",
         *allocated1, *allocated2,
         *allocated1 + *allocated2);
  
  /* Resources will be automatically freed upon exit */
  return;
}

/* *** WRAPPER GENERATION *** */

/* Include the generator header. */
#include <gen_wrapper.h>

/* This will generate the wrapper via macros. No need to #undef them manually.
   NOTE: GEN_STATIC will not be undefined. */

/* *** Main function wrapper *** */

/* Wrap the main function. Using MAIN_VOID for void arguments: */
#define MAIN_VOID
#include <main_wrapper.h>

int Main(AtExitHeader* main_atexit) {
  /* Do NOT manually call AtExitFree(main_atexit)
     because the main wrapper handles it. */

  AtExitHeader* test_atexit = NULL;

  test_atexit = InitAtExit(0);
  if (test_atexit == NULL) {
    perror("InitAtExit");
    return 1;
  }

  defer(main_atexit, free, test_atexit);

  /* Call functions cleanly without manual free(), fclose(), etc. */
  TestFuncWrapper(test_atexit, 6, 7);

  return 0;
}
