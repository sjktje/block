PROG=   block
SRCS=   block.c
BINDIR= ${PREFIX}/bin
MAN=

CFLAGS+=-std=c99 -pedantic -Wall -W
CFLAGS+=-Wno-missing-field-initializers -Wundef -Wendif-labels -Wshadow 
CFLAGS+=-Wpointer-arith -Wbad-function-cast -Wcast-align -Wwrite-strings 
CFLAGS+=-Wstrict-prototypes -Wmissing-prototypes -Wnested-externs -O2


.include <bsd.prog.mk>

