CXX=gcc
PROGRAM_NAME=calc


all: $(PROGRAM_NAME)

gram.tab.c gram.tab.h:	gram.y
	bison -d gram.y -Wother

lex.yy.c: lex.l gram.tab.h
	flex lex.l

$(PROGRAM_NAME): lex.yy.c gram.tab.c gram.tab.h
	$(CXX) gram.tab.c lex.yy.c astsrc.c -lfl -lm -ly -o $(PROGRAM_NAME)

clean:
	rm -f gram.tab.c gram.tab.h lex.yy.c *.o $(PROGRAM_NAME)
