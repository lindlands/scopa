#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "macros.h"

void toLower(char* str){
    /*converts string to lowercase*/
    int i;
    int j = strlen(str);
    for(int i = 0; i < j; i++){
        if (str[i] == '\000'){
            break;
        }
        str[i] = tolower(str[i]);
    }
}

int getCommand(char *arr){
    fgets(arr, LENGTH, stdin);
    toLower(arr);
    printf("\n");
}

int compCom(char *input, char *match){
    /*compares two strings to see if they match*/
    int result;
    int len;
    //toLower(match);
    len = strlen(match);
    if(strlen(input) < len){
        return 1;
    }
    result = strncmp(input,match,len);
    if(result == 0){
        if ( !result && input[len] && (input[len] != '\n' || input[len+1]) ){
            return -1;
        }
        return result;
    }
}

int convertToNum(char* command){
    /* takes in string and returns int if possible. else, returns -1. */
    int i;
    for (int i = 0; i < strlen(command) - 1; i++){
        if(!isdigit(command[i])){
            return -1;
        }
    }
    return atoi(command);
}

void parseCommand(char *str, int nums[INPUTLEN]){
    /*takes in string and fills in array with numbers from string*/
    int j = 0;
    int i;
    for (int i = 0; i < strlen(str); i++){
        if (str[i] != ' ' && str[i] != '\n'){
            if (str[i] == 1 && str[i + 1] == 0){
                nums[j] = 10;
                j++;
                i++;
            }else{
                nums[j] = str[i] - 48;
                j++;
            }
        }
    }
    
}