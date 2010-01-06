all: block

CC = gcc
INSTDIR = /usr/local/bin
MANDIR = /usr/local/share/man/man1
CFLAGS = -Wall -ansi
INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644



block: block.o 
	$(CC) -o block block.o
block.o: block.c
	$(CC) $(CFLAGS) -c block.c
clean:
	-rm block.o block
install: block
	$(INSTALL_PROGRAM) block $(INSTDIR)
	$(INSTALL_DATA) block.1 $(MANDIR)
deinstall:
	-rm $(INSTDIR)/block
	-rm $(MANDIR)/block.1
