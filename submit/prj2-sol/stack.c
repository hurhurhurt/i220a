#include "stack.h"

#include <assert.h>
#include <stdlib.h>

//make abstract type concrete by providing definition for `struct Stack`.
struct Stack {
  /** Max number of entries possible with current allocation */
  size_t size;

  /** Index of next free entry in stk[] */
  size_t index;

  /** pointer to dynamically allocated array of stack entries */
  const void **stk;
};

enum {
  /** Increment for stack size */
  STACK_INC = 4
};

/** Return a new empty stack.
 *
 *  Returns NULL on error with errno set appropriately.
 */
Stack *
newStack(void)
{
  //calloc() initializes everything to zero: so size == index == stk == 0
  Stack *stack = calloc(1, sizeof(Stack));
  if (stack != NULL) stack->stk = NULL; //necessary if NULL is not all 0 bits
  return stack;
}

/** Free all resources used by stack. */
void
freeStack(Stack *stack)
{
  free(stack->stk);
  free(stack);
}

/** Return number of entries in stack */
int
nEntriesStack(const Stack *stack)
{
  return stack->index;
}

/** Push entry onto stack.  Returns entry on succcess.
 *
 *  Returns NULL on error with errno set appropriately.
 */
const void *
pushStack(Stack *stack, const void *entry)
{
  if (stack->index >= stack->size) {
    stack->size += STACK_INC;
    stack->stk = realloc(stack->stk, stack->size*sizeof(void *));
    if (!stack->stk) return NULL;
  }
  assert(stack->index < stack->size); //we have space
  stack->stk[stack->index++] = entry;
  return entry;
}

/** Pop and return topmost entry from stack. Returns NULL if stack is
 *  empty.
 */
const void *
popStack(Stack *stack)
{
  return (stack->index == 0) ? NULL : stack->stk[--stack->index];
}
