TARGETS = big-bits

BIG_BITS_OFILES = \
  main.o \
  big-bits.o \
  errors.o \
  hex-util.o \
  stack.o

CFLAGS = -g -Wall -std=c11 -D_POSIX_C_SOURCE=200809L

all: 		$(TARGETS)

big-bits: 	$(BIG_BITS_OFILES)
		$(CC) $(BIG_BITS_OFILES) $(LDFLAGS) -o $@

.phony:		clean

clean:
		rm -f *~ *.o $(TARGETS) 


big-bits.o: big-bits.c big-bits.h
errors.o: errors.c errors.h
hex-util.o: hex-util.c hex-util.h
stack.o: stack.c stack.h

