#ifndef STACK_H_
#define STACK_H_

/** A stack can contain any kinds of objects.  Hence a stack entry is
 *  represented as a "void *" generic pointer which is the modern way
 *  of representing an untyped pointer.
 */


/** Declare Stack as a type which is a synonym for the opaque
 *  structure struct Stack representing an abstract data type.
 */
typedef struct Stack Stack;

//Note that even though they have the same spelling, the Stack
//identifier in `struct Stack` is a structure tag and has no
//relationship to the `Stack` typedef; the structure tag context is
//triggered when the `Stack` identifier is used immediately after a
//struct keyword; when the `Stack` identifier is used in a context
//not immediately following the struct keyword, it refers to the
//typedef name.

//The above is a *definition* for the Stack typedef, but merely
//an incomplete *declaration* for the `struct Stack` tagged
//structure.  Providing a definition for `struct Stack` will
//make the abstract data type concrete.

/** Return a new empty stack.
 *
 *  Returns NULL on error with errno set appropriately.
 */
Stack *newStack(void);

/** Free all resources used by stack. */
void freeStack(Stack *stack);

/** Return number of entries in stack */
int nEntriesStack(const Stack *stack);

/** Push entry onto stack.  Returns entry on succcess.
 *
 *  Returns NULL on error with errno set appropriately.
 */
const void *pushStack(Stack *stack, const void *entry);

/** Pop and return topmost entry from stack. Returns NULL if stack is
 *  empty.
 */
const void *popStack(Stack *stack);


#endif //ifndef STACK_H_
