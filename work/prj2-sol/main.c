#include "big-bits.h"
#include "errors.h"
#include "hex-util.h"
#include "stack.h"

#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

typedef struct {
  /** dynamically allocated buffer */
  char *buf;

  /** number of bytes currently allocated to buf[] */
  size_t size;

  /** index of first unused byte in buf[] */
  size_t index;
} SizedBuf;

enum {
  /** increment SizedBuf size in multiple of this */
  SIZED_BUF_INC = 32
};

/** Ensure buf has sufficient space for incSize additional bytes.
 */
static void
ensureSizedBuf(SizedBuf *buf, size_t incSize)
{
  if (buf->index + incSize > buf->size) {
    const size_t needed = buf->index + incSize - buf->size;
    const size_t inc = //round up needed to multiple of SIZED_BUF_INC
      ((needed + SIZED_BUF_INC - 1)/SIZED_BUF_INC) * SIZED_BUF_INC;
    buf->size += inc;
    if (!(buf->buf = realloc(buf->buf, buf->size))) {
      fatal("cannot alloc %zu for SizedBuf: %s", buf->size, strerror(errno));
    }
  }
}

/** Set hex->buf to hex chars from str.  Ignores '_' chars in str.  If
 *  str contains a '*' followed immediately by a positive hex-char,
 *  then the hex-char indicates the number of times the string
 *  accumulated so far should be repeated.  For example, "AB*31*2"
 *  abbreviates "AB_AB_AB_1_AB_AB_AB_1".
 */
static void
getHex(const char *str, const char **end, SizedBuf *hex)
{
  const char *p = str;
  hex->index = 0;
  while (1) {
    char c = *p;
    if (isHexChar(c)) { //accumulate
      p++;
      ensureSizedBuf(hex, 1);
      hex->buf[hex->index++] = c;
    }
    else if (c == '_') { //skip
      p++;
    }
    else if (c == '*') {
      int nRepeat = charToHexet(p[1]);
      if (nRepeat <= 0) {
        break;
      }
      else {
        p += 2;
        int nChars = hex->index;
        ensureSizedBuf(hex, nChars * (nRepeat - 1));
        for (int i = 0; i < nRepeat - 1; i++) {
          strncpy(&hex->buf[hex->index], hex->buf, nChars);
          hex->index += nChars;
        }
      }
    }
    else {
      break;
    }
  }
  ensureSizedBuf(hex, 1);
  hex->buf[hex->index++] = '\0';
  *end = p;
}

/** Print out line with ^ indicating current position */
static void
showErrorPointer(const SizedBuf *line)
{
  fprintf(stderr, "%s%*s^\n", line->buf, (int)(line->index), " ");
}

static bool
isStackEntries(const SizedBuf *line, const Stack *stk, int minNEntries)
{
  const int nEntries = nEntriesStack(stk);
  bool isOk = true;
  if (nEntries < minNEntries) {
    error("stack only has %d entries; need at least %d", nEntries, minNEntries);
    showErrorPointer(line);
    isOk = false;
  }
  return isOk;
}

/** Pop off all big-bits on stack, freeing them up */
static void
cleanStack(Stack *stk)
{
  int nEntries = nEntriesStack(stk);
  for (int i = 0; i < nEntries; i++) {
    const BigBits *bits = popStack(stk);
    freeBigBits((BigBits *)bits);
  }
}

static void
doError(int err, SizedBuf *line, Stack *stk)
{
  cleanStack(stk);
  if (err != 0) error(strerror(err));
  showErrorPointer(line);
}

/** Perform operation specified by op() function pointer on stack stk.
 */
static bool
doOp(SizedBuf *line, Stack *stk,
     const BigBits *(*op)(const BigBits *bits1, const BigBits *bits2))
{
  bool isOk = isStackEntries(line, stk, 2);
  int err = 0;
  if (isOk) {
    const BigBits *bits2 = popStack(stk);
    const BigBits *bits1 = popStack(stk);
    const BigBits *result = op(bits1, bits2);
    freeBigBits((BigBits *)bits1);
    freeBigBits((BigBits *)bits2);
    if (result) {
      pushStack(stk, result);
    }
    else {
      err = errno;
    }
  }
  if (!isOk) doError(err, line, stk);
  return isOk;
}

static void
help(FILE *out)
{
  fprintf(out,
          "This program is a postfix big-bits calculator with operators\n"
          "'&' for bitwise-and, '|' for bitwise-or and '^' for bitwise-xor.\n"
          "Literals consist of a '0x' or '0X' prefix followed by one-or-more\n"
          "hexadecimal characters or underscores (which are ignored).\n\n"
          "Postfix allows arbitrary complex bitwise expressions without the\n"
          "use of parentheses: for example '0xa 0x1 &' denotes the infix\n"
          "expression '0xa & 0x1' while '0xa 0xb & 0xc 0xd & |' denotes the\n"
          "infix expression '(0xa & 0xb) | (0xc & 0xd)'.\n\n"
          "Input of long literals is facilitated by a repetition feature\n"
          "which allows a literal to contain a '*' followed by a single\n"
          "character positive hexadecimal repetition count N; the preceeding\n"
          "literal string is repeated N times.  For example, '0xa*3' denotes\n"
          "'0xaaa' and '0xa*3bb*2' denotes '0xaa_abba_aabb'.\n");
}


static void
doLine(SizedBuf *line, SizedBuf *hex, Stack *stk, FILE *out)
{
  line->index = 0;
  const char *const end = line->buf + strlen(line->buf);
  for (const char *p = line->buf; *p != '\0' && p < end;) {
    while (isspace(*p)) p++;
    line->index = p - line->buf;
    switch (*p) {
    case '0':
      if (p + 3 >= end || !strchr("xX", p[1]) || !isHexChar(p[2])) {
        error("bad hex value");
        doError(0, line, stk);
        return;
      }
      else {
        const char *endP;
        getHex(&p[2], &endP, hex);
        p = endP;
        const BigBits *bits = newBigBits(hex->buf);
        if (bits) {
          pushStack(stk, bits);
        }
        else {
          doError(errno, line, stk);
          return;
        }
      }
    break;
    case '|':
      if (!doOp(line, stk, orBigBits)) return;
      p++;
      break;
    case '&':
      if (!doOp(line, stk, andBigBits)) return;
      p++;
      break;
    case '^':
      if (!doOp(line, stk, xorBigBits)) return;
      p++;
      break;
    case '\0':
      break;
    case '?': case 'h': case 'H':
      help(out);
      return;
    default:
      error("invalid char '%c'", *p);
      doError(0, line, stk);
      return;
    }
  } //for
}

/** Output string hex, with _ separating groups of hex chars */
static void
outHexString(const char *hex, FILE *out)
{
  enum { GROUP_N = 4 };
  int len = strlen(hex);
  int len1 = len - 1;
  for (int i = 0; i < len; i++) {
    fprintf(out, "%c", hex[i]);
    if (i < len1 && (len1 - i) % GROUP_N == 0) fprintf(out, "_");
  }
}

/** Dump out all big-bits contained in stack stk; more than one result
 *  indicates a missing operator.
 */
static void
outResults(Stack *stk, FILE *out)
{
  int nEntries = nEntriesStack(stk);
  if (nEntries > 0) {
    if (nEntries > 1) error("missing operator");
    for (int i = 0; i < nEntries; i++) {
      const BigBits *bits = popStack(stk);
      const char *hexStr = stringBigBits(bits);
      if (!hexStr) {
        error(strerror(errno));
      }
      else {
        fprintf(out, "0x");
        outHexString(hexStr, out);
        fprintf(out, "\n");
      }
      freeBigBits((BigBits *)bits);
    }
  }
}

static void
go(FILE *in, FILE *out)
{
  SizedBuf line = { NULL, 0, 0 };
  SizedBuf hex = { NULL, 0, 0 };
  Stack *stk = newStack();
  bool isTty = isatty(fileno(in));
  int n;
  if (isTty) fprintf(out, "enter 'h', 'H' or '?' for help\n");
  do {
    if (isTty) {
      fprintf(out, ">> ");
      fflush(out);
    }
    errno = 0;
    n = getline(&line.buf, &line.size, in);
    if (n > 0) {
      doLine(&line, &hex, stk, out);
      outResults(stk, out);
    }
  } while (n >= 0);
  if (errno > 0) {
    fprintf(stderr, "getline() error: %s\n", strerror(errno));
  }
  freeStack(stk);
  free(hex.buf);
  free(line.buf);
}

int
main()
{
  go(stdin, stdout);
  return 0;
}
