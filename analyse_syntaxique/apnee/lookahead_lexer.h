//
// Created by Gilles Sérasset on 09/10/2019.
//

#ifndef SIMPLE_ASSIGN_LOOKAHEAD_LEXER_H
#define SIMPLE_ASSIGN_LOOKAHEAD_LEXER_H
#include <stdio.h>
#include "tokens.h"

// Declare the lexer function as external.
extern YY_DECL ;

#define MAX_LOOKAHEAD 10

/**
 * Initialize the lexer.
 * @param f
 */
void initLexer(const char *f);

/**
 * returns the currentToken in the lexer flow. The currentToken is defined immediately after
 * initLexer has been called, you should not call next() to get the first token.
 * This function does not consume any token and leave the lexer flow untouched.
 * This function is the same as lookup(1).
 *
 * MEMORY MANAGEMENT WARNING: the returned token will be freed when consumed. Hence, the token address
 * should not be kept by the caller, and used after consuming. If you need to use the token or it's attributes,
 * you should keep a copy of it.
 *
 * @return the currentToken in the lexer flow.
 */
Token *currentToken();

/**
 * return the nth token in advance in the lexer flow.
 * This function does not consume any token and leave the lexer flow untouched (the current Token is still the same).
 *
 * MEMORY MANAGEMENT WARNING: the returned token will be freed when consumed. Hence, the token address
 * should not be kept by the caller, and used after consuming. If you need to use the token or it's attributes,
 * you should keep a copy of it.
 *
 * @param offset the lookup ahead offset (1 means current (unconsumed) Token).
 * @return a token read from the lexer flow.
 */
Token *lookup(int n);

/**
 * advances to next token in the lexer flow, if the current token is of the given type.
 * This function breaks the program if the expected type is not the type of the current token.
 *
 * MEMORY MANAGEMENT WARNING: this function frees the memory allocated to the currentToken. Hence, the token address
 * should not be kept by the caller, and used after this action. If you need to use the token or it's attributes,
 * you should keep a copy of it.
 * @param tokenType
 */
void consume(int expectedTokenType);

/**
 * advance to next token in the lexer flow.
 */
void next();

/**
 * make a copy of token text.
 * It is your responsibility to free the text memory when necessary.
 */
char *get_text(Token *t);

/**
 * make a copy of the token.
 * It is your responsibility to free the token memory (including its text) when necessary.
 */
Token *dup_token(Token *t);

#endif //SIMPLE_ASSIGN_LOOKAHEAD_LEXER_H
