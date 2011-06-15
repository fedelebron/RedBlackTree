CFLAGS = -ansi -pedantic -Wall -Wfatal-errors -O3
RM = rm
CC = gcc
COMPILE = ${CC} ${CFLAGS}

all: rbtree 

rbtree: rbtree.o rb_delete.o rb_tests.o main.c
	${COMPILE} rbtree.o rb_tests.o rb_delete.o main.c -o rbtree

rbtree.o: rbtree.c rbtree.h
	${COMPILE} -c rbtree.c -o rbtree.o

rb_delete.o: rb_delete.c
	${COMPILE} -c rb_delete.c -o rb_delete.o

rb_tests.o: rb_tests.c rb_tests.h
	${COMPILE} -c rb_tests.c -o rb_tests.o

clean: 
	${RM} -f rbtree.o rb_delete.o rb_tests.o rbtree

