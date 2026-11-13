#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"
#include "lookahead_lexer.h"

/* A very crude command that shows the sequence of tokens present in 
   the file given as first and unique parameter. If "-" is given as a parameter, 
   then the stding will be processed */
int main(int argc, char *argv[]) {
    initLexer(argv[1]);
    while ( currentToken() != NULL ) {
        fprintf(stdout, "%d (%s) || ", currentToken()->type, currentToken()->text);
        // Memory management warning: after the next() action, the token struct is released from memory and any pointer
        // to it will lead to unallocated data. Dereferencing it MAY temporarily work as expected but will nevertheless
        // be a programming error.
        next();
    }
    return 0;
}

