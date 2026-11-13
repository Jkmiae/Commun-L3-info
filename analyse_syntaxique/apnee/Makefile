CC=gcc
CFLAGS=-I.
DEPS = lexer.h lookahead_lexer.h
OBJS = lookahead_lexer.o lexer.o
LIBS = -lm

all: assignations show_tokens

assignations: assignations.o $(OBJS)
	gcc -o assignations assignations.o $(OBJS) $(LIBS)

show_tokens: show_tokens.o $(OBJS)
	gcc -o show_tokens show_tokens.o $(OBJS) $(LIBS)

lexer.c lexer.h: expr.lex
	flex --header-file=lexer.h -olexer.c expr.lex

clean:
	rm -f assignations.o show_tokens.o $(OBJS) lexer.c lexer.h

distclean: clean
	rm -f assignations show_tokens

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)