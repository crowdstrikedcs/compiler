calculator: calculator.tab.o lex.yy.o ast.o
	cc -o calculator calculator.tab.o lex.yy.o ast.o -lm
calculator.tab.o: calculator.tab.c
	cc -c calculator.tab.c
lex.yy.o: lex.yy.c
	cc -c lex.yy.c
ast.o: ast.c ast.h
	cc -c ast.c 
calculator.tab.c: calculator.y
	bison -d calculator.y
lex.yy.c: calculator.l
	lex calculator.l
clean:
	rm *.tab.c
	rm *.tab.h
	rm *.yy.c
	rm *.o