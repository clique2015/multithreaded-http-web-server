#ifndef __PARSER_H__
#define __PARSER_H__
using namespace std;
#include <string>
#include <vector>

typedef enum {
    TOKEN_NOT_EQUAL,     
    TOKEN_GR_EQUAL, 
    TOKEN_LESS_EQUAL,       
    TOKEN_GREATER,
    TOKEN_LESS,    
    TOKEN_EQ_EQUAL,
    TOKEN_EQUAL, 
    TOKEN_BANG,
    TOKEN_SETC, TOKEN_HEADER,TOKEN_ELSE,
    TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
    TOKEN_LEFT_BLOCK, TOKEN_RIGHT_BLOCK,
    TOKEN_COMMA, TOKEN_DOT, TOKEN_SEMICOLON,  

    TOKEN_tokenStore, TOKEN_END,

    TOKEN_BOOL, TOKEN_VAR, TOKEN_STRINGVAR, TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_NUMBER, 
    TOKEN_PIN,TOKEN_FALSE, TOKEN_TRUE, TOKEN_FUNCALL, TOKEN_OUTPUT, TOKEN_ENDL,

    TOKEN_INFINITE, TOKEN_FUN, TOKEN_IF, TOKEN_VOID,TOKEN_ECHO,

    TOKEN_BREAK,TOKEN_WHILE, TOKEN_POUND, M_QUERY, M_CONNECT, TOKEN_EOF

}TokenType;

#define FREE_ARRAY(type, pointer, oldCount) \
reallocate(pointer, sizeof(type) * (oldCount), 0)

#define GROW_ARRAY(type, pointer, oldCount, newCount) \
(type*)reallocate(pointer, sizeof(type) * (oldCount), \
sizeof(type) * (newCount))

extern std::vector<std::string> serverData;
extern string tokenName[];

struct tokenClass{
    TokenType* type; 
    int* lineNo; 
    int count;
    int capacity; 
};
extern tokenClass* Token;

struct tableL{
    string tablename; 
    int* columnsze; 
    int tablesize;
    int columns; 
};

struct operate{
    string field;
    string value;
    int sign;
    int cts;
    int offset;
    int result; 
    int nexts;     
    operate* next;
};

struct tokenVar{
    string name;
    string value;
    int line;
    tokenVar* next; 
};

void writeToken(TokenType type, int line, string numToken);
void writeTxt(string file, string Data);
void error(string error, int lineNo);
string getStr(string sql, char end);
bool openFile(char* fileOpen );
int  parse(int pointer);
void initToken();
void freeToken();
#endif
