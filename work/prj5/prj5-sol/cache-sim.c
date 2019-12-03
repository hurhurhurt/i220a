#include "cache-sim.h"
#include "math.h"
#include "memalloc.h"
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdbool.h>

/** Create and return a new cache-simulation structure for a
 *  cache for main memory withe the specified cache parameters params.
 *  No guarantee that *params is valid after this call.
 */


typedef struct Block Block;
struct Block{
  MemAddr addr;
  bool isValid;
};
  
typedef struct CacheSimImpl{
  Block ** block;
  CacheParams param;
}CacheSim;
  
CacheSim *
new_cache_sim(const CacheParams *params)
{
  CacheParams param;
  param.nSetBits = params->nSetBits;
  param.nLinesPerSet = params->nLinesPerSet;
  param.nLineBits = params->nLineBits;
  param.nMemAddrBits = params->nMemAddrBits;
  int sets = 1 << param.nSetBits;
  CacheSim * sim = mallocChk(sets * sizeof(CacheSim));
  sim->param = param;
  sim->block = mallocChk(sets * sizeof(struct Block *));
  for (int i = 0; i < sets; i++){
    sim->block[i] = mallocChk(param.nSetBits);
  }
  for (int i = 0; i < sets; i++){
    for (int j = 0; j < params->nSetBits; j++){
      sim->block[i][j].isValid = false;
    }
  }
  return sim;
  
}

/** Free all resources used by cache-simulation structure *cache */
void
free_cache_sim(CacheSim *cache)
{/*
  free(cache->tag);
  free(cache->block);
  free(cache);
 */
}

/** Return result for addr requested from cache */
CacheResult
cache_sim_result(CacheSim *cache, MemAddr addr)
{/*
  CacheStatus stat;
  int block = addr/cache->param->nSetBits;
  if (cache[block].valid) stat = 0;
  else{
    if (cache[block+1].valid && cache[block].tag == cache[addr].tag){
      stat = 1;
    }
    else{
      stat = 2;
    }
  }
  return (CacheResult) { .status = stat, .replaceAddr = 0x0 };
  }*/
}
