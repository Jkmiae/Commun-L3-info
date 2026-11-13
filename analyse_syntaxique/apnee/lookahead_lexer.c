//
// Created by Gilles Sérasset on 09/10/2019.
//

#include "lookahead_lexer.h"
#include "lexer.h"
#include <stdlib.h>
#include <string.h>

extern YY_DECL ;

Token * __lookahead[MAX_LOOKAHEAD]; // The maximum number of lookahead tokens
int __lookahead_current = 0;
int __lookahead_available = 0;

// lookahead = ---****-------
//                1: __lookahead_current is the position of the current Token in the look ahead queue.
//                    2: __lookahead_available is the first position available in the queue.
// Lookahead is empty when __lookahead_current == __lookahead_available
// lookahead is full when __lookahead_current == __lookahead_available + 1

int __available_lookaheads() {
    int diff = __lookahead_available - __lookahead_current;
    if (diff < 0) diff = diff + MAX_LOOKAHEAD;
    return diff;
}

void __lookahead_queue(Token *t) {
    __lookahead[__lookahead_available] = t;
    __lookahead_available = (__lookahead_available + 1) % MAX_LOOKAHEAD ;
    if (__lookahead_available == __lookahead_current) {
        fprintf(stderr, "LOKAHEAD overflow in lookahead scanner.");
        exit(1);
    }
}

Token * __lookahead_peek(int n) {
    int avail = __available_lookaheads();
    // populate the lookahead (warning, n = 0 means I want the first lookahead)
    while(avail < n+1) {
        __lookahead_queue(yylex());
        avail++;
    }
    return __lookahead[(__lookahead_current + n) % MAX_LOOKAHEAD];
}

void __lookahead_dequeue() {
    int avail = __available_lookaheads();
    if (avail > 0) {
        Token * t = __lookahead_peek(0);
        _freeToken(t);
    }
    __lookahead_current = (__lookahead_current + 1) % MAX_LOOKAHEAD;
}

void __lookahead_init() {
    __lookahead_current = __lookahead_available = 0;
}

void initLexer(const char* f) {
    if (strlen(f) == 1 && f[0] == '-') {
        yyin = stdin;
    } else if ( (yyin = fopen(f,"r")) == 0 ) {
        perror(f);
        exit(1);
    }
    __lookahead_init();
}

Token *currentToken() {
    return __lookahead_peek(0);
}
Token *lookup(int offset) {
    return __lookahead_peek(offset-1);
}

void consume(int tokenType) {
    Token *t = currentToken();
    if (t == NULL) {
        fprintf(stderr, "Token mismatch while consuming token (expected type %d/actual EOF)", tokenType);
        exit(1);
    }
    else if (t->type == tokenType) {
        __lookahead_dequeue();
    } else {
        fprintf(stderr, "Token mismatch while consuming token (expected type %d/actual type %d)", tokenType, t->type);
        exit(1);
    }
}

void next() {
    __lookahead_dequeue();
}

char *get_text(Token *t) {
    return strdup(t->text);
}

Token *dup_token(Token *t) {
    Token *dup_t = malloc(sizeof *t);
    *dup_t = *t;
    dup_t->text = get_text(t); // creates a copy of the string.
    return dup_t;
}