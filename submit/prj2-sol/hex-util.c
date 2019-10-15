#include "hex-util.h"

#include <assert.h>

/** Return non-zero iff c is hex char '0'-'9', 'a'-'f', 'A'-'F'.
 */
int
isHexChar(char c) {
  return ('0' <= c && c <= '9') ||
    ('a' <= c && c <= 'f') ||
    ('A' <= c && c <= 'F');
}

/** Given a char hexChar, return corresponding hexet value; return < 0
 *  if hexChar is not a valid hex char.
 */
int
charToHexet(char hexChar) {
  if ('0' <= hexChar && hexChar <= '9') {
    return hexChar - '0';
  }
  else if ('a' <= hexChar && hexChar <= 'f') {
    return hexChar - 'a' + 10;
  }
  else if ('A' <= hexChar && hexChar <= 'F') {
    return hexChar - 'A' + 10;
  }
  else { //hexChar is in error
    return -1; //indicate error to caller
  }
}

/** Given a valid hexet value in 0-15, return corresponding lowercase
 *  hex char.
 */
char
hexetToChar(int hexet)
{
  assert(0 <= hexet && hexet < 16);

  //static below means that chars[] is created/initialized only once
  //when the program starts up; by being declared within this function
  //it is not visible outside.
  const static char chars[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'a', 'b', 'c', 'd', 'e', 'f'
  };
  return chars[hexet];
}
