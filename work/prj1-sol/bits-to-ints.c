#include "bits-to-ints.h"
#include "errors.h"
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//@TODO: auxiliary definitions

/** This function should be called with inFile set to an input FILE
 *  stream corresponding to a file named inName.  This FILE should
 *  contain a stream of '0' or '1' bit-value characters optionally
 *  separated by whitespace (as determined by isspace()) characters.
 *  The function should return the next unsigned integer value given
 *  by the next next nBits bit characters read from in.
 *
 *  The nBits argument (which should be > 0) will specify the number
 *  of bits from FILE stream inFile which are to be grouped to form an
 *  unsigned integer value.
 *
 *  The bytes within the bit-stream are assumed to be in big-endian
 *  order (with the most significant byte first), and the bits within
 *  each byte are ordered little-endian with the least-significant bit
 *  first.
 *
 *  For example, assume that nBits is 16 and the least significant bit
 *  in the result is at index 0, and most significant bit in the
 *  result at index 15; i.e., the bits in the BitsValue result are
 *  indexed 15-14-...-1-0.  The order of the corresponding bits in the
 *  input stream will be 8-9-10-11-12-13-14-15 - 0-1-2-3-4-5-6-7.  So
 *  the bit-stream "1101 0101 1011 0011" will result in the BitsValue
 *  0xabcd (which has binary representation 1010_1011_1100_1101).
 *
 *  When the function returns, *isEof should be set to true if
 *  end-of-file is encountered on inFile.  If EOF is encountered
 *  within a partial value, then a suitable error message should be
 *  printed before returning with *isEof true.
 *
 *  If any character other than '0', '1' or whitespace is encountered
 *  in inFile, then a suitable error message should be printed and the
 *  function should return with *isEof set to true.
 */

char *strrev(char *str){
  if (!str || ! *str) return str;
  char ch;
  int i = strlen(str) - 1, j = 0;
  while (i > j){
    ch = str[i];
    str[i]=str[j];
    str[j]=ch;
    i--;
    j++;
  }
  return str;
}

int allocate_space(FILE *inFile){
  int counter = 0;
  while (!feof(inFile)){
    if (inFile == NULL){
      printf("Invalid File");
      return 1;
    }
    int c = fgetc(inFile);
    if (c != 10){
      if (c == 32){
	c = fgetc(inFile);
      }
      else{
	counter +=1;
	c = fgetc(inFile);
      }
    }  
  }
  return counter; // accounts for the extra EOF character
}

BitsValue
bits_to_ints(FILE *inFile, const char *inName, int nBits, bool *isEof)
{ 
  //nBits value should make sense
  assert(0 < nBits && nBits <= CHAR_BIT*sizeof(BitsValue));
  BitsValue value = 0;
  
  long int space = allocate_space(inFile);
  char returnString[space];
  printf("Value inside space variable: %ld\n", space);
  printf("Size of return string: %ld\n", sizeof(returnString));
  returnString[sizeof(returnString) - 1] = '\0';
  rewind(inFile);
  
  int c = fgetc(inFile);
  int i = 0;
  while (c != EOF){
    if (c == 10 || c == 32){
      c = fgetc(inFile);
    }
    else{
      returnString[i] = c;
      i++;
      c = fgetc(inFile);
    }
  }
  printf("\nBefore reversing: %s\n", returnString);  
  printf("After reversing: %s\n", strrev(returnString));


  for (int i = 0, i <= space, i+=nBits){
    
  char *a = strrev(returnString);
  int num = 0;
  do{
    int b = *a == '1'?1:0;
    num = (num<<1)|b;
    a++;
      }while (*a);
  printf("%X\n", num);
    //printf(strtol(strrev(returnString), NULL, 2));
  fclose(inFile);
  *isEof = true;
  return value;
}
