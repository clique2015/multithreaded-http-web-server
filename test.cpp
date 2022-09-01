#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"


std::vector<std::string> serverData;
int main(int argc, char const *argv[])
{
    string requestFile = "./public/chat.php";
    int n = requestFile.length();
    char char_array[n + 1];
    strcpy(char_array, requestFile.c_str());
    printf("requestng . %s\n",char_array);
    openFile(char_array);
}

