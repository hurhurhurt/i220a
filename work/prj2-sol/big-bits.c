#include "big-bits.h"
#include "hex-util.h"
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/** Provide concrete definition for struct BigBits to flesh out
 *  abstract BigBits data type.
 */
struct BigBits {
  int *arr;
  unsigned int size;
};



/** Return a pointer to a representation of a big integer with value
 *  corresponding to the non-empty hexadecimal string hex.  Note that
 *  hex will only contain hexadecimal characters '0' - '9', 'a' - 'f'
 *  and 'A' - 'F' terminated by a NUL '\0' char.
 *
 *  The string hex may not remain valid after this function returns.
 *
 *  Returns NULL on error with errno set "appropriately".
 */

const BigBits * newBigBits(const char *hex)
{
  assert(CHAR_BIT == 8);
  int sizeHex = strlen(hex);
  struct BigBits * convertBits = malloc(sizeHex*sizeof(struct BigBits));
  convertBits->arr = malloc(sizeHex * (sizeof(int)));
  convertBits->size = sizeHex;
  for (int i = 0; i < sizeHex; i++){
    convertBits->arr[i] = charToHexet(hex[i]);
  }
  return convertBits;
}

/** Frees all resources used by currently valid bigBits.  bigBits
 *  is not valid after this function returns.
 */
void
freeBigBits(BigBits *bigBits)
{
  //free(bigBits->arr);
  //free(bigBits);
}


/** Return a lower-case hex string representing bigBits. Returned
 *  string should not contain any non-significant leading zeros.
 *  Returns NULL on error with errno set "appropriately".  (Note that
 *  there is no call to free the corresponding string).
 */
const char * stringBigBits(const BigBits *bigBits)
{
  int size = bigBits->size;
  char * temp = malloc(size*sizeof(char));
  
  for (int i = 0; i < size; i++){
    if(hexetToChar(bigBits->arr[i]) < 0) return NULL;
    temp[i] = hexetToChar(bigBits->arr[i]);
  }
  return temp;
}

/** Return a new BigBits which is the bitwise-& of bigBits1 and bigBits2.
 *  Returns NULL on error with errno set "appropriately".
 */
const BigBits *
andBigBits(const BigBits *bigBits1, const BigBits *bigBits2)
{
  int smaller, diff; //smaller tracks size of small BigBits, diff tracks difference so i can shift over when bitwise anding
  const BigBits * bigBit = bigBits1;
  const BigBits * smallBit = bigBits2;
  if (bigBit->size >= smallBit->size) smaller = smallBit->size;
  else {
    smaller = bigBit->size;
    bigBit = bigBits2;
    smallBit = bigBits1;
  }
  diff = bigBit->size - smallBit->size;
  for (int i = 0; i < smaller; i++){
    smallBit->arr[i] &= bigBit->arr[i+diff];
  }
  return smallBit;
}

/** Return a new BigBits which is the bitwise-| of bigBits1 and bigBits2.
 *  Returns NULL on error with errno set "appropriately".
 */
const BigBits *
orBigBits(const BigBits *bigBits1, const BigBits *bigBits2)
{
  int bigger, diff;
  const BigBits * bigBit = bigBits1;
  const BigBits * smallBit = bigBits2;
  if (bigBit->size >= smallBit->size) bigger = bigBit->size;
  else{
    bigger = smallBit->size;
    bigBit = bigBits2;
    smallBit = bigBits1;
  }
  diff = bigBit->size - smallBit->size;
  for (int i = 0; i < bigger; i++){
    bigBit->arr[i+diff] |= smallBit->arr[i];
  }
  return bigBit;
    
}

/** Return a new BigBits which is the bitwise-^ of bigBits1 and bigBits2.
 *  Returns NULL on error with errno set "appropriately".
 */
const BigBits *
xorBigBits(const BigBits *bigBits1, const BigBits *bigBits2)
{
  int bigger, diff;
  const BigBits * bigBit = bigBits1;
  const BigBits * smallBit = bigBits2;
  if (bigBit->size >= smallBit->size) bigger = bigBit->size;
  else{
    bigger = smallBit->size;
    bigBit = bigBits2;
    smallBit = bigBits1;
  }
  diff = bigBit->size - smallBit->size;
  for (int i = 0; i < bigger; i++){
    bigBit->arr[i+diff] ^= smallBit->arr[i];
  }
  return bigBit;
}
