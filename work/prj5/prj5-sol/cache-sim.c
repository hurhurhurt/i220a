#include "cache-sim.h"
#include "math.h"
#include "memalloc.h"

#include <stdbool.h>
#include <stddef.h>

/** Create and return a new cache-simulation structure for a
 *  cache for main memory withe the specified cache parameters params.
 *  No guarantee that *params is valid after this call.
 */

typedef struct CacheSimImpl{
  int valid;
  char * tag;
  CacheParams * param;
}CacheSim;
  
  
CacheSim *
new_cache_sim(const CacheParams *params)
{
  unsigned s = params->nSetBits;
  unsigned E = params->nLinesPerSet;  //#cache lines per set 
  unsigned b = params->nLineBits;   
  unsigned m = params->nMemAddrBits;  //#of bits in primary mem address
  int tagSize = m -(b + s);
  int cacheSize = s>>1;
  int blockSize = b>>1;
  int sets = s>>=1;
  CacheSim * returnSim = mallocChk(sets*sizeof(CacheSim));
  returnSim->param = mallocChk(sizeof(params));
  for (int i = 0; i < sets; i++){
    returnSim[i].tag = malloc(sizeof(char)*(tagSize + 1));
    for (int j = 0; j < tagSize; j++){
      returnSim[i].tag[j] = '0';
    }
    returnSim[i].valid= 0;
    }
  return returnSim;
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
{
  //@TODO
  return (CacheResult) { .status = CACHE_N_STATUS, .replaceAddr = 0x0 };
}
