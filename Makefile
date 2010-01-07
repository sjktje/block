
CC = gcc
INSTDIR = /usr/local/bin
MANDIR = /usr/local/man/man1
CFLAGS = -Wall -ansi
INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644


all: block unblock

block: block.o common.o
	$(CC) -o block block.o common.o
unblock: unblock.o common.o
	$(CC) -o unblock unblock.o common.o
block.o: block.c
	$(CC) $(CFLAGS) -c block.c
unblock.o: unblock.c
	$(CC) $(CFLAGS) -c unblock.c
common.o: common.c
	$(CC) $(CFLAGS) -c common.c
clean:
	-rm block.o unblock.o common.o block unblock 
install: block
	$(INSTALL_PROGRAM) block $(INSTDIR)
	$(INSTALL_PROGRAM) unblock $(INSTDIR)
	$(INSTALL_DATA) block.1 $(MANDIR)
deinstall:
	-rm $(INSTDIR)/block
	-rm $(MANDIR)/block.1
