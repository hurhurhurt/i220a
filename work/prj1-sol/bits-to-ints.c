
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

void append(char* s, char c){
  int len = strlen(s);
  s[len] = c;
  s[len+1] = '\0';
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
  fclose(inFile);
  return counter -  1; // accounts for the extra EOF character
}

BitsValue
bits_to_ints(FILE *inFile, const char *inName, int nBits, bool *isEof)
{ 
  //nBits value should make sense
  assert(0 < nBits && nBits <= CHAR_BIT*sizeof(BitsValue));
  BitsValue value = 0;
  printf("%d", allocate_space(inFile));
  *isEof = true;
  return value;
  /*
  // allocate enough space for array
  
  while (!feof(inFile)){
    if (inFile == NULL){
      printf("Invalid File");
      return 0;
    }
    int c = fgetc(inFile);
    
    if (c == EOF){
      printf("End of file.");
      break;
      }
    if (c != 10){
      if (c == 32){
	c = fgetc(inFile);
      }
      else{
	append(str, c);
	c = fgetc(inFile);
      }   
    }
  }
  fclose(inFile);
  *isEof = true;

  printf("\n Contents of file: %s", str);
  printf("\n Length of string: %ld\n", sizeof(str));
  
  char newArray[8][8];
  int count = 0;
  for (int z = 0;
    for (int i = 0; i < 8; i++){
      for (int j = 0; j < 8; j++){
        if (count < 8){
	newArray[j][i] = 
      
  
  int counter = 0;
  for (int i = 0; i < strlen(str); i++){
    if (counter < 8){
      printf("%c", str[i]);
      counter++;
    }
    else{
      printf("%c\n", str[i]);
      counter = 0;
	}
  }
  */
}
