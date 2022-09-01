#include <bits/stdc++.h>
#include "parser.h"
using namespace std;
char* token;
int line = 1;
int lineNo = 1;
int parenCount = 0;
int braceCount = 0;
int deleteOld = 0;

string tokenName[] = {
    "TOKEN_NOT_EQUAL",     
    "TOKEN_GR_EQUAL", 
    "TOKEN_LESS_EQUAL",       
    "TOKEN_GREATER",
    "TOKEN_LESS",    
    "TOKEN_EQ_EQUAL",
    "TOKEN_EQUAL", 
    "TOKEN_BANG",
    "TOKEN_SETC", "TOKEN_HEADER", "TOKEN_ELSE",
    // Single-character "TOKENs.
    "TOKEN_LEFT_PAREN", "TOKEN_RIGHT_PAREN",
    "TOKEN_LEFT_BRACE", "TOKEN_RIGHT_BRACE",
    "TOKEN_LEFT_BLOCK", "TOKEN_RIGHT_BLOCK",
    "TOKEN_COMMA", "TOKEN_DOT", "TOKEN_SEMICOLON",
    "TOKEN_START", "TOKEN_END",
    // One or two character "TOKENs.
    // Literals.
    "TOKEN_BOOL", "TOKEN_VAR", "TOKEN_STRINGVAR", 
    "TOKEN_IDENTIFIER", "TOKEN_STRING", "TOKEN_NUMBER",
    "TOKEN_PIN","TOKEN_FALSE", "TOKEN_TRUE", "TOKEN_FUNCALL",
    "TOKEN_OUTPUT","TOKEN_ENDL",
    // Keywords.
    "TOKEN_INFINITE",   "TOKEN_FUN", "TOKEN_IF",
    "TOKEN_VOID", "TOKEN_ECHO",

    "TOKEN_BREAK", "TOKEN_WHILE", "TOKEN_POUND","M_QUERY", "M_CONNECT", "TOKEN_EOF"
};

void writeFile(string Data){
    int n = Data.length();
    char char_array[n + 1];
    strcpy(char_array, Data.c_str());

    FILE *Files ;
    Files = fopen("tokens.txt", "a");
	fputs(char_array, Files);
    fputs("\n", Files);
	fclose(Files);
}
void makeToken(TokenType type){
    string code =   "("+to_string(lineNo)+").  " + tokenName[type] +"--"+ to_string(line); 
    lineNo++;  
    writeFile(code); 
    writeToken(type, line, "0"); 
}
void makeToken(string tokenData, TokenType type){
    if(tokenData==""){tokenData = " ";}
    string code = "("+to_string(lineNo)+").  " +tokenName[type] +"--"+ to_string(line) +"--"+ tokenData;
    lineNo++; 
    writeFile(code); 
    writeToken(type, line,  tokenData);       
}
static bool end_of_file(){
    if(token[0] == '\0'){
        return true;
    };    
    return false;
}
static void skipToken(char A){

    while((!end_of_file())&& !(token[0] == A)){
        token++;
    }
    if(end_of_file()){
        error("Unexpected end of file on line", line);
    }
}
static void skipToken(char A, char B){
    while((!end_of_file())&& !((token[0]== A)&&(token[1] == B))){
        token++;
    }token++;  
        if(end_of_file()){            
        error("Unexpected end of file on line", line);
    }else{token = token + 2;
        }
    
}
static void skip_whitespace(){

    switch(*token){
        case ' ':
            token++;
            skip_whitespace();
            break;
        case '\r':
            token++;
            skip_whitespace();
            break;
        case '\t':
            token++;
            skip_whitespace();
            break;            
        case '\n':
            token++;
            line++;
            skip_whitespace();
            break;
        case '/':
            if(token[1] == '/'){
                token++;
                skipToken('\n');
                line++;
            }else if(token[1] == '*'){
                token++;
                skipToken('*', '/');
                line++;
            }else{error("invalid tokens on line ", line);}
            skip_whitespace();
            break;
        default:
            break;
    }
}
static bool isNum(char B){
    int A = B - '0';
    if(A >= 0 && A <= 9 ){
        return true;
    }else{ return false;}
}
static bool isAlphabet(char A){
    if((A >= 'a' && A <= 'z') || (A >= 'A' && A <= 'Z') || A == '_'){
        return true;
    }else{ return false;}
};
static void generateToken(TokenType type){
    string tokenData = "";
    if(type == TOKEN_NUMBER){
        while((!end_of_file())&&(isNum(*token))){
            tokenData = tokenData + token[0]; 
            token++;
        }
        token--;
        if(end_of_file()){
            error("Unexpected end of file on line ", line);
        } else {
            makeToken(tokenData, TOKEN_NUMBER);
        }           
    }else
    if(type == TOKEN_STRING){      
        while((!end_of_file())&&(*token != '"')){
            tokenData = tokenData + token[0]; 
            token++;
        }
        if(token[0] == '"'){
            makeToken(tokenData, TOKEN_STRING);
           // token++;
        }
        else{
            error("expecting double quotes on line", line);
        }
    }
    else 
    if(type == TOKEN_IDENTIFIER){  token++;    
      while((!end_of_file())&&(isNum(*token)||isAlphabet(*token)||*token == '_'))
      {     tokenData = tokenData + token[0]; 
            token++; 
        }token--;          
        if(end_of_file()){
            error("Unterminated identifier on line", line);

        }else {
            makeToken(tokenData, TOKEN_IDENTIFIER); 
        }                                           
    }  
}
static bool checkKeyword(string charValue, int numChar, TokenType type){
    if(!(isAlphabet(token[numChar+1])) && !(isNum(token[numChar+1]))){

        for(int i = numChar; i > 0; i--){ 
                    
            if(charValue[i-1] != token[i]){

                return false;
            }
        }
        makeToken(type); 
        return true;
    }return false;
}
static void keywordToken(){
    switch(*token){      
        case 'e':
            if(checkKeyword("cho", 3, TOKEN_ECHO)){token = token+3;}
            else if(checkKeyword("lse", 3, TOKEN_ELSE)){token = token+3;}   
            break;    
        case 'i':
            if(checkKeyword("f", 1, TOKEN_IF)){token++;}  
            break;  
        case 'h':
            if(checkKeyword("eader", 5, TOKEN_HEADER)){token +=5;}  
            break;                                        
        case 'm':
            if(checkKeyword("ysqli_connect", 13, M_CONNECT)){token = token+13;}
            else if(checkKeyword("ysqli_query", 11, M_QUERY)){token = token+11;}           
            break;  
        case 's':
            if(checkKeyword("etcookie", 8, TOKEN_SETC)){token += 8 ;}
            break;                      
        case 'w':
            if(checkKeyword("hile", 4, TOKEN_WHILE)){token = token+4;}
            break;          
    }
}
static void checkField(int type){
    switch(type){
        case 1:
        if(parenCount == 0){error("Unexpected ')' on line ", line);}
        break;
        case 2:
        if(parenCount != 0){error("Unexpected '{' on line ", line);}
        break;        
        case 3:
        if(braceCount == 0){error("Unexpected '}' on line ", line);}
        if(parenCount != 0){error("Unexpected '}' on line ", line);} 
        break; 
    }
}
static void charToken(){
    if(end_of_file()){
        return;}
    switch(*token){
        case '(':
        checkField(0);
        parenCount++;
        makeToken(TOKEN_LEFT_PAREN);
        break;
        case ')':
        checkField(1);
        parenCount--;
        makeToken(TOKEN_RIGHT_PAREN);
        break;
        case '{':
        checkField(2);
        braceCount++;
        makeToken(TOKEN_LEFT_BRACE);
        break;
        case '>':  
           if(token[1] == '='){
            makeToken(TOKEN_GR_EQUAL);
            token++; }else{makeToken(TOKEN_GREATER);}
            break;
        case '<':     
           if(token[1] == '='){
            makeToken(TOKEN_LESS_EQUAL);
            token++; }else{makeToken(TOKEN_LESS);}
            break;
        case '=':
           if(token[1] == '='){
            makeToken(TOKEN_EQ_EQUAL);
            token++; }else{makeToken(TOKEN_EQUAL);}
            break;
        case '!':     
           if(token[1] == '='){
            makeToken(TOKEN_NOT_EQUAL);
            token++; }else{makeToken(TOKEN_BANG);}
            break;
        case '}':
        checkField(3);
        makeToken(TOKEN_RIGHT_BRACE);
        braceCount--;
        break;
        case '[':
        makeToken(TOKEN_LEFT_BLOCK);
        generateToken(TOKEN_IDENTIFIER);        
        break;
        case ']':
        makeToken(TOKEN_RIGHT_BLOCK);
        break;

        case ';':     
        makeToken(TOKEN_SEMICOLON);
        break;
        case ',':
        makeToken(TOKEN_COMMA);
        break;        
        case '$':
        generateToken(TOKEN_IDENTIFIER);
        break;
        case '.':
        makeToken(TOKEN_DOT);
        break;
        case '"':
        token++;
        generateToken(TOKEN_STRING);
        break;         
        default:
        skip_whitespace();                   
    }
}
void writePhp(string file, string Data){
    int n = Data.length();
    char char_array[n + 1];
    strcpy(char_array, Data.c_str());

    FILE *Files ;
    Files = fopen(file.c_str(), "a");
	fputs(char_array, Files);   
	fclose(Files);
}
void scanner(char* buffer){
    string newf = "./public/new.php";
    FILE* fpc = fopen(newf.c_str(),"w");   
    fclose(fpc);   

    newf = "tokens.txt";
    fpc = fopen(newf.c_str(),"w");   
    fclose(fpc);  

    bool scan = false;
    token = buffer;
    int coun = 0;
    initToken();
    while(!end_of_file()){      
        if(scan == true){

            skip_whitespace(); 
            if(token[0] =='?' && token[1] =='>' ){         
                token++;
                scan = false;
                makeToken(TOKEN_EOF);
                coun = parse(coun);
            }else                  
            if(isNum(*token)){         
                generateToken(TOKEN_NUMBER);
            }else 
            if(isAlphabet(*token)){
                keywordToken();
            }else{
                charToken();
            }

        }else
        if(token[0] =='<' && token[1] =='?' && token[2] =='p' && token[3] =='h' && token[4] =='p' ){ 
            token += 4;
            scan = true;
            
            
        }else
        {   string tmp_string(1, token[0]);
            writePhp("./public/new.php", tmp_string);

        }

        token++;
    }freeToken();
}


bool openFile(char* fileOpen )
{

	FILE *filePointer ;
	filePointer = fopen(fileOpen, "r") ;

	if ( filePointer == NULL )
	{
		printf("Failed to open file. %s\n",fileOpen);
        return false;
	}
	else
	{
        fseek(filePointer, 0, SEEK_END);	
        int fileSize = ftell(filePointer);
        fseek(filePointer, 0, SEEK_SET);
        char* buffer = NULL;
        buffer = new(nothrow) char[fileSize+1];
        if (!buffer){
            cout << "allocation of memory failed\n";
        }else
        {

            fread(buffer, sizeof(char), fileSize, filePointer);
            scanner(buffer);

        }
        delete buffer;
        return true;
	}
    
	fclose(filePointer);
	return false;	

}
