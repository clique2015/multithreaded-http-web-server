#ifndef __SEMANTICS_H__
#define __SEMANTICS_H__
#include "parser.h"
extern string tokenNames[];
bool expect(TokenType typeA, TokenType typeB, TokenType typeC, TokenType typeD, TokenType typeE, int count);
bool expect(TokenType typeA, TokenType typeB, TokenType typeC, TokenType typeD, int count);
bool expect(TokenType typeA, TokenType typeB, TokenType typeC, int count);
bool expect(TokenType typeA, TokenType typeB, int count);
bool expect(TokenType typeA, int count);
void error(string error, int lineNo);

#endif


