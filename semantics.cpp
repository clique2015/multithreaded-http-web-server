#include <bits/stdc++.h>
#include "parser.h"
#include "semantics.h"
using namespace std;

int deleteold = 0;
int deleteOnce = 0;
int deleteCnt = 0;
int swCase     = 0;
int funArgc = 0;
int tenary = 0;
string Fname = "";
string track_fname="";
int LvIndex = 0;
int lIndex = 0;





bool expect(TokenType typeA, TokenType typeB, TokenType typeC, TokenType typeD, TokenType typeE, int count){
    if((typeA ==   Token->type[count])&&(typeB ==  Token->type[count+1])
    &&(typeC ==  Token->type[count+2])&&(typeD ==  Token->type[count+3])
    &&(typeE ==  Token->type[count+4])){
            
        return true;
    }
    return false;}

bool expect(TokenType typeA, TokenType typeB, TokenType typeC, TokenType typeD, int count){
    if((typeA ==   Token->type[count])&&(typeB ==  Token->type[count+1])
    &&(typeC ==  Token->type[count+2])&&(typeD ==  Token->type[count+3])){
        return true;
    }
    return false;}
bool expect(TokenType typeA, TokenType typeB, TokenType typeC, int  count){
    if((typeA ==   Token->type[count])&&(typeB ==  Token->type[count+1])
    &&(typeC ==  Token->type[count+2])){
        return true;
    }
    return false;}
bool expect(TokenType typeA, TokenType typeB, int  count){
    if((typeA ==   Token->type[count])&&(typeB ==  Token->type[count+1])){
        return true;
    }
    return false;}
bool expect(TokenType typeA, int  count){
    if(typeA ==   Token->type[count]){
        return true;
    }
    return false;}
    
/**
static void codeGenerator(string write){
    int n = write.length();
    char char_array[n + 1;
    strcpy(char_array, write.c_str());

    FILE *Files ;
    if(deleteold == 0){
        deleteold++;
        if (Files = fopen("compile.txt", "r")) {
            fclose(Files);
            if (remove("compile.txt") == 0){
                
            }
        }
    }
    
    Files = fopen("compile.txt", "a");
	fputs(char_array, Files);
    fputs("\n", Files);
	fclose(Files);

}

static void assignReg(int count){
    
    if(st.indexE > 1){
        int startV = st.funcTable[st.indexE-1->Vs;        
        int endV   = st.indexV; 
        int startA = st.funcTable[st.indexE-1->As;
        int endA   = st.indexA;
        while(startV < endV){
            if(st.localVariable[startV->Vname ==   Token->type[count->identifier)
            {     Token->type[count->regName = saveV[st.localVariable[startV->vNo;
                  Token->type[count->Vtype   = st.localVariable[startV->vType;
                return;
            }startV++;
        }
        while(startA < endA){
            if(st.ArgumentTable[startA->argName ==   Token->type[count->identifier){
                  Token->type[count->regName  =  argV[5+st.ArgumentTable[startA->argNo;
                  Token->type[count->Vtype    =  st.ArgumentTable[startA->argType;
                return;
            }startA++;
        }        
    }
    int gvcount= st.indexG-1;
    while(gvcount >= 0){
        if(st.globalVariable[gvcount->Gname ==   Token->type[count->identifier){
             Token->type[count->regName   =  saveG[gvcount;
             Token->type[count->Vtype     =  st.globalVariable[gvcount->vType;
           return;}
        gvcount-- ;
    }     
    error("Cannot recognize variable on line",   Token->type[count->lineNo);
        
}

static void alu(int count){
    tokenClass* token1 = st.expressList[count;    
    tokenClass* token2 = st.expressList[count+1; 
    
    if((token1]== TOKEN_STRING)||(token1]== TOKEN_STRINGVAR)){
        error("A string variable is not expected in the expression on line", st.expressList[count->lineNo);
    }
    switch(token2->type){
        case TOKEN_ADD_SELF:
            if(token1->Vtype == TOKEN_VAR){
                codeGenerator("addi  "+token1->regName+"  "+token1->regName+"  1");
            }else{error("Invalid expression on line", st.expressList[count->lineNo);}
        break;
        case TOKEN_SUB_SELF:
            if(token1->Vtype == TOKEN_VAR){
                codeGenerator("addi  "+token1->regName+"  "+token1->regName+"  -1");
            }else{error("Invalid expression on line", st.expressList[count->lineNo);}
        break;
        default:
        error("invalid unary expression on line",  st.expressList[count->lineNo);

    }    
}

static void aluB(int count){
    tokenClass* token1 = st.expressList[count;    
    tokenClass* token2 = st.expressList[count+1;    
    tokenClass* token3 = st.expressList[count+2;
    string printfunc;
    string command;
    string storeReg;
    string RS1;
    string RS2;
    
    if((token1]== TOKEN_IDENTIFIER)||(token1]== TOKEN_OUTPUT)){
        RS2 = token1->regName;
    }else
    if(token1]== TOKEN_NUMBER){
        RS2 = token1->identifier;
    }    
    if(token1]== TOKEN_STRING){
        RS2 = "STR"+to_string(scopeDepth);
        scopeDepth++;
    }
    if((token2]== TOKEN_IDENTIFIER)||(token2]== TOKEN_OUTPUT)){
        RS1 = token2->regName;
    }else
    if(token2]== TOKEN_NUMBER){
        codeGenerator("ldi  "+tempV[tempcount + " "+token2->identifier);
        RS1 = tempV[tempcount;
        tempcount++;
    }else
    if(token2]== TOKEN_STRING){
        codeGenerator("ldi  "+tempV[tempcount + "   STR"+to_string(scopeDepth));
        RS1 = tempV[tempcount;
        tempcount++;
    }
    if(token2]!= TOKEN_IDENTIFIER)
    {   storeReg = RS1;
    }else
    if(token1]== TOKEN_OUTPUT){
        storeReg = RS2;
    }else
    {   storeReg = tempV[tempcount;
        tempcount++;
    }  

    if(token3]== TOKEN_EQUAL)
    {   command = branch[token3->type;
        if((token1]== TOKEN_STRING)||(token1]== TOKEN_NUMBER))
        { command = command + "i";   
        }
        codeGenerator(command+"  "+ token2->regName +"  "+RS2);
       
    }else 
    if(token3]<= TOKEN_BANG_EQUAL)
    {   command = branch[token3->type;

    if(scopecount.top().find("*") != string::npos){  
        printfunc = scopecount.top();
        printfunc.erase(0, 5); 
        }else{
            printfunc = scopecount.top();
        }
        codeGenerator(command+"   "+RS1+"   "+RS2+" "+printfunc+"_exit");
    }else
    if(token3]<= TOKEN_ASS_AND)
    {   command = instruction[token3]- TOKEN_TENARY;
        if(token1]== TOKEN_NUMBER)
            { command = command + "i"; }    
        codeGenerator(command+"   "+ token2->regName +"   "+RS1+"   "+RS2);          
    }else
    if(token3]<= TOKEN_STAR)
    {   command = instruction[token3]- TOKEN_TENARY;
        if(token1]== TOKEN_NUMBER)
            { command = command + "i";   }
        codeGenerator(command+"   "+storeReg+"   "+RS1+"   "+RS2);    
        st.expressList[count]   = TOKEN_OUTPUT;
        st.expressList[count->regName = storeReg;            
    }   

    if(token1]== TOKEN_STRING){     
        codeGenerator(".data"); 
        codeGenerator("STR"+to_string(scopeDepth)+":");
        codeGenerator("asciiz'"+token1->identifier+" ' "); 
        codeGenerator(".text");
        scopeDepth++;        
    } 
}

static int checkBang(int start, int end){
    int operand = 0;
    for(int x = end; x < start; x++){ 
        if(st.expressList[x]== TOKEN_BANG)
        {  
            for(int i = x-1; i>= 0; i--)
            {
                if((st.expressList[i]== TOKEN_NUMBER)||(st.expressList[i]== TOKEN_IDENTIFIER)
                ||(st.expressList[i]== TOKEN_FUNCALL))
                {   operand++;
                    if(st.expressList[i->Vtype == TOKEN_BOOL)
                    {   codeGenerator("xor  "+tempV[tempcount+"  "+st.expressList[i->regName+"   xFFFF");
                        st.expressList[i->regName = tempV[tempcount;
                        tempcount++;
                        st.expressList[i]= TOKEN_OUTPUT;                               
                    }
                }else 
                if(st.expressList[i]<= TOKEN_STAR)
                {   operand--;
                    if(st.expressList[i]<= TOKEN_BANG_EQUAL){
                        st.expressList[i]= changeSign[st.expressList[i->type;
                    }   
                }  
                if(operand == 1){
                    break;
                }
            }                 
            for(int n = x; n < st.indexL;  n++){
                st.expressList[n = st.expressList[n+1;}          
            start--; 
            st.indexL--;                                   
        }
    }
    return(start);     
}

static void readExpressList(int start, int end);

static int checkLand(int start, int end){
    int operand = 0;
    int secondC = 0; 
    int cnt     = 0;  
    for(int x = start-1; x>= end; x--)
    {
        if(st.expressList[x]== TOKEN_LOGICAL_AND)
        {
            for(int n = x; n < st.indexL;  n++){
                st.expressList[n = st.expressList[n+1;}          
            start--; 
            st.indexL--;  
            for(int i = x-1; i>= end; i--)
            {
                if(st.expressList[i]> TOKEN_STAR){
                    operand++;
                }else if(st.expressList[i]<= TOKEN_STAR){
                    operand--;
                }
                if(operand == 1){
                    secondC++;
                    operand = 0;
                    if(deleteOnce == 0){
                        deleteOnce = 1;
                        readExpressList(x, i);
                        deleteOnce = 0;
                        cnt = x - i;                         
                        for(int o = i; o < st.indexL - cnt;  o++){
                            st.expressList[o = st.expressList[o + cnt;}                      
                        start -=     cnt;
                        st.indexL -= cnt;  
                    }else
                    {
                    readExpressList(x, i);                       
                    }                      
          
                    x = i;                              
                } 
                if(secondC == 2){
                    break;
                }                                 
            }
        }
    } return(start);               
}

int  checkLor(int start, int end){

    int operand = 0;
    int secondC = 0; 
    int cnt     = 0; 
    string scope = "";
    string printfunc = "";
    for(int x = start-1; x>= end; x--)
    {
        if(st.expressList[x]== TOKEN_LOGICAL_OR)
        {
            for(int n = x; n < st.indexL;  n++){
                st.expressList[n = st.expressList[n+1;}          
            start--; 
            st.indexL--;  
            for(int i = x-1; i>= end; i--)
            {
                if(st.expressList[i]> TOKEN_STAR){
                    operand++;
                }else if(st.expressList[i]<= TOKEN_STAR){
                    operand--;
                }
                if(operand == 1){
                    secondC++;
                    operand = 0;
                    if(secondC == 1){
                        scopecount.push(scopecount.top()+"_"+to_string(scopeDepth)); 
                        scopeDepth++;                       
                    }
                    if(deleteCnt == 0){
                        deleteCnt = 1;
                        readExpressList(x, i);
                        deleteCnt = 0;
                        cnt = x - i;                         
                        for(int o = i; o < st.indexL - cnt;  o++){
                            st.expressList[o = st.expressList[o + cnt;}                      
                        start -=     cnt;
                        st.indexL -= cnt;  
                    }else
                    {
                    readExpressList(x, i);                       
                    } 
                    x = i;                                            
                    if(secondC == 1){
                    printfunc = scopecount.top();     
                    printfunc.erase(0, 5);  
                    codeGenerator("beq   x0    0  "+ printfunc); 
                    codeGenerator( printfunc+"_exit:"); 
                    scopecount.pop(); 
                    }else
                    if(secondC == 2){
                        codeGenerator(printfunc+":");
                        break;
                    }                     
                }                
            }
        }
    }return(start);   
            
}

static void readExpressList(int start, int end){

    start = checkBang(start, end); 
    start = checkLand(start, end);
    start = checkLor(start, end);

    tempcount = 0;
    for(int i = end; i< start-1; i++){
       
       if((st.expressList[i]> TOKEN_SEMICOLON)
       &&((st.expressList[i+1]== TOKEN_ADD_SELF)||(st.expressList[i+1]== TOKEN_SUB_SELF))){        
            alu(i);             
            for(int n = i+1; n < start-1;  n++){
                st.expressList[n = st.expressList[n+1;
            }          
            start--;
            st.indexL--;
            i = end;
        }else 
        if(start > 2){
            if((st.expressList[i]> TOKEN_SEMICOLON)
            &&(st.expressList[i+1]> TOKEN_SEMICOLON)&&(st.expressList[i+2]< TOKEN_BANG)){ 

            aluB(i);
            if(st.expressList[i+2]<= TOKEN_ASS_AND){
                for(int n = i; n < start-3;  n++){
                     st.expressList[n = st.expressList[n+3;
                } 
                start -=3;
                st.indexL -=3;
            }else{
                for(int n = i+1; n < start-2;  n++){
                    st.expressList[n = st.expressList[n+2;} 
                start -=2;
                st.indexL -=2;
                } i = end; 
            } 
        }                  
    }   
    if((start - end == 1)
    &&((st.expressList[end->Vtype == TOKEN_BOOL)
    ||(st.expressList[end->identifier == "0" && st.expressList[end->identifier == "1"))
    ){  string printfunc = scopecount.top();
        printfunc.erase(0, 5);
        codeGenerator("ble   "+st.expressList[end->regName+"   x0   "+printfunc+"_exit");
        start--;
        st.indexL--;
    }
    
    int check = start - end;

    if(check > 1){
        readExpressList(start , end);
    }  
}

int expression(int count, TokenType express){
    st.indexL = 0;
    int braces = 0;
    int countStart = count;  
    while(!(expect(express, count))||(braces != 0)){

      if((  Token->type[count] > TOKEN_OUTPUT)
      ||(  Token->type[count] == TOKEN_SEMICOLON)){
            error("invalid syntax in line",   Token->type[count->lineNo);
        }else    
        if(  Token->type[count] == TOKEN_EOF){
            error("Unexpected end of file on line",   Token->type[count-2->lineNo);
        }else 
        if((expect(TOKEN_LEFT_PAREN, count))){
            braces++;
        }else
        if((expect(TOKEN_RIGHT_PAREN, count))){
            braces--;
        }else
        if((expect(TOKEN_TENARY, count))){
            scopecount.push("*ten_"+to_string(scopeDepth));
            scopeDepth++; 
            tenary = countStart;
            countStart = countStart + 2;                  
            break;
        }else
        if((expect(TOKEN_FUNCALL, count))){         
            c_tokenfuncall(count); 

        }else
        if(
        ((  Token->type[count+1]> TOKEN_END)&&(  Token->type[count]> TOKEN_END))
        ||(  Token->type[count]> TOKEN_INFINITE)
        ){
            error("Invalid token expression in line",   Token->type[count->lineNo);
              
        }        
        if((expect(express, count))&&(braces == 0)){
            break;
        }
      
        count++;
    }
    int countEnd = count; 
     countEnd--;  

    while(countEnd >= countStart){

        if((  Token->type[countEnd]== TOKEN_STRING) &&(!expect(TOKEN_SEMICOLON, countEnd+1))
        &&(!expect(TOKEN_EQUAL, countEnd-1)))
        {
            error("Invalid string expression on line", st.expressList[countEnd->lineNo);
        }

        if(expect(TOKEN_START, countEnd)){ 
            while(!expect(TOKEN_END, countEnd)){
                countEnd--;
            }countEnd--;
            st.expressList[st.indexL =   Token->type[countEnd;
            st.indexL++;
        }else     
        if((expect(TOKEN_STRING, countEnd))
        ||(expect(TOKEN_IDENTIFIER, countEnd))||(expect(TOKEN_NUMBER, countEnd))){
            if((expect(TOKEN_RIGHT_PAREN, countEnd-1))||(expect(TOKEN_LEFT_PAREN, countEnd+1))){ 
                if(!(expect(TOKEN_RIGHT_PAREN, countStart-1))&& !(expect(TOKEN_LEFT_PAREN, count+1))){                 
                    char_stack.push(new tokenClass(TOKEN_STAR, 0, 0));
                }
            }
            if(  Token->type[countEnd]== TOKEN_IDENTIFIER){
            assignReg(countEnd); 
            }
            st.expressList[st.indexL =   Token->type[countEnd;
            st.indexL++;
            
        }else  
        if((expect(TOKEN_FALSE, countEnd))||(expect(TOKEN_TRUE, countEnd))){
            if(expect(TOKEN_FALSE, countEnd)){
                st.expressList[st.indexL = new tokenClass(TOKEN_NUMBER, 0, "0");
                st.indexL++;
            }else{
                st.expressList[st.indexL = new tokenClass(TOKEN_NUMBER, 0, "1");
                st.indexL++;
            }
        }else 
        if(expect(TOKEN_RIGHT_PAREN, countEnd)){
            char_stack.push(  Token->type[countEnd);

        }else 
        if (expect(TOKEN_LEFT_PAREN, countEnd)) {
			while (!(char_stack.top()]== TOKEN_RIGHT_PAREN)) {
                st.expressList[st.indexL = char_stack.top();      
                st.indexL++;
				char_stack.pop();
                if(char_stack.empty())                        
                    error("Missing ')' in expression on line",   Token->type[countEnd->lineNo);                
			}
			char_stack.pop();
		}else                 
        if((  Token->type[countEnd]== TOKEN_PLUS)
        &&(  Token->type[countEnd-1]< TOKEN_BANG)){

        }else
        if((  Token->type[countEnd]== TOKEN_SUB)
        &&(  Token->type[countEnd-1]< TOKEN_BANG)){
                st.expressList[st.indexL = new tokenClass(TOKEN_NUMBER, 0, "0");
                st.indexL++;
                st.expressList[st.indexL = new tokenClass(TOKEN_SUB, 0, 0);
                st.indexL++;                
        }else  
        if (  Token->type[countEnd]< TOKEN_LEFT_PAREN){ 
            if(!char_stack.empty()){
                while ((  Token->type[countEnd]<= char_stack.top()->type)
                &&(char_stack.top()]!= TOKEN_RIGHT_PAREN))
                {
                    st.expressList[st.indexL = char_stack.top();
                    st.indexL++;
                    char_stack.pop();
                    if(char_stack.empty())                        
                        break;
                }
                char_stack.push(  Token->type[countEnd);
                
            }else{
                char_stack.push(  Token->type[countEnd);
                }
		}              
        countEnd--;
    }
   
    while(!char_stack.empty()){
        if(char_stack.top()]!= TOKEN_RIGHT_PAREN){ 
        st.expressList[st.indexL = char_stack.top();
        st.indexL++;
        }
		char_stack.pop();
	} 
   
    if(funArgc > 0){ 
        funArgc--;
        st.expressList[st.indexL  = new tokenClass(TOKEN_IDENTIFIER,  0, "0");
        st.expressList[st.indexL->regName = argV[funArgc;
        st.indexL++;
        st.expressList[st.indexL  = new tokenClass(TOKEN_EQUAL, 0);
        st.indexL++;
         
    }          
    readExpressList(st.indexL, 0);
    count++;  
    return(count);
}

int c_rightBrace(int count){
    count++; 
    if(scopecount.top().find("*fun_") != string::npos){ 
        string printfunc = scopecount.top();
        printfunc.erase(0, 5);
        codeGenerator(printfunc+"_exit:");       
        scopecount.pop();   
        int NoLocalVariables = st.funcTable[st.indexE -1->Vn;
        int vCnt = ( NoLocalVariables * 16 ) +  32;
        int cnt =0;
        while(vCnt > 32)
        {
            vCnt -= 16;
            codeGenerator("ld    "+saveV[cnt+",  "+to_string(vCnt)+"(sp)");  
            cnt++; 
        }
        codeGenerator("ld    x8,  16(sp)"); 
        codeGenerator("ld    x1,  0(sp)"); 
        codeGenerator("addi  sp   sp   "+to_string(NoLocalVariables)); 
        codeGenerator("jalr  x0,   0(x1)");     
    }else
    if(scopecount.top().find("*whi") != string::npos){  
        string printfunc = scopecount.top();
        printfunc.erase(0, 5);                 
        codeGenerator("beq  x0   x0,  "+printfunc);  
        codeGenerator(printfunc+"_exit");
        scopecount.pop();
    }else          
    if(scopecount.top().find("*for") != string::npos){
          Token->type[forcount-1 = new tokenClass(TOKEN_LEFT_PAREN, 0, 0);
        expression(forcount-1, TOKEN_LEFT_BRACE);
        string printfunc = scopecount.top();
        printfunc.erase(0, 5); 
        codeGenerator("beq x0   x0,  "+printfunc);                                  
        codeGenerator(printfunc+"_exit:");
        scopecount.pop();
    }else
    if((scopecount.top().find("*ifs") != string::npos)&& (expect(TOKEN_ELSE, count))){
        string printfunc = scopecount.top();
        printfunc.erase(0, 5);     
        scopecount.pop();

        scopecount.push("*ifs_"+to_string(scopeDepth));
        scopeDepth++;
        string printEL = scopecount.top();
        printEL.erase(0, 5); 
        codeGenerator("beq  x0  x0,  "+printEL+"_exit"); 
        codeGenerator(printfunc+"_exit:");            
        return(count);         
    }else    
    if((scopecount.top().find("*ifs") != string::npos)&& !(expect(TOKEN_ELSE, count))){
        string printfunc = scopecount.top();
        printfunc.erase(0, 5);          
        codeGenerator(printfunc+"_exit:"); 
        scopecount.pop(); 
    }else
    if(scopecount.top().find("*elf") != string::npos){
        string printfunc = scopecount.top();
        printfunc.erase(0, 5);     
        scopecount.pop();

        string printEL = scopecount.top();
        printEL.erase(0, 5); 
        codeGenerator("beq  x0  x0,  "+printEL+"_exit"); 
        codeGenerator(printfunc+"_exit:"); 
        if(!(expect(TOKEN_ELSE, count))){
            printfunc = scopecount.top();
            printfunc.erase(0, 5);   
            codeGenerator(printfunc+"_exit:");               
            scopecount.pop();   
        }return(count);       
    }else      
    if(scopecount.top().find("*swi") != string::npos){ 
        string printfunc = scopecount.top();
        printfunc.erase(0, 5);          
        codeGenerator(printfunc+"_exit:"); 
        scopecount.pop();                        
    }   
    return count;
}

int c_tokenfun(int count){
    int funcIndex = st.indexE -1; 
    count++;
    while(funcIndex > 0){
        if((  Token->type[count->identifier == st.funcTable[funcIndex->Fname)
        &&(st.funcTable[funcIndex->call == 1)){
            error("function already exists",   Token->type[count->lineNo);          
        }
        funcIndex--;   
    }
    Fname =   Token->type[count->identifier;
    st.funcTable[st.indexE  = new funcList(0, Fname, 0, 0, 0, 0);
    st.indexE++; 
    st.funcTable[st.indexE-1->As = st.indexA;
    count +=2;
    int  count = count;
    int argNo = 0;
    if(  Token->type[count]!= TOKEN_RIGHT_PAREN){
        argNo = 1;
        while(!expect(TOKEN_RIGHT_PAREN, count)){
            if(  Token->type[count]== TOKEN_COMMA){
                argNo++;
            }
            count++;
        }
    }  
    st.funcTable[st.indexE-1->An = argNo; 
    if(argNo == 0){
        count++;
    } 
    while(argNo != 0){
        st.ArgumentTable[st.indexA  = new ArgumentList(  Token->type[count->type,
           Token->type[count+1->identifier, argNo-1);  
        count +=3;
        st.indexA++;
        argNo--;
    } 
    if(expect(TOKEN_LEFT_BRACE, count)){
        st.funcTable[st.indexE-1->call = 1;
        scopecount.push("*fun_"+Fname);

        codeGenerator(Fname+":"); 
        int braces = 1;
        int variable = 0;
        count++;
        count =  count;
        while(braces != 0){
            if(  Token->type[count]== TOKEN_LEFT_BRACE){
                braces++;
            }else 
            if(  Token->type[count]== TOKEN_RIGHT_BRACE){
                braces--;
            }else 
            if(expect(TOKEN_VAR, TOKEN_IDENTIFIER, count) 
            || expect(TOKEN_STRINGVAR, TOKEN_IDENTIFIER, count) 
            || expect(TOKEN_BOOL, TOKEN_IDENTIFIER, count)){
                variable++;   
            }else
            if(  Token->type[count]== TOKEN_EOF){
                error("Function  "+Fname+"  have incompatible number of braces", 0);
            }
            count++;
        }
        braces = variable;
        variable = (variable * 16) + 32; 

        count = 0;
        codeGenerator("addi  sp,  sp,  -"+to_string(variable));

        while(variable > 32){
            variable -= 16;
            codeGenerator("sd    "+saveV[count+",  "+to_string(variable)+"(sp)"); 
            count++; 
        }
        codeGenerator("sd    x8,  16(sp)");
        codeGenerator("sd    x1,  0(sp)");
        codeGenerator("addi  fp   sp   "+to_string(braces));

        st.funcTable[st.indexE-1->Vn = braces;                
    }else
    if(expect(TOKEN_SEMICOLON, count)){
        st.funcTable[st.indexE-1->call = 0;
        count++;
    }else{
        error("MISSING ; or { in function "+Fname+ " on line",   Token->type[count->lineNo);
        }
    return count;            
}

int c_varDeclaration(int count){
    if(track_fname != Fname){
        track_fname = Fname;
        LvIndex = 0; 
        lIndex = st.indexV;
        st.funcTable[st.indexE -1->Vs = lIndex;
    }
    int Lsearch = st.indexV;
    if(!scopecount.empty()){
    while(Lsearch > lIndex){
        if(  Token->type[count + 1->identifier == st.localVariable[Lsearch-1->Vname ){
            error("variable already declared on line",   Token->type[count->lineNo);}
            Lsearch--;
    }
        st.localVariable[st.indexV  = new lVList(  Token->type[count->type,
           Token->type[count+1->identifier, LvIndex);
        LvIndex++;
        st.indexV++;   
        
    }else
    if(scopecount.empty()){
        int Lsearch = st.indexG;        
        while(Lsearch > 0){
            if(  Token->type[count+1->identifier == st.globalVariable[Lsearch-1->Gname ){
                error("variable already declared on line",   Token->type[count->lineNo);}
            Lsearch--;
        }      
        st.globalVariable[st.indexG  = new gVList(  Token->type[count->type,
           Token->type[count+1->identifier);
        st.indexG++;   
    }count++;  
      
    if(expect(TOKEN_SEMICOLON, count+1)){ 
    count = count + 2;
    }else
    if(expect(TOKEN_COMMA, count+1)){ 
          Token->type[count+1 =   Token->type[count-1;
        count = c_varDeclaration(count+1);
    }else
    {
        count = expression(count, TOKEN_SEMICOLON);
    } return count;
    
}

int c_while(int count){
    int paren = 0;
    count++; 
    int  count = count;
    while(!expect(TOKEN_LEFT_BRACE, count)){
        if(expect(TOKEN_LEFT_PAREN, count)){
            paren++;
        }else
        if(expect(TOKEN_RIGHT_PAREN, count)){
            paren--;
        }
        if(expect(TOKEN_EOF, count)){
            error("Unexpected end of file on line ",   Token->type[count->lineNo );   
        }
        if((paren == 0)&&(!expect(TOKEN_LEFT_BRACE, count+1))){
            error("Missing opening '{' in while loop on line",   Token->type[count->lineNo );       
        } 
        if((paren == 0)&&(expect(TOKEN_LEFT_BRACE, count+1))){
            break;         
        }                 
    count++;
    }    
    codeGenerator("while_"+to_string(scopeDepth)+":");
    scopecount.push("*whi_"+to_string(scopeDepth));
    scopeDepth++; 
    count = expression(count, TOKEN_LEFT_BRACE);
    return count;
}

int c_forloop(int count){
    forcount = 0;
    int paren = 1;
    count = count + 2;

    if(expect(TOKEN_VAR, TOKEN_IDENTIFIER, count)){
        count = c_varDeclaration(count);
    }else
    if(expect(TOKEN_IDENTIFIER, count)){                    
        count = expression(count, TOKEN_SEMICOLON);
    }else{
        error("Syntax error in for loop in line",   Token->type[count->lineNo);
        }
    scopecount.push("*for_"+to_string(scopeDepth));
        
    codeGenerator("for_"+to_string(scopeDepth)+":");            
    scopeDepth++;
    count = expression(count, TOKEN_SEMICOLON); 
    forcount = count;     
    while(!expect(TOKEN_LEFT_BRACE, count)){
        if(expect(TOKEN_LEFT_PAREN, count)){
            paren++;
        }else
        if(expect(TOKEN_RIGHT_PAREN, count)){
            paren--;
        }
        if((paren == 0) && !(expect(TOKEN_LEFT_BRACE, count+1))){
            error("Missing opening '{' in for loop on line",   Token->type[count->lineNo );
        }
        if((paren == 0)&&(expect(TOKEN_LEFT_BRACE, count+1))){
            break;         
        }         
        count++;
    }count += 2;     
    return count;
}

int c_if(int count){
    int paren = 0;
    count++;     
    int  count = count;
    while(!expect(TOKEN_LEFT_BRACE, count)){
        if(expect(TOKEN_LEFT_PAREN, count)){
            paren++;
        }else
        if(expect(TOKEN_RIGHT_PAREN, count)){
            paren--;          
        }
        if((paren == 0)&&(!expect(TOKEN_LEFT_BRACE, count+1))){        
            error("Missing opening '{' in if statement on line",   Token->type[count->lineNo );          
        } 
        if((paren == 0)&&(expect(TOKEN_LEFT_BRACE, count+1))){
            break;         
        }count++;
    }
    scopecount.push("*ifs_"+to_string(scopeDepth));
    scopeDepth++;                    
    count = expression(count, TOKEN_LEFT_BRACE);
    return(count); 
}

int c_else(int count){
    scopecount.push("*elf_"+to_string(scopeDepth));
    scopeDepth++;
    int paren = 0;
    count+=2;     
    int  count = count;
    while(!expect(TOKEN_LEFT_BRACE, count)){
        if(expect(TOKEN_LEFT_PAREN, count)){
            paren++;
        }else
        if(expect(TOKEN_RIGHT_PAREN, count)){
            paren--;          
        }
        if((paren == 0)&&(!expect(TOKEN_LEFT_BRACE, count+1))){        
            error("Missing opening '{' in if statement on line",   Token->type[count->lineNo );          
        } 
        if((paren == 0)&&(expect(TOKEN_LEFT_BRACE, count+1))){
            break;         
        }count++;
    }                 
    count = expression(count, TOKEN_LEFT_BRACE);
    return(count); 
}

int c_switch(int count){
    count = count + 2;
    swCase = count;
    count++;
    scopecount.push("*swi_"+to_string(scopeDepth));
    scopeDepth++;
    if(! expect(TOKEN_RIGHT_PAREN, TOKEN_LEFT_BRACE, TOKEN_CASE, count)){ 
        error("Switch expects '){' on line ",   Token->type[count->lineNo);}
    count = count + 2;
    return(count);    
}

int c_case(int count){ 
    scopecount.push("*cas_"+to_string(scopeDepth));
    scopeDepth++;
      Token->type[count-1 =   Token->type[swCase; 
      Token->type[count  = new tokenClass(TOKEN_EQUAL_EQUAL, 0);     
    count = expression(count-1, TOKEN_COLON);
    if(expect(TOKEN_LEFT_BRACE, count)){
        error("Case statement does not need a '{' on line ",   Token->type[count->lineNo);
    } 
    return(count);
}

int c_swbreak(int count){
    string printfunc = scopecount.top();
    printfunc.erase(0, 5);     
    if(scopecount.top().find("*swi") != string::npos){ 
    error("Unexpected break statement, this maybe due to adding"
     "a break statement after the default case",   Token->type[count->lineNo);
        }   
    scopecount.pop();

    string printSw = scopecount.top();
    printSw.erase(0, 5); 

    codeGenerator("beq  x0  x0,  "+printSw+"_exit"); 
    codeGenerator(printfunc+"_exit"); 

    count = count + 2;
    return(count);
}

int c_default(int count){
    count = count +2 ;
    if(expect(TOKEN_LEFT_BRACE, count)){
        error("default statement does not need a '{' on line ",   Token->type[count->lineNo);
    }
    return(count);
}

int c_tokenfuncall( int count){
    int cnt   = 0;
    int brace = 0;
    int funcIndex = st.indexE -1;
    int argument = 0;
    int storeC = 0;
    while(funcIndex > 0){
        if((  Token->type[count->identifier == st.funcTable[funcIndex->Fname)){
            cnt++;
            argument = st.funcTable[funcIndex->An;       
        }
        funcIndex--;   
    }
    if(cnt == 0){
        error("The function "+  Token->type[count->identifier+
        " has not been declared. Error on line",   Token->type[count->lineNo);}
    
    if((tempcount) > 0){
        brace = tempcount *  16;
        cnt   = 0;
        codeGenerator("addi  sp, sp,  -"+brace);
        brace -= 16;
        while(brace >=  0){
            codeGenerator("sd  "+tempV[cnt+"  "+to_string(brace)+"(sp)");
            cnt++;          
            brace -= 16;
        }
    } 
    count++;    
    storeC = count;
    count++;
    cnt   = 0;
    funArgc = argument;
    while(argument > cnt){
       
        if(argument - cnt  == 1){
            count = expression(count, TOKEN_RIGHT_PAREN);
            count--;
        }else 
        if(argument - cnt  > 1){          
            count = expression(count, TOKEN_COMMA);
        }
        cnt++;
    } 
    codeGenerator("jal  XI "+  Token->type[storeC-1->identifier);   
      Token->type[storeC-1   = new tokenClass(TOKEN_IDENTIFIER,  0, "");  
      Token->type[storeC-1->regName = argV[0;              
      Token->type[storeC     = new tokenClass(TOKEN_END,  0);
      Token->type[count  = new tokenClass(TOKEN_START,  0);      
    return(count);    
}

int c_tenary(int count){

    string printfunc = scopecount.top();
    printfunc.erase(0, 5);  

      Token->type[count-2     =   Token->type[tenary;
      Token->type[count-1     =   Token->type[tenary+1;         
    count = expression(count-2, TOKEN_COLON);
    codeGenerator("beq x0    x0  "+printfunc+"f_exit");
    codeGenerator(printfunc+"_exit");

      Token->type[count-2     =   Token->type[tenary;
      Token->type[count-1     =   Token->type[tenary+1;    
    count = expression(count-2, TOKEN_SEMICOLON);           
    codeGenerator(printfunc+"f_exit:");

    scopecount.pop();
    return(count); 
}

int c_cout(int count){
    int  count  = count;
    int argCnt = 0;
    while(!expect(TOKEN_SEMICOLON, count)){
        if(  Token->type[count]== TOKEN_SHIFT_LEFT){
            argCnt++;
        }
        if(  Token->type[count]== TOKEN_EOF){
            error("Unexpected end of file due to print error on line",   Token->type[count-2->lineNo);
        }        
        count++;       
    }
    if(argCnt == 0){
        error("Invalid print statement on line",   Token->type[count-2->lineNo);
    }     
    count+=2;
    funArgc = argCnt;        
    while(argCnt != 0){
        if(argCnt == 1){
            count = expression(count, TOKEN_SEMICOLON);
        }else
        if(argCnt > 1){
            count = expression(count, TOKEN_SHIFT_LEFT);     
        }
        argCnt--;
    }
    codeGenerator("jal cout");
    return count;
}

int c_return(int count){
    count++;   
    if(!(expect(TOKEN_SEMICOLON, count))){
        funArgc = 1;
        count = expression(count, TOKEN_SEMICOLON);  
        count--;
    }
    count++;          
    codeGenerator("j  "+Fname+"_exit"); 
    return(count);  
}

int c_break(int count){
    while((scopecount.top().find("*for_") == string::npos)
    &&(scopecount.top().find("*whi_") == string::npos)
    &&(!scopecount.empty())){
        scopecount.pop();
    }
    if(((scopecount.top().find("*for_") != string::npos)
    ||(scopecount.top().find("*whi_") != string::npos))
    &&(!scopecount.empty())){
        string printfunc = scopecount.top();
        printfunc.erase(0, 5); 
        codeGenerator("j  "+printfunc+"_exit:");        
        scopecount.pop();
    }else
    {
        error("breaks are only used within a loop ",   Token->type[count->lineNo);
    }
    count += 2;  
    return count;    
}

int c_continue(int count){
    while((scopecount.top().find("*for_") == string::npos)
    &&(scopecount.top().find("*whi_") == string::npos)
    &&(!scopecount.empty())){
        scopecount.pop();
    }
    if(((scopecount.top().find("*for_") != string::npos)
    ||(scopecount.top().find("*whi_") != string::npos))
    &&(!scopecount.empty())){
        string printfunc = scopecount.top();
        printfunc.erase(0, 5); 
        codeGenerator("j  "+printfunc+":");        
        scopecount.pop();
    }else
    {
        error("breaks are only used within a loop ",   Token->type[count->lineNo);
    }
    count += 2;  
    return count;    
}

int c_goto(int count){
    count += 2;     
    codeGenerator("j  "+  Token->type[count->identifier); 
    if(expect(TOKEN_SEMICOLON, count+1)){
        count += 2; 
    }else{
        error("invalid goto keyword call on line ",   Token->type[count->lineNo);
    } 

    return count; 
}

int c_label(int count){
    codeGenerator(  Token->type[count->identifier+":");  
    count += 2;  
    return count; 
}
void sendToparser(TokenType type, int line, std::string numToken){
      Token->type[st.index  = new tokenClass(type,  line, numToken);
    st.index++;
}
void sendToparser(TokenType type, int line){
      Token->type[st.index  = new tokenClass(type,  line);
    st.index++;
    if(type == TOKEN_EOF){
        codeGenerator(".text");
        codeGenerator(".extern printf");
        codeGenerator(".globl main");
        parse(1);
        
    }
} **/