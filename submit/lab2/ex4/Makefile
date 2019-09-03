PROJECT = key-value

SRC_FILES = key-value.c key-value.o main.o process-file.c process-file.o key-value.h main.c Makefile process-file.h README

CFLAGS = -g -Wall -std=c11
LDFLAGS = -lm

$(PROJECT):	$(SRC_FILES)
	$(CC) *.c $(LDFLAGS) -o $@

make: $(SRC_FILES)
	
clean:
	rm -f *.o $(PROJECT) $(PROJECT).tar.gz

depend:
	$(CC) -MM $(CPPFLAGS) *.c

dist:	$(PROJECT).tar.gz
$(PROJECT).tar.gz:	$(SRC_FILES)
	tar -cf $(PROJECT).tar $(SRC_FILES)
	gzip -f $(PROJECT).tar




