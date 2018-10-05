#!/bin/bash
yacc yacc.y -d 
flex lex.l
gcc node.c -c
gcc symtab.c -c
ar rcs node.a node.o
ar rcs symtab.a symtab.o
gcc lex.yy.c y.tab.c node.a symtab.a
./a.out $1
exit 0
