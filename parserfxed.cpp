#include <bits/stdc++.h>
#ifndef __PARSER_H__
#include "parser.h"
#define __PARSER_H__
#endif
#include "semantics.h"
using namespace std;
#include <string>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include  <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <vector>
int delet = 0;
int deletv = 0;
int oelet = 0;
int oeletv = 0;
int order = 0;
char offset;
int connect = 0;
int counter = 0;
string dbname;
string dbtxt;
string myText;
struct stat sb;
int sel_count = 0;
std::vector<std::string> tokenData;
std::vector<std::string> tokenSt;

tokenClass* Token;
tableL* tab;
tokenVar *tokenStore=NULL;
operate *tokenStores=NULL;
operate *tokenStorese=NULL;



void* reallocate(void* pointer, size_t oldSize, size_t newSize) {
  if (newSize == 0) {
    free(pointer);
    return NULL;
  }
  void* result = realloc(pointer, newSize);
  if (result == NULL) exit(1);
  return result;
}

void initToken() {
    Token = new tokenClass();
    Token->type= new TokenType(); 
    Token->lineNo= new int(); 
    Token->count = 0;
    Token->capacity = 0;  
}

void writeToken(TokenType type, int line, string data) {
    if (Token->capacity < Token->count + 1) 
    {
        int oldCapacity = Token->capacity;
        Token->capacity = Token->capacity + 24;
        Token->lineNo = GROW_ARRAY(int, Token->lineNo, oldCapacity, Token->capacity);
        Token->type = GROW_ARRAY(TokenType, Token->type, oldCapacity, Token->capacity);
    }   
    Token->type[Token->count] = type;   
    Token->lineNo[Token->count] = line; 
    tokenData.push_back(data); 
    Token->count++;  
   
}
void printToken() {
    cout<<"tablename : "<<tab->tablename<<endl; 
    cout<<"tablesize : "<<tab->tablesize<<endl;  
    cout<<"columns : "<<tab->columns<<endl; 
    int counter = 1;
    while(counter <= tab->columns){
        cout<<"fldsze : "<<tab->columnsze[counter];
        counter ++;
    }
}
void initTable(int column){
  tab = new tableL(); 
  tab->tablename = ""; 
  tab->tablesize = 0; 
  tab->columns = 0;  
  tab->columnsze = new int();  
}
void freeToken() {
  FREE_ARRAY(TokenType, Token->type, Token->capacity);
  FREE_ARRAY(int, Token->lineNo, Token->capacity);  
  FREE_ARRAY(TokenType, Token->type, Token->capacity);
}
void error(string error, int lineNo){
    cout<<error<<" "<<lineNo<<endl;
    exit(1);
}

string remove_space(string sql){
    while(sql[0] == ' '){
        sql.erase(0,1);
    }
    return sql;
}

string remove_spaces(string sql){
    counter = 0;
    while(counter <= sql.length()){
        if(sql[counter] == ' ' || sql[counter] == '\n'){        
            sql.erase(counter,1);
        }counter++;
    }
    return sql;
}

string getStr(string sql, char end){
    counter = 0;
    while(sql[counter] != end){
        counter++;
        if(counter == sql.length()){
            break;
        }
    }
    sql.erase(counter, sql.length());
    return(remove_space(sql));
}

string findData(string varname){
    tokenVar *search;
    if(tokenStore == NULL)
    {
        error("variable2 not declared", 0);
    }
    search = tokenStore;
    while(search !=NULL)
    { 
        if(search->name == varname){
            return search->value;
            break;
        }search = search->next;
    }
    error("variable1 not declared", 0);
    return ("0");          
}

string findrow(string varname, string column){
    string flesData;
    string dbname1 = varname.substr(0, varname.find("`"));
    varname.erase(0, varname.find("`"));
    string tablename1 = varname.substr(0, varname.find("`"));
    varname.erase(0, varname.find("`")); 
    string tbloc =  "/home/projectfpga/Downloads/webserver/compiler/"+dbname1+"/"+dbname1+".txt";
    ifstream MyRead1(tbloc);

    while (getline (MyRead1, flesData)) 
    {
        if(flesData.substr(0,tablename1.length())== tablename1){break;}
    }
    MyRead1.close(); 
    counter = 1;
    column.append(11 - column.length(), ' '); 
    column = column.substr(0, 11);
    int ee = 0;    
    int sze = 0;
    while(counter <= stoi(flesData.substr(13,2))){
        if(flesData.substr(counter*13+2,11) == column){
            sze = stoi(flesData.substr(counter*13+13,2));
            break;
        }
        ee += stoi(flesData.substr(counter*13+13,2));       
        counter ++;
    }  
    return(varname);        
}

void insertVar(string varname, string value, int line) {
    
    tokenVar *ptr; 
    tokenVar  *temp = NULL; 
    temp =new tokenVar();
    if(temp == NULL)
    {
        error("nOut of Memory Space:n", 0);
        exit(0);
    }
    temp->name = varname;
    temp->value = value;
    temp->line  = line;
    temp->next =NULL;
    if(tokenStore==NULL)
    {   
        tokenStore=temp;
    }
    else
    {
        ptr=tokenStore;
        while(ptr->next !=NULL)
        {
            if(ptr->name == temp->name){
                ptr->value = temp->value;
                ptr->line  = temp->line;
                break;
            }
            ptr=ptr->next;
        }
        if(ptr->name == temp->name){
            ptr->value = temp->value;
            ptr->line  = temp->line;
        }else{
        ptr->next=temp;
        }            
    }
}

void writeTxt(string file, string Data){
    int n = Data.length();
    char char_array[n + 1];
    strcpy(char_array, Data.c_str());

    FILE *Files ;
    Files = fopen(file.c_str(), "a");
	fputs(char_array, Files);
    fputs("\n", Files);
	fclose(Files);
}

string makeStr(string varname)
{
    if(varname[0] == '$'){
        int findBlock = varname.find("[");
    if (findBlock == 0){varname = findData(varname.substr(1, varname.length()-1));}
    else{
        string dt = varname;
        varname = findData(varname.substr(1, findBlock -2));     
        varname = findrow(varname, dt.substr(findBlock, dt.length()));                           
    } 
    }return varname;
}

void create_database(string sql){
    sql = remove_space(sql);
    string database = makeStr(getStr(sql, ' ')); 
    sql.erase(0, database.length()); 

    sql = remove_space(sql);
    string username = makeStr(getStr(sql, ' ')); 
    sql.erase(0, username.length()); 

    sql = remove_space(sql);
    string password = makeStr(getStr(sql, ' ')); 
    sql.erase(0, password.length()); 

    sql = remove_space(sql);      
    dbname = database+username+password;
    dbtxt = dbname+".txt";

    ifstream MyReadF("connect.txt");
    while (getline (MyReadF, myText))
    {
        if(myText == dbname){error("Database already exists", 0);}
    }
    if (stat(dbname.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) {
        error("Database already exists", 0);
    }else
    {
        if (mkdir(dbname.c_str(), 0777) == -1)
        {
            error("unable to create database "+dbname, 0);}else{
            FILE* fps = fopen(dbtxt.c_str(),"w");
            fclose(fps);
        }
    }
    writeTxt("connect.txt", dbname);
    connect = 2;              
}

void create_table(string sql){
    int columns = 1;
    int counter = 0;
    int parencount = 0;  
    int sza = 0;  
    if(connect < 2)
    {
        error("No database selected", 0);
    }    
    sql.erase(0, 5);
    sql = remove_space(sql);
    string tbname = getStr(sql, '('); 
    sql.erase(0, tbname.length());  
    if(tbname.length() > 13){
        error("table name is too long", 0);
    }
    string tbloc =  "/home/projectfpga/Downloads/webserver/compiler/"+dbname+"/"+dbname+".txt";
    string tbloc1 =  "/home/projectfpga/Downloads/webserver/compiler/"+dbname+"/"+tbname+".txt";    
    FILE* fpc = fopen(tbloc1.c_str(),"w");   
    fclose(fpc);         
    FILE* fp = fopen(tbloc.c_str(),"r+");
    fseek(fp, 0, SEEK_END);
    fputs("               ", fp );
    if(sql[0] == '(')
    {
        sql.erase(0, 1);
        while(counter < 16){

            if(parencount == 0 && sql[0] == ')'){
                break;
            }else
            if(sql[0] == '\n' || sql[0] == ' '|| sql[0] == '\t' || sql[0] == '\r'){
                sql.erase(0, 1);  
            
            }else 
            if(sql[0] != '(' && counter == 12){
                counter--;
                sql.erase(0, 1);                  
            }else 
            if(sql[0] == '(' && counter != 12){
                fputc (' ', fp );                
            }else                                 
            if(sql[0] == '(' && counter == 12){
                parencount++;
                sql.erase(0, 1);                  
            }else 

            if(sql[0] != ')' && counter == 15){
                counter--;
                sql.erase(0, 1);                  
            }else 
            if(counter == 13){
                if(sql[1] == ')' ){                    
                    fputc ('0', fp ); 
                    fputc (sql[0], fp ); 
                    sza += stoi(sql.substr(0,1));   
                    sql.erase(0, 1);                         
                }else
                {
                    sza += stoi(sql.substr(0,2));   
                    fputc (sql[0], fp ); 
                    fputc (sql[1], fp );  
                    sql.erase(0, 2); 
                }
                        counter++;                                  
            }else                                 
            if(sql[0] == ')' && counter == 15){
                parencount--;
                sql.erase(0, 1);                  
            }else
            if(sql[0] == ','){
                columns++;
                sql.erase(0, 1);   
                counter = 14;                 
            }else                                                
            if(counter > 13 && parencount != 1){
                counter--;
                sql.erase(0, 1);                     
            }else{
                fputc (sql[0] , fp );
                sql.erase(0, 1); 
            }                
            if(counter == 15){
                counter = 0;
            }
            counter++;
        }	
        columns++;
            
        fseek(fp, 0-(columns*13+2),SEEK_CUR);
        sql = remove_space(tbname);
        counter = 0;
        while(counter < 16)
        {

            if(sql[0] == '\n' || sql[0] == ' ' || sql[0] == '\n' || sql[0] == '\t' || sql[0] == '\r'){
                sql.erase(0, 1);   
                counter--;              
            }else 
            if(counter == 10)
            {
                sql = to_string(sza);
                if(sza < 10){
                    fputs ("00", fp );
                    fputc (sql[0], fp ); 
                }else
                if(sza < 100){
                    fputc ('0', fp );
                    fputc (sql[0], fp ); 
                    fputc (sql[1], fp );                         
                }
                else{
                    fputc (sql[0], fp ); 
                    fputc (sql[1], fp );  
                    fputc (sql[2], fp );
                }

                sql = to_string(columns -1);
                if(columns > 10){
                    fputc (sql[0], fp ); 
                    fputc (sql[1], fp ); 
                }else{
                    fputc ('0', fp ); 
                    fputc (sql[0], fp ); 
                }
                break;                
            }else 
            if(sql[0] == '\0' && counter <= 12){
                fputc (' ', fp );                  
            }else{
                fputc (sql[0] , fp );
                sql.erase(0, 1); 
            }                
            if(counter == 15){
                break;
            }
            counter++;
        }//fseek(fp, 0,SEEK_END);   
        fseek(fp, 0,SEEK_END);
        fputc('\n', fp );                                                
        fclose(fp);       
    }else{
        error("invalid syntax", 0);            
    }  
}

void insert_table(string sql){
    sql = remove_spaces(sql);
    string tablename = getStr(sql, '(');
    tablename = remove_spaces(tablename);
    sql.erase(0, tablename.length());

    string fleData;
    string tbloc =  "/home/projectfpga/Downloads/webserver/compiler/"+dbname+"/"+dbname+".txt"; 
    string tbloc1 =  "/home/projectfpga/Downloads/webserver/compiler/"+dbname+"/"+tablename+".txt";
    ifstream MyReadFile(tbloc);

    while (getline (MyReadFile, fleData)) 
    {
       // cout << fleData.substr(0,tablename.length())<<" and the tablename s :"<<tablename<<endl;
        if(fleData.substr(0,tablename.length())== tablename){
            break;
        }
    }
    MyReadFile.close(); 
    initTable(stoi(fleData.substr(13,2)));
    tab->tablename = fleData.substr(0,tablename.length()); 
    tab->tablesize = stoi(fleData.substr(10,3));  
    tab->columns   = stoi(fleData.substr(13,2)); 
    int counter = 1;
    tokenSt.push_back("Yelggglow");
    while(counter <= tab->columns){
        tab->columnsze[counter] = stoi(fleData.substr(counter*13+13,2)); 
        tokenSt.push_back(fleData.substr(counter*13+2,11));
      //  cout<<tokenSt[counter] <<"--------"<<tab->columnsze[counter]<<endl;         
        counter ++;
      //  off +=2;
    }
 //   printToken(); 

    sql.erase(0, 1);
    
    string cols = getStr(sql, ')');
    sql.erase(0, cols.length());
 //   cout<<cols<<endl;
    sql.erase(0, getStr(sql, '(').length());
    sql.erase(0, 1);
    sql.erase(sql.length()-1, 1);
  //  cout<<sql<<endl;
    counter = 1;
    string data = ""; 
    string data1 = "";         
    string str = sql; 
    FILE* fpa = fopen(tbloc1.c_str(),"a");
   // fseek(fpa, 0, SEEK_END);

    while(counter <= tab->columns){

        for(int g=1; g <= tab->columns; g++)
        {
            data1 = getStr(cols, ',');  
            data = getStr(str, ',');            
            cols.erase(0, data1.length()+1); 
            str.erase(0, data.length()+1);  
            data1 = data1.substr(0, 11); 
            data1.append(11 - data1.length(), ' ');                
            if(remove_spaces(tokenSt[counter]) == remove_spaces(data1)){
               // cout<<";hell"<<endl;
                break;
            }else{
                cols += ","+data1;
                str += ","+data; 
                    if(g == tab->columns){
                        data = "";
                    }          
                }
        }
      //  sql.erase(0, data.length()+1);

        if(data[0] == '$'){
            int slength = data.find("[");
        if (slength == 0){
            data = findData(data.substr(1, data.length()-1));}
        else{
            string dt = data;
            data = findData(data.substr(1, slength -2));     
            data = findrow(data, dt.substr(slength, dt.length()));                           
        } }       
        data = data.substr(0, tab->columnsze[counter]);
        data.append(tab->columnsze[counter] - data.length(), ' ');
        fputs(data.c_str(), fpa );  
        counter++;
    }fputc('\n', fpa);
    fclose(fpa);
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
}

bool compares(string A, int B, string C){
    switch(B){

        case 0:
          if(A != C){
            return true;
          }return false;  
          break;        
        case 1:
          if(A >= C){
            return true;
          }return false;  
          break;        
        case 2: 
          if(A <= C){
            return true;
          }return false;    
          break;                          
        case 3:
          if(A > C){
            return true;
          }return false;  
          break;        
        case 4:        
          if(A < C){
            return true;
          }return false;  
          break;
        case 5:
          if(A == C){
            return true;
          }return false;  
          break;          
    }
    return false;
}

string extract(string sql){
        string field = "";
     string value = "";
    int sign;
    int next;
        int cts;
    bool result = 0; 
    int counter =1 ;
     int offset = 0;
    string feld = "";
  sql = remove_spaces(sql);  
  next = 0;
    if(!(tokenStores == NULL))
    {
        if(sql.substr(0,2) == "OR"){
            next = 1;
            sql.erase(0, 2);
        }else
        if(sql.substr(0,3) == "AND"){
            next = 2;
            sql.erase(0, 3);
        }else          
        if(sql.substr(0,5) == "OFFSET"){
            sql.erase(0, 6);
            oelet = 1;
            offset =   sql[0];
                sql.erase(0, 1);      
            while(isNum(sql[0])){
                feld += sql[0];
                sql.erase(0, 1);
            }
            oeletv = stoi(feld);
            return (sql);
        } else          
        if(sql.substr(0,5) == "LIMIT"){
            sql.erase(0, 5);
            delet = 1;
            while(isNum(sql[0])){
                feld += sql[0];
                sql.erase(0, 1);
            }
            deletv = stoi(feld);
            return (sql);
        } 
    }    
    while(isAlphabet(sql[0]) || isNum(sql[0])){
        field += sql[0];
        sql.erase(0, 1);
    }
    

    while(counter <= tab->columns){
            field = field.substr(0, 11); 
            field.append(11 - field.length(), ' '); 
        if(field == tokenSt[counter]){
            
            cts = tab->columnsze[counter];
            break;
        }   offset += tab->columnsze[counter]; 
         
        counter ++;
    }
    if(counter > tab->columns && field != "id"){
        error("field "+field+"  is undefined", 6);
    }
   string arr[] = {"!=", ">=", "<=", ">", "<", "="};

    for(int c = 0; c <= 5; c++){
        if(arr[c][0] == sql[0] && arr[c][1] == sql[1] && c < 5){
            sign = c;
            sql.erase(0, 2);
        }else
        if(arr[c] == sql.substr(0,1) && c > 2){
            sign = c;
            sql.erase(0, 1);
        }              
    }
    sql = remove_spaces(sql);
   // cout<<sql<<endl;
if(sql[0] != '\''){
    error("add values n hyphen",73);
}sql.erase(0, 1);
    while(isAlphabet(sql[0]) || isNum(sql[0])){
        value += sql[0];
        sql.erase(0, 1);
    }
if(value[0] == '$'){
    value = findData(value);
        if(sql[0] == '['){
        sql.erase(0, 1);
        string fend = "";
        while(sql[0] != ']'){
            fend + sql[0];
            sql.erase(0, 1);
        }sql.erase(0, 1);
    value = findrow(value,fend);                          
    } 
}
if(sql[0] != '\''){
    error("add values n hyphen",53);
}sql.erase(0, 1);

    operate *ptr; 
    operate  *temps = NULL; 
    temps =new operate();
    if(temps == NULL)
    {
        error("nOut of Memory Space:n", 0);
        exit(0);
    }

    temps->field = field;
    temps->value = value;
    temps->sign  = sign;
    temps->nexts = next;
    temps->cts = cts;
    temps->offset = offset;
    temps->result = false;

    if(tokenStores==NULL)
    {   
        tokenStores=temps;
    }
    else
    {   
         operate *ptrs=tokenStores;
        while(ptrs->next !=NULL)
        {
            if(ptrs->field == temps->field){
                ptrs->value = temps->value;
                ptrs->sign  = temps->sign;
                ptrs->nexts = temps->nexts;
                ptrs->cts = temps->cts;
                ptrs->offset = temps->offset;                                
                ptrs->result  = temps->result;                
                break;
            }
            ptrs=ptrs->next;
        }
        if(ptrs->field == temps->field){
                ptrs->value = temps->value;
                ptrs->sign  = temps->sign;
                ptrs->nexts = temps->nexts;
                ptrs->cts = temps->cts;
                ptrs->offset = temps->offset;                 
                ptrs->result  = temps->result;   
        }else{
        ptrs->next=temps;
        }           
    }        

    return sql;
}

string extracts(string sql){
        string field = "";
     string value = "";
    int sign;
    int next = 0;
        int cts;
    bool result = 0; 
    int counter =1 ;
     int offset = 0;
    string feld = "";
 
    if(!(tokenStorese == NULL))
    {
        if(sql.substr(0,5) == "WHERE"){
            return "0";
        }
    }    
    while(isAlphabet(sql[0]) || isNum(sql[0])){
        field += sql[0];
        sql.erase(0, 1);
    }
    

    while(counter <= tab->columns){
            field = field.substr(0, 11); 
            field.append(11 - field.length(), ' '); 
        if(field == tokenSt[counter]){
            
            cts = tab->columnsze[counter];
            break;
        }   offset += tab->columnsze[counter]; 
         
        counter ++;
    }
    if(counter > tab->columns && field != "id"){
        error("field "+field+"  is undefined", 6);
    }
   string arr[] = {"!=", ">=", "<=", ">", "<", "="};
    
   for(int c = 0; c <= 5; c++){
        if(arr[c][0] == sql[0] && arr[c][1] == sql[1] && c < 5){
            sign = c;
            sql.erase(0, 2);
        }else
        if(arr[c] == sql.substr(0,1) && c > 2){
            sign = c;
            
            sql.erase(0, 1);
        }              
    }
    sql = remove_spaces(sql);

if(sql.substr(0,1) != "'"){
    error("add values n hyphen",23);
}sql.erase(0, 1);
    while(isAlphabet(sql[0]) || isNum(sql[0])){
        value += sql[0];
        sql.erase(0, 1);
    }
if(value[0] == '$'){
    value = findData(value);
    if(sql[0] == '['){
        sql.erase(0, 1);
        string fend = "";
        while(sql[0] != ']'){
            fend + sql[0];
            sql.erase(0, 1);
        }sql.erase(0, 1);
    value = findrow(value,fend);                          
    }  
}
if(sql[0] != '\''){
    error("add values n hyphen",13);
}sql.erase(0, 1);

    operate *ptr; 
    operate  *ted = NULL; 
    ted =new operate();
    if(ted == NULL)
    {
        error("nOut of Memory Space:n", 0);
        exit(0);
    }

    ted->field = field;
    ted->value = value;
    ted->sign  = sign;
    ted->nexts = next;
    ted->cts = cts;
    ted->offset = offset;
    ted->result = false;

    if(tokenStorese==NULL)
    {   
        tokenStorese=ted;
    }
    else
    {
         operate *ps=tokenStorese;
        while(ps->next !=NULL)
        {
            if(ps->field == ted->field){
                ps->value = ted->value;
                ps->sign  = ted->sign;
                ps->nexts = ted->nexts;
                ps->cts = ted->cts;
                ps->offset = ted->offset;                                
                ps->result  = ted->result;                
                break;
            }
            ps=ps->next;
        }
        if(ps->field == ted->field){
                ps->value = ted->value;
                ps->sign  = ted->sign;
                ps->nexts = ted->nexts;
                ps->cts = ted->cts;
                ps->offset = ted->offset;                 
                ps->result  = ted->result;   
        }else{
        ps->next=ted;
        }            
    }        

    return sql;
}

void delete_database(string sql){
    string tbloc =  "/home/projectfpga/Downloads/webserver/compiler/"+sql; 
   DIR *dr;
   struct dirent *en;
   dr = opendir(tbloc.c_str()); //open all or present directory
   if (dr) {
      while ((en = readdir(dr)) != NULL) {
        string tt= "/home/projectfpga/Downloads/webserver/compiler/"+sql+"/"+en->d_name;
        remove(tt.c_str());
         printf("%s\n", tt.c_str()); //print all directory name
      }
      closedir(dr); //close all directory
      rmdir(tbloc.c_str());
   }
}

void delete_table(string sql){
    if(connect != 2){error("No database selected",5);}
    string fleDatax;
    string tw = "";
    string tbloc =  "/home/projectfpga/Downloads/webserver/compiler/"+dbname+"/"+dbname+".txt"; 
    string tbloc1 =  "/home/projectfpga/Downloads/webserver/compiler/"+dbname+"/"+sql+".txt";
    ifstream MyRead(tbloc);
    ofstream MyReads(tbloc1);
        while (getline (MyRead, fleDatax)) 
    {
       // cout << fleData.substr(0,tablename.length())<<" and the tablename s :"<<tablename<<endl;
       
        if(fleDatax.substr(0,sql.length()) != sql){
            MyReads << fleDatax;
        MyReads << "\n";}
    }MyRead.close();
    MyReads.close();
    remove(tbloc.c_str());
    rename(tbloc1.c_str(), tbloc.c_str());
  //  error("table "+sql+" does not exist", 23);
}

void delete_row(string sql){
     sql = remove_space(sql);
    string tablename = getStr(sql, ' ');
    tablename = remove_spaces(tablename);
    sql.erase(0, tablename.length()+1);
    sql = remove_space(sql);

    string fleData;
    string tbloc =  "/home/projectfpga/Downloads/webserver/compiler/"+dbname+"/"+dbname+".txt"; 
    string tbloc1 =  "/home/projectfpga/Downloads/webserver/compiler/"+dbname+"/"+tablename+".txt";
    ifstream MyReadFile(tbloc);

    while (getline (MyReadFile, fleData)) 
    {
       // cout << fleData.substr(0,tablename.length())<<" and the tablename s :"<<tablename<<endl;
        if(fleData.substr(0,tablename.length())== tablename){
            break;
        }
    }
    MyReadFile.close(); 
 
   tab->tablename = fleData.substr(0,tablename.length()); 
    tab->tablesize = stoi(fleData.substr(10,3));  
    tab->columns   = stoi(fleData.substr(13,2)); 
    int counter = 1;
    tokenSt.push_back(fleData.substr(counter*13+2,11));  
    while(counter <= tab->columns){
        tab->columnsze[counter] = stoi(fleData.substr(counter*13+13,2)); 
        tokenSt.push_back(fleData.substr(counter*13+2,11));        
        counter ++;
    } 

    if(getStr(sql, ' ') == "WHERE"){
        sql.erase(0, 6);
        sql = remove_spaces(sql);
    }else{error("wrong syntax", 0);}

    while(sql != ""){
        sql = extract(sql);
    }

    FILE* pFile = fopen(tbloc1.c_str(),"r+");
    fseek(pFile, 0, SEEK_END);
    int size=ftell (pFile);
    rewind(pFile);
    int totalsze =0;
    int result =0;  
    int res =0;  
    int cc =0;      
    string c =""; 
     string str ="";     
     string myText =""; 
     char t;
    str.append(tab->tablesize, ' '); 

    ifstream MyReadF(tbloc1.c_str());

    while (getline (MyReadF, myText)){
      //  cout<<"totalsze "<<totalsze<<" size "<<size<<endl;
        operate *prs;
        if(tokenStores == NULL)
        {
            error("variable not declared", 8);
        }
        prs = tokenStores;

        while(prs !=NULL)
        { 
            
            if(prs->field == "id"){
                c = cc;
            }else{
            c = myText.substr(prs->offset, prs->cts);}
            prs->value.append(prs->cts - prs->value.length(), ' '); 
            prs->result = compares(c, prs->sign, prs->value);
            cout<<prs->result<<endl;
            if(prs->nexts > 1){
                 result = result || prs->result;
            }else
            if(prs->nexts > 2){
                 result = result && prs->result;
            }else            
            {
                result = prs->result;
            }
            prs=prs->next;
        }
        if(result == 1){
            int ss = ftell(pFile);
                fputs(str.c_str(), pFile);
                 fputc('\n', pFile);
                fseek(pFile, -(str.length()), SEEK_CUR);
            if(delet == 1){
                deletv--;
                if(deletv == 0){break;}
            }
        }                fseek(pFile, tab->tablesize , SEEK_CUR);
        cc++;
        totalsze  += tab->tablesize;
           }
    delet =0;
    MyReadFile.close(); 
    fclose(pFile);
   
}


void update_table(string tablename, string sql){
    
    string fleData;
    string tbloc =  "/home/projectfpga/Downloads/webserver/compiler/"+dbname+"/"+dbname+".txt"; 
    string tbloc1 =  "/home/projectfpga/Downloads/webserver/compiler/"+dbname+"/"+tablename+".txt";
    ifstream MyReadFisle(tbloc);

    while (getline (MyReadFisle, fleData)) 
    {
       // cout << fleData.substr(0,tablename.length())<<" and the tablename s :"<<tablename<<endl;
        if(fleData.substr(0,tablename.length())== tablename){
            break;
        }
    }
    MyReadFisle.close(); 
 
   tab->tablename = fleData.substr(0,tablename.length()); 
    tab->tablesize = stoi(fleData.substr(10,3));  
    tab->columns   = stoi(fleData.substr(13,2)); 
    int counter = 1;
    tokenSt.push_back(fleData.substr(counter*13+2,11));  
    while(counter <= tab->columns){
        tab->columnsze[counter] = stoi(fleData.substr(counter*13+13,2)); 
        tokenSt.push_back(fleData.substr(counter*13+2,11));          
        counter ++;
    } 
    sql = extracts(sql);
    while(sql[0] ==','){
        sql = extracts(sql);
    }
    if(sql.substr(0,5) == "WHERE"){
    sql.erase(0, 5);
    while(sql != ""){
        sql = extract(sql);
    }}

    FILE* pFile = fopen(tbloc1.c_str(),"r+");
    fseek(pFile, 0, SEEK_END);
    int size=ftell (pFile);
    rewind(pFile);
    int totalsze =0;
    int result =0;  
    int res =0;  
    int cc =1;      
    string c =""; 
     string str ="";     
     string myText =""; 
     string id ="id"; 
     char t;
     result = 1;
    str.append(tab->tablesize, ' '); 

    ifstream MyReadF(tbloc1.c_str());

    while (getline (MyReadF, myText)){
       // cout<<cc<<endl;
      //  cout<<"totalsze "<<totalsze<<" size "<<size<<endl;
        operate *pnn;
        if(tokenStores == NULL)
        {
            error("variable not declared", 8);
        }
        pnn = tokenStores;
//cout<<cc<<endl;
        while(pnn !=NULL)
        { 
            id.append(11 - 2, ' '); 
           id =id.substr(0, pnn->cts+1);
            
            id.append(11, ' '); 
           id =id.substr(0, 11);

            if(pnn->field == id){
                c = to_string(cc);
            }else{
            c = myText.substr(pnn->offset, pnn->cts);}
              c.append(pnn->cts - c.length(), ' '); 
           c = c.substr(0, pnn->cts);

            pnn->value.append(pnn->cts - pnn->value.length(), ' '); 
            pnn->result = compares(c, pnn->sign, pnn->value);
       //     cout<<pnn->field<<"--"<<pnn->result<<"--"<<c<<"--"<<pnn->value<<"--"<<pnn->sign<<endl;
            if(pnn->nexts > 1){//cout<<result;
                 result = result || pnn->result;
            }else
            if(pnn->nexts > 2){
                 result = result && pnn->result;
            }else            
            {
                result = pnn->result;
            }
            pnn=pnn->next;
        }
        if(result == 1){
            string dat = myText;
           // cout<<dat.length()<<endl;
            operate* pnnm = tokenStorese;
        while(pnnm !=NULL)
        { 
            
            if(pnnm->field == id){
                error("id is unmodifiable", 9);
            }else{
                pnnm->value.append(pnnm->cts - pnnm->value.length(), ' '); 
                pnnm->value = pnnm->value.substr(0, pnnm->cts);
//cout<<pnnm->offset<<"--"<<dat.length()<<"--"<<pnnm->value.length()<<"--"<<pnnm->cts<<endl;   
            dat = dat.substr(0, pnnm->offset) 
            + pnnm->value 
            + dat.substr(pnnm->cts+pnnm->offset, dat.length());
                }

         //   cout<<dat<<endl;

            fseek(pFile, totalsze, SEEK_SET);
           // fputc('\n', pFile);            
            fputs(dat.c_str(), pFile);
            fseek(pFile, -(dat.length()), SEEK_CUR);

            pnnm=pnnm->next;
            }


            if(delet == 1){
                deletv--;
                if(deletv == 0){break;}
            }
        }  
        
                   //   fseek(pFile, tab->tablesize , SEEK_CUR);
                      
        cc++;
        totalsze  += tab->tablesize+1;
           }
    delet =0;
    MyReadFisle.close(); 
    fclose(pFile);
  
}

void selects(string  sql, string  row, int pointer){
    
     sql = remove_space(sql);
     if(getStr(sql, ' ')=="SELECT"){
        sql.erase(0, 6);
     }else{error("invalid select statement", 22);}
    sql = remove_space(sql);
    
    while(sql.substr(0, 5) != "FROM "){
        sql.erase(0, 1);
        if(sql.length() < 7){
            error("Reached the end of your select statement",34);
        }
    }sql.erase(0, 5);
    sql = remove_space(sql);
    string tablename = getStr(sql, ' ');
    sql.erase(0, tablename.length()+1);
    sql = remove_space(sql);
    string fleData;
    string tbloc =  "/home/projectfpga/Downloads/webserver/compiler/"+dbname+"/"+dbname+".txt"; 
    string tbloc1 =  "/home/projectfpga/Downloads/webserver/compiler/"+dbname+"/"+tablename+".txt";
    ifstream MyReadFisle(tbloc);

    while (getline (MyReadFisle, fleData)) 
    {
       // cout << fleData.substr(0,tablename.length())<<" and the tablename s :"<<tablename<<endl;
        if(fleData.substr(0,tablename.length())== tablename){
            
            break;
        }
    }
    MyReadFisle.close(); 
    
   tab->tablename = fleData.substr(0,tablename.length()); 
    tab->tablesize = stoi(fleData.substr(10,3));  
    tab->columns   = stoi(fleData.substr(13,2)); 
    int counter = 1;
    tokenSt.push_back(fleData.substr(counter*13+2,11));  
    while(counter <= tab->columns){
        tab->columnsze[counter] = stoi(fleData.substr(counter*13+13,2)); 
        tokenSt.push_back(fleData.substr(counter*13+2,11));          
        counter ++;
    } 

    if(sql.substr(0,5) == "WHERE"){
    sql.erase(0, 5);
    while(sql != ""){
        sql = extract(sql);
    }}

    FILE* pFile = fopen(tbloc1.c_str(),"r+");
    fseek(pFile, 0, SEEK_END);
    int size=ftell (pFile);
    rewind(pFile);
    int totalsze =0;
    int result =0;  
    int res =0;  
    int cc =1;      
    string c =""; 
     string str ="";     
     string myText =""; 
     string id ="id"; 
     char t;
     result = 1;
    str.append(tab->tablesize, ' '); 

    ifstream MyReadF(tbloc1.c_str());

    while (getline (MyReadF, myText)){
     
      //  cout<<"totalsze "<<totalsze<<" size "<<size<<endl;

            if(oelet == 1){
                if(offset == '+' && cc <= oeletv)
                { 
                    cc++;  
                    continue; 
                }else
                if(offset == '-' && cc >= oeletv){
                    break;
                }            
            }

        operate *pnn;
        if(tokenStores == NULL)
        {
            error("variable not declared", 8);
        }
        pnn = tokenStores;
//cout<<cc<<endl;
        while(pnn !=NULL)
        { 
            id.append(11 - 2, ' '); 
           id =id.substr(0, pnn->cts+1);
            
            id.append(11, ' '); 
           id =id.substr(0, 11);

            if(pnn->field == id){
                c = to_string(cc);
            }else{
            c = myText.substr(pnn->offset, pnn->cts);}
              c.append(pnn->cts - c.length(), ' '); 
           c = c.substr(0, pnn->cts);

            pnn->value.append(pnn->cts - pnn->value.length(), ' '); 
            pnn->result = compares(c, pnn->sign, pnn->value);
       //     cout<<pnn->field<<"--"<<pnn->result<<"--"<<c<<"--"<<pnn->value<<"--"<<pnn->sign<<endl;
            if(pnn->nexts > 1){//cout<<result;
                 result = result || pnn->result;
            }else
            if(pnn->nexts > 2){
                 result = result && pnn->result;
            }else            
            {
                result = pnn->result;
            }
            pnn=pnn->next;
        }
        if(result == 1){  // cout<<cc<<"--"<<oelet<<delet<<endl;
            string dow = dbname+"`"+tablename+"`"+myText;
            insertVar(row, dow, 0);
            int count = pointer;
            sel_count++;
            
            parse(count);
            sel_count--;
            if(delet == 1){
                deletv--;
                if(deletv == 0){break;}
            }
        }       
        cc++;
        totalsze  += tab->tablesize;
    }
    delet =0;
    MyReadFisle.close(); 
    fclose(pFile);
  
}

void parse(int pointer){  
    string sql; 
//cout<<"--"<<pointer<<tokenName[Token->type[pointer]]<<"--"<<tokenName[Token->type[pointer+1]]<<"--"<<tokenName[Token->type[pointer+2]]<<endl;
    while(!expect(TOKEN_EOF, pointer))
    { 
        if((expect(TOKEN_ECHO, pointer)))
        {    
            while(!expect(TOKEN_SEMICOLON, pointer))
            {
                if(expect(TOKEN_IDENTIFIER, pointer+1))
                {
                    sql = findData(tokenData[pointer+1]);
                    if(expect(TOKEN_LEFT_BLOCK, pointer+2))
                    {
                        sql = findrow(sql, tokenData[pointer + 3]); 
                        cout<<sql;
                        pointer+=3;                          
                    }                  
                }else
                if(expect(TOKEN_NUMBER, pointer+1) || expect(TOKEN_STRING, pointer+1)) 
                {
                    cout<<tokenData[pointer+1];                     
                }pointer = pointer +  2;                        
            }pointer++;
         cout<<endl;  
        }else         
        if((expect(TOKEN_IDENTIFIER, TOKEN_EQUAL, pointer)))
        {
                string value     ="";
                string varName   =  tokenData[pointer]; 
                pointer +=  2;

            if(expect(TOKEN_STRING, TOKEN_SEMICOLON, pointer) 
            || expect(TOKEN_NUMBER, TOKEN_SEMICOLON, pointer)
            || expect(TOKEN_IDENTIFIER, TOKEN_SEMICOLON, pointer))
            { 
                value =  tokenData[pointer];          
            }else 
            if(expect(TOKEN_IDENTIFIER, TOKEN_LEFT_BLOCK,  pointer))
            { 
                value =  tokenData[pointer];          
                value = findrow(value, tokenData[pointer+2]); 
                pointer+=3;                          
            }                     
            insertVar(varName, value, Token->lineNo[pointer-5]);
             pointer = pointer +  2;
        }else
        if((expect(M_QUERY, TOKEN_LEFT_PAREN, pointer)))
        {
            pointer += 2;
            if(expect(TOKEN_IDENTIFIER, pointer))
            {
                sql = findData(tokenData[pointer]);
            }else
            {
                sql = tokenData[pointer];
            }   
            if(getStr(sql, ' ') == "CREATE"){
                sql.erase(0, 6);
                sql = remove_space(sql);
                if(getStr(sql, ' ') == "DATABASE"){
                    sql.erase(0, 8);                    
                    create_database(sql);
                }else
                if(getStr(sql, ' ') == "TABLE"){
                    create_table(sql);
                }                  
            }else
            if(getStr(sql, ' ') == "INSERT"){
                sql.erase(0, 6);
                sql = remove_space(sql);
                if(getStr(sql, ' ') == "INTO"){
                    sql.erase(0, 4);
                    sql = remove_space(sql);
                    insert_table(sql);
                }else{
                    error("invalid syntax",0);
                }              
            }else
            if(getStr(sql, ' ') == "DELETE"){
                sql.erase(0, 6);
                sql = remove_space(sql);
                if(getStr(sql, ' ') == "DATABASE"){
                    sql.erase(0, 8);
                    sql = remove_spaces(sql);
                    delete_database(sql);
                }else
                if(getStr(sql, ' ') == "TABLE"){
                    sql.erase(0, 5);
                    sql = remove_spaces(sql);
                    delete_table(sql);
                }else
                if(getStr(sql, ' ') == "FROM"){
                    sql.erase(0, 4);
                    sql = remove_space(sql);
                    delete_row(sql);
                }                                
                else{
                    error("invalid syntax",0);
                }              
            }else
            if(getStr(sql, ' ') == "UPDATE"){
                sql.erase(0, 6);
                sql = remove_space(sql);
                string tabname = getStr(sql, ' ');
                sql.erase(0, tabname.length());
                sql = remove_space(sql);
                if(getStr(sql, ' ') == "SET"){
                    sql.erase(0, 3);
                    sql = remove_spaces(sql);
                    update_table(tabname, sql);
                }                               
                else{
                    error("invalid syntax",10);
                }              
            }
            pointer++;
            if((expect(TOKEN_RIGHT_PAREN, TOKEN_SEMICOLON, pointer))){ 
                pointer += 2;
            }else{error("expected a closing parenthesis and semi colon",0);}                            
        }else   
        if(expect(TOKEN_WHILE, TOKEN_LEFT_PAREN, TOKEN_IDENTIFIER, pointer))
        {
            string row = tokenData[pointer+2];
            pointer += 3;
            if(expect(TOKEN_EQUAL, M_QUERY, TOKEN_LEFT_PAREN, pointer))
            {        
                pointer += 3;
                if(connect < 2){
                    error("login to database",14);
                }
                if(expect(TOKEN_IDENTIFIER, pointer))
                {
                    sql = findData(tokenData[pointer]);
                    if(expect(TOKEN_LEFT_BLOCK, pointer+1))
                    {
                        sql = findrow(sql, tokenData[pointer+2]); 
                        pointer += 3;                          
                    }                  
                }else
                {
                    sql = tokenData[pointer];
                }pointer++;
                
                if((expect(TOKEN_RIGHT_PAREN, TOKEN_RIGHT_PAREN, TOKEN_LEFT_BRACE, pointer)))
                { 
                    pointer += 3;
                    selects(sql, row, pointer);
                }else
                {
                    error("expected a closing parenthesis and semi colon",9);
                } 
                int bracec = 1;
                while(!(expect(TOKEN_RIGHT_BRACE, pointer)) || bracec > 1){ 
                    if(expect(TOKEN_LEFT_BRACE, pointer)){ 
                        bracec++;
                    }else
                    if(expect(TOKEN_RIGHT_BRACE, pointer)){ 
                        bracec--;
                    }                
                    pointer ++;
                } pointer ++; 
            }                       
        }else       
        if((expect(M_CONNECT, TOKEN_LEFT_PAREN, pointer)))
        {
            pointer += 2;
            string servername, username, password, data;
            if(expect(TOKEN_IDENTIFIER, pointer))
            { 
                servername = findData(tokenData[pointer]);
                if(expect(TOKEN_LEFT_BLOCK, pointer+1)){
                servername = findrow(servername, tokenData[pointer+2]); 
                pointer += 3;                          
                }                  
            }else{servername = tokenData[pointer];}

            pointer += 2;
            if(expect(TOKEN_IDENTIFIER, pointer))
            { 
                username = findData(tokenData[pointer]);
                if(expect(TOKEN_LEFT_BLOCK, pointer+1)){
                username = findrow(username, tokenData[pointer+2]); 
                pointer += 3;                          
                }                  
            }else{username = tokenData[pointer];}  

            pointer += 2;
            if(expect(TOKEN_IDENTIFIER, pointer))
            {
                password = findData(tokenData[pointer]);
                if(expect(TOKEN_LEFT_BLOCK, pointer+1)){
                password = findrow(password, tokenData[pointer+2]); 
                pointer += 3;                          
                }                
            }else{password = tokenData[pointer];} 

            pointer +=2;
            if(expect(TOKEN_IDENTIFIER, pointer))
            {
                dbname = findData(tokenData[pointer]);
                if(expect(TOKEN_LEFT_BLOCK, pointer+1)){
                dbname = findrow(dbname, tokenData[pointer+2]); 
                pointer += 3;                          
                }
            }else{dbname = tokenData[pointer];}    
            connect =2;
            pointer++;          
 
            string datalog = dbname+servername+username+password;
            ifstream MyReadFile("connect.txt"); 
            while (getline (MyReadFile, data)) {
               if(datalog == data){
                connect = 2;
                break;
               }
            }
            if(connect != 2){
                error("unable to login to database", 0);
            }
            MyReadFile.close();           
            if((expect(TOKEN_RIGHT_PAREN, TOKEN_SEMICOLON, pointer))){ 
             pointer += 2;
            }else{error("expected a closing parenthesis and semi colon",0);}   

        }else       
        if((expect(TOKEN_RIGHT_BRACE, pointer)) && sel_count > 0){   
            return;
        }else{
            error("invalid token on line", Token->lineNo[pointer]);
        }
    }  
    if(expect(TOKEN_EOF, pointer)){
        return;
    }
}
