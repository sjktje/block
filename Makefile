
CC = gcc
INSTDIR = /usr/local/bin
MANDIR = /usr/local/man/man1
# Release
CFLAGS = -Wall -ansi -I/usr/local/include
LDFLAGS = -lsjk
# Debug
#CFLAGS = -Wall -ansi -DDEBUG
INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644


all: block unblock

block: block.o 
	$(CC) -o block block.o $(LDFLAGS)
unblock: unblock.o 
	$(CC) -o unblock unblock.o $(LDFLAGS)
block.o: block.c
	$(CC) $(CFLAGS) -c block.c
unblock.o: unblock.c
	$(CC) $(CFLAGS) -c unblock.c
clean:
	-rm block.o unblock.o block unblock 
install: block
	$(INSTALL_PROGRAM) block $(INSTDIR)
	$(INSTALL_PROGRAM) unblock $(INSTDIR)
	$(INSTALL_DATA) block.1 $(MANDIR)
deinstall:
	-rm $(INSTDIR)/block
	-rm $(MANDIR)/block.1
