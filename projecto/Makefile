# Makefile for the mv driver (based on that of hello).

COMPILER_TYPE= gnu

CC=gcc

PROG=	projecto
SRCS=	main.c vbe.c video_gr.c test4.c

CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS} liblm.a
LDADD+=	-llm -ldriver -lsys

LDFLAGS+= -L . 
  
MAN=

BINDIR?= /usr/sbin

.include <bsd.prog.mk>
.include <bsd.gcc.mk>