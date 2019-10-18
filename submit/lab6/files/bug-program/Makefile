CFLAGS = -g -Wall -std=c11 -O1

TARGETS = bug-program

all:		$(TARGETS)

run: memcheck

clean:
		rm -f *.o *~ $(TARGETS)

memcheck: $(TARGETS)
	valgrind ./$(TARGETS)
