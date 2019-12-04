#include "cache-sim.h"
#include "memalloc.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>


int power(int x, int y){
  int result = 1;
  for (int i = 0; i < y; i++){
    result *= x;
  }
  return result;
}

typedef struct Block Block;
struct Block{
  bool isValid;
  MemAddr address;
  unsigned long age;
};

struct CacheSimImpl {
  Block ** blocks;
  CacheParams param;
};

/** Create and return a new cache-simulation structure for a
 *  cache for main memory withe the specified cache parameters params.
 *  No guarantee that *params is valid after this call.
 */
CacheSim *
new_cache_sim(const CacheParams *params)
{
  CacheParams param;
  param.nSetBits = params->nSetBits;
  param.nLinesPerSet = params->nLinesPerSet;
  param.nLineBits = params->nLineBits;
  param.nMemAddrBits = params->nMemAddrBits;
  param.replacement = params->replacement;

  CacheSim * sim = malloc(sizeof(param) * sizeof(struct CacheSimImpl));
  int nSets = power(2, params->nSetBits);
  sim->param = param;
  sim->blocks = malloc(nSets * sizeof(struct Block *) + 1);
  
  for(int i = 0; i < nSets; i++){
    sim->blocks[i] = malloc(params->nLinesPerSet * sizeof(struct Block));
    for (int j = 0; j < params->nLinesPerSet; j++){
      sim->blocks[i][j].isValid = false;
      sim->blocks[i][j].age = 0;
      sim->blocks[i][j].address = 0x0;
    }
  }
  return sim;
}

/** Free all resources used by cache-simulation structure *cache */
void
free_cache_sim(CacheSim *cache)
{
  for(int i = 0; i < power(2, cache->param.nSetBits); i++){
    free(cache->blocks[i]);
  }
  free(cache->blocks);
  free(cache);
}

/** Return result for addr requested from cache */
CacheResult
cache_sim_result(CacheSim *cache, MemAddr addr)
{
  MemAddr newAddr = addr >> cache->param.nLineBits;
  newAddr <<= cache->param.nLineBits;

  long set = (power(2, cache->param.nSetBits) - 1) << cache->param.nLineBits;
  set = (set & addr) >> cache->param.nLineBits;

  long nLines = cache->param.nLinesPerSet;

  for(long i = 0; i < nLines; i++){
    cache->blocks[set][i].age++;
  }

  for(long i = 0; i < nLines; i++){
    if(cache->blocks[set][i].address == newAddr){
      cache->blocks[set][i].age = 0;
      return (CacheResult) { .status = CACHE_HIT, .replaceAddr = 0x0};
    }
  }

  for(long i = 0; i < nLines; i++ ){
    if(!cache->blocks[set][i].isValid){
      cache->blocks[set][i].address = newAddr;
      cache->blocks[set][i].isValid = true;
      cache->blocks[set][i].age = 0;
      return (CacheResult) { .status = CACHE_MISS_WITHOUT_REPLACE, .replaceAddr = 0x0};
    }
  }

  MemAddr repAddr;

  if(cache->param.replacement == LRU_R){
    long index = 0;
    long maxAge = 0;
    for(long i = 0; i < nLines; i++){
      if(maxAge < cache->blocks[set][i].age){
	maxAge = cache->blocks[set][i].age;
	index = i;
      }
    }

    repAddr = cache->blocks[set][index].address;
    cache->blocks[set][index].address = newAddr;
  }
  else if(cache->param.replacement == MRU_R){
    long index = 0;
    long minAge = cache->blocks[set][0].age;
    for(long i = 0; i < nLines; i++){
      if(minAge > cache->blocks[set][i].age){
	minAge = cache->blocks[set][i].age;
	index = i;
      }
    }

    repAddr = cache->blocks[set][index].address;
    cache->blocks[set][index].address = newAddr;
  }
  else{
    long index = rand() % nLines;
    repAddr = cache->blocks[set][index].address;
    cache->blocks[set][index].address = newAddr;
  }

  return (CacheResult) { .status = CACHE_MISS_WITH_REPLACE, .replaceAddr = repAddr};
}
