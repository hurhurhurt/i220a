#ifndef HEX_UTIL_H_
#define HEX_UTIL_H_

/** Return non-zero iff c is hex char '0'-'9', 'a'-'f', 'A'-'F'.
 */
int isHexChar(char c);

/** Given a char hexChar, return corresponding hexet value; return < 0
 *  if hexChar is not a valid hex char.
 */
int charToHexet(char hexChar);

/** Given a valid hexet value in 0-15, return corresponding lowercase
 *  hex char.
 */
char hexetToChar(int hexet);

#endif //ifndef HEX_UTIL_H_
