PROG=   block
SRCS=   block.c
BINDIR= ${PREFIX}/bin
MAN1=	block.1

CFLAGS+=-std=c99 -pedantic -Wall -W
CFLAGS+=-Wundef -Wendif-labels -Wshadow 
CFLAGS+=-Wpointer-arith -Wbad-function-cast -Wcast-align -Wwrite-strings 
CFLAGS+=-Wstrict-prototypes -Wmissing-prototypes -Wnested-externs -O2

.include <bsd.prog.mk>
