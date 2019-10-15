#ifndef BIG_INT_H_
#define BIG_INT_H_

/** Declare BigBits as a type which is a synonym for the opaque
 *  structure struct BigBits representing a big integer abstract
 *  data type.
 */
typedef struct BigBits BigBits;

//Note that even though they have the same spelling, the BigBits
//identifier in `struct BigBits` is a structure tag and has no
//relationship to the `BigBits` typedef; the structure tag context is
//triggered when the `BigBits` identifier is used immediately after a
//struct keyword; when the `BigBits` identifier is used in a context
//not immediately following the struct keyword, it refers to the
//typedef name.

//The above is a *definition* for the BigBits typedef, but merely
//an incomplete *declaration* for the `struct BigBits` tagged
//structure.  Providing a definition for `struct BigBits` will
//make the abstract data type concrete.

/** Return a pointer to a representation of a big integer with value
 *  corresponding to non-empty hexadecimal string hex.  Note that hex
 *  will only contain hexadecimal characters '0' - '9', 'a' - 'f' and
 *  'A' - 'F' terminated by a NUL '\0' char.
 *
 *  The string hex may not remain valid after this function returns.
 *
 *  Returns NULL on error with errno set "appropriately".
 */
const BigBits *newBigBits(const char *hex);

/** Frees all resources used by currently valid bigBits.  bigBits
 *  is not valid after this function returns.
 */
void freeBigBits(BigBits *bigBits);

/** Return a lower-case hex string representing bigBits. Returned
 *  string should not contain any non-significant leading zeros.
 *  Returns NULL on error with errno set "appropriately".  (Note that
 *  there is no call to free the corresponding string).
 */
const char *stringBigBits(const BigBits *bigBits);

/** Return a new BigBits which is the bitwise-& of bigBits1 and bigBits2.
 *  Returns NULL on error with errno set "appropriately".
 */
const BigBits *andBigBits(const BigBits *bigBits1, const BigBits *bigBits2);

/** Return a new BigBits which is the bitwise-| of bigBits1 and bigBits2.
 *  Returns NULL on error with errno set "appropriately".
 */
const BigBits *orBigBits(const BigBits *bigBits1, const BigBits *bigBits2);

/** Return a new BigBits which is the bitwise-^ of bigBits1 and bigBits2.
 *  Returns NULL on error with errno set "appropriately".
 */
const BigBits *xorBigBits(const BigBits *bigBits1, const BigBits *bigBits2);

#endif //ifndef BIG_INT_H_
