all: block

CC = gcc
INSTDIR = $(HOME)/bin
CFLAGS = -Wall -ansi
block: block.o 
	$(CC) -o block block.o
block.o: block.c
	$(CC) $(CFLAGS) -c block.c
clean:
	-rm block.o block
install: block
	@if [ -d $(INSTDIR) ]; \
		then \
		cp block $(INSTDIR); \
		chmod a+x $(INSTDIR)/block; \
		chmod og-w $(INSTDIR)/block; \
		echo "Installed in $(INSTDIR)"; \
	else \
		echo "$(INSTDIR) does not exist."; \
	fi
