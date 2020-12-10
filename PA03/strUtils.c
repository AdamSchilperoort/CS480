#include "booleans.h"
#include "mathUtils.h"
#include "errors.h"
#include <stdlib.h>

//ALL STRING-RELATED UTILITIES

/*
    returns the length of a string w/o null terminator
*/
int strLen(const char* string) 
{
    int len = 0;
    
    while (string[len] != '\0') 
    {
        len++;
    }

    return len;
}

/*
    copies one string into another
*/
void strCopy(char *src, char *dest) 
{
    int len = strLen(src);
    int pos;

    //if both strings are allocated
    if (src && dest) 
    {
        for (pos = 0; pos < len; pos++) 
        {
            dest[pos] = src[pos];
        }

        //adds null terminator to end of string
        dest[pos] = '\0';
    }
}

/*
    gets and returns the file extension from a string
*/
char* getFileExt(char* src) 
{
    int len = strLen(src);
    //allocates 5 chars in memory
    char *ext = (char*) calloc(5, sizeof(char));
    
    //sets null terminator on string first
    ext[4] = '\0';

    //iterates over src in reverse order for 4 positions
    for (int pos = 4; pos > 0; pos--) 
    {
        ext[4-pos] = src[len-pos];
    }

    return ext;
}

/*
    compares two strings and returns a boolean value 
*/
int strCmp(const char* src, char* target) 
{
    int srcLen = strLen(src);

    //returns FALSE if string lengths do not match
    if (srcLen != strLen(target)) 
    {
        return FALSE;
    }

    //iterates over src string
    for (int pos = 0; pos < srcLen; pos++) 
    {

        //if any characters do not match, return FALSE
        if (src[pos] != target[pos])
        {
            return FALSE;
        }
    }

    //retrn TRUE if all characters match as well as length
    return TRUE;
}

/*
    checks to see if a string contains another string:
        * iterates over a string
        * checks for characters from substring
            * if starting character is found, move on to next in substring
            * otherwise, reset to first character in substring
*/
int strContains(char* src, char* substr) 
{
    int srcIter = 0;
    int substrIter = 0;
    char srcCurr = src[0];
    char substrChar = substr[0];

    //while both characters are not NULL
    while (srcCurr && substrChar) 
    {
        
        //iterate over src string
        srcCurr = src[srcIter];

        //if characters match
        if (srcCurr == substrChar) 
        {
            //move to the next character in both strings
            substrIter++;
            substrChar = substr[substrIter];
            srcIter++;

        } 

        //if characters do not match
        else 
        {

            //iterates over src only if substr is on the first character
            if (substrIter == 0) 
            {
                srcIter++;
            }

            //resets substring iterator and substring character
            substrIter = 0;
            substrChar = substr[0];
        }
    
    }

    //when loop broken, if substring character is NULL, this will return TRUE
    return !substrChar;
}

/*
    finds a substring in a string and returns the end character position
*/
int substrPos(char *src, char *target) 
{
    int srcIter = 0;
    int targetIter = 0;
    char srcChar = src[0];
    char targetChar = target[0];

    //if the string doesn't contain the target string, return -1
    if (!strContains(src, target)) 
    {
        return -1;
    }

    //while src and target characters are not NULL
    while (srcChar && targetChar) 
    {
        srcChar = src[srcIter];
        
        //if characters match, iterate both strings one position
        if (srcChar == targetChar) 
        {
            targetIter++;
            targetChar = target[targetIter];
            srcIter++;

        } 

        //if characters don't match
        else 
        {

            //only iterate src on target character being in first position
            if (targetIter == 0) 
            {
                srcIter++;
            }

            //reset target characters on no match
            targetIter = 0;
            targetChar = target[0];
        }
    }

    //return src iterator as position value
    return srcIter;
}

/*
    reverses a string in place
*/
void rev_str(char *src) 
{
    char tmp;
    int len = strLen(src) - 1; //IGNORE NULL TERMINATOR
    int halfLen = len / 2;

    //logic for dealing with odd lengths
    if (len % 2 != 0) 
    {
        halfLen++;
    }

    //iterates up to the halfway point of the string
    for (int pos = 0; pos < halfLen; pos++) 
    {

        //sets temp value and then switches length - position and position charactersin string
        tmp = src[len - pos];
        src[len - pos] = src[pos];
        src[pos] = tmp;
    }
}

/*
    creates a substring from a string:
        * takes a source string, start index, end index, and substring as inputs
        * iterates over string from start to end indices and sets corresponding indices in substring
*/
void substr(char *src, int start, int end, char *substr) 
{
    int substrPos = 0;
    int pos;

    //iterates over string from start to end positions
    for (pos = start; pos < end; pos++) 
    {

        //sets substring equivalent position to the src character
        substr[substrPos] = src[pos];
        substrPos++;
    }

    //terminates the string
    substr[substrPos] = '\0';
}

/*
    only retains certain characters in a string:
        * alphabetical
        * numerical
        * '-'
        * '.'
*/
void removeNonSymbols(char *src) 
{
    int len = strLen(src);
    int tmpPos = 0;
    int pos;
    char sym;

    //create and allocate memory for a temporary storage string
    char *tmp = (char*) calloc(100, sizeof(char));

    //iterates over string
    for (pos = 0; pos < len; pos++) 
    {
        sym = src[pos];

        //if the symbol is alphanumerica or '.' or '-', add it to the temp string
        if ((sym >= 'A' && sym <= 'Z') 
        || (sym >= 'a' && sym <= 'z') 
        || (sym >= '0' && sym <= '9')
        || sym == '-' 
        || sym == '.') 
        {
            tmp[tmpPos] = sym;
            tmpPos++;
        }
    }

    //terminate temp string and then copy it back into the original src string
    tmp[pos] = '\0';
    strCopy(tmp, src);

    free(tmp);
}

/*
    converts a character to an integer:
        * returns int value if character is numeric
        * returns -1 if character is not numeric
*/
int c2i(char src)
{
    int num;

    switch (src)
    {
        case '0':
            num = 0;
            break;
        case '1':
            num = 1;
            break;
        case '2':
            num = 2;
            break;
        case '3':
            num = 3;
            break;
        case '4':
            num = 4;
            break;
        case '5':
            num = 5;
            break;
        case '6':
            num = 6;
            break;
        case '7':
            num = 7;
            break;
        case '8':
            num = 8;
            break;
        case '9':
            num = 9;
            break;
        default:
            num = -1;
    }
    
    return num;
}

/*
    converts a string to a float value
*/
float s2f(char *src) 
{
    int iter = 0;
    char currChar = src[0];
    int decimalPos = substrPos(src, ".");
    int places = decimalPos - 2; //SUBTRACT 2 IN ORDER TO GET TO ONE BEFORE THE TARGET CHAR
    float num = 0.0f;

    //while current character is not NULL
    while (currChar) 
    {
        //if the current character is numeric
        if (currChar >= '0' && currChar <= '9') 
        {
            /*increase num by character int value * 10^places.
            this works by taking an int and moving it x positions
            to the left or right.*/
            num += c2i(currChar) * raiseToPower(10, places);
            places--;
        } else if (currChar == '.') {
            //if the current character is '.', set places to -1
            places = -1;
        }

        iter++;
        currChar = src[iter];
    }

    return num;
}

/*
    converts a string to an int value
*/
int s2i(char *src) 
{
    int len = strLen(src);
    int num = 0;
    int currNum;

    //iterates over string
    for (int pos = 0; pos < len; pos++) 
    {
        //moves current value in num one position to the left
        num *= 10;

        //gets current number from src string
        currNum = c2i(src[pos]);

        //if current number is actually numeric logic
        if (currNum >= 0 && currNum < 10) 
        {
            num += currNum;
        } 

        //return current number if it is not numeric
        else 
        {
            return currNum;
        }
    }

    return num;
}

/*
    checks to see if src string is a supported cpu scheduling type
    returns TRUE or FALSE
*/
int checkCpuSched(char *src) 
{
    return strCmp(src, "FCFS-N")
    || strCmp(src, "SJF-N")
    || strCmp(src, "SRTF-P")
    || strCmp(src, "FCFS-P") 
    || strCmp(src, "RR-P");
}

/*
    checks to see if src is a supported log to type
    returns TRUE or FALSE
*/
int checkLogTo(char *src) 
{
    return strCmp(src, "Monitor") 
    || strCmp(src, "File") 
    || strCmp(src, "Both");
}

/*
    sets all values in a string to NULL
*/
void strClear(char *src) 
{
    int len = strLen(src);

    for (int pos = len; pos > 0; pos--) 
    {
        src[pos] = '\0';
    }
}

/*
    removes all non number characters from a string
*/
void strRmNonNumbers(char *src) 
{
    int pos, len, num;

    pos = 0;
    len = strLen(src);

    //iterates over string
    for (pos = 0; pos < len; pos++) 
    {

        //converts character to a number
        num = c2i(src[pos]);

        //if number not numeric, moves string one position left
        if (num > 9 || num < 0) 
        {
            src[pos] = src[pos+1];
        }
    }
}

/*
    checks to see if character is numeric
*/
int charIsNum(char src) 
{
    return src >= '0' && src <= '9';
}

/*
    checks to see if character is uppercase
*/
int charIsUpper(char src) 
{
    return src >= 'A' && src <= 'Z';
}

