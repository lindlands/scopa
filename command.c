#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "macros.h"

#define CHAR_TO_NUM 48

/**
 * Converts string to lowercase
 * 
 * @param str: string
 */
void toLower(char* str){
    int i;
    int j = strlen(str);
    for(i = 0; i < j; i++){
        if (str[i] == '\000'){
            break;
        }
        str[i] = tolower(str[i]);
    }
}

void getCommand(char *arr){
    fgets(arr, LENGTH, stdin);
    toLower(arr);
    printf("\n");
}

/**
 * Compares two strings to see if they match
 * 
 * @param inputOne: string
 * @param inputTwo: string
 * @return int: 0 if the two inputs match
 */
int compCom(char *inputOne, char *inputTwo){
    int result;
    int len;
    len = strlen(inputTwo);
    if(strlen(inputOne) < len){
        return 0;
    }
    result = strncmp(inputOne,inputTwo,len);
    if(result == 0){
        if ( !result && inputOne[len] && (inputOne[len] != '\n' || inputOne[len+1]) ){
            return 0;
        }
        return 1;
    }
    return 0;
}

/**
 * Converts given string to a positive integer.
 * 
 * @param str: string
 * @return int: positive integer or -1 if not possible
 */
int convertToNum(char* str){
    int i;
    for (i = 0; i < strlen(str) - 1; i++){
        if(!isdigit(str[i])){
            return -1;
        }
    }
    return atoi(str);
}

/**
 * takes in string and fills in array with numbers from string 
 * 
 * @param str: string
 * @param nums: empty array 
 * @return int: 0 if successful
 */
int parseCommand(char *str, int nums[INPUTLEN]){
    int j = 0;
    int i;
    for (i = 0; i < strlen(str); i++){
        if (isdigit(str[i])){
            if (nums[j] == 0){
                nums[j] = str[i] - CHAR_TO_NUM;
            }else{
                nums[j] = nums[j] * 10 + (str[i] - CHAR_TO_NUM);
            }
        }else if(str[i] == ' '){
            j++;
        }else if (str[i] != '\n'){
            return 0;
        }
    }
    return 1;
}