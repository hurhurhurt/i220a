
#include "fn-trace.h"
#include "x86-64_lde.h"
#include "memalloc.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

enum {
  CALL_OP = 0xE8,
  RET_FAR_OP = 0xCB,
  RET_FAR_WITH_POP_OP = 0xCA,
  RET_NEAR_OP = 0xC3,
  RET_NEAR_WITH_POP_OP = 0xC2
};

static inline bool is_call(unsigned op) { return op == CALL_OP; }
static inline bool is_ret(unsigned op) {
  return
    op == RET_NEAR_OP || op == RET_NEAR_WITH_POP_OP ||
    op == RET_FAR_OP || op == RET_FAR_WITH_POP_OP;
}

/** FnsData is an opaque struct FnsData which can be implemented as
 *  desired by an implementation. This struct will hold a collection
 *  of FnInfo's.
 */
/** Return pointer to opaque data structure containing collection of
 *  FnInfo's for functions which are callable directly or indirectly
 *  from the function whose address is rootFn.
 */

int compare(const void *addr1, const void *addr2){
  FnInfo *uno = *(FnInfo**) addr1;
  FnInfo *dos = *(FnInfo**) addr2;
  return uno->address - dos->address;
}

typedef struct FnsData{
  int size;
  int index;
  FnInfo ** info;
}FnsData;

void
fn_trace(void *addr, FnsData *data){
  FnInfo *fn = malloc(sizeof *fn);
  fn->address = addr;
  fn->length = 0;
  fn->nInCalls = 1;
  fn->nOutCalls = 0;
  int length = 0;

  if (data->size <= data->index){
    data->size++;
    data->info = reallocChk(data->info, sizeof(fn) * data->size);
  }
  
  data->info[data->index] = fn;
  unsigned char op = *(unsigned char *) addr;
  
  while (!is_ret(op)){
    if (is_call(op)){
      bool inside = true;
      fn->nOutCalls++;
      int disp = *(int*) addr;
      disp >>= 8;
      disp += get_op_length(addr);

      if (data->size != 0){
	for (unsigned int i = 0; i < data->size; i++){
	  if (data->info[i]->address == addr + disp){
	    data->info[i]->nInCalls++;
	    inside = false;
	  }
	}
      }
      if (inside){
	data->index++;
	fn_trace(addr + disp, data);
      }
    }
    fn->length += get_op_length(addr);
    addr += get_op_length(addr);
    op = *(unsigned char *)addr;
  }
  fn->length += get_op_length(addr);
}  
const FnsData * new_fns_data (void *rootFn)
{
  //verify assumption used when decoding call address
  assert(sizeof(int) == 4);
  FnsData * data = malloc(sizeof(struct FnsData));
  data->info = malloc(0);
  data->index = 0;
  data->size = 0;
  fn_trace(rootFn, data);
  qsort(data->info, data->size, sizeof(FnInfo), compare);
  return data;
}


/** Free all resources occupied by fnsData. fnsData must have been
 *  returned by new_fns_data().  It is not ok to use to fnsData after
 *  this call.
 */
void
free_fns_data(FnsData *fnsData)
{
  for (int i = 0; i < fnsData->size; i++){
    free(fnsData->info[i]);
  }
  free(fnsData);
}

/** Iterate over all FnInfo's in fnsData.  Make initial call with NULL
 *  lastFnInfo.  Keep calling with return value as lastFnInfo, until
 *  next_fn_info() returns NULL.  Values must be returned sorted in
 *  increasing order by fnInfoP->address.
 *
 *  Sample iteration:
 *
 *  for (FnInfo *fnInfoP = next_fn_info(fnsData, NULL); fnInfoP != NULL;
 *       fnInfoP = next_fn_info(fnsData, fnInfoP)) {
 *    //body of iteration
 *  }
 *
 */
const FnInfo *
next_fn_info(const FnsData *fnsData, const FnInfo *lastFnInfo)
{
  //@TODO
  unsigned int index = 0;
  if (lastFnInfo == NULL) return fnsData->info[0];
  for (unsigned int i = 0; i < fnsData->size; i++){
    if (fnsData->info[i]->address == lastFnInfo->address) index = i;
  }
  index++;
  if (index > fnsData->index) return NULL;
  return fnsData->info[index];
} 
