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

int    offsetCount = 0;
bool   offset      = false;
bool   limitB      = false;
int    limit       = 0;
int    marker      = 0;
bool   connect     = false;
bool   desc        = false;
bool   ALL         = false;
int    counter     = 0;
int    old         = 0;
char   offDr       = '0';
string database    ="";
string myText      ="";
string phpdata      ="./public/new.php";
struct stat sb;

std::vector<std::string> tokenData;
std::vector<std::string> tokenSt;
std::stack<std::string> scope;
std::vector<std::string> calc;

tokenClass* Token;
tableL* tab;
tokenVar *tokenStore=NULL;
 operate *valueStore=NULL;
operate *valueSt=NULL;

void* reallocate(void* pointer, size_t oldSize, size_t newSize) {
  if (newSize == 0) {
    free(pointer);
    return NULL;
  }
  void* result = realloc(pointer, newSize);
  if (result == NULL) exit(0);
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
}

void initTable(){
  tab = new tableL(); 
  tab->tablename = ""; 
  tab->tablesize = 0; 
  tab->columns = 0;  
}

void freeToken() {
  //  FREE_ARRAY(TokenType, Token->type, Token->capacity);
   // FREE_ARRAY(int, Token->lineNo, Token->capacity);  
  //  FREE_ARRAY(TokenType, Token->type, Token->capacity);
    initTable();
    serverData.clear();
    tokenData.clear();
    tokenSt.clear();
    calc.clear();

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

string remove_space(string sql){
    while(sql[sql.length()-1] == ' ' || sql[sql.length()-1] == '\r' || sql[sql.length()-1] == '\n' || sql[sql.length()-1] == '\t'){
        sql.erase(sql.length()-1,1);
    }
    while(sql[0] == ' ' || sql[0] == '\r' || sql[0] == '\n' || sql[0] == '\t'){
        sql.erase(0,1);
    }

    return sql;
}

string remove_spaces(string sql){
    counter = 0;
    while(sql[counter] != '\0'){
        if(sql[counter] == ' ' || sql[counter] == '\n'){        
            sql.erase(counter,1);
            counter--;
        }counter++;
    }
    return sql;
}

string getStr(string sql, char end){
    int counter = 0;
    string retStr = "";
    while(sql[counter] != '\0'){
        if(sql[counter] == end){
            break;
        }
        retStr += sql[counter];
        counter++;
    }
    return(retStr);
}

string findData(string varname){
    tokenVar *search;
    if(varname == "GET" || varname == "POST" )
    {
        return("`POST");
    }    
    if(tokenStore == NULL)
    {
        error("No variable initialized", 0);
        return "0";
    }
    search = tokenStore;
    while(search !=NULL)
    { 
        if(search->name == varname){
            
            return search->value;
        }else
        if(search->next == NULL){
            error("variable  "+varname+" not declared", 0);
            break;
        }else
        {
            search = search->next;
        }        
        
    }
    return ("0");          
}

string vectorSearch(string varname){

    for (int i = 0; i < serverData.size(); ++i) {
        if(getStr(serverData[i], '=') == varname ){
            string vectorS = serverData[i];
            vectorS.erase(0, getStr(serverData[i], '=').length()+1);
            return vectorS;
        }        
    }return "0";
}

string findrow(string varname, string column){
    if(varname == "`POST" )
    {
        return(vectorSearch(column));
    }  
    string flesData;
    string dbname1 = getStr(varname, '`');
    varname.erase(0, dbname1.length()+1);
    dbname1       += "`"+getStr(varname, '`');
    varname.erase(0, getStr(varname, '`').length()+1);
    string tablename1 = getStr(varname, '`');
    varname.erase(0, tablename1.length()+1); 

    string tbloc =  "/home/projectfpga/Downloads/webserver/compiler/"+dbname1+"/"+dbname1+".txt";
    ifstream MyRead1(tbloc);
    int counter = 0;
    while (getline (MyRead1, flesData)) 
    {
        if(flesData.substr(0,tablename1.length())== tablename1)
        {
            counter++;
            break;
        }
    }
    MyRead1.close(); 
    if(counter == 0){
        error("table "+tablename1+" does not exist", 79);
        return "0";}
    
    counter = 0;
    
    while(flesData.length() > 0){
        if(getStr(flesData, '`') == column){

            break;
        }
        flesData = flesData.erase(0, getStr(flesData, '`').length()+1);
        counter++;
    } 

    if(flesData.length() <= 0){
        error("column "+column+" does not exist", 23);
        return "0";
        
    }

    while(varname.length() > 0){
        if(counter-2 == 0){
            varname = getStr(varname, '`');
            break;
        }
        varname = varname.erase(0, getStr(varname, '`').length()+1);
        counter--;
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
        return;
        
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
//	fputs("\n", Files);    
	fclose(Files);
}

void error(string error, int lineNo){
    writeTxt(phpdata, error+"--"+to_string(lineNo));
    cout<<error<<" "<<lineNo<<endl;
}

string makeStr(string varname){
    if(varname[0] == '$'){
        int findBlock = varname.find("[");
            
    if (findBlock == -1){    
        varname = findData(varname.substr(1, varname.length()-1));
        }
    else{
        string dt = varname;
        varname = findData(varname.substr(1, findBlock -1));     
        varname = findrow(varname, dt.substr(findBlock+1, dt.length()-findBlock-2));                             
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
    database = database+"`"+username+password;
    string dbtxt =  "/home/projectfpga/Downloads/webserver/compiler/"+database+"/"+database+".txt";
    ifstream MyReadF("connect.txt");
    while (getline (MyReadF, myText))
    {
        if(myText == database){
            writeTxt(phpdata, "Database already exists");
            return;
            }
    }
    if (stat(database.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) {
        writeTxt(phpdata, "Database already exists");
        return;
    }else
    {
        if (mkdir(database.c_str(), 0777) == -1)
        {
            error("unable to create database "+database, 0);
            return;
            
        }else{
            FILE* fps = fopen(dbtxt.c_str(),"w");
            fclose(fps);
        }
    }
    writeTxt("connect.txt", database);            
}

void create_table(string sql, string dbname){
    int columns = 1;
    counter = 0;
    int parencount = 0;  
    int sza = 0;  
    if(connect == false)
    {
        error("No database selected", 0);
        return;
    }    
    
    sql = remove_spaces(sql);
    string tbname = getStr(sql, '('); 
    sql.erase(0, tbname.length()); 
    tbname = remove_spaces(tbname);
    

    string tbloc =  "/home/projectfpga/Downloads/webserver/compiler/"+dbname+"/"+dbname+".txt";
    string tbloc1 =  "/home/projectfpga/Downloads/webserver/compiler/"+dbname+"/"+tbname+".txt"; 
    ifstream MyReadF(tbloc.c_str());
    while (getline (MyReadF, myText))
    {
        if(getStr(myText, '`') == tbname){
            error("Table "+tbname+" already exists in database "+getStr(dbname, '`'), 0);
            return;
            }
    }
 
    FILE* fpc = fopen(tbloc1.c_str(),"w");   
    fclose(fpc); 

    if(sql[0] == '('){ sql.erase(0, 1);}else{
        error("invalid syntax", 0);
        } 
        int counter = 2;
        myText = "id`";

        while(sql.find(",") != -1)
        {
            if(getStr(sql, ',') == "id"){error("id is a reserved word ", 0);
            return;}
            myText += getStr(sql, ',') + "`";
            sql.erase(0, getStr(sql, ',').length()+1); 
            counter++;
           
        }
        myText = tbname +"`"+to_string(counter)+"`"+ myText + getStr(sql, ')')+"`";
        writeTxt(tbloc, myText);
}

void insert_table(string sql, string dbname){
    sql = remove_spaces(sql);
    string tablename = getStr(sql, '(');
    tablename = remove_spaces(tablename);
    sql.erase(0, tablename.length() + 1);

    string tbloc =  "/home/projectfpga/Downloads/webserver/compiler/"+dbname+"/"+dbname+".txt"; 
    string tbloc1 =  "/home/projectfpga/Downloads/webserver/compiler/"+dbname+"/"+tablename+".txt";
    ifstream MyReadFile(tbloc);

    while (getline (MyReadFile, myText)) 
    {
        if(getStr(myText, '`') == tablename)
        {
            myText = myText.erase(0, getStr(myText, '`').length() + 1);
            break;
        }
    }
    MyReadFile.close(); 
 
    int column   = stoi(getStr(myText, '`')); 
        myText   = myText.erase(0, getStr(myText, '`').length() + 1);
    
    string cols = getStr(sql, ')');
    sql.erase(0, cols.length());
    sql.erase(0, getStr(sql, '(').length() + 1);
    sql.erase(sql.length()-1, 1);

    string data0 = ""; 
    string data1 = "";         
    string data2 = ""; 
    int counts   = 0;

    while(myText.length() > 0)
    {
        if(getStr(myText, '`') == getStr(cols, ','))
        {
            data0   += makeStr(getStr(sql, ','))+"`";
            myText   = myText.erase(0, getStr(myText, '`').length() + 1);  
            cols     = cols.erase(0, getStr(cols, ',').length() + 1);
            sql      = sql.erase(0, getStr(sql, ',').length() + 1);
            counts  = 0;
            column--;
        }else
        if(counts <= column)        
        {
            data1    = getStr(cols, ',');
            data2    = getStr(sql, ',');            
            cols     = cols.erase(0, getStr(cols, ',').length() + 1) + "," + data1;
            sql      = sql.erase(0, getStr(sql, ',').length() + 1) + "," + data2;
            counts++;
        }else
        {
            data0   += "`";
            myText   = myText.erase(0, getStr(myText, '`').length() + 1); 
            counts  = 0;
            column--;
        }
    }
    writeTxt(tbloc1, "\n");
    writeTxt(tbloc1, data0);
    
}

bool compares(string A, int B, string C){
    A = remove_space(A);
    C = remove_space(C);

    switch(B){

        case 0:
          if(A != C){
            return true;
          }return false;  
          break;        
        case 1:
          if(stoi(A) >= stoi(C)){
            return true;
          }return false;  
          break;        
        case 2: 
          if(stoi(A) <= stoi(C)){
            return true;
          }return false;    
          break;                          
        case 3:
          if(stoi(A) > stoi(C)){
            return true;
          }return false;  
          break;        
        case 4:        
          if(stoi(A) < stoi(C)){
            return true;
          }return false;  
          break;
        case 5:
          if((A.compare(C)) == 0){
            return true;
          }
          return false;  
          break;          
    }
    return false;
}

void delete_database(string sql){
    myText = makeStr(getStr(sql, ' '));
    sql.erase(0, getStr(sql, ' ').length());
    sql = remove_space(sql);   
    myText =  myText+"`"+  makeStr(getStr(sql, ' '));
    sql.erase(0, getStr(sql, ' ').length());
    sql = remove_space(sql);
    myText += makeStr(getStr(sql, ' '));
    sql = myText;
    string tbloc =  "/home/projectfpga/Downloads/webserver/compiler/"+sql; 
    DIR *dr;
    struct dirent *en;
    dr = opendir(tbloc.c_str()); //open all or present directory
   if (dr)
   {
      while ((en = readdir(dr)) != NULL) {
        myText = en->d_name;
        myText = tbloc+"/"+myText;
        remove(myText.c_str());
         printf("deleting %s\n", myText.c_str()); //print all directory name
      }
      closedir(dr); //close all directory
      rmdir(tbloc.c_str());
   }
            tbloc =   "/home/projectfpga/Downloads/webserver/compiler/connect.txt"; 
    string tbloc1 =  "/home/projectfpga/Downloads/webserver/compiler/connect1.txt";
    FILE* fpc = fopen(tbloc1.c_str(),"w");   
    fclose(fpc);    
    ifstream MyRead(tbloc);
    ofstream MyReads(tbloc1);
    string fileDatax;

    while (getline (MyRead, fileDatax)) 
    {
        if(fileDatax != sql)
        {
            MyReads << fileDatax;
            MyReads << "\n";
        }       
    }MyRead.close();
     MyReads.close(); 
    remove(tbloc.c_str()); 
    rename(tbloc1.c_str(), tbloc.c_str()); 
}

string extract(string sql, string fileData){
    sql = remove_spaces(sql); 
           myText = ""; 
           counter= 0;
    string field  = "";
    string value  = "";
    int    sign   = 0;
    int    next   = 0;
    bool   result = 0; 


    if(sql.substr(0,1) == "*"){
        sql.erase(0, 1);
        ALL = true;
        return (sql);
    }else  
    if(sql.substr(0,4) == "DESC"){
        sql.erase(0, 4);
        desc = true;
        return (sql);
    }else     
    if(sql.substr(0,3) == "ASC"){
        sql.erase(0, 3);
        return (sql);
    }else 
    if(sql.substr(0,5) == "WHERE"){
        sql.erase(0, 5);
    }else         
    if(sql.substr(0,2) == "OR"){
        next = 1;
        sql.erase(0, 2);
    }else
    if(sql.substr(0,3) == "AND"){
        next = 2;
        sql.erase(0, 3);
    }else
    if(sql.substr(0,6) == "OFFSET"){
        sql.erase(0, 6);
        offset = true; 
        sql = remove_space(sql); 
        offDr  = sql[0];  
        sql.erase(0, 1);    
        sql = remove_space(sql);      
        while(isNum(sql[0])){
            myText += sql[0];
            sql.erase(0, 1);
        }
        offsetCount = stoi(myText);
        return (sql);
    } else          
    if(sql.substr(0,5) == "LIMIT"){
        sql.erase(0, 5);
        limitB = true;    
        while(isNum(sql[0])){
            myText += sql[0];
            sql.erase(0, 1);
        }
        limit= stoi(myText);
        return (sql);
    } 
  
    while(isAlphabet(sql[0]) || isNum(sql[0]) || sql[0] == '_'){
        field += sql[0];
        sql.erase(0, 1);
    }
    
    fileData.erase(0, getStr(fileData, '`').length()+1);
    int column  = stoi(getStr(fileData, '`'));
    fileData.erase(0, getStr(fileData, '`').length()+1);
    int coun = 1;
    while(fileData.length() > 0){
        if(field == getStr(fileData, '`')){
            break;
        }
        fileData.erase(0, getStr(fileData, '`').length()+1);        
        coun++;
    }
    if(coun == column+1 && field != "id"){
        error("field "+field+"  is undefined", 7);
        return "0";
    }
   string arr[] = {"!=", ">=", "<=", ">", "<", "="};

    for(int i = 0; i <= 5; i++){
        if(arr[i][0] == sql[0] && arr[i][1] == sql[1] && i < 5){
            sign = i;
            sql.erase(0, 2);
        }else
        if(arr[i] == sql.substr(0,1) && i > 2){
            sign = i;
            sql.erase(0, 1);
        }              
    }

    if(sql[0] != '\''){
        error("add values n hyphen",73);
        return "0";
    }sql.erase(0, 1);
    value = makeStr(getStr(sql, '\''));
    sql.erase(0, getStr(sql, '\'').length());
    if(sql[0] != '\''){
        error("add values n hyphen",59);
        return "0";
    }sql.erase(0, 1);

    operate *ptr; 
    operate  *temps = NULL; 
    temps =new operate();
    if(temps == NULL)
    {
        error("nOut of Memory Space:n", 0);
        return "0";
    }
    temps->field = field;
    temps->value = value;
    temps->sign  = sign;
    temps->offset = coun;
    temps->nexts = next;
    temps->result = false;

    if(valueStore ==NULL)
    {   
        valueStore =temps;
    }
    else
    {   
         operate *ptrs = valueStore ;
        while(ptrs->next !=NULL)
        {
            ptrs=ptrs->next;
        }
        ptrs->next = temps;
                  
    }        
    return sql;
}

string extracts(string sql, string fileData){
    string field = "";
    string value = "";
    int sign;
    int next = 0;
    int cts;
    bool result = 0; 
    counter =1 ;
    int offset = 0;
    string feld = "";

    if(sql.substr(0,5) == "WHERE"){
        sql.erase(0, 6);
        return sql;
    }

     while(isAlphabet(sql[0]) || isNum(sql[0]) || sql[0] == '_'){
        field += sql[0];
        sql.erase(0, 1);
    }
    
    fileData.erase(0, getStr(fileData, '`').length()+1);
    int column  = stoi(getStr(fileData, '`'));
    fileData.erase(0, getStr(fileData, '`').length()+1);
    
    int counte = 1;
    while(fileData.length() > 0){
        if(field == getStr(fileData, '`')){
            break;
        }
        fileData.erase(0, getStr(fileData, '`').length()+1);        
        counte++;
    }

    if(counte == column+1 && field != "id"){
        error("field "+field+"  is undefined", 6);
    }
    sql = remove_space(sql);

    if(sql[0] == '='){
        sql.erase(0, 1);
    }else
    {
        error("invalid syntax", 6);
        return "0";
    }              

    sql = remove_space(sql);

    if(sql[0] != '\''){
        error("add values n hyphen",79);
        return "0";
    }sql.erase(0, 1);
    value = makeStr(getStr(sql, '\''));
    sql.erase(0, getStr(sql, '\'').length());
    if(sql[0] != '\''){
        error("add values n hyphen",53);
        return "0";
    }sql.erase(0, 1);

    operate *ptr; 
    operate  *ted = NULL; 
    ted =new operate();
    if(ted == NULL)
    {
        error("nOut of Memory Space:n", 0);
        return "0";
    }
    ted->field = field;
    ted->value = value;
    ted->nexts = next;
    ted->offset = counte;
    ted->result = false;

    if(valueSt==NULL)
    {   
        valueSt=ted;
    }
    else
    {
         operate *ps = valueSt;
        while(ps->next !=NULL)
        {
            if(ps->field == ted->field){
                ps->value = ted->value;
                ps->nexts = ted->nexts;
                ps->offset = ted->offset;                                
                ps->result  = ted->result;                
                break;
            }
            ps=ps->next;
        }
        if(ps->field == ted->field){
                ps->value = ted->value;
                ps->nexts = ted->nexts;
                ps->offset = ted->offset;                 
                ps->result  = ted->result;   
        }else{
        ps->next=ted;
        }            
    }        
    return sql;
}

void delete_table(string sql, string dbname){

    counter = 0;
    string tbloc =  "/home/projectfpga/Downloads/webserver/compiler/"+dbname+"/"+dbname+".txt"; 
    string tbloc1 =  "/home/projectfpga/Downloads/webserver/compiler/"+dbname+"/"+sql+".txt";
    ifstream MyRead(tbloc);
    ofstream MyReads(tbloc1);
    string fileDatax;

    while (getline (MyRead, fileDatax)) 
    {
        if(fileDatax.substr(0,sql.length()) != sql)
        {
            MyReads << fileDatax;
            MyReads << "\n";
        }else
        if(fileDatax.substr(0,sql.length()) == sql)
        {
            counter++;
        }        
    }MyRead.close();
     MyReads.close();
    remove(tbloc.c_str());
    rename(tbloc1.c_str(), tbloc.c_str());
    if(counter == 0)
    {
        error("table "+sql+" does not exist", 23);
        return;
    }      
}

string getCol(string myText9, int offset){
    int cunter = 1;
    while (cunter != offset) 
    {
        myText9.erase(0, getStr(myText9, '`').length() + 1);
        cunter++;
    }
    myText9 = getStr(myText9, '`');
    return myText9;
}

string repStr(string myText5,string value, int mark){
    int counte = 1;
    string newStr;
   
    while (myText5.length() > 0) 
    {
        if(counte == mark){
            newStr += value+"`" ;
        }else
        {
            newStr += getStr(myText5, '`') + '`';
        }
  
        myText5.erase(0, getStr(myText5, '`').length() + 1);
        counte++;
    }
    return newStr;
}

void delete_row(string sql, string dbname){
    myText = "";
    counter = 0;

     sql = remove_space(sql);
    string tablename = getStr(sql, ' ');
    tablename = remove_spaces(tablename);
    sql.erase(0, tablename.length()+1);
    sql = remove_space(sql);

    string fileData;
    string tbloc =  "/home/projectfpga/Downloads/webserver/compiler/"+dbname+"/"+dbname+".txt"; 
    string tbloc1 =  "/home/projectfpga/Downloads/webserver/compiler/"+dbname+"/"+tablename+".txt";
    ifstream MyReadFile(tbloc);
    counter = 0;
    while (getline (MyReadFile, fileData)) 
    {
        if(fileData.substr(0,tablename.length())== tablename){
            counter++;
            break;
        }
    }
    MyReadFile.close(); 
    if(counter == 0){error("table "+tablename+" does not exist", 49);
    return;}
    sql = remove_spaces(sql);
    valueStore = NULL;
    while(sql != ""){
        sql = extract(sql, fileData);
    }
    operate *value    = valueStore;
    bool Loffset      = offset;
    int  LoffsetCount = offsetCount;
    int  Llimit       = limit;
    bool Ldesc        = desc;
    bool Lall         = ALL;
    bool LlimitB      = limitB;
    char LoffDr       = offDr;
    offset      = false;
    offsetCount = 0;
    limit       = 0;
    desc        = false;
    ALL         = false;
    limitB      = false;
    offDr       = '0';
    int totalsze =0;
    int result =0;  
    int res =0;  
    int count =0;      
    string c =""; 
     string str ="";     
     string myText9 =""; 
    
    ifstream MyReadF(tbloc1.c_str());
    tbloc =  "/home/projectfpga/Downloads/webserver/compiler/"+dbname+"/~"+to_string(marker)+tablename+".txt";
    ofstream MyReads(tbloc);
    while (getline (MyReadF, myText9))
    {
        if(Lall == true){
            result == 1;
        }else
        if(Loffset == true && ((LoffsetCount < count && LoffDr == '-')||(LoffsetCount > count && LoffDr == '+')))
        {   
            result = 0;
        }else 
        if((LlimitB == true && Llimit == 0)  || valueStore == NULL)
        {   
            result = 0;
        }else                   
        {      
            operate *prs;
            prs = valueStore;
            while(prs !=NULL)
            {    
                if(prs->field == "id"){
                    c = to_string(count);
                }else{
                c = getCol(myText9, prs->offset);}
                prs->result = compares(c, prs->sign, prs->value);
                if(prs->nexts == 1){
                    result = result || prs->result;
                }else
                if(prs->nexts == 2){
                    result = result && prs->result;
                }else            
                {
                    result = prs->result;
                }
                prs=prs->next;
            }
        if(LlimitB  == true && result ==1 ){Llimit--; }             
        }
        if(Loffset == true && LoffsetCount > count){
            result == 0;
        }      
        if(result == 0){
            MyReads<<myText9;
            MyReads<<endl;
        } 
        count++;
    }
    marker++;
    MyReadF.close(); 
    MyReads.close();     
    remove(tbloc1.c_str());
    rename(tbloc.c_str(), tbloc1.c_str());   
}

void update_table(string tablename, string sql, string dbname){
    
    string fileData;
    string tbloc =  "/home/projectfpga/Downloads/webserver/compiler/"+dbname+"/"+dbname+".txt"; 
    string tbloc1 =  "/home/projectfpga/Downloads/webserver/compiler/"+dbname+"/"+tablename+".txt";
    ifstream MyReadFisle(tbloc);
    counter = 0;
    while (getline (MyReadFisle, fileData)) 
    {
        if(fileData.substr(0,tablename.length())== tablename){
            counter++;
            break;
        }
    }
    MyReadFisle.close(); 
    if(counter == 0){error("table "+tablename+" does not exist", 49);
    return;} 

    sql = remove_space(sql);
    sql = extracts(sql, fileData);
    while(sql[0] ==','){
        sql = extracts(sql, fileData);
    }
    valueStore = NULL;
    while(sql != ""){
        sql = extract(sql, fileData);
    }
    operate *value    = valueStore;
    bool Loffset      = offset;
    int  LoffsetCount = offsetCount;
    int  Llimit       = limit;
    bool Ldesc        = desc;
    bool Lall         = ALL;
    bool LlimitB      = limitB;
    char LoffDr       = offDr;
        
    offset      = false;
    offsetCount = 0;
    limit       = 0;
    desc        = false;
    ALL         = false;
    limitB      = false;
    offDr       = '0';

    int totalsze =0;
    int result =0;  
    int res =0;  
    int count =1;      
    string c =""; 
     string str ="";     
     string myText9 =""; 

    ifstream MyReadF(tbloc1.c_str());
    tbloc =  "/home/projectfpga/Downloads/webserver/compiler/"+dbname+"/"+to_string(marker)+tablename+".txt";
    ofstream MyReads(tbloc);
    while (getline (MyReadF, myText9))
    {
        if(Lall == true){
            result = 1;
        }else
        if(Loffset == true && ((LoffsetCount < count && LoffDr == '-')||(LoffsetCount > count && LoffDr == '+')))
        {   
            result = 0;
        }else
        if(LlimitB == true && Llimit == 0 || valueStore == NULL)
        {   
            result = 0;
        }else
        {
            operate *prs;
            prs = valueStore;
            while(prs !=NULL)
            {    
                if(prs->field == "id"){
                    c = to_string(count);
                }else{
                c = getCol(myText9, prs->offset);}
                prs->result = compares(c, prs->sign, prs->value);
                if(prs->nexts == 1){
                    result = result || prs->result;
                }else
                if(prs->nexts == 2){
                    result = result && prs->result;
                }else            
                {
                    result = prs->result;
                }
                prs=prs->next;
                
            }
            if(LlimitB  == true && result ==1 ){Llimit--; } 
        }
        
        operate* pnnm = valueSt;  
        while(pnnm !=NULL && result == 1)
        {     
            if(pnnm->field == "id"){
                error("id is unmodifiable", 9);
                return;
            }else{
                
                myText9 = repStr(myText9, pnnm->value, pnnm->offset);
            }
            pnnm=pnnm->next;
        } 
        MyReads<<myText9;
        MyReads<<endl;        

        count++;
    }
    marker++;
    MyReadF.close(); 
    MyReads.close();     
    remove(tbloc1.c_str());
    rename(tbloc.c_str(), tbloc1.c_str());   
}

void selects(string  sql, string  row, int pointer, string dbname){
    sql = remove_space(sql);
     if(getStr(sql, ' ')=="SELECT"){
        sql.erase(0, 6);
     }else{error("invalid select statement", 22);
     return;}
    sql = remove_space(sql);
    while(sql.substr(0, 5) != "FROM "){
        sql.erase(0, 1);
        if(sql.length() < 7){
            error("Reached the end of your select statement",34);
            return;
        }
    }sql.erase(0, 5);
    sql = remove_space(sql);
    string tablename = getStr(sql, ' ');
    sql.erase(0, tablename.length()+1);
    sql = remove_space(sql);
    string fileData;
    string tbloc =  "/home/projectfpga/Downloads/webserver/compiler/"+dbname+"/"+dbname+".txt"; 
    string tbloc1 =  "/home/projectfpga/Downloads/webserver/compiler/"+dbname+"/"+tablename+".txt";
    ifstream MyReadFisle(tbloc);
    counter = 0;
    while (getline (MyReadFisle, fileData)) 
    {
        if(fileData.substr(0,tablename.length())== tablename){
            counter++;
            break;
        }
    }
    MyReadFisle.close(); 
    if(counter == 0){error("table "+tablename+" does not exist", 50);
    return;} 
    valueStore = NULL;
    while(sql != ""){
        sql = extract(sql, fileData);
    }
    
    bool Loffset      = offset;
    int  LoffsetCount = offsetCount;
    int  Llimit       = limit;
    bool Ldesc        = desc;
    bool Lall         = ALL;
    bool LlimitB      = limitB;
    char LoffDr       = offDr;

    offset      = false;
    offsetCount = 0;
    limit       = 0;
    desc        = false;
    ALL         = false;
    limitB      = false;
    offDr       = '0';

    int result = 1;  
    int count =0;      
    string c =""; 
     string myText4 =""; 
     char t;

    ifstream MyReade(tbloc1.c_str());
     
    while (getline (MyReade, myText4)){
        if(Loffset == true && ((LoffsetCount < count && LoffDr == '-')||(LoffsetCount > count && LoffDr == '+')))
        {   
            result = 0;
        }else
        if(valueStore  == NULL || Lall == true)
        {
            result == 1;
        }else
        {   operate *pnnc    = valueStore;
            while(pnnc != NULL)
            {
                if(pnnc->field == "id"){
                    c = to_string(count);
                }else{
                c = getCol(myText4, pnnc->offset);}
                
                pnnc->result = compares(c, pnnc->sign, pnnc->value); 
                if(pnnc->nexts == 1){
                    result = result || pnnc->result;
                }else
                if(pnnc->nexts == 2){
                    result = result && pnnc->result;
                }else            
                {
                    result = pnnc->result;
                    
                }     
                if(pnnc->next != NULL){          
                    pnnc = pnnc->next;}else{break;}
            }
        }
        if(result == 1){ 
            myText4.erase(0,getStr(myText4,'`').length()+1);
            string str = dbname+"`"+tablename+"`"+to_string(count+1)+"`"+myText4;            
            insertVar(row, str, 0);
            int count = pointer;
            scope.push("loop");  
            
            parse(count);
            scope.pop(); 
            if(LlimitB  == true){
                Llimit--;
                if(Llimit == 0){
                    
                    break;}
            }
        }   
        count++;
    }
    MyReade.close(); 
  
}

int parse(int pointer){  
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
                        writeTxt(phpdata, sql);
                        
                        pointer = pointer +  3;                         
                    }else{writeTxt(phpdata, sql);
                   }                  
                }else
                if(expect(TOKEN_NUMBER, pointer+1) || expect(TOKEN_STRING, pointer+1)) 
                {
                    writeTxt(phpdata, tokenData[pointer+1]); 
                                   
                }pointer = pointer +  2;                        
            }
            pointer++;

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
                if(getStr(sql, ' ') == "TABLE" && connect == true){
                    sql.erase(0, 5);
                    create_table(sql, database);
                }                  
            }else
            if(getStr(sql, ' ') == "INSERT" && connect == true){
                sql.erase(0, 6);
                sql = remove_space(sql);
                if(getStr(sql, ' ') == "INTO"){
                    sql.erase(0, 4);
                    sql = remove_space(sql);
                    insert_table(sql, database);
                }else{
                    error("invalid syntax",0);
                    return pointer;
                }              
            }else
            if(getStr(sql, ' ') == "DELETE"){
                sql.erase(0, 6);
                sql = remove_space(sql);
                if(getStr(sql, ' ') == "DATABASE"){
                    sql.erase(0, 8);
                    sql = remove_space(sql);                  
                    delete_database(sql);
                }else
                if(getStr(sql, ' ') == "TABLE"  && connect == true){
                    sql.erase(0, 5);
                    sql = remove_spaces(sql);
                    delete_table(sql, database);
                }else
                if(getStr(sql, ' ') == "FROM"  && connect == true){
                    sql.erase(0, 4);
                    sql = remove_space(sql);
                    delete_row(sql, database);
                }                                
                else{
                    error("invalid syntax",0);
                    return pointer;
                }              
            }else
            if(getStr(sql, ' ') == "UPDATE" && connect == true){
                sql.erase(0, 6);
                sql = remove_space(sql);
                string tabname = getStr(sql, ' ');
                sql.erase(0, tabname.length());
                sql = remove_space(sql);
                if(getStr(sql, ' ') == "SET"){
                    sql.erase(0, 3);
                    sql = remove_spaces(sql);
                    update_table(tabname, sql, database);
                }                               
                else{
                    error("invalid syntax",10);
                    return pointer;
                }              
            }
            pointer++;
            if((expect(TOKEN_RIGHT_PAREN, TOKEN_SEMICOLON, pointer))){ 
                pointer += 2;
            }else{error("expected a closing parenthesis and semi colon",0);
            return pointer;}                            
        }else   
        if(expect(TOKEN_WHILE, TOKEN_LEFT_PAREN, TOKEN_IDENTIFIER, pointer))
        {
            string row = tokenData[pointer+2];
            pointer += 3;
            if(expect(TOKEN_EQ_EQUAL, M_QUERY, TOKEN_LEFT_PAREN, pointer))
            {        
                pointer += 3;
                if(connect == false){
                    error("login to database",14);
                    return pointer;
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
                    selects(sql, row, pointer, database);
                }else
                {
                    error("expected a closing parenthesis and semi colon",9);
                    return pointer;
                } 
               
                int bracec = 1;
                while(bracec > 0)
                {
                    if(expect(TOKEN_RIGHT_BRACE, pointer)){bracec--;}
                    else
                    if(expect(TOKEN_LEFT_BRACE, pointer)){bracec++;}
                    pointer++;
                } 
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
                string database;
                database = findData(tokenData[pointer]);
                if(expect(TOKEN_LEFT_BLOCK, pointer+1)){
                database = findrow(database, tokenData[pointer+2]); 
                pointer += 3;                          
                }
            }else{database = tokenData[pointer];}    
            pointer++;          
            
            database = database+"`"+username+password;
            ifstream MyReadFile("connect.txt"); 
            while (getline (MyReadFile, data)) {
               if(database  == data && servername == "localhost"){
                connect = true;
                break;
               }
            }
            if(connect == false){
                error("unable to login to database", 0);
                return pointer;
            }
            MyReadFile.close();           
            if((expect(TOKEN_RIGHT_PAREN, TOKEN_SEMICOLON, pointer))){ 
             pointer += 2;
            }else{error("expected a closing parenthesis and semi colon",0);
            return pointer;}   

        }else    
        if((expect(TOKEN_RIGHT_BRACE, TOKEN_ELSE, TOKEN_LEFT_BRACE,  pointer))&& !scope.empty() && scope.top() == "if"){  
            pointer += 3;
            scope.pop();
            int braceCount = 1;
            while(braceCount != 0)
            {
                if(expect(TOKEN_RIGHT_BRACE, pointer)){braceCount--;}
                else
                if(expect(TOKEN_LEFT_BRACE, pointer)){braceCount++;}
                pointer++;
            }
        }else                    
        if((expect(TOKEN_RIGHT_BRACE, pointer)) && !scope.empty() && scope.top() == "if"){  
             pointer++;
            scope.pop();
        }else
        if((expect(TOKEN_RIGHT_BRACE, pointer)) && !scope.empty() && scope.top() == "loop"){ 

            return 0;
        }else         
        if(expect(TOKEN_IF, TOKEN_LEFT_PAREN, pointer))
        {   pointer += 2;
            TokenType calculate;
            while(!expect(TOKEN_RIGHT_PAREN, pointer))
            {
                if(expect(TOKEN_IDENTIFIER, pointer))
                {
                    sql = findData(tokenData[pointer]);
                    pointer++;
                    if(expect(TOKEN_LEFT_BLOCK, pointer))
                    { 
                        sql = findrow(sql, tokenData[pointer + 1]); 
                        pointer+=3;                          
                    }                
                }else
                if(expect(TOKEN_NUMBER, pointer) || expect(TOKEN_STRING, pointer)) 
                {
                    sql = tokenData[pointer];   
                    pointer++;                
                }else{
                    calculate = Token->type[pointer];
                    pointer++;} 
                calc.push_back(sql);                      
            }  
            pointer += 2;
            bool result = compares(calc[0], calculate, calc[2]);
            calc.clear();    
            if(!result){
                int braceC = 1;
                while(braceC > 0)
                {
                    if(expect(TOKEN_RIGHT_BRACE, pointer)){braceC--;}
                    else
                    if(expect(TOKEN_LEFT_BRACE, pointer)){braceC++;}
                    pointer++;
                }
                if(expect(TOKEN_ELSE, pointer)){
                    scope.push("if");
                    pointer += 2;
                }
            }else{scope.push("if");}

        }else    
        if(expect(TOKEN_SETC, TOKEN_LEFT_PAREN, pointer))
        {   pointer += 2;
            while(!expect(TOKEN_SEMICOLON, pointer))
            {
                if(expect(TOKEN_IDENTIFIER, pointer))
                {
                    sql = findData(tokenData[pointer]);
                    pointer++;
                    if(expect(TOKEN_LEFT_BLOCK, pointer))
                    { 
                        sql = findrow(sql, tokenData[pointer + 1]); 
                        pointer+=3;                          
                    }                
                }else
                if(expect(TOKEN_NUMBER, pointer) || expect(TOKEN_STRING, pointer)) 
                {
                    sql = tokenData[pointer];   
                    pointer++;                
                }
                pointer++;
                calc.push_back(sql);                      
            }  
            pointer ++;
            writeTxt(phpdata, "<script>function setcookie() {");
            writeTxt(phpdata, "const d = new Date();");
            writeTxt(phpdata, "d.setTime(d.getTime() + ("+calc[2]+"*24*60*60*1000));");
            writeTxt(phpdata, "let expires = 'expires=' + d.toUTCString();");
            writeTxt(phpdata, "document.cookie = '"+calc[0]+" = "+calc[1]+ ";' + expires + ';path=/';}</script>"); 
            calc.clear();
        }else               
        if(expect(TOKEN_HEADER, TOKEN_LEFT_PAREN, TOKEN_NUMBER, pointer))
        {   
            string delay = tokenData[pointer+2]; 
            pointer += 3;
            if(expect(TOKEN_IDENTIFIER, pointer))
            {
                sql = findData(tokenData[pointer]);
                pointer++;
                if(expect(TOKEN_LEFT_BLOCK, pointer))
                { 
                    sql = findrow(sql, tokenData[pointer + 1]); 
                    pointer+=3;                          
                }                
            }else
            if(expect(TOKEN_NUMBER, pointer) || expect(TOKEN_STRING, pointer)) 
            {
                sql = tokenData[pointer];   
                pointer++;                
            }else{pointer++;} 
            pointer += 2;
             writeTxt(phpdata, "<meta http-equiv='Refresh' content=\""+delay+"; url='"+sql+"'\" />"); 

        }else{
            error("token type "+tokenName[Token->type[pointer]]+" not expected ", pointer);
            return pointer;
        }
    }  
    if(expect(TOKEN_EOF, pointer)){
        pointer++;
        
    }
    return pointer;
}
