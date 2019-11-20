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

typedef struct FnsData {
  int index;
  int size;
  FnInfo **info;
}FnsData;

static inline bool is_call(unsigned op) { return op == CALL_OP; }
static inline bool is_ret(unsigned op) {
    return
          op == RET_NEAR_OP || op == RET_NEAR_WITH_POP_OP ||
      op == RET_FAR_OP || op == RET_FAR_WITH_POP_OP;
}

int compare(const void * addr1, const void * addr2){
  FnInfo * uno = *(FnInfo **)addr1;
  FnInfo * dos = *(FnInfo **)addr2;
  return (uno->address - dos->address);
}

void fn_trace(void *addr, FnsData *data){
  FnInfo *info = malloc(sizeof *info);
  info->address = addr;
  info->length = 0;
  info->nInCalls = 1;
  info->nOutCalls = 0;

  if(data->size <= data->index){
    data->size++;
    data->info = reallocChk(data->info, 2 *(sizeof(*info) * data->size));
  }

  data->info[data->index] = info;

  unsigned char op = *(unsigned char *)addr;

  while(!is_ret(op)){
    if(is_call(op)){
      bool inside = true;
      info->nOutCalls++;

      int displace = *(int *)addr;
      displace >>= 8;
      displace += get_op_length(addr);

      if(data->size != 0){
	for(unsigned int i = 0; i < data->size; i++){
	  if(data->info[i]->address == addr + displace){
	    data->info[i]->nInCalls++;
	    inside = false;
	  }
	}
      }

      if(inside){
	data->index++;
	fn_trace(addr + displace, data);
      }
    }
    info->length += get_op_length(addr);
    addr += get_op_length(addr);
    op = *(unsigned char *)addr;
  }
  info->length += get_op_length(addr);
}

const FnsData * new_fns_data(void *rootFn)
{
  assert(sizeof(int) == 4);
  FnsData *data = malloc(sizeof(struct FnsData));
  data->info = malloc(sizeof(FnInfo));
  data->index = 0;
  data->size = 0;

  fn_trace(rootFn, data);
  qsort(data->info, data->size, sizeof(data->info[0]), compare);
  return data;
}

void free_fns_data(FnsData *fnsData)
{
  for(unsigned int i = 0; i < fnsData->size; i++) free(fnsData->info[i]);
  free(fnsData->info);
  free(fnsData);
}

const FnInfo * next_fn_info(const FnsData *fnsData, const FnInfo *lastFnInfo)
{
  unsigned int index = 0;
  if(lastFnInfo == NULL) return fnsData->info[0];

  for(unsigned int i = 0; i < fnsData->size; i++){
    if(fnsData->info[i]->address == lastFnInfo->address) index = i;
  }

  index++;
  if(index > fnsData->index) return NULL;
  return fnsData->info[index];
}
