#include "../include/atexit.h"

#include <assert.h>

static void MemCheck(void) {
  AtExitHeader* test = InitAtExit(0);
  int* var = NULL;
  
  /* Test fails if can't get memory */
  assert(test != NULL);

  var = (int*)AtExitMalloc(test, sizeof(int));
  /* Get NULL is bad because size of AtExit is big */
  assert(var != NULL);

  *var = 5;
  assert(*var == 5);

  AtExitFree(test);

  DropAllocated();

  assert(GetAllocated() == 0);
  return;
}

int main(void) {
  MemCheck();
  return 0;
}
