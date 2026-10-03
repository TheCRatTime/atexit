#include "../include/atexit.h"

#include <assert.h>

static void MemCheck(void) {
  int bad = InitFreeMap();
  AtExitHeader* test = InitAtExit(0);
  int* var = NULL;
  DropAllocRet stats;
  
  /* Test fails if can't get memory */
  assert(test != NULL);
  assert(bad != 1);

  var = (int*)AtExitMalloc(test, sizeof(int), "var");
  /* Get NULL is bad because size of AtExit is big */
  assert(var != NULL);

  *var = 5;
  assert(*var == 5);

  AtExitFree(test);

  DropAllocated();

  WriteAllocated(&stats);

  assert(stats.allocated == stats.freed);
  return;
}

int main(void) {
  MemCheck();
  return 0;
}
